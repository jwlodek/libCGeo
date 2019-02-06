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
#include "libCGeo.h"

int main(int argc, char** argv){
    CGPoint_t p1;
    CGPoint_t p2;
    CGPoint_t p3;
    p1.dims = CG_2D;
    p2.dims = CG_2D;
    p3.dims = CG_3D;
    p1.type = CG_INT;
    p2.type = CG_INT;
    p3.type = CG_FLOAT;
    p1.xcoord = 0;
    p1.ycoord = 0;
    p2.xcoord = 1;
    p2.ycoord = 3;
    p3.xcoord = 0;
    p3.ycoord = 5;
    p3.zcoord = 10;
    CGPointSet_t ps;
    ps.num_points = 3;
    ps.points = malloc(ps.num_points*sizeof(CGPoint_t));
    ps.points[0] = p1;
    ps.points[1] = p2;
    ps.points[2] = p3;

    // this will print the information stored in the above points to stdout
    print_points(&ps);

    // this will print an invalid input error message
    print_points(NULL);

    // this will print an invalid dims error message
    print_cg_error(CG_INVALID_DIMS, "main");
}