#include "solver.h"

int klusolver(int n, int *Ap, int *Ai, double *Ax, double *x) {
  klu_common c;
  klu_symbolic *sy;
  klu_numeric *nu;

  // set defaults
  klu_defaults(&c);

  if (!Ap || !Ai || !Ax || !x)
  {
    goto PrintError;
  }

  if (!(sy = klu_analyze(n, Ap, Ai, &c)))
  {
    goto PrintError;
  }

  if (!(nu = klu_factor(Ap, Ai, Ax, sy, &c)))
  {
    klu_free_symbolic(&sy, &c);
    goto PrintError;
  }

  // solver Ax=b, A is n-by-n and b is size-n and stored in x
  klu_solve(sy, nu, n, 1, x, &c);

  // free & return sucess
  klu_free_numeric(&nu, &c);
  klu_free_symbolic(&sy, &c);
  return 1;

PrintError:
  printf("error occurred!\n");
  return 0;
}

cholmod_sparse *cholmod_generate(
  int nn,
  struct mesh *mg_x,
  struct mesh *mg_y,
  int **ngc,
  cholmod_common *cc
) {
  int nrows, ncols, nnz;
  nrows = ncols = (mg_x->num_mesh + 1) * (mg_y->num_mesh + 1);
  nnz = (mg_x->num_mesh + 1) * (mg_y->num_mesh + 1) * 5;
  cholmod_triplet *T = cholmod_allocate_triplet(nrows, ncols, nnz, 0, CHOLMOD_REAL, cc);
  T->nnz = nnz;
  int *Ti = (int *)T->i;
  int *Tj = (int *)T->j;
  double *Tx = (double *)T->x;
  double x_fn, x_lt, y_fn, y_lt;
  int k = 0;
  int flag = 1;

  for (int j = 0; j <= mg_y->num_mesh; j++) {
    for (int i = 0; i <= mg_x->num_mesh; i++) {
      if ((i == 0) || (j == 0) || (i == mg_x->num_mesh) || (j == mg_y->num_mesh)) {
        Ti[k] = j * (mg_x->num_mesh + 1) + i;
        Tj[k] = j * (mg_x->num_mesh + 1) + i;
        Tx[k] = 1;
        flag = 0;
      }
      for (int q = 0; q < nn; q++) {
        if ((i <= ngc[q][2]) && (i >= ngc[q][0]) && (j <= ngc[q][3]) && (j >= ngc[q][1])) {
          Ti[k] = j * (mg_x->num_mesh + 1) + i;
          Tj[k] = j * (mg_x->num_mesh + 1) + i;
          Tx[k] = 1;
          flag = 0;
        }
      }
      if (flag == 1) {
        x_fn = mg_x->l_mesh[i];
        x_lt = mg_x->l_mesh[i - 1];
        y_fn = mg_y->l_mesh[j];
        y_lt = mg_y->l_mesh[j - 1];

        Ti[k] = j * (mg_x->num_mesh + 1) + i;
        Tj[k] = j * (mg_x->num_mesh + 1) + i + 1;
        Tx[k] = (y_fn + y_lt) / x_fn;
        k++;

        Ti[k] = j * (mg_x->num_mesh + 1) + i;
        Tj[k] = j * (mg_x->num_mesh + 1) + i - 1;
        Tx[k] = (y_fn + y_lt) / x_lt;
        k++;

        Ti[k] = j * (mg_x->num_mesh + 1) + i;
        Tj[k] = j * (mg_x->num_mesh + 1) + i;
        Tx[k] = -(y_fn + y_lt) / x_fn - (y_fn + y_lt) / x_lt - (x_fn + x_lt) / y_fn - (x_fn + x_lt) / y_lt;
        k++;

        Ti[k] = j * (mg_x->num_mesh + 1) + i;
        Tj[k] = (j + 1) * (mg_x->num_mesh + 1) + i;
        Tx[k] = (x_fn + x_lt) / y_fn;
        k++;

        Ti[k] = j * (mg_x->num_mesh + 1) + i;
        Tj[k] = (j - 1) * (mg_x->num_mesh + 1) + i;
        Tx[k] = (x_fn + x_lt) / y_lt;
      }
      k++;
      flag = 1;
    } 
  }
  // convert triplet to sparse matrix
  cholmod_sparse *A = cholmod_triplet_to_sparse(T, k, cc);

  // free the triplet
  cholmod_free_triplet(&T, cc);

  // drop zeros in matrix A
  cholmod_drop(0.0, A, cc);
  return A;
}

double *rhs_generate(
    int n,
    lf *f,
    struct mesh *mg_x,
    int **ngc
) {
  // x has the same size as the matrix A
  double *x = NULL;
  x = (double *) calloc(sizeof(double), n);
  if (x == NULL) {
    printf("failed to create the rhs\n");
    exit(EXIT_FAILURE);
  }

  if (f->ns[0] != 1) {
    for (int u = 0; u < f->ns[0]; u++) {
      for (int i = ngc[u][0]; i <= ngc[u][2]; i++) {
        for (int j = ngc[u][1]; j <= ngc[u][3]; j++) {
          x[j * (mg_x->num_mesh + 1) + i] = 1;
        }
      }
    }
  } else {
    for (int i = ngc[0][0]; i <= ngc[0][2]; i++){
      for (int j = ngc[0][1]; j <= ngc[0][3]; j++){
        x[j * (mg_x->num_mesh + 1) + i] = 1;
      }
    }
  }

  return x; 
}
