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
 * @defgroup pttypes Points and Point Sets
 * @breif Initialize point sets, points, and point set Data Structures.
 * @defgroup file File Parsing and Writing.
 * @defgroup ptops Operations on Points.
 * @breif Mathematics calculations and operations between points (distance, angle, etc.)
 * @defgroup setops Operations on Point Sets
 * @breif Operations performed on point sets (sorting, reallocation...)
 */


// libCGeo includes
#include "libCGeo/libCGeo.h"

// Maximum number of characters read per line in .csv files
#define LINE_BUFFER 256


//----------------------------------------------------------------
// Functions - Init and free point sets
//----------------------------------------------------------------


/**
 * Function that initializes a point set with a set of empty points.
 * @ingroup pttypes
 * @param num_points The number of empty points to place in the set.
 * @return NULL if num_points is invalid, otherwise pointer to allocated point set.
 */
CGPointSet_t* init_point_set(int num_points){
    if(num_points <= 0){
        return NULL;
    }
    else{
        CGPointSet_t* point_set = (CGPointSet_t*) calloc(1, sizeof(CGPointSet_t));
        point_set->num_points = num_points;
        point_set->points = (CGPoint_t*) calloc(1, point_set->num_points * sizeof(CGPoint_t));
        return point_set;
    }
}


/**
 * Function that frees memory allocated by init_point_set.
 * @ingroup pttypes
 * @param point_set Point set to free.
 * @return INVALID INPUT error if set isnt allocated, or SUCCESS otherwise.
 */
CGError_t free_point_set(CGPointSet_t* point_set){
    if(point_set == NULL)
        return CG_INVALID_INPUT;
    else if(point_set->points == NULL)
        return CG_INVALID_INPUT;
    else{
        free(point_set->points);
        free(point_set);
        return CG_SUCCESS;
    }
}


/**
 * Function that copies points from one point set to another point set.
 * @ingroup pttypes
 * @param in_point_set Initialized input point set
 * @param out_point_set Initialized output point set (must have same num_points as input set)
 * @return CG_INVALID_INPUT if either param is Null or invalid, otherwise CG_SUCCESS
 */
CGError_t copy_point_set(CGPointSet_t* in_point_set, CGPointSet_t* out_point_set){
    if(in_point_set == NULL || out_point_set == NULL)
        return CG_INVALID_INPUT;
    else if(in_point_set->num_points != out_point_set->num_points)
        return CG_INVALID_INPUT;
    else{
        int i;
        for(i = 0; i < in_point_set->num_points; i++){
            out_point_set->points[i] = in_point_set->points[i];
        }
        return CG_SUCCESS;
    }
}


//----------------------------------------------------------------
// Functions - Reading from and writing to files
//----------------------------------------------------------------


/**
 * Function that reads point information from a comma separated values file.
 * @ingroup file
 * @param point_set The point set into which the points will be written.
 * @param file file pointer to the .csv file
 * @param type Type of points stored in file (int, float)
 * @return Success if read without error, error if num_points invalid or point_set non-empty.
 */
CGError_t point_set_from_csv_file(CGPointSet_t* point_set, FILE* file, CGType_t type){
    CGError_t status = CG_SUCCESS;

    if(point_set == NULL)
        return CG_INVALID_INPUT;
    else if(point_set->points == NULL)
        return CG_INVALID_INPUT;
    else if(file == NULL)
        return CG_NO_FILE;

    char* buffer = (char*) calloc(1, LINE_BUFFER);
    int counter = 0;
    while(fgets(buffer, LINE_BUFFER, file) != NULL){
        if(counter == point_set->num_points)
            break;
        char* token = (char*) strtok(buffer, ",");
        point_set->points[counter].type = type;
        if(type == CG_INT)
            point_set->points[counter].xcoord = (int) strtod(token, NULL);
        else
            point_set->points[counter].xcoord = (int) strtod(token, NULL);
        token = (char*) strtok(NULL, "\n");
        if(type == CG_INT)
            point_set->points[counter].ycoord = strtod(token, NULL);
        else
            point_set->points[counter].ycoord = strtod(token, NULL);

        counter++;
    }
    free(buffer);
    if(status == CG_SUCCESS && counter != point_set->num_points){
        printf("%d\n", counter);
        status = CG_INVALID_INPUT;
    }
    return status;
}


/**
 * Function for writing a point set to a .csv file
 * @ingroup file
 * @param point_set Point set to write into the file
 * @param file_pointer File pointer of file to write into.
 * @return INVALID_INPUT if either is null or can't write to file, otherwise success.
 */
CGError_t csv_file_from_point_set(CGPointSet_t* point_set, FILE* file_pointer){
    if(point_set == NULL || file_pointer == NULL)
        return CG_INVALID_INPUT;
    int i;
    for(i = 0; i < point_set->num_points; i++){
        if(point_set->points[i].type == CG_INT)
            fprintf(file_pointer, "%d,%d\n", (int) point_set->points[i].xcoord, (int) point_set->points[i].ycoord);
        else
            fprintf(file_pointer, "%lf,%lf\n", point_set->points[i].xcoord, point_set->points[i].ycoord);
    }
    return CG_SUCCESS;
}


//----------------------------------------------------------------
// Math Operations and relationships for points
//----------------------------------------------------------------


/**
 * Function that finds the turn made by three consecutive points.
 * @ingroup ptops
 * @param point_A The first point out of the three.
 * @param point_B The center point out of the three.
 * @param point_C The last point out of the three.
 * @return Inline if points are colinear, otherwise left or right
 */
CGTurn_t find_turn_type(CGPoint_t* point_A, CGPoint_t* point_B, CGPoint_t* point_C){
    int value = (point_B->ycoord - point_A->ycoord)*(point_C->xcoord - point_B->xcoord) -
                (point_B->xcoord - point_A->xcoord)*(point_C->ycoord - point_B->ycoord);
    if(value == 0) return CG_TURN_INLINE;
    return (value > 0) ? CG_TURN_RIGHT : CG_TURN_LEFT;
}


/**
 * Function that finds the minimum point by y-coordinate, and if there is a tie, it finds the min x-coordinate as well. 
 * @ingroup ptops
 * @param point_set Point Set to search through
 * @return NULL if point set is NULL or empty, otherwise lowest point by y-coordinate
 */
CGPoint_t* find_lowest_point_in_set(CGPointSet_t* point_set){
    if(point_set == NULL)
        return NULL;
    else if(point_set->num_points == 0)
        return NULL;
    CGPoint_t* point = &(point_set->points[0]);
    int i;
    for(i = 0; i < point_set->num_points; i++){
        if(point_set->points[i].ycoord < point->ycoord){
            point = &(point_set->points[i]);
        }
        else if(point_set->points[i].ycoord == point->ycoord){
            if(point_set->points[i].xcoord < point->xcoord) point = &(point_set->points[i]);
        }
    }
    return point;
}


/**
 * Simple function that finds the distance between two points via the distance formula.
 * @ingroup ptops
 * @param point_A First point.
 * @param point_B Second point.
 * @return Straight line distance between point_A and point_B, negative if invalid.
 */
double distance_between(CGPoint_t* point_A, CGPoint_t* point_B){
    if(point_A == NULL || point_B == NULL){
        return -1;
    }
    double delta_x = point_A->xcoord - point_B->xcoord;
    double delta_y = point_A->ycoord - point_B->ycoord;
    double hypotenuse_squared = pow(delta_x, 2) + pow(delta_y, 2);
    return pow(hypotenuse_squared, 0.5);
}


/**
 * Function that finds the angle between two points.
 * @ingroup ptops
 * @param initial_point The lower starting point
 * @param end_point The ending point for the angle ray
 * @return -1 if invalid, otherwise angle made by ray between two points and horizontal line.
 */
double angle_between(CGPoint_t* initial_point, CGPoint_t* end_point){
    if(initial_point->ycoord > end_point->ycoord) return -1;
    else if(initial_point->ycoord == end_point->ycoord && initial_point->xcoord > end_point->xcoord)
        return -1;
    if(initial_point == NULL || end_point == NULL) return -1;
    int eq = compare_points(initial_point, end_point);
    if(eq == 0) return -1;
    else{
        return acos((end_point->xcoord - initial_point->xcoord)/distance_between(initial_point, end_point));
    }
}


//----------------------------------------------------------------
// Point Set operations (sorting, realloc, etc.)
//----------------------------------------------------------------


/**
 * Function that sorts point set based on the values in each point's sort_val.
 * @param point_set Point Set to be sorted
 * @param output_point_set Point Set into which the sorted set is placed. If this is NULL or equal to point_set, then point_set overwritten by output
 * @return CG_INVALID_INPUT if point_set is null or empty, otherwise success.
 */
CGError_t sort_point_set(CGPointSet_t* point_set, CGPointSet_t* output_point_set){
    CGError_t status = CG_SUCCESS;
    if(point_set == NULL) return CG_INVALID_INPUT;
    else if(point_set->points == NULL) return CG_INVALID_INPUT;
    else{
        if(output_point_set == point_set || output_point_set == NULL){
            status = sort_points(point_set->points, 0, point_set->num_points - 1);
        }
        else{
            if(point_set->num_points != output_point_set->num_points || output_point_set->points == NULL)
                status = CG_INVALID_INPUT;
            else{
                copy_point_set(point_set, output_point_set);
                status = sort_points(output_point_set->points, 0, output_point_set->num_points - 1);
            }
        }
        return status;
    }
}


/**
 * Helper function that sorts array of points.
 * @ingroup setops
 * @param points Pointer to array of points to sort
 * @param left_index The starting point for left sub-array
 * @param right_index The starting point for the right sub-array
 * @return INVALID INPUT if points is null or left and right are invalid, otherwise success.
 */
CGError_t sort_points(CGPoint_t* points, int left_index, int right_index){
    if(left_index < right_index){
        int center_index = left_index+((right_index - left_index)/2);
        CGError_t status_left = sort_points(points, left_index, center_index);
        CGError_t status_right = sort_points(points, center_index + 1, right_index);

        CGError_t status_merge = merge_halves(points, left_index, center_index, right_index);
        return status_merge;
    }
    return CG_INVALID_INPUT;
}


/**
 * Function that implements merge-sort merging for Points array.
 * @ingroup setops
 * @param points Pointer to array of points
 * @param left_index index of the left sub-array
 * @param center_index index of the center of the points sub arrays
 * @param right_index index of the right sub-array
 * @return CG_INVALID_INPUT if error occurs, otherwise, CG_SUCCESS
 */
CGError_t merge_halves(CGPoint_t* points, int left_index, int center_index, int right_index){
    int left_counter, right_counter, merge_index;

    int left_sub_length = center_index - left_index + 1;
    int right_sub_length = right_index - center_index;

    CGPoint_t temp_left_array[left_sub_length];
    CGPoint_t temp_right_array[right_sub_length];

    for(left_counter = 0; left_counter < left_sub_length; left_counter++){
        if(points[left_index + left_counter].sort_val_desc == NULL) return CG_INVALID_INPUT;
        temp_left_array[left_counter] = points[left_index + left_counter];
    }

    for(right_counter = 0; right_counter < right_sub_length; right_counter++){
        if(points[center_index + 1 + right_counter].sort_val_desc == NULL) return CG_INVALID_INPUT;
        temp_right_array[right_counter] = points[center_index + 1 + right_counter];
    }

    left_counter = 0;
    right_counter = 0;
    merge_index = left_index;

    while(left_counter < left_sub_length && right_counter < right_sub_length){
        if(temp_left_array[left_counter].sort_val <= temp_right_array[right_counter].sort_val){
            points[merge_index] = temp_left_array[left_counter];
            left_counter++;
        }
        else{
            points[merge_index] = temp_right_array[right_counter];
            right_counter++;
        }
        merge_index++;
    }

    while(left_counter < left_sub_length){
        points[merge_index] = temp_left_array[left_counter];
        left_counter++;
        merge_index++;
    }
    while(right_counter < right_sub_length){
        points[merge_index] = temp_right_array[right_counter];
        right_counter++;
        merge_index++;
    }
    return CG_SUCCESS;
}