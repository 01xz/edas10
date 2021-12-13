#ifndef _TYPES_H
#define _TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * lf - layout information generated from input.data
 * @nn: the number of nets
 * @nr: the number of rectangles
 * @d:  the dielectric
 * @bp: the boundary of field, size 4
 * @rp: the rectangles position list, size nr * 4
 * @ns: the nets shape indicator, size nn
 */
typedef struct layout_field {
  int     nn; 
  int     nr;
  double  d;
  double *bp;
  double *rp;
  int    *ns;
} lf;

/**
 * calnum_grid - Calculate the number of grids
 * @divtyp_1 : Store  the rightmost or topmost division type
 * @divtyp_2:  Store  the leftmost or bottommost division type
 * @divtyp_mid: Stores the type of division in the middle position
 * @nm: The grid number
 * @nmedge_uni_1: Number of uniform divided meshes at edge positions(the rightmost or topmost)
 * @nmedge_nuni_1: Number of nonuniform divided meshes at edge positions(the rightmost or topmost)
 * @nmedge_uni_2: Number of uniform divided meshes at edge positions( leftmost or bottommost)
 * @nmedge_nuni_2: Number of nonuniform divided meshes at edge positions(leftmost or bottommost)
 * @len_uni_1: Edge position evenly divided mesh length (the rightmost or topmost)
 * @len_uni_2: Edge position evenly divided mesh length (leftmost or bottommost)
 * @nmid_nuni: The number of non-uniform meshes in the middle position
 * @nmid_nuni: The number of uniform meshes in the middle position
 * @lmid_uni: Middle position uniform meshed length
 */
typedef struct calnum_grid {
  int  divtyp_1;
  int  divtyp_2;   
  int *divtyp_mid;
  int  nm;      

  int    nmedge_uni_1;
  int    nmedge_nuni_1;
  int    nmedge_uni_2;
  int    nmedge_nuni_2;
  double len_uni_1;
  double len_uni_2;

  int*    nmid_nuni;
  int*    nmid_uni;
  double* lmid_uni;

} calnum_grid;

/**
 * mesh - Log information about the mesh
 * @l_mesh: Length of mesh
 * @num_mesh: Number of mesh
 * @pos_coord:  Position of coordinates
 * @numpos: Number of positions
 */
typedef struct mesh {
  double *l_mesh; 
  int     num_mesh;  
  int    *pos_coords; 
  int     numpos; 
} mesh;

/**
 * min_space - A structure that records information such as minimum spacing
 * @coords: The coordinates projected by the conductor onto the coordinate axis
 * @nmcoords: nmcoord
 * @mins:  Minimum spacing of the mesh
 */
typedef struct min_space {
  double *coords; 
  int     nmcoords;   
  double  mins;      
} min_space;

/**
 * net_pos - Coordinates of the net with respect to the grid
 * @ms_x: The minimum distance in the x direction
 * @ms_y: The minimum distance in the y direction
 * @nmcoords_x: The number of coordinates in the x direction
 * @nmcoords_y: The number of coordinates in the y direction
 * @coords_x: The projected coordinates in the x direction
 * @coords_y: The projected coordinates in the y direction
 * @pos_coords_x:
 * @pos_coords_y:
 * @ngc: Store net grid coordinates
 */
typedef struct net_pos {
  double  ms_x;
  double  ms_y;
  int     nmcoords_x;//nm_del_x
  int     nmcoords_y;
  double* coords_x;
  double* coords_y;
  int*    pos_coords_x;
  int*    pos_coords_y;
  int**   ngc;
} net_pos;

/**
 * setting - Recording initial Settings
 * @axis: Specify x direction or y direction
 * @dir:  Coordinate with Axis to specify left, right, up, down
 * @pnuni_r: The proportion of the non-uniform region
 * @cr: A common ratio that is not uniformly divided
 */
typedef struct setting {
  int axis;
  int dir;
  double pnuni_r[3];  
  double cr[3]; 
} setting;

#ifdef __cplusplus
}
#endif

#endif

