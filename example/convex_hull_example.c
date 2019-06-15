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
 * This example program demonstrates the computation of the convex hull using
 * the various supported algorithms in libCGeo
 */

#include <stdio.h>
#include <stdlib.h>
#include "libCGeo/libCGeo.h"

int main(int argc, char** argv){

    if(argc != 2){
        print_cg_error(CG_INVALID_INPUT, "main");
        return -1;
    }

    const char* input_test_file = argv[1];
    FILE* input = fopen(input_test_file, "r");

    if(input == NULL){
        print_cg_error(CG_NO_FILE, "main");
        return -1;
    }

    CGPointSet_t* point_set = init_point_set(7);
    printf("Initialized empty point set\n");
    if(point_set_from_csv_file(point_set, input, CG_INT) != CG_SUCCESS){
        print_cg_error(CG_INVALID_INPUT, "point_set_from_csv_file");
        return -1;
    }

    fclose(input);
    
    printf("Points read from input file ... \n");
    print_points(point_set);
    printf("---------------------------\n");
    CGPointSet_t* convexHull = compute_graham_scan(point_set, CG_NO_DEGENERACY);
    //CGError_t status = compute_convex_hull(point_set, convexHull, CG_GRAHAM_SCAN, CG_NO_DEGENERACY);

    printf("ConvexHull of input is:\n");
    print_points(convexHull);
}
