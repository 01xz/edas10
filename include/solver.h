#ifndef _SOLVER_H
#define _SOLVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cholmod.h>
#include <klu.h>

#include "types.h"
#include "constants.h"

int klusolver(
  int     n,
  int    *Ap,
  int    *Ai,
  double *Ax,
  double *x
);

cholmod_sparse *cholmod_generate(
  int nn,
  struct mesh *mg_x,
  struct mesh *mg_y,
  int **ngc,
  cholmod_common * cc
);

double *rhs_generate(
  int n,
  lf * f,
  struct mesh *mg_x,
  int **ngc
);

#ifdef __cplusplus
}
#endif

#endif

