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
    {CG_INVALID_DIMS  , "Invalid point dimensions"},
    {CG_POINTS_TOO_FEW, "Not enough points"},
    {CG_INVALID_INPUT , "Invalid input"}
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
    if(point->type == CG_INT && point->dims == CG_2D)
        result = fprintf(fp, "2D, int, coords[x: %d, y: %d]\n", (int) point->xcoord, (int) point->ycoord);
    else if(point->type == CG_INT && point->dims == CG_3D)
        result = fprintf(fp, "3D, int, coords[x: %d, y: %d, z: %d]\n", (int) point->xcoord, (int) point->ycoord, (int) point->zcoord);
    else if(point->type == CG_FLOAT && point->dims == CG_2D)
        result = fprintf(fp, "2D, float, coords[x: %lf, y: %lf]\n", point->xcoord, point->ycoord);
    else if(point->type == CG_FLOAT && point->dims == CG_3D)
        result = fprintf(fp, "3D, float, coords[x: %lf, y: %lfm z: %lf]\n", point->xcoord, point->ycoord, point->zcoord);   
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
        status = CG_INVALID_INPUT;
        print_cg_error(status, function_name);
        return status;
    }
    else if(point_set->points == NULL || point_set->num_points == 0){
        status = CG_INVALID_INPUT;
        print_cg_error(status, function_name);
        return status;
    }
    else{
        int i;
        for(i = 0; i < point_set->num_points; i++){
            if(point_set->points+i == NULL){
                status = CG_INVALID_INPUT;
                print_cg_error(status, function_name);
                return status;
            }
            print_point_to_file(point_set->points+i, fp);
        }
    }
}