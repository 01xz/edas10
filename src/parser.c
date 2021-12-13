#include "parser.h"

/**
 * get_lf - get the layout information from the input.data
 * @fp: the file pointer of the input.data
 *
 * get_lf will not close @fp, you may have to close @fp
 * manually
 */
lf * get_lf(FILE * fp) {
  char word[WORD_LEN];
  char nname[MAX_NET_NAME_LEN] = INIT_NET_NAME;

  lf * f = (lf *) malloc(sizeof(lf));
  if (!f) {
    printf("internal error: 1\n");
    goto PrintError;
  }

  // get number of nets and rectangles
  f->nn = 0, f->nr = 0;
  while (fscanf(fp, "%s", word) == 1) {
    if (!strcmp(word, nname)) {
      f->nn++;
      if (f->nn < MAX_NNETS) {
        sprintf(nname, NET_NAME "%d", f->nn);
      } else {
        printf("internal error 2\n");
        goto PrintError;
      }
    }
    if (!strcmp(word, NET_NAME)) {
      f->nr++;
    }
  }
  rewind(fp);

  f->bp = (double *) malloc(sizeof(double) * RECT_SIZE);
  f->rp = (double *) malloc(sizeof(double) * RECT_SIZE * f->nr);
  f->ns = (int *) calloc(f->nn, sizeof(int));

  if (!f->bp || !f->rp || !f->ns) {
    printf("internal error 3\n");
    goto PrintError;
  }

  // file parsing
  double * rp = f->rp;
  while (fscanf(fp, "%s", word) == 1) {
    if (!strcmp(word, "boundary")) {
      for (int i = 0; i < RECT_SIZE; i++) {
        fscanf(fp, "%lf", f->bp + i);
      }
      continue;
    }
    if (!strcmp(word, "dielectric")) {
      fscanf(fp, "%lf", &(f->d));
      continue;
    }
    if (!strcmp(word, NET_NAME)) {
      continue;
    }
    if (!strncmp(word, NET_NAME, NET_NAME_LEN)) {
      (f->ns)[atoi(word + NET_NAME_LEN)]++;
      for (int i = 0; i < RECT_SIZE; i++) {
        fscanf(fp, "%lf", rp + i);
      }
      rp += RECT_SIZE;
    }
  }

  return f;

PrintError:
  printf("error occurred in get_lf()\n");
  exit(EXIT_FAILURE);
}

/**
 * free_lf - free the lf pointer
 * @f: the lf pointer to be freed
 *
 * usage: f = free_lf(f)
 */
void * free_lf(lf * f) {
  if (f) {
    if (f->bp) {
      free(f->bp);
      f->bp = NULL;
    }
    if (f->rp) {
      free(f->rp);
      f->rp = NULL;
    }
    if (f->ns) {
      free(f->ns);
      f->ns = NULL;
    }
    free(f);
  }
  return NULL;
}
