#ifndef __MIN_SPACE_H_
#define __MIN_SPACE_H_

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "constants.h"

min_space* min_space_gen(lf *f, int axis);
void quick_sort(double *arr, int low, int high);
void del_same(min_space *msp, int nr);
void ex_coord(lf *f,  min_space *minsp, int axis);
double cal_minside(lf* f, int axis);
double cal_mind( min_space *coords, int axis, double b[]);

#endif
