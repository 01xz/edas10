#include "cal_capacitor.h"

/**
 * cal_capacitor - Calculated capacitance
 * @f: A structure containing information such as the number of conductors
 * @ngc: coordinates of the net with respect to the mesh
 * @x: The value of the potential
 * @mesh_x: A structure that records mesh information along the X axis
 * @mesh_y: A structure that records mesh information along the Y axis
 * note : cal_capacitor() will return capacitance value
 */
double *cal_capacitor(lf *f, int **ngc, double *x, mesh *mesh_x, mesh *mesh_y) {

  int** rect = rect_pos2coord(f, ngc);

  int lenz = 2;
  int leng = 1;

  int max_row = 0;
  int max_col = 0;
  for (int u = 0; u < f->nn; u++) {
    if (max_row < rect[u][3] - rect[u][1] + 2 * lenz + 1)
      max_row = rect[u][3] - rect[u][1] + 2 * lenz + 1;
    if (max_col < rect[u][2] - rect[u][0] + 2 * lenz + 1)
      max_col = rect[u][2] - rect[u][0] + 2 * lenz + 1;
  }
 
  double** z = (double**)malloc((max_row + 1) * sizeof(double*));
  if (z == NULL) {
    printf("Failed to allocate memory\n");
  } else {
    for (int i = 0; i < (max_row + 1); ++i) {
      z[i] = (double*)malloc((max_col + 1) * sizeof(double));
      if (z[i] == NULL)
        printf("Failed to allocate memory\n");
    }
  }

  double* dx1 = (double*)malloc((max_row + 1 - 2 * leng + 1) * sizeof(double));
  if (dx1 == NULL)
    printf("Failed to allocate memory\n");
  double* dx2 = (double*)malloc((max_row + 1 - 2 * leng + 1) * sizeof(double));
  if (dx2 == NULL)
    printf("Failed to allocate memory\n");
  double* dy1 = (double*)malloc((max_col + 1 - 2 * leng + 1) * sizeof(double));
  if (dy1 == NULL)
    printf("Failed to allocate memory\n");
  double* dy2 = (double*)malloc((max_col + 1 - 2 * leng + 1) * sizeof(double));
  if (dy2 == NULL)
    printf("Failed to allocate memory\n");

  double q = 0;
  double q1 = 0;
  double p = 0;
  double p1 = 0;
  double* C = (double*)malloc(f->nn * sizeof(double));
  
  if (C == NULL)
    printf("Failed to allocate memory\n");

    
  for (int u = 0; u < f->nn; u++)
  {
    for (int j = 0; j < rect[u][3] - rect[u][1] + 2 * lenz + 1; j++)
    {
      for (int i = 0; i < rect[u][2] - rect[u][0] + 2 * lenz + 1; i++) 
      {
        z[j][i] = x[(int)((j + rect[u][1] - lenz - 2 + 2) * (mesh_x->num_mesh + 1) + i + rect[u][0] - lenz - 1 + 1)];
      }

    }

    for (int j = leng; j < rect[u][3] - rect[u][1] + 2 * lenz + 1 - leng; j++) {
      dx1[j] = (z[j][leng + 2 - 1] - z[j][leng - 1]) / (mesh_x->l_mesh[rect[u][0] - 1] + mesh_x->l_mesh[rect[u][0] - 2]);
      dx2[j] = -(z[j][rect[u][2] - (rect[u][0] + 1) + 2 * lenz + 1 - leng - 1] - z[j][rect[u][2] - (rect[u][0] + 1) + 2 * lenz + 1 - leng + 1]) / (mesh_x->l_mesh[rect[u][2] + 1] + mesh_x->l_mesh[rect[u][2]]);
    }


    for (int i = leng; i < rect[u][2] - rect[u][0] + 2 * lenz + 1 - leng; i++) {
      dy1[i] = (z[leng + 2 - 1][i] - z[leng - 1][i]) / (mesh_y->l_mesh[rect[u][1] - 1] + mesh_y->l_mesh[rect[u][1] - 2]);
      dy2[i] = -(z[rect[u][3] - rect[u][1] + 2 * lenz + 1 - leng - 1 - 1][i] - z[rect[u][3] - rect[u][1] + 2 * lenz + 1 - leng + 1 - 1][i]) / (mesh_y->l_mesh[rect[u][3]] + mesh_y->l_mesh[rect[u][3] + 1]);
    }
    
    for (int v = leng; v < rect[u][2] - rect[u][0] + 2 * lenz + 1 - leng - 1; v++) {
      p = p + mesh_x->l_mesh[(v + rect[u][0] - 1 - lenz + 1)] * (fabs(dy1[v]) + fabs(dy1[v + 1])) / 2;
    }

    for (int v = leng; v < rect[u][2] - rect[u][0] + 2 * lenz + 1 - leng - 1; v++) {
      q = q + mesh_x->l_mesh[v + rect[u][0] - 1 - lenz + 1] * (fabs(dy2[v]) + fabs(dy2[v + 1])) / 2;
    }

    for (int v = leng; v < rect[u][3] - rect[u][1] + 2 * lenz + 1 - leng - 1; v++) {
      q1 = q1 +mesh_y->l_mesh[v + rect[u][1] - 1 - lenz + 1] * (fabs(dx1[v]) + fabs(dx1[v + 1])) / 2;
    }

    for (int v = leng; v < rect[u][3] - rect[u][1] + 2 * lenz + 1 - leng - 1; v++) {
      p1 = p1 +mesh_y->l_mesh[v + rect[u][1] - 1 - lenz + 1] * (fabs(dx2[v]) + fabs(dx2[v + 1])) / 2;
    }

    C[u] = f->d * 8.854 * (p + q + p1 + q1) / 1000;
 
    q = 0;
    p = 0;
    q1 = 0;
    p1 = 0;
  }

  for (int i = 0; i < f->nn; i++)
  {
    free(rect[i]);
  }

  free(rect);
  free(z);
  free(dx1);
  free(dx2);
  free(dy1);
  free(dy2);

  return C;
}
