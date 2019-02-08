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
 * @defgroup pttypes Point Types
 * @breif Managing point types
 * @defgroup ptrels Point Relationships
 * @breif Calculating relationships between Points
 * @defgroup input Inputs
 * @breif Collection of functions that allow for generating point-sets
 */

// System level includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// libCGeo includes
#include "libCGeo.h"

/**
 * Function that reads point information from a comma separated values file.
 * @param point_set The point set into which the points will be written.
 * @param file file pointer to the .csv file
 * @param type Type of points stored in file (int, float)
 * @param dims Are the points in 2D or 3D space
 * @return Success if 
 */
CGError_t point_set_from_file(CGPointSet_t* point_set, FILE* file, CGType_t type, CGDims_t dims){
    CGError_t status = CG_SUCCESS;
    const char* function_name = "point_set_from_file";
    return status;
}


/**
 * Function that parses a .csv line into a CGPoint.
 * @param point Pointer to the point into which the line is read. Cannot be NULL
 * @param csv_line Line read from .csv file
 * @param type Type of point (int, float)
 * @param dims Is the point in 2D or 3D space
 * @return Success if parsed correctly, otherwise INVALID INPUT error
 */
CGError_t point_from_csv(CGPoint_t* point, char* csv_line, CGType_t type, CGDims_t dims){
    CGError_t status = CG_SUCCESS;
    const char* function_name = "point_from_csv";
    if(point == NULL || csv_line == NULL){
        status = CG_INVALID_INPUT;
        print_cg_error(status, function_name);
        return status;
    }
    // because it is a .csv, we split on commas
    char* delimeter, xval, yval, zval;
    delimeter = ",";
    xval = strtok(csv_line, delimeter);
    if(xval == NULL){
        status = CG_INVALID_INPUT;
        print_cg_error(status, function_name);
        return status;
    }
    yval = srtok(NULL, delimeter);
    if(yval == NULL){
        status = CG_INVALID_INPUT;
        print_cg_error(status, function_name);
        return status;
    }
    if(dims == CG_3D){
        zval = strtok(NULL, delimeter);
        if(xval == NULL){
            status = CG_INVALID_INPUT;
            print_cg_error(status, function_name);
            return status;
        }
    }
    point->dims = dims;
    point->type = type;
    if(type == CG_INT){
        point->xcoord = atoi(xval);
        point->ycoord = atoi(yval);
        if(dims == CG_3D) point->zcoord = atoi(zval);
    }
    else if(type == CG_FLOAT){
        point->xcoord = strtod(xval);
        point->ycoord = strtod(yval);
        if(dims == CG_3D) point->zcoord = strtod(zval);
    }
    else{
        status = CG_INVALID_TYPE;
        print_cg_error(status, function_name);
    }
    return status;
}


