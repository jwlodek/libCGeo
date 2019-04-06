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
 * Example program for running diagnostics on points
 * and points sets.
 */

#include <stdio.h>
#include <stdlib.h>
#include "libCGeo/libCGeo.h"

int main(int argc, char** argv){

    //initialize the point set
    CGPointSet_t* point_set = init_point_set(3);
    CGPointSet_t* point_set_random = init_point_set(12);

    // add values to the points
    CGPoint_t* p1 = &(point_set->points[0]);
    CGPoint_t* p2 = &(point_set->points[1]);
    CGPoint_t* p3 = &(point_set->points[2]);
    p1->type = CG_INT;
    p2->type = CG_INT;
    p3->type = CG_FLOAT;
    p1->xcoord = 0;
    p1->ycoord = 0;
    p2->xcoord = 1;
    p2->ycoord = 3;
    p3->xcoord = 0.1;
    p3->ycoord = 5.4;

    // generate a point set with 12 random points
    CGError_t gen_status = generate_random_point_set(point_set_random, CG_INT);
    printf("We have generated a random point set:\n");
    print_points(point_set_random);

    // print the point sin the set
    printf("Printing preconfigured point set:\n");
    print_points(point_set);

    // this will return an INVALID_INPUT Error
    CGError_t err = print_points(NULL);

    // this will print an invalid type error message
    if(err != CG_SUCCESS)
        print_cg_error(err, "print_points");

    // free the point set
    free_point_set(point_set);
    free_point_set(point_set_random);
}
