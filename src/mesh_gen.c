#include "mesh_gen.h"

/**
 * cal_lmesh_edge -Calculates the mesh length at edge positions
 * @b: boundary
 * @mg: A structure that records information such as mesh length
 * @minsp: A structure that stores information such as minimum spacing
 * @calnum: A structure that stores information such as the number of grids
 * @set: A structure that stores direction information
 * note : cal_lmesh_edge() will calculates the mesh length at edge positions
 */
void cal_lmesh_edge(double *b, mesh *mg, calnum_grid *calnum, min_space *minsp, setting *set) {
  int divtype = 0;
  int nm_uni =0;
  int nm_nuni;
  double len_uni;
  double b2net = 0;

  if ((set->axis == 0) && (set->dir == 0)) {
    divtype = calnum->divtyp_1;
    nm_uni = calnum->nmedge_uni_1;
    nm_nuni = calnum->nmedge_nuni_1;
    len_uni = calnum-> len_uni_1;
    b2net = b[2] - minsp->coords[minsp->nmcoords - 1];
  } else if ((set->axis == 0) && (set->dir == 1)) {
    divtype = calnum->divtyp_2;
    nm_uni = calnum->nmedge_uni_2;
    nm_nuni = calnum->nmedge_nuni_2;
    len_uni = calnum->len_uni_2;
    b2net = minsp->coords[0] - b[0];
  } else if ((set->axis == 1) && (set->dir == 0)) {
    divtype = calnum->divtyp_1;
    nm_uni = calnum->nmedge_uni_1;
    nm_nuni = calnum->nmedge_nuni_1;
    len_uni = calnum->len_uni_1;
    b2net = b[3] - minsp->coords[minsp->nmcoords - 1];
  } else if ((set->axis == 1) && (set->dir == 1)) {
    divtype = calnum->divtyp_2;
    nm_uni = calnum->nmedge_uni_2;
    nm_nuni = calnum->nmedge_nuni_2;
    len_uni = calnum->len_uni_2;
    b2net = minsp->coords[0] - b[1];
  }
 
  int index;
  if (divtype > 0) {
    if(set->dir == 1) { 
      for (int i = 0; i < nm_uni; i++) {
        mg->l_mesh[mg->num_mesh] = len_uni;
        mg->num_mesh++;
      }
    } else {
      for (int i = 0; i < NUMUNI; i++) {
        mg->l_mesh[mg->num_mesh] = minsp->mins;
        mg->num_mesh++;
      }
    }

    for (int i = 0; i < nm_nuni; i++) {
      if (set->dir == 1) index = nm_nuni - i -1;
      else index = i;
      
      mg->l_mesh[mg->num_mesh] = pow(set->cr[divtype - 1], (double)index) * minsp->mins;
      mg->num_mesh++;
    }

    if (set->dir == 0) {
      for (int i = 0; i < nm_uni; i++) {
        mg->l_mesh[mg->num_mesh] = len_uni;
        mg->num_mesh++;
      }
    } else {
      for (int i = 0; i < NUMUNI; i++) {
        mg->l_mesh[mg->num_mesh] = minsp->mins;
        mg->num_mesh++;
      }
    }
  } else {
    len_uni = b2net / nm_uni;
    for (int i = 0; i < nm_uni; i++) {
      mg->l_mesh[mg->num_mesh] = len_uni;
      mg->num_mesh++;
    }
  }
  mg->pos_coords[mg->numpos] = mg->num_mesh;
  mg->numpos++;
}

/**
 * cal_lmesh_mid -Calculates the mesh length for the middle position
 * @mg: A structure that records information such as mesh length
 * @calnum: A structure that stores information such as the number of grids
 * @minsp: A structure that stores information such as minimum spacing
 * @set: A structure that stores direction information
 * note : cal_lmesh_edge() will calculates the mesh length at edge positions
 */
void cal_lmesh_mid(mesh *mg, calnum_grid *calnum, min_space *minsp, setting *set) {
  double l_uni;
  int index;
  for (int i = 0; i < minsp->nmcoords - 1; i++) {
    if (calnum->divtyp_mid[i] > 0) {
      for (int j = 0; j < NUMUNI; j++) {
        mg->l_mesh[mg->num_mesh] = minsp->mins;
        mg->num_mesh++;
      }
      for (int j = 0; j <calnum->nmid_nuni[i]; j++) {
        mg->l_mesh[mg->num_mesh] = pow(set->cr[calnum->divtyp_mid[i] - 1], j ) * minsp->mins;
        mg->num_mesh++;
      }
      for (int j = 0; j < calnum->nmid_uni[i]; j++) {
        mg->l_mesh[mg->num_mesh] = calnum->lmid_uni[i];
        mg->num_mesh++;
      }
      for (int j = 0; j < calnum->nmid_nuni[i]; j++) {
        index = calnum->nmid_nuni[i] - j - 1;
        mg->l_mesh[mg->num_mesh] = pow(set->cr[calnum->divtyp_mid[i] - 1], (double)index) * minsp->mins;
        mg->num_mesh++;
      }
      for (int i = 0; i < NUMUNI; i++) {
        mg->l_mesh[mg->num_mesh] = minsp->mins;
        mg->num_mesh++;
      }
      mg->pos_coords[mg->numpos] = mg->num_mesh;
      mg->numpos++;
    } else {
      l_uni = (minsp->coords[i + 1] - minsp->coords[i]) / calnum->nmid_uni[i];
      for (int k = 0; k < calnum->nmid_uni[i]; k++) {
        mg->l_mesh[mg->num_mesh] = l_uni;
        mg->num_mesh++;
      }
      mg->pos_coords[mg->numpos] = mg->num_mesh;
      mg->numpos++;
    }
  }
}

/**
 * mesh_gen -generate a meshes
 * @b: boundary
 * @minsp: A structure that stores information such as minimum spacing
 * @set: A structure that stores direction information
 * note : mesh_gen() Returns information such as mesh length
 */
mesh *mesh_gen(double *b, min_space *minsp, setting *set) {
  mesh *mgp = NULL;
  mgp = (mesh *) malloc(sizeof(mesh));
  if (mgp == NULL) {
    exit(EXIT_FAILURE);
  }

  struct calnum_grid calnum;
  cal_nmesh(b, minsp, set, &calnum);

  mgp->l_mesh = (double *) malloc((calnum.nm + 1) * sizeof(double));
  if (mgp->l_mesh == NULL) {
    printf("failed to create memory ofmg->l_mesh\n");
    exit(EXIT_FAILURE);
  }
  mgp->pos_coords = NULL;
  mgp->pos_coords = (int *) malloc(calnum.nm * sizeof(int));
  if (mgp->pos_coords == NULL) { 
    printf("failed to create memory of mg->pos_coords\n");
    exit(EXIT_FAILURE);
  }
  set->dir = 1;
  mgp->num_mesh = 0;
  mgp->numpos = 0;
  cal_lmesh_edge(b, mgp, &calnum, minsp, set);
  cal_lmesh_mid(mgp, &calnum, minsp, set);
  
  free(calnum.divtyp_mid);
  free(calnum.nmid_uni);
  free(calnum.nmid_nuni);
  free(calnum.lmid_uni);

  set->dir = 0;
  cal_lmesh_edge(b, mgp, &calnum, minsp, set);

  return mgp;
}
