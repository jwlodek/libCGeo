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


// libCGeo includes
#include "libCGeo.h"


//----------------------------------------------------------------
// Functions - Reading from files
//----------------------------------------------------------------

/**
 * Function that reads point information from a comma separated values file.
 * @param point_set The point set into which the points will be written.
 * @param file file pointer to the .csv file
 * @param type Type of points stored in file (int, float)
 * @param dims Are the points in 2D or 3D space
 * @return Success if 
 */
CGError_t point_set_from_file(CGPointSet_t* point_set, FILE* file, CGType_t type){
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
CGError_t point_from_csv(CGPoint_t* point, char* csv_line, CGType_t type){
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
    yval = strtok(NULL, delimeter);
    if(yval == NULL){
        status = CG_INVALID_INPUT;
        print_cg_error(status, function_name);
        return status;
    }
    point->type = type;
    if(type == CG_INT){
        point->xcoord = atoi(xval);
        point->ycoord = atoi(yval);
    }
    else if(type == CG_FLOAT){
        point->xcoord = atof(xval);
        point->ycoord = atof(yval);
    }
    else{
        status = CG_INVALID_TYPE;
        print_cg_error(status, function_name);
    }
    return status;
}


/**
 * Function that finds the turn made by three consecutive points.
 * param point_A The first point out of the three.
 * param point_B The center point out of the three.
 * param point_C The last point out of the three.
 * return Inline if points are colinear, otherwise left or right
 */
CGTurn_t find_turn_type(CGPoint_t* point_A, CGPoint_t* point_B, CGPoint_t* point_C){
    int value = (point_B->ycoord - point_A->ycoord)*(point_C->xcoord - point_B->xcoord) -
                (point_B->xcoord - point_A->xcoord)*(point_C->ycoord - point_B->ycoord);
    if(value == 0) return CG_TURN_INLINE;
    return (value > 0) ? CG_TURN_RIGHT : CG_TURN_LEFT;
}


/**
 * Function that finds the minimum point by y-coordinate, and if there is a tie, it finds the min x-coordinate as well. 
 * param point_set Point Set to search through
 * return NULL if point set is NULL or empty, otherwise lowest point by y-coordinate
 */
CGPoint_t* find_lowest_point(CGPointSet_t* point_set){
    if(point_set == NULL){
        print_cg_error(CG_INVALID_INPUT, "find_lowest_point");
        return NULL;
    }
    else if(point_set->num_points == 0){
        print_cg_error(CG_INVALID_INPUT, "find_lowest_point");
        return NULL;
    }
    CGPoint_t point = point_set->points[0];
    int i;
    for(i = 0; i < point_set->num_points; i++){
        if(point_set->points[i].ycoord < point.ycoord){
            point = point_set->points[i];
        }
        else if(point_set->points[i].ycoord == point.ycoord){
            if(point_set->points[i].xcoord < point.xcoord){
                point = point_set->points[i];
            }
        }
    }
    return &point;
}


/**
 * Simple function that finds the distance between two points via the distance formula.
 * param point_A First point.
 * param point_B Second point.
 * return Straight line distance between point_A and point_B, negative if invalid.
 */
double distance_between(CGPoint_t* point_A, CGPoint_t* point_B){
    if(point_A == NULL || point_B == NULL){
        print_cg_error(CG_INVALID_INPUT, "distance_between");
        return -1;
    }
    double delta_x = point_A->xcoord - point_B->xcoord;
    double delta_y = point_A->ycoord - point_B->ycoord;
    double hypotenuse_squared = pow(delta_x, 2) + pow(delta_y, 2);
    return pow(hypotenuse_squared, 0.5);
}