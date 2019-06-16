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
 * This is the source file that contains implementations of all functions specific to
 * computing the convex hull of a point set
 *
 * @defgroup chull Convex Hull Functions
 * @breif Functions and data types used by libCGeo for finding the Convex Hull of a Point Set.
 */


#include "libCGeo/libCGeo.h"


/**
 * Function that computes the angle of each point with the lowest point in the set.
 * @ingroup chull
 * @param point_set Point set for which to compute angles
 * @return INVALID_INPUT if input is null or angle cannot be computed, 
 *      POINTS_TO_FEW  if the point set is empty, otherwise SUCCESS
 */
CGError_t compute_point_angles(CGPointSet_t* point_set){
    if(point_set == NULL)
        return CG_INVALID_INPUT;
    else if(point_set->num_points == 0 || point_set->head == NULL)
        return CG_POINTS_TOO_FEW;
    else{
        CGPoint_t* lowest_point = find_lowest_point_in_set(point_set);
        // angles in radians between 0 and pi, lowest point gets -1
        lowest_point->sort_val = -1;
        lowest_point->sort_val_desc = "lowest_point";
        CGPointNode_t* current_node = point_set->head;
        while(current_node != NULL){
            if(current_node->point != lowest_point){
                double angle = angle_between(lowest_point, current_node->point);
                if(angle < 0)
                    return CG_INVALID_INPUT;
                current_node->point->sort_val = angle;
                current_node->point->sort_val_desc = "angle with lowest point";
            }
        }
        return CG_SUCCESS;
    }
}


/**
 * Function that removes 3-point colinear degeneracies for convex hull calculations.
 * First, initialize the no-degeneracy set with the starting point. Then, iterate
 * over points and check if they make an inline turn. If yes, skip over them.
 * Once iteration is done, check if last 3 points consist of a colinear degeneracy
 * @ingroup chull
 * @param input_set Point set containing calculated convex hull
 * @param output_set Initialized but empty point set that will contain the convex hull without colinear degeneracies
 * @return NULL if error encountered, otherwise point_set with colinear points removed.
 */
CGError_t remove_colinear_degeneracies(CGPointSet_t* input_set, CGPointSet_t* output_set){
    CGError_t status = CG_SUCCESS;
    if (input_set == NULL || output_set == NULL) status = CG_INVALID_INPUT;
    else if(input_set->num_points < 3) {
        status = CG_POINTS_TOO_FEW;
    }
    else{
        CGPointNode_t* input_node_A = input_set->head;
        CGPointNode_t* input_node_B = input_node_A->next;
        CGPointNode_t* input_node_C = input_node_B->next;
        add_point_to_set(output_set, input_node_A->point);
        while(input_node_C != NULL){
            CGTurn_t turn_type = find_turn_type(input_node_A->point, input_node_B->point, input_node_C->point);
            if(turn_type != CG_TURN_INLINE){
                
                input_node_A = input_node_B;
                input_node_B = input_node_C;
                input_node_C = input_node_C->next;
            }
            else{
                input_node_B = input_node_C;
                input_node_C = input_node_C->next;
            }
            if(input_node_C == NULL){
                CGTurn_t wrap_around = find_turn_type(input_node_A->point, input_node_B->point, input_set->head->point);
                if(wrap_around != CG_TURN_INLINE){
                    add_point_to_set(output_set, input_node_B->point);
                }
            }
        }
    }
    return status;
}


/**
 * Function that computes the convex hull using the graham scan approach.
 * First, compute angle each point makes with lowest point. Then sort by said angles.
 * Then, initialze a stack of points, and only add to the stack if the three top points
 * make a left turn. Finally, remove the degeneracies caused by 3-point colinear.
 * @ingroup chull
 * @param point_set Point set for which to find convex hull.
 * @param compute_type Flag to toggle between handling degeneracies or not.
 * @return NULL if invalid inputs or error, otherwise CGPointSet_t with convex hull.
 */
CGError_t compute_graham_scan(CGPointSet_t* point_set, CGPointSet_t* output_set, CGCompute_t compute_type){
    // basic error checking
    if(point_set == NULL || output_set == NULL)
        return CG_INVALID_INPUT;
    else if(point_set->num_points < 3 || point_set->head == NULL)
        return CG_POINTS_TOO_FEW;

    // find angles w.r.t. lowest point
    CGError_t status;
    status = compute_point_angles(point_set);
    if(status != CG_SUCCESS)
        return status;

    CGPointSet_t* p_scratch = init_point_set();

    // sort the points into a temp set
    status = sort_point_set(point_set, p_scratch);
    if(status != CG_SUCCESS){
        free_point_set(p_scratch);
        return status;
    }

    CGPointNode_t* p_stack = p_scratch->head;

    // allocate a stack to keep track of the points we want.
    CGPoint_t* stack = (CGPoint_t*) malloc(p_scratch->num_points * sizeof(CGPoint_t));

    // initialize the stack
    stack[0] = *(p_stack->point);
    stack[1] = *(p_stack->next->point);
    stack[2] = *(p_stack->next->next->point);
    p_stack = p_stack->next->next->next;

    // Main Graham scan algorithm loop
    int stack_counter = 2;
    while(p_stack != NULL){
        // until we find a left turn, continue to step back through the stack
        while(find_turn_type(&stack[stack_counter - 1], &stack[stack_counter], p_stack->point) == CG_TURN_RIGHT){
            stack_counter--;
        }
        // when we find a left turn, increment the stack, and insert the point into it.
        stack_counter++;
        stack[stack_counter] = *(p_stack->point);
        p_stack = p_stack->next;
    }

    // write all of the points in the stack into the output set
    int i;
    for(i = 0; i< stack_counter + 1; i++){
        add_point_to_set(output_set, &stack[i]);
    }

    // free memory
    free(stack);
    free_point_set(p_scratch);

    // if we account for degeneracy, run the remove degeneracy function
    if(compute_type == CG_W_DEGENERACY){
        CGPointSet_t* p_no_degeneracy = init_point_set();
        status = remove_colinear_degeneracies(output_set, p_no_degeneracy);
        if(status == CG_SUCCESS){
            CGPointSet_t* temp = output_set;
            output_set = p_no_degeneracy;
            free_point_set(output_set);
        }
    }

    return status;
}


/**
 * Function that switches on all of the convex hull functions.
 * @ingroup chull
 * @param point_set Initialized point set to perform convex hull on.
 * @param output_set Unallocated pointer to point set, where computed hull will be placed.
 * @param convex_hull_method Which convex hull algorithm to perform
 * @param compute_type Toggle for computing with or without degeneracy
 * @return UNIMPLEMENTED if invalid chull method, INVALID_INPUT if failure, SUCCESS otherwise
 */
CGError_t compute_convex_hull(CGPointSet_t* point_set, CGPointSet_t* output_set, CGConvexHull_t convex_hull_method, CGCompute_t compute_type){
    CGError_t status;
    switch(convex_hull_method){
        case CG_GRAHAM_SCAN:
            status = compute_graham_scan(point_set, output_set, compute_type);
            break;
        default:
            return CG_UNIMPLEMENTED;
    }
    return status;
}
