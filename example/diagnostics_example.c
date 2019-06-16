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
    printf(INTRO_MESSAGE);
    printf("\nlibCGeo diagnostics example program.\n");

    // initialize two empty point sets
    CGPointSet_t* point_set = init_point_set();
    CGPointSet_t* point_set_random = init_point_set();

    // add points to the first set by coord
    add_coords_to_set(point_set, 0.0, 0.0);
    add_coords_to_set(point_set, 1.5, -9.0);
    add_coords_to_set(point_set, -12.0, 1.0);

    // generate a point set with 12 random points
    CGError_t gen_status = generate_random_point_set(point_set_random, 12);
    printf("We have generated a random point set:\n");
    // print to a file stream with verbose detail
    print_points_to_file(point_set_random, stdout, CG_VERBOSE);

    // print the point sin the set
    printf("Printing preconfigured point set:\n");
    // print to stdout with minimum verbosity by default
    print_points(point_set);

    printf("Error information can be printed as follows:\n");
    //prints out error information
    CGError_t err = CG_INVALID_INPUT;
    print_cg_error(err, "print_points");

    // free the point set
    free_point_set(point_set);
    free_point_set(point_set_random);
}
