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
#include <time.h>
#include "libCGeo/libCGeo.h"


//----------------------------------------------------------------
// Printing error information
//----------------------------------------------------------------


/** @internal */
typedef struct CG_ERROR_MESSAGE {
    CGError_t error;
    const char* message;
} CGErrorMessage_t;


/** @internal */
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
 * Simple function for printing basic point information to stdout
 * @ingroup diagnostics
 * @param point Pointer to the point to print.
 * @return void
 */
void print_point(CGPoint_t* point){
    print_point_to_file(point, stdout, CG_MIN);
}


/**
 * Simple function for printing out point information to file stream
 * @ingroup diagnostics
 * @param point pointer to CGPoint.
 * @param fp Pointer to file stream to print into.
 * @param desc_detail Flag specifying how much point information to print.
 * @return void
 */
void print_point_to_file(CGPoint_t* point, FILE* fp, CGDescDetail_t desc_detail){
    const char* function_name = "print_point_to_file";
    switch(desc_detail){
        case CG_MIN:
            fprintf(fp, "x: %lf, y: %lf\n", point->xcoord, point->ycoord);
            break;
        case CG_VERBOSE:
        case CG_FULL:
            fprintf(fp, "x: %lf, y: %lf\n", point->xcoord, point->ycoord);
            fprintf(fp, "Sort by: %s, with value: %lf\n", point->sort_val_desc, point->sort_val);
            fprintf(fp,"------------------------------\n");
            break;
        default:
            print_cg_error(CG_INVALID_INPUT, function_name);
            break;
    }
}


/**
 * Simple function for printing out values in point set directly to stdout.
 * @ingroup diagnostics
 * @param point_set Input set of points
 * @return void
 */
void print_points(CGPointSet_t* point_set){
    print_points_to_file(point_set, stdout, CG_MIN);
}


/**
 * Simple function for printing out values in point set into a file
 * @ingroup diagnostics
 * @param point_set Input set of points
 * @param fp File pointer of file in which to write
 * @return void
 */
void print_points_to_file(CGPointSet_t* point_set, FILE* fp, CGDescDetail_t desc_detail){
    const char* function_name = "print_points_to_file";

    // if stream NULL print to error
    if(fp == NULL) fp = stderr;

    if(point_set == NULL)
        print_cg_error(CG_INVALID_INPUT, function_name);
    else if(point_set->head == NULL || point_set->num_points == 0)
        print_cg_error(CG_POINTS_TOO_FEW, function_name);
    else{
        CGPointNode_t* current_node = point_set->head;
        while(current_node->next != NULL){
            print_point_to_file(current_node->point, fp, desc_detail);
            current_node = current_node->next;
        }
    }
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
CGError_t generate_random_point_set(CGPointSet_t* point_set, int num_points){
    if(point_set == NULL)
        return CG_INVALID_INPUT;
    int i;
    srand(time(NULL));
    for(i = 0; i < num_points; i++){
        int xcoord = ((int) rand()/RAND_MAX*2.0 - 1.0) * 100.0;
        int ycoord = ((int) rand()/RAND_MAX*2.0 - 1.0) * 100.0;
        add_coords_to_set(point_set, (double) xcoord, (double) ycoord);
    }
    return CG_SUCCESS;
}


/**
 * Function for comparing two points.
 * @ingroup diag
 * @param point_A First point to compare
 * @param point_B Second point to compare
 * @return 0 if the two points are the same, otherwise -1
 */
int compare_points(CGPoint_t* point_A, CGPoint_t* point_B){
    if(fabs(point_A->xcoord - point_B->xcoord) > FLOAT_TOLERANCE)
        return -1;
    else if(fabs(point_A->ycoord - point_B->ycoord) > FLOAT_TOLERANCE)
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
        CGPointNode_t* current_node_A = point_set_A->head;
        CGPointNode_t* current_node_B = point_set_B->head;
        while(current_node_A != NULL){
            if(compare_points(current_node_A->point, current_node_B->point) != 0)
                return -1;
            current_node_A = current_node_A->next;
            current_node_B = current_node_B->next;
        }
        return 0;
    }
}
