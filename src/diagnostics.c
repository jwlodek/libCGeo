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
 * @defgroup diagnostics Diagnostics
 * @breif Functions and enums that aid in analyzing error codes etc.
 */



// includes
#include <stdio.h>
#include <stdlib.h>
#include "libCGeo.h"


//----------------------------------------------------------------
// Printing error information
//----------------------------------------------------------------


/** @internal */
typedef struct CG_ERROR_MESSAGE {
    CGError_t error;
    const char* message;
} CGErrorMessage_t;


static const CGErrorMessage_t error_messages[] = {
    {CG_SUCCESS, "Success"},
    {CG_INVALID_TYPE  , "Invalid point type"},
    {CG_POINTS_TOO_FEW, "Not enough points"},
    {CG_INVALID_INPUT , "Invalid input"},
    {CG_NO_FILE, "File cannot be opened, or does not exist"},
    {CG_UNIMPLEMENTED, "Function has not yet been implemented"}
};


/**
 * Function for printing out error information for libCGeo.
 * @ingroup diagnostics
 * @param error libCGeo error code
 * @param function_name function in which error code occurred
 * @return void
 */
void print_cg_error(CGError_t error, const char* function_name){
    FILE* stream = stderr;
    const char* error_message;
    int i;
    for(i = 0; i < (int) sizeof(error_messages) / sizeof(error_messages[0]); i++){
        if(error_messages[i].error == error){
            error_message = error_messages[i].message;
            break;
        }
    }
    if(error_message == NULL) error_message = "Unknown error";
    fprintf(stream, "libCGeo ERROR in %s: %s\n", function_name, error_message);
}


//----------------------------------------------------------------
// Printing points
//----------------------------------------------------------------


/**
 * Simple function for printing out point information to file stream
 * @ingroup diagnostics
 * @param point pointer to CGPoint
 * @return The appropriate error code
 */
CGError_t print_point_to_file(CGPoint_t* point, FILE* fp){
    const char* function_name = "print_point_to_file";
    int result;
    if(point->type == CG_INT)
        result = fprintf(fp, "int, coords[x: %d, y: %d]\n", (int) point->xcoord, (int) point->ycoord);
    else if(point->type == CG_FLOAT)
        result = fprintf(fp, "float, coords[x: %lf, y: %lf]\n", point->xcoord, point->ycoord);
	if (result != 0) return CG_INVALID_INPUT;
	else return CG_SUCCESS;
}


/**
 * Simple function for printing out values in point set directly to stdout.
 * @ingroup diagnostics
 * @param point_set Input set of points
 * @return The appropriate error code
 */
CGError_t print_points(CGPointSet_t* point_set){
    CGError_t status = print_points_to_file(point_set, stdout);
    return status;
}


/**
 * Simple function for printing out values in point set into a file
 * @ingroup diagnostics
 * @param point_set Input set of points
 * @param fp File pointer of file in which to write
 * @return The appropriate error code
 */
CGError_t print_points_to_file(CGPointSet_t* point_set, FILE* fp){
    CGError_t status = CG_SUCCESS;
    const char* function_name = "print_points_to_file";

    // if stream NULL print to error
    if(fp == NULL) fp = stderr;

    if(point_set == NULL){
        return CG_INVALID_INPUT;
    }
    else if(point_set->points == NULL || point_set->num_points == 0){
        return CG_INVALID_INPUT;
    }
    else{
        int i;
        for(i = 0; i < point_set->num_points; i++){
            if(point_set->points+i == NULL){
                return CG_INVALID_INPUT;
            }
            print_point_to_file(point_set->points+i, fp);
        }
    }
	return CG_SUCCESS;
}


//----------------------------------------------------------------
// Generating and comparing point sets
//----------------------------------------------------------------


/**
 * Function used to generate random point sets for testing purposes.
 * @ingroup diag
 * @param point_set Initialized, but empty point set which will be filled with random data.
 * @param type Type of points to be written into the point set
 * @return INVALID_INPUT if point set is uninitialized or empty, otherwise SUCCESS.
 */
CGError_t generate_random_point_set(CGPointSet_t* point_set, CGType_t type){
    if(point_set == NULL)
        return CG_INVALID_INPUT;
    else if(point_set->num_points == 0 || point_set->points == NULL)
        return CG_INVALID_INPUT;
    int i;
    for(i = 0; i < point_set->num_points; i++){
        if(type == CG_INT){
            point_set->points[i].xcoord = (int) = 0;
            point_set->points[i].ycoord = (int) = 0;
        }
        else{
            point_set->points[i].xcoord = 0;
            point_set->points[i].ycoord = 0;
        }
        point_set->points[i].type = type;
    }
    return CG_UNIMPLEMENTED;
}


/**
 * Function for comparing two points.
 * @ingroup diag
 * @param point_A First point to compare
 * @param point_B Second point to compare
 * @return 0 if the two points are the same, otherwise -1
 */
int compare_points(CGPoint_t* point_A, CGPoint_t* point_B){
    if(point_A->xcoord != point_B->xcoord)
        return -1;
    else if(point_A->ycoord != point_B->ycoord)
        return -1;
    else if(point_A->type != point_B->type)
        return -1;
    else
        return 0;
}


/**
 * Function that compares the contents of two point sets.
 * param point_set_A First point set to compare.
 * param point_set_B Second point set to compare.
 * return 0 if they are the same, -1 if they are not.
 */
int compare_point_sets(CGPointSet_t* point_set_A, CGPointSet_t* point_set_B){
    if(point_set_A == NULL || point_set_B == NULL)
        return -1;
    else{
        if(point_set_A->num_points != point_set_B->num_points)
            return -1;
        int i;
        for(i = 0; i < point_set_A->num_points; i++){
            if(compare_points(&(point_set_A->points[i]), &(point_set_B->points[i])) != 0)
                return -1;
        }
        return 0;
    }
}
