#ifndef _CAL_NUM_GRID_H_
#define _CAL_NUM_GRID_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "constants.h"

double cal_b2net(double b[], min_space *minsp, setting *set);
void cal_nmesh_edge(double b[], min_space *minsp, calnum_grid *calnum, setting *set);
void cal_nmesh_mid(min_space *minsp, calnum_grid *calnum, setting *set);
void cal_nmesh(double b[], min_space *ms, setting *set, calnum_grid *calnum);

#endif