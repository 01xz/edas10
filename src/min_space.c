#include "min_space.h"

/**
 * quick_sort - Sort the elements of an array
 * @arr: The array to sort
 * @low: The lowest index of a sorted array
 * @high: The lowest index of a sorted array
 * note : quick_sort() will rort the elements of an array
 */
void quick_sort(double *arr, int low, int high){
  if (low < high){
    int i = low;
    int j = high;
    double k = arr[low];
    while (i < j) {
      while (i < j && arr[j] >= k)
        j--;
      if (i < j)
        arr[i++] = arr[j];
      while (i < j && arr[i] < k)
        i++;
      if (i < j)
        arr[j--] = arr[i];
    }
    arr[i] = k;
    quick_sort(arr, low, i - 1);
    quick_sort(arr, i + 1, high);
  }
}

/**
 * ex_coord -Extract the coordinates of the net
 * @f: A structure containing information such as the number of conductors
 * @minsp: A structure that stores information such as minimum spacing
 * @axis: Specify direction of operation
 */
void ex_coord(lf *f,  min_space *minsp, int axis) {
  minsp->coords = NULL;
  minsp->coords = (double *)malloc(f->nr * 2 * sizeof(double));
  if (minsp->coords == NULL) {
    printf("Failed to create memory of minsp->coords \n");
    exit(EXIT_FAILURE);
  }
  int cnt = 0;
  int rank1, rank2;

  for (int i = 0; i < f->nr; i++) {
    if (axis == 0) {
      rank1 = i * 4 + 0;
      rank2 = i * 4 + 2;
    } else{
      rank1 = i * 4 + 1;
      rank2 = i * 4 + 3;
    }
    minsp->coords[cnt] = f->rp[rank1];
    cnt++;
    minsp->coords[cnt] = f->rp[rank2];
    cnt++;
  }
  quick_sort(minsp->coords, 0, cnt - 1);
}

/**
 * cal_minside -Calculate the minimum side length of the net
 * @f: A structure containing information such as the number of net
 * @coord: Coordinate of net
 * @axis: Specify direction of operation
 * note : cal_minside() will returns the minimum side length of the net
 */
double cal_minside(lf *f, int axis) {
  int rank1, rank2;
  double tmpmin = 0;   
  double min_side = 0; 
  if (axis == 0)
    min_side = f->rp[2] - f->rp[0];
  else
    min_side = f->rp[3] - f->rp[1];

  for (int i = 0; i < f->nr; i++) {
    if (axis == 0) {
      rank1 = i * 4 + 0;
      rank2 = i * 4 + 2;
    } else {
      rank1 = i * 4 + 1;
      rank2 = i * 4 + 3;
    }
    tmpmin = f->rp[rank2] - f->rp[rank1];
    if (tmpmin < min_side)
      min_side = tmpmin;
  }
  return min_side;
}

/**
 * cal_mind -get the minimum difference between elements in position vector
 * @msp: A structure that stores information such as minimum spacing
 * @axis: Specify direction of operation
 * @b: boundary
 * note : this function only return the min_diff in single direction
 */
double cal_mind( min_space *msp, int axis, double b[]) {
  double min_d = msp->coords[1] - msp->coords[0];
  double tmpmin = 0;
  for (int i = 1; i < msp->nmcoords - 1; i++) {
    tmpmin = (msp->coords[i + 1] - msp->coords[i]);
    if (tmpmin < min_d)
      min_d = tmpmin;
  }

  double boundary1, boundary2;
  if (axis == 0) {
    boundary1 = b[0];
    boundary2 = b[2];
  } else {
    boundary1 = b[1];
    boundary2 = b[3];
  }

  if (msp->coords[0] - boundary1 < min_d)
    min_d = (msp->coords[0] - boundary1);

  if (boundary2 - msp->coords[msp->nmcoords - 1] < min_d)
    min_d = (boundary2 - msp->coords[msp->nmcoords - 1]);

  return min_d;
}

/**
 * del_same -removes the same element from the array
 * @minsp: A structure that stores information such as minimum spacing
 * @nr:Number of net
 */
void del_same(min_space *msp, int nr) {
  msp->nmcoords = 0;
  int sum = 1;
  for (int i = 1; i < 2 * nr - sum + 1;) {
    if (msp->coords[i - 1] == msp->coords[i]) {
      for (int j = i; j < 2 * nr - sum + 1; j++) {
        msp->coords[j - 1] = msp->coords[j];
      }
      sum++;
    } else {
      i++;
    }
  }
  msp->nmcoords = 2 * nr - sum + 1;
}


/**
 * min_space_gen -Minimum spacing to generate mesh
 * @f: A structure containing information such as the number of conductors
 * @axis: Specify direction of operation
 * note : cal_minside() will returns the minimum spacing of the mesh
 */
min_space *min_space_gen(lf *f, int axis) {
  min_space *msp = NULL;
  msp = (min_space *)malloc(sizeof(min_space));
  if (msp == NULL) {
    printf("failed to allocate memory for msp\n");
    exit(EXIT_FAILURE);
  }

  ex_coord(f, msp, axis);
  double min_side = cal_minside(f, axis);
  del_same(msp, f->nr);
  double min_d = cal_mind(msp, axis, f->bp);
  msp->mins = (min_side / 8 < min_d / 2) ? min_side / 8 : min_d / 2;

  return msp;
}
  