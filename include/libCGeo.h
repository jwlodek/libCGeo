/**
 * This is the main header file for libCGeo. It contains all definitions of data structures used by all of the
 * library, in addition to all function definitions for the library.
 *
 * Author: Jakub Wlodek
 * Created on: 5-Feb-2019
 * Last modified: 5-Feb-2019
 *
 * Copyright (c): MIT 2019 Jakub Wlodek
 */


#ifndef LIBCGEO_H
#define LIBCGEO_H



//----------------------------------------------------------------
// Enums and Types
//----------------------------------------------------------------

/** Enum that specifies degree of precision.
 * libCGeo supports treating points with integer precision as well as floating point precision
 */
typedef enum CG_DTYP {
    CG_INT,   /**< Integer precision */
    CG_FLOAT  /**< Floating point precision */
} CGType_t;


/** Enum that specifies whether the point is in 2D or 3D space.
 * libCGeo supports 2D cartesian points and 3D points, though not all functions support both.
 */
typedef enum CG_DIMS {
    CG_2D,  /**< Two Dimensional coordinates */
    CG_3D   /**< Three Dimensional coordinates */
} CGDims_t;


/**
 * Enum that specifies the type of turn between three points.
 */
typedef enum CG_TURN {
    CG_TURN_LEFT,   /**< Left turn */
    CG_TURN_RIGHT,  /**< Right turn */
    CG_TURN_INLINE  /**< Inline turn, meaning that the three points are colinear */
} CGTurn_t;

typedef enum CG_ERROR {
    CG_SUCCESS              = 0,
    CG_INVALID_TYPE         = -1,
    CG_INVALID_DIMS         = -2,
    CG_POINTS_TOO_FEW       = -3
} CGError_t;

//----------------------------------------------------------------
// Data Structures
//----------------------------------------------------------------


/** Struct for point in cartesian plane with floating-point coordinates.
 * Contains x, y, and z coordinates. In 2D points, dims should be set to CG_2D, and the zcoord will
 * not be used.
 */
typedef struct CG_Point {
    double xcoord;  /**< Double x-coordinate */
    double ycoord;  /**< Double y-coordinate */
    double zcoord;  /**< Double z-coordinate, only used in CG_3D */
    CGDims_t dims;   /**< Dimensions of point (does it use 2 or 3) */
    CGType_t type;   /**< Data type used by point, int or double */
} CGPoint_t;


typedef struct CG_PointSet {
    CGPoint_t* points;
    int num_points;
} CGPointSet_t;


//----------------------------------------------------------------
// Function Definitions - Common
//----------------------------------------------------------------




//----------------------------------------------------------------
// Function Definitions - Graham-Scan
//----------------------------------------------------------------



//----------------------------------------------------------------
// Function Definitions - common
//----------------------------------------------------------------


//----------------------------------------------------------------
// Function Definitions - common
//----------------------------------------------------------------





#endif
