#ifndef _MESH_GEN_H_
#define _MESH_GEN_H_

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "constants.h"
#include "cal_num_grid.h"

void cal_lmesh_edge(double* b, mesh *mg, calnum_grid *calnum, min_space *minsp, setting *set);
void cal_lmesh_mid(mesh *mg, calnum_grid* calnum, min_space* minsp, setting *set);
mesh *mesh_gen(double *b, min_space *minsp, setting *set);

#endif
