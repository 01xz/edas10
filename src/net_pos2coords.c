#include "net_pos2coords.h"

/**
 * net_coords -Calculates the coordinates of the net with respect to the mesh
 * @f: A structure containing information such as the number of conductors
 * @net_pc: A structure containing information such as the position of conductors
 * note : net_coords() will calculates the coordinates of the net with respect to the mesh
 */
void net_pos2coords(lf *f, net_pos *net_pc) {
  net_pc->ngc = NULL;
  net_pc->ngc = (int **) malloc(sizeof(int *) * f->nr);
  if (net_pc->ngc == NULL) {
    printf("Failed to allocate memory\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < f->nr; ++i) {
    net_pc->ngc[i] = (int *)malloc(4 * sizeof(int));
    if (net_pc->ngc[i] == NULL) {
      printf("Failed to allocate memory\n");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < f->nr; i++) {
    for (int j = 0; j <= (net_pc->nmcoords_x - 1); j++) {
      if ((f->rp[i * 4 + 0] > (net_pc->coords_x[j] - net_pc->ms_x)) && (f->rp[i * 4 + 0] < net_pc->coords_x[j] + net_pc->ms_x))
        net_pc->ngc[i][0] = net_pc->pos_coords_x[j];
      if ((f->rp[i * 4 + 2] > (net_pc->coords_x[j] - net_pc->ms_x)) && (f->rp[i * 4 + 2] < net_pc->coords_x[j] + net_pc->ms_x))
        net_pc->ngc[i][2] = net_pc->pos_coords_x[j];
    }
  }

  for (int i = 0; i < f->nr; i++) {
    for (int j = 0; j <= (net_pc->nmcoords_y - 1); j++) {
      if ((f->rp[i * 4 + 1] > (net_pc->coords_y[j] - net_pc->ms_y)) && (f->rp[i * 4 + 1] < net_pc->coords_y[j] + net_pc->ms_y))
        net_pc->ngc[i][1] = net_pc->pos_coords_y[j];
      if ((f->rp[i * 4 + 3] > (net_pc->coords_y[j] - net_pc->ms_y)) && (f->rp[i * 4 + 3] < net_pc->coords_y[j] + net_pc->ms_y))
        net_pc->ngc[i][3] = net_pc->pos_coords_y[j];
    }
  }

  free(net_pc->coords_x);
  free(net_pc->coords_y);
  free(net_pc->pos_coords_x);
  free(net_pc->pos_coords_y);
}
