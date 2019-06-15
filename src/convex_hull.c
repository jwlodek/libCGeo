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
 * @return INVALID_INPUT if input is null or angle cannot be computed, otherwise SUCCESS
 */
CGError_t compute_point_angles(CGPointSet_t* point_set){
    if(point_set == NULL)
        return CG_INVALID_INPUT;
    else if(point_set->num_points == 0 || point_set->points == NULL)
        return CG_INVALID_INPUT;
    else{
        CGPoint_t* lowest_point = find_lowest_point_in_set(point_set);
        // angles in radians between 0 and pi, lowest point gets -1
        lowest_point->sort_val = -1;
        lowest_point->sort_val_desc = "lowest_point";
        int i;
        for(i = 0; i < point_set->num_points; i++){
            if(point_set->points + i != lowest_point){
                double angle = angle_between(lowest_point, &(point_set->points[i]));
                if(angle < 0)
                    return CG_INVALID_INPUT;
                point_set->points[i].sort_val = angle;
                point_set->points[i].sort_val_desc = "angle with lowest point";
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
 * @param point_set Point set containing calculated convex hull
 * @return NULL if error encountered, otherwise point_set with colinear points removed.
 */
CGPointSet_t* remove_colinear_degeneracies(CGPointSet_t* point_set){
    CGPointSet_t* no_degeneracy = init_point_set(point_set->num_points);
    int i = 0;
    int j = 1;
    int k = 2;
    int counter = 1;
    no_degeneracy->points[0] = point_set->points[0];
    while(k < point_set->num_points){
        CGTurn_t turn_type = find_turn_type(&(point_set->points[i]), &(point_set->points[j]), &(point_set->points[k]));
        if(turn_type != CG_TURN_INLINE){
            no_degeneracy->points[counter] = point_set->points[j];
            counter++;
            i = j;
            j = k;
            k++;
        }
        else{
            j = k;
            k++;
        }
        if(k == point_set->num_points){
            CGTurn_t wrap_around = find_turn_type(&(point_set->points[i]), &(point_set->points[j]), &(point_set->points[0]));
            if(wrap_around != CG_TURN_INLINE){
                no_degeneracy->points[counter] = point_set->points[j];
                counter++;
            }
        }
    }
    no_degeneracy->num_points = counter;
    realloc(no_degeneracy->points, no_degeneracy->num_points * sizeof(CGPoint_t));
    return no_degeneracy;
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
CGPointSet_t* compute_graham_scan(CGPointSet_t* point_set, CGCompute_t compute_type){
    if(point_set == NULL)
        return NULL;
    else if(point_set->num_points == 0 || point_set->points == NULL)
        return NULL;

    CGPointSet_t* stack_set = init_point_set(point_set->num_points);
    CGError_t status;
    status = compute_point_angles(point_set);
    if(status != CG_SUCCESS)
        return NULL;

    // initialize the stack
    int stack_top = 2;
    CGPoint_t* stack = stack_set->points;
    stack[0] = point_set->points[0];
    stack[1] = point_set->points[1];
    stack[2] = point_set->points[2];

    // Main Graham scan algorithm loop
    int i;
    for(i = 0; i < point_set->num_points; i++){
        while(find_turn_type(&(stack[stack_top - 1]), &(stack[stack_top]), &(point_set->points[i])) == CG_TURN_RIGHT){
            stack_top = stack_top - 1;
        }
        stack_top ++;
        stack[stack_top] = point_set->points[i];
    }

    // resize the output set (not all points in original are in Convex Hull)
    stack_set->num_points = stack_top + 1;
    realloc(stack, stack_set->num_points * sizeof(CGPoint_t));

    if(compute_type == CG_W_DEGENERACY){
        CGPointSet_t* no_degeneracy =  remove_colinear_degeneracies(stack_set);
        free_point_set(stack_set);
        return no_degeneracy;
    }
    else
        return stack_set;
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
    CGPointSet_t* convex_hull;
    switch(convex_hull_method){
        case CG_GRAHAM_SCAN:
            convex_hull = compute_graham_scan(point_set, compute_type);
            break;
        default:
            return CG_UNIMPLEMENTED;
    }
    if(convex_hull == NULL)
        return CG_INVALID_INPUT;
    else{
        output_set = convex_hull;
        return CG_SUCCESS;
    }
}
