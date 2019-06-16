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
 * Function that initializes an empty point set point set
 * @ingroup pttypes
 * @return pointer to allocated point set.
 */
CGPointSet_t* init_point_set(){
    CGPointSet_t* point_set = (CGPointSet_t*) calloc(1, sizeof(CGPointSet_t));
    point_set->num_points = 0;
    return point_set;
}


CGError_t add_coords_to_set(CGPointSet_t* point_set, double xCoord, double yCoord){
    CGPoint_t* point = malloc(sizeof(CGPoint_t));
    point->xcoord = xCoord;
    point->ycoord = yCoord;
    CGError_t err = add_point_to_set(point_set, point);
    if(err != CG_SUCCESS)
        free(point);
    return err;
}


CGError_t add_point_to_set(CGPointSet_t* point_set, CGPoint_t* point){
    if(point_set == NULL || point == NULL) return CG_INVALID_INPUT;
    else {
        CGPointNode_t* pnode = (CGPointNode_t*) malloc(sizeof(CGPointNode_t));
        pnode->point = point;
        if(point_set->head == NULL){
            point_set->head = pnode;
            point_set->tail = pnode;
        }
        else{
            pnode->prev = point_set->tail;
            point_set->tail->next = pnode;
            point_set->tail = pnode;
        }
        point_set->num_points++;
    }
    return CG_SUCCESS;
}


CGPoint_t* get_point_at_index(CGPointSet_t* point_set, int index){
    if(point_set->num_points <= index)
        return NULL;
    CGPointNode_t* current_node = point_set->head;
    int i;
    for(i = 0; i< index; i++){
        current_node = current_node->next;
        if(current_node == NULL){
            return NULL;
        }
    }
    return current_node->point;
}

/**
 * Function that frees memory allocated by init_point_set.
 * @ingroup pttypes
 * @param point_set Point set to free.
 * @return INVALID INPUT error if set isnt allocated, or SUCCESS otherwise.
 */
CGError_t free_point_set(CGPointSet_t* point_set){
    CGError_t status = CG_SUCCESS;
    if(point_set == NULL)
        return CG_INVALID_INPUT;
    else if(point_set->head == NULL || point_set->num_points == 0)
        status = CG_POINTS_TOO_FEW;
    else{
        CGPointNode_t* current = point_set->head;
        while(current != NULL){
            free(current->point);
            CGPointNode_t* temp = current->next;
            free(current);
            current = temp;
        }
    }
    free(point_set);
    return status;
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
    else{
        CGPointNode_t* current_node = in_point_set->head;
        while(current_node != NULL){
            CGPoint_t* point = malloc(sizeof(CGPoint_t));
            memcpy((void*) point, (void*) current_node->point, sizeof(CGPoint_t));
            add_point_to_set(out_point_set, point);
            current_node = current_node->next;
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
CGError_t point_set_from_csv_file(CGPointSet_t* point_set, FILE* file){
    CGError_t status = CG_SUCCESS;
    if(point_set == NULL)
        return CG_INVALID_INPUT;
    else if(file == NULL)
        return CG_NO_FILE;

    char buffer[LINE_BUFFER];

    while(fgets(buffer, LINE_BUFFER, file) != NULL){
        if(strlen(buffer) > 0){
            char* xstr = strtok(buffer, ",");
            double xcoord = atof(xstr);
            char* ystr = strtok(NULL, "\n");
            if(ystr == NULL)
                ystr = strtok(NULL, EOF);
            double ycoord = atof(ystr);
            add_coords_to_set(point_set, xcoord, ycoord);
        }
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
    CGPointNode_t* current_node = point_set->head;
    while(current_node != NULL){
        fprintf(file_pointer, "%lf,%lf\n", current_node->point->xcoord, current_node->point->ycoord);
        current_node = current_node->next;
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
    CGPoint_t* point = point_set->head->point;
    CGPointNode_t* current_node = point_set->head;
    while(current_node != NULL){
        if(current_node->point->ycoord < point->ycoord){
            point = current_node->point;
        }
        else if(current_node->point->ycoord == point->ycoord && 
                current_node->point->xcoord < point->xcoord){
                    point = current_node->point;
        }
        current_node = current_node->next;
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
    else if(point_set->head == NULL) return CG_INVALID_INPUT;
    else{
        if(output_point_set == NULL){
            status = sort_points(&(point_set->head));
        }
        else{
            status = copy_point_set(point_set, output_point_set);
            if(status == CG_SUCCESS)
                status = sort_points(&(output_point_set->head));
        }
        return status;
    }
}


/**
 * Helper function that sorts array of points.
 * @ingroup setops
 * @param phead Reference to the head node of the linked list of points
 * @return INVALID INPUT if points is null or left and right are invalid, otherwise success.
 */
CGError_t sort_points(CGPointNode_t** phead){

    CGPointNode_t* head = *phead;
    CGPointNode_t* left_list;
    CGPointNode_t* right_list;

    if(head == NULL) return CG_INVALID_INPUT;
    else if (head->next == NULL) return CG_SUCCESS;

    split_lists(head, &left_list, &right_list);

    sort_points(&left_list);
    sort_points(&right_list);

    *phead = merge_halves(left_list, right_list);
    return CG_SUCCESS;
}


void split_lists(CGPointNode_t* head, CGPointNode_t** left, CGPointNode_t** right){
    CGPointNode_t* two_step;
    CGPointNode_t* one_step;
    one_step = head;
    two_step = head->next;

    // Jump two steps with one pointer, one step with the other.
    // When two step reaches end, one step is halfway
    while(two_step != NULL) {
        two_step = two_step->next;
        if(two_step != NULL){
            one_step = one_step->next;
            two_step = two_step->next;
        }
    }

    *left = head;
    *right = one_step->next;
    //detatch the lists
    one_step->next = NULL;
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
CGPointNode_t* merge_halves(CGPointNode_t* left_list, CGPointNode_t* right_list){
    if(left_list == NULL && right_list == NULL)
        return NULL;

    CGPointNode_t* result_head = NULL;
    if(left_list == NULL) return right_list;
    else if(right_list == NULL) return left_list;

    if(left_list->point->sort_val <= right_list->point->sort_val) {
        result_head = left_list;
        result_head->next = merge_halves(left_list->next, right_list);
    }
    else{
        result_head = right_list;
        result_head->next = merge_halves(left_list, right_list->next);
    }
    return result_head;
}