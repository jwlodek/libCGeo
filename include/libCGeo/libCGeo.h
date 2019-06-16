/********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2019 Jakub Wlodek
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

/**
 * This is the main header file for libCGeo. It contains all definitions of data structures used by all of the
 * library, in addition to all function definitions for the library.
 */


#ifndef LIBCGEO_H
#define LIBCGEO_H


// Tolerance to account for double precision roundoff errors
#define FLOAT_TOLERANCE 0.000001


// include files
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//----------------------------------------------------------------
// Enums and Types
//----------------------------------------------------------------


/**
 * Enum that specifies the type of turn between three points.
 * @ingroup ptrels
 */
typedef enum CG_TURN {
    CG_TURN_LEFT,   /**< Left turn */
    CG_TURN_RIGHT,  /**< Right turn */
    CG_TURN_INLINE,  /**< Inline turn, meaning that the three points are colinear */
} CGTurn_t;


/**
 * Enum that specifies the type of error encountered by libCGeo.
 * @ingroup diagnostics
 */
typedef enum CG_ERROR {
    CG_SUCCESS              = 0,    /**< Successful completion */
    CG_INVALID_TYPE         = -1,   /**< Invalid Point Type for operation */
    CG_POINTS_TOO_FEW       = -2,   /**< Not enough points in set to perform operation (Usually >3) */
    CG_INVALID_INPUT        = -3,   /**< Invalid or null input */
    CG_NO_FILE              = -4,   /**< File pointer received by funciton is NULL */
    CG_UNIMPLEMENTED        = -5,   /**< Function is not yet implemented */
} CGError_t;


/**
 * Enum that allows for selecting the amount of output information to print.
 * @ingroup diagnostics
 */
typedef enum CG_DESC_TYPE {
    CG_MIN                  = 0,    /**< Minimum output information */
    CG_VERBOSE              = 1,    /**< More detailed output information */
    CG_FULL                 = 2,    /**< All available output information */
} CGDescDetail_t;


/**
 * Enum for specifying Convex hull algorithm to use on point set.
 * @ingroup chull
 */
typedef enum CG_CONVEX_HULL {
    CG_GRAHAM_SCAN,     /**< Compute convex hull with Graham Scan */
} CGConvexHull_t;


typedef enum CG_COMPUTE_TYPE {
    CG_NO_DEGENERACY,   /**< Compute without accounting for degeneracy */
    CG_W_DEGENERACY,    /**< Compute while taking degenracies into account */
} CGCompute_t;


//----------------------------------------------------------------
// Data Structures
//----------------------------------------------------------------


/** 
 * Struct for point in cartesian plane with floating-point coordinates.
 * Contains x, y, and z coordinates. In 2D points, dims should be set to CG_2D, and the zcoord will
 * not be used.
 */
typedef struct CG_Point {
    double xcoord;          /**< Double x-coordinate */
    double ycoord;          /**< Double y-coordinate */
    double sort_val;        /**< Double value used for sorting points */
    char* sort_val_desc;    /**< Description of what measurement is stored in the sort_val */
} CGPoint_t;


/**
 * Struct representing a node containing a point in a doubly linked list.
 * Contains pointers to previous and next point node elements
 */
typedef struct CG_PointNode {
    CGPoint_t* point;               /**< Point stored in the node */
    struct CG_PointNode* next;      /**< Next Node in linked list */
    struct CG_PointNode* prev;      /**< Previous node in linked list */
} CGPointNode_t;


/**
 * Struct for storing set of points.
 * Contains pointer to array of points and a counter for the number of points
 */
typedef struct CG_PointSet {
    CGPointNode_t* head;        /**< Pointer to head node of Linked list of points */
    CGPointNode_t* tail;        /**< Pointer to the tail node of Linked list of points */
    int num_points;             /**< Count of number of points */
} CGPointSet_t;


//----------------------------------------------------------------
// Function Definitions - Common
//----------------------------------------------------------------


// Basic point set operations
CGPointSet_t* init_point_set();
CGError_t add_coords_to_set(CGPointSet_t* point_set, double xCoord, double yCoord);
CGError_t add_point_to_set(CGPointSet_t* point_set, CGPoint_t* point);
CGPoint_t* get_point_at_index(CGPointSet_t* point_set, int index);
CGError_t free_point_set(CGPointSet_t* point_set);

// reading / writing .csv files
CGError_t point_set_from_csv_file(CGPointSet_t* point_set, FILE* file_pointer);
CGError_t csv_file_from_point_set(CGPointSet_t* point_set, FILE* file_pointer);

// sorting points (uses merge-sort)
CGError_t sort_point_set(CGPointSet_t* point_set, CGPointSet_t* output_point_set);
CGError_t sort_points(CGPointNode_t** phead);
void split_lists(CGPointNode_t* head, CGPointNode_t** left_list, CGPointNode_t** right_list);
CGPointNode_t* merge_halves(CGPointNode_t* left_list, CGPointNode_t* right_list);

// Point operations and calculations
double distance_between(CGPoint_t* point_A, CGPoint_t* point_B);
CGPoint_t* find_lowest_point_in_set(CGPointSet_t* point_set);
double angle_between(CGPoint_t* initial_point, CGPoint_t* end_point);

// Other calculations
CGTurn_t find_turn_type(CGPoint_t* point_A, CGPoint_t* point_B, CGPoint_t* point_C);


//----------------------------------------------------------------
// Function Definitions - Diagnostics
//----------------------------------------------------------------


// error printing
void print_cg_error(CGError_t error, const char* function_name);

// point printing
void print_point_to_file(CGPoint_t* point, FILE* fp, CGDescDetail_t desc_detail);
void print_points(CGPointSet_t* point_set);
void print_points_to_file(CGPointSet_t* point_set, FILE* fp, CGDescDetail_t desc_detail);

// point set comparisons
int compare_point_sets(CGPointSet_t* point_set_A, CGPointSet_t* point_set_B);
int compare_points(CGPoint_t* point_A, CGPoint_t* point_B);

// random point generation for testing (ints only)
CGError_t generate_random_point_set(CGPointSet_t* point_set, int num_points);


//----------------------------------------------------------------
// Function Definitions - Convex Hull
//----------------------------------------------------------------


CGError_t compute_point_angles(CGPointSet_t* point_set);
CGError_t compute_graham_scan(CGPointSet_t* input_set, CGPointSet_t* output_set, CGCompute_t compute_type);
CGError_t remove_colinear_degeneracies(CGPointSet_t* input_set, CGPointSet_t* output_set);
CGError_t compute_convex_hull(CGPointSet_t* point_set, CGPointSet_t* output_set, CGConvexHull_t convex_hull_method, CGCompute_t compute_type);


//----------------------------------------------------------------
// Function Definitions - Triangulation
//----------------------------------------------------------------


//----------------------------------------------------------------
// Function Definitions - Sweeping Line
//----------------------------------------------------------------




#endif
