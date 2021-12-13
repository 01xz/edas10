#ifndef _CAL_CAPACITOR_H
#define _CAL_CAPACITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "constants.h"
#include "rect_pos2coords.h"

double *cal_capacitor(lf *f, int **ngc, double *x, mesh *mesh_x, mesh *mesh_y);

#endif