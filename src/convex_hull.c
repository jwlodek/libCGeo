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
 * param point_set Point set for which to compute angles
 * return INVALID_INPUT if input is null or angle cannot be computed, otherwise SUCCESS
 * @ingroup graham
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
            double angle = angle_between(lowest_point, &(point_set->points[i]));
            if(angle < 0)
                return CG_INVALID_INPUT;
            point_set->points[i].sort_val = angle;
            point_set->points[i].sort_val_desc = "angle with lowest point";
        }
        return CG_SUCCESS;
    }
}


CGError_t remove_gs_degeneracies(CGPointSet_t* point_set){
    //TODO
    return CG_UNIMPLEMENTED;
}


CGError_t compute_graham_scan(CGPointSet_t* input_set, CGPointSet_t* output_set){
    //TODO
    return CG_UNIMPLEMENTED;
}
