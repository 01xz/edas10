#include "cal_num_grid.h"

/**
 * cal_b2net - calculate the distance between the boundary and the net
 * @b: boundary
 * @minsp: A structure that stores information such as minimum spacing
 * @set: A structure that stores direction information
 * note: cal_b2net() will return the distance between the boundary and the nety
 */
double cal_b2net(double b[], min_space *minsp, setting *set) {
  double b2net = 0;
  if ((set->axis == 0) && (set->dir == 0))
    b2net = b[2] -minsp->coords[minsp->nmcoords - 1];
  else if ((set->axis == 0) && (set->dir == 1))
    b2net =minsp->coords[0] - b[0];
  else if ((set->axis == 1) && (set->dir == 0))
    b2net = b[3] -minsp->coords[minsp->nmcoords - 1];
  else if ((set->axis == 1) && (set->dir == 1))
    b2net =minsp->coords[0] - b[1];
  return b2net;
}

/**
 * cal_nmesh_edge - Calculates the number of meshs at edge locations
 * @b: boundary
 * @minsp: A structure that stores information such as minimum spacing
 * @calnum: A structure that stores information such as the number of grids
 * @set: A structure that stores direction information
 * note : cal_nmesh_edge() will calculates the number of meshs at edge locations
 */
void cal_nmesh_edge(double b[], min_space *minsp, calnum_grid *calnum, setting *set) {
  int divtyp = 0;
  double b2net = cal_b2net(b, minsp, set);
  if ((b2net > ((RANGE1 - 0.01) * minsp->mins)) && (b2net < ((RANGE2 + 0.01) * minsp->mins)))
    divtyp = 1;
  else if ((b2net > ((RANGE2 - 0.01) * minsp->mins)) && (b2net < ((RANGE3 + 0.01) * minsp->mins)))
    divtyp = 2;
  else if (b2net > ((RANGE3 - 0.01) * minsp->mins))
    divtyp = 3;
  else if (b2net < ((RANGE1 + 0.01) * minsp->mins))
    divtyp = 0;
  int nm_nuni = 1;
  double len_nuni = 0;
  int nm_uni = 0;
  double len_uni = 0;
  double l_accum = 0;
  double l_multi = 0;

  if (divtyp > 0) {
    l_multi = set->cr[divtyp - 1];
    len_nuni = (b2net - NUMUNI * minsp->mins) * set->pnuni_r[divtyp - 1];
    l_accum = (l_multi - 1) * minsp->mins / (set->cr[divtyp - 1] - 1);
    while (l_accum < len_nuni) {
      l_multi = l_multi * set->cr[divtyp - 1];
      l_accum = (l_multi - 1) * minsp->mins / (set->cr[divtyp - 1] - 1);
      nm_nuni = nm_nuni + 1;
    }

    //nm_uni = (int)round((len_nuni / minsp->mins / set->pnuni_r[divtyp - 1] - (l_multi - 1) / (set->cr[divtyp - 1] - 1)) / pow(set->cr[divtyp - 1], (double)(nm_nuni - 1.0)));
    //len_uni = ((len_nuni / minsp->mins / set->pnuni_r[divtyp - 1] - (l_multi - 1) / (set->cr[divtyp - 1] - 1))) / nm_uni * minsp->mins;
     nm_uni = (int)round((len_nuni/set->pnuni_r[divtyp - 1] - l_accum) / (minsp->mins*pow(set->cr[divtyp - 1], (double)(nm_nuni - 1.0))));
     len_uni = ((len_nuni / set->pnuni_r[divtyp - 1] - l_accum)) / nm_uni ;
     calnum->nm = calnum->nm + nm_nuni + nm_uni + (int)NUMUNI;
  } else {
    nm_uni = (int)round(b2net / minsp->mins);
    calnum->nm = calnum->nm + nm_uni;
  }

  if (set->dir == 0) {
    calnum->divtyp_1 = divtyp;
    calnum->nmedge_nuni_1 = nm_nuni;
    calnum->nmedge_uni_1 = nm_uni;
    calnum->len_uni_1 = len_uni;
  } else {
    calnum->divtyp_2 = divtyp;
    calnum->nmedge_nuni_2 = nm_nuni;
    calnum->nmedge_uni_2 = nm_uni;
    calnum->len_uni_2 = len_uni;
  }
}

/**
 * cal_nmesh_mid - Calculate the number of meshes in the middle position
 * @minsp: A structure that stores information such as minimum spacing
 * @calnum: A structure that stores information such as the number of grids
 * @set: A structure that stores direction information
 * note : cal_nmesh_mid() will calculate the number of meshes in the middle position
 */
void cal_nmesh_mid(min_space *minsp, calnum_grid *calnum, setting *set){
  calnum->divtyp_mid = NULL;
  calnum->divtyp_mid = (int *)malloc(minsp->nmcoords * sizeof(int));
  if (calnum->divtyp_mid == NULL)
    printf("Failed to create memory of calnum->divtyp_mid \n");

  calnum->nmid_nuni = NULL;
  calnum->nmid_nuni = (int *)malloc(minsp->nmcoords * sizeof(int));
  if (calnum->nmid_nuni == NULL)
    printf("Failed to create memory of calnum->nmid_nuni \n");

  calnum->nmid_uni = NULL;
  calnum->nmid_uni = (int *)malloc(minsp->nmcoords * sizeof(int));
  if (calnum->nmid_uni == NULL)
    printf("Failed to create memory of calnum->nmid_uni \n");

  calnum->lmid_uni = NULL;
  calnum->lmid_uni = (double *)malloc(minsp->nmcoords * sizeof(double));
  if (calnum->lmid_uni == NULL)
    printf("Failed to create memory of calnum->lmid_uni \n");

  double l_nuni = 0;
  double l_accum = 0;
  double l_multi = 0;
  double len = 0;
  for (int i = 0; i < minsp->nmcoords - 1; i++) {
    if (((minsp->coords[i + 1] -minsp->coords[i]) > (RANGE1 - 0.01) * minsp->mins) && ((minsp->coords[i + 1] -minsp->coords[i]) < (RANGE2 + 0.01) * minsp->mins))
      calnum->divtyp_mid[i] = 1;
    else if (((minsp->coords[i + 1] -minsp->coords[i]) > (RANGE2 - 0.01) * minsp->mins) && ((minsp->coords[i + 1] -minsp->coords[i]) < (RANGE3 + 0.01) * minsp->mins))
      calnum->divtyp_mid[i] = 2;
    else if ((minsp->coords[i + 1] -minsp->coords[i]) > (RANGE3 - 0.01) * minsp->mins)
      calnum->divtyp_mid[i] = 3;
    else if ((minsp->coords[i + 1] -minsp->coords[i]) < (RANGE1 + 0.01) * minsp->mins)
      calnum->divtyp_mid[i] = 0;

    if (calnum->divtyp_mid[i] > 0){
      calnum->nmid_nuni[i] = 1;
      l_multi = set->cr[calnum->divtyp_mid[i] - 1];
      l_accum = (l_multi - 1) / (set->cr[calnum->divtyp_mid[i] - 1] - 1) * minsp->mins;
      l_nuni = (minsp->coords[i + 1] -minsp->coords[i] - 2 * NUMUNI * minsp->mins) * set->pnuni_r[calnum->divtyp_mid[i] - 1] / 2;
      while (l_accum < l_nuni) {
        l_multi = l_multi * set->cr[calnum->divtyp_mid[i] - 1];
        l_accum = (l_multi - 1) / (set->cr[calnum->divtyp_mid[i] - 1] - 1) * minsp->mins;
        calnum->nmid_nuni[i] = calnum->nmid_nuni[i] + 1;
      }

      //len_sum = (l_multi - 1) / (set->cr[calnum->divtyp_mid[i] - 1] - 1);
      // calnum->nmid_uni[i] = (int)round((2 * l_nuni / minsp->mins / set->pnuni_r[calnum->divtyp_mid[i] - 1] - 2 * len_sum) / pow(set->cr[calnum->divtyp_mid[i] - 1], (double)calnum->nmid_nuni[i] - 1.0));
      //calnum->lmid_uni[i] = (2 * l_nuni / minsp->mins / set->pnuni_r[calnum->divtyp_mid[i] - 1] - 2 * len_sum) / calnum->nmid_uni[i] * minsp->mins;
      len = 2 * l_nuni / set->pnuni_r[calnum->divtyp_mid[i] - 1] - 2 * l_accum;
      calnum->nmid_uni[i] = (int)round(len / (minsp->mins*pow(set->cr[calnum->divtyp_mid[i] - 1], (double)calnum->nmid_nuni[i] - 1.0)));
      calnum->lmid_uni[i] = len / calnum->nmid_uni[i];
      calnum->nm = calnum->nm + calnum->nmid_uni[i] + 2 * calnum->nmid_nuni[i] + 2 * (int)NUMUNI;
    } else {
      calnum->nmid_uni[i] = (int)round((minsp->coords[i + 1] -minsp->coords[i]) / minsp->mins);
      calnum->nm = calnum->nm + calnum->nmid_uni[i];
    }
  }
}

/**
 * cal_nmesh - Calculate the number of meshes
 * @b: boundary
 * @minsp: A structure that stores information such as minimum spacing
 * @calnum: A structure that stores information such as the number of grids
 * @set: A structure that stores direction information
 * note : cal_nmesh() will calculate the number of meshes
 */
void cal_nmesh(double b[], min_space *minsp, setting *set, calnum_grid *calnum) {
  set->dir = 0;
  calnum->nm = 0;
  cal_nmesh_edge(b, minsp, calnum, set);
  set->dir = 1;
  cal_nmesh_edge(b, minsp, calnum, set);
  cal_nmesh_mid(minsp, calnum, set);
}
