#include "rect_pos2coords.h"
#include "net_pos2coords.h"

/**
 * rect_pos2coord -Calculates the coordinates of the rectangle with respect to the mesh
 * @f: A structure containing information such as the number of conductors
 * @ngc: coordinates of the net with respect to the mesh
 * note : rect_pos2coord() will returns the coordinate information of the rectangle
 */
int **rect_pos2coord(lf *f, int **ngc) {
 
  int cnt = 0;
  int **rect = NULL;
  rect = (int **) malloc(sizeof(int *) * f->nn);
  if (NULL == rect) {
    printf("Failed to create memory of nets_coord \n");
    exit(EXIT_FAILURE);
  } else {
    for (int i = 0; i < f->nn; i++) {
      rect[i] = (int *) malloc(sizeof(int) * RECT_SIZE);
      if (rect[i] == NULL) { 
        printf("Failed to create memory of nets_coord[i]\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  for (int i = 0; i < f->nn; i++) {
    if (f->ns[i] == 1) {
      rect[i][0] = ngc[cnt][0];
      rect[i][1] = ngc[cnt][1];
      rect[i][2] = ngc[cnt][2];
      rect[i][3] = ngc[cnt][3];
      cnt++;
    } else {
      rect[i][0] = ngc[cnt][0];
      rect[i][2] = ngc[cnt][2];
      for(int j =0;j<f->ns[i]-1;j++) {
        if(ngc[cnt+j+1][0]<rect[i][0])
          rect[i][0] = ngc[cnt+j+1][0];
        if(ngc[cnt+j+1][2]>rect[i][2])
          rect[i][2] = ngc[cnt+j+1][2];
      }
      rect[i][1] = ngc[cnt][1];
      rect[i][3] = ngc[cnt+f->ns[i]-1][3];
      cnt = cnt +f->ns[i];
    }
  }

  for (int i = 0; i < f->nn; i++) {
    free(ngc[i]);
  }
  free(ngc);
  return rect;
}
