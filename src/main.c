#include "main.h"

int main(int argc, char **argv) {

#ifdef DEBUG
  // measure the main running time and solver running time
  long start_usec, end_usec;
  long solver_start_usec, solver_end_usec;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  start_usec = tv.tv_sec * 1000000 + tv.tv_usec;
#endif

#ifdef DEBUG
  int ch = 0;
  FILE *f_status = fopen("/proc/self/status", "r");
#endif

  // the input path index and the output path index
  int ipind = 0, opind = 0;
  FILE *fp = NULL;

  // options parsing related
  int opt;
  int opt_idx;
  char *opstr = "i:o:";
  const struct option loptstr[] = {
    {"in", required_argument, NULL, 'i'},
    {"out", required_argument, NULL, 'o'}
  };

  // quit if using the wrong options
  if (argc != 5) {
    printf("Usage: fieldsolver2d -in input.data –out result.out\n");
    exit(EXIT_FAILURE);
  }

  // options parsing
  while ((opt = getopt_long_only(argc, argv, opstr, loptstr, &opt_idx)) != -1) {
    if (opt == 'i') {
      ipind = optind - 1;
    } else {
      opind = optind - 1;
    }
  }

  // open the input file
  if ((fp = fopen(argv[ipind], "r")) == NULL) {
    printf("failed to open the input data!\n");
    exit(EXIT_FAILURE);
  }

  // get the field information
  lf *field = get_lf(fp);
  fclose(fp);

  struct setting set = {
    0,
    0,
    {0.6, 0.5, 0.4},  //pnuni
    {1.2, 1.4, 1.8}   //cr
  };

  // calculate minimum space
  set.axis = 0;
  min_space *ms_x = min_space_gen(field, set.axis);
  set.axis = 1;
  min_space *ms_y = min_space_gen(field, set.axis);

  // calculate space between each grid
  set.axis = 0;
  mesh *mg_x = mesh_gen(field->bp, ms_x, &set);
  set.axis = 1;
  mesh *mg_y = mesh_gen(field->bp, ms_y, &set);
  
  // calculate coordinates of nets
  net_pos net_gc = {
    ms_x->mins,
    ms_y->mins,
    ms_x->nmcoords,
    ms_y->nmcoords,
    ms_x->coords,
    ms_y->coords,
    mg_x->pos_coords,
    mg_y->pos_coords,
    NULL
  };
  net_pos2coords(field, &net_gc);

  free(ms_x);
  free(ms_y);

  // A common struct that cholmod always needs
  cholmod_common c;

  // start cholmod
  cholmod_start(&c);

  // generate the matrix A

  cholmod_sparse *A = cholmod_generate(field->nr, mg_x, mg_y, net_gc.ngc, &c);

  // generate the right hand side b
  double *x = rhs_generate(A->nrow, field, mg_x, net_gc.ngc);

#ifdef DEBUG
  gettimeofday(&tv, NULL);
  solver_start_usec = tv.tv_sec * 1000000 + tv.tv_usec;
#endif

  // solve Ax=b
  if (A && x) {
    // solve Ax=b using klu
    klusolver(A->nrow, A->p, A->i, A->x, x);
    cholmod_free_sparse(&A, &c);
  }

#ifdef DEBUG
  gettimeofday(&tv, NULL);
  solver_end_usec = tv.tv_sec * 1000000 + tv.tv_usec;
#endif

  // calculate capacitor
  double *cap = cal_capacitor(field, net_gc.ngc, x, mg_x, mg_y);
  free(mg_x);
  free(mg_y);

  free(x);

  cholmod_finish(&c);

  // print to file
  if ((fp = fopen(argv[opind], "w")) == NULL) {
    printf("Failed to create the output data!\n");
    exit(EXIT_FAILURE);
  }

  // output data test
  //fprintf(fp, "# length = 1um\n");
  //fprintf(fp, "# master = net0\n");
  for (int i = 0; i < (field->nn); i++) {
    fprintf(fp, "%*s" NET_NAME "%d", 10, "", i);
  }
  fprintf(fp, "\n " INIT_NET_NAME ":");
  for (int i = 0; i < (field->nn); i++) {
    fprintf(fp, "%*s%gfF", i ? 3 : 1, "", cap[i]);
  }
  fprintf(fp, "\n");

  fclose(fp);

  free(cap);
  field = free_lf(field);

#ifdef DEBUG
  gettimeofday(&tv, NULL);
  end_usec = tv.tv_sec * 1000000 + tv.tv_usec;
  printf("\nmain running time (s): %g\n", (double)(end_usec - start_usec) / 1000000);
  printf("solver running time (s): %g\n", (double)(solver_end_usec - solver_start_usec) / 1000000);
#endif

#ifdef DEBUG
  while ((ch = getc(f_status)) != EOF) {
    printf("%c", ch);
  }
  fclose(f_status);
#endif

  return 0;
}
