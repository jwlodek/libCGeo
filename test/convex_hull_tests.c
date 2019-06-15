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
 * Unit tests for the convex hull algorithm implementations for libCGeo
 */

#include "libCGeo/libCGeo.h"
#include <criterion/criterion.h>
#include <criterion/assert.h>


CGPointSet_t* point_set_A;
CGPointSet_t* point_set_B;
CGPointSet_t* point_set_C;

FILE* input_test_file;
FILE* output_test_file;


void setup_convex_hull_test(void){
    input_test_file = fopen("/home/jwlodek/Documents/ProgrammingWorkspace/libCGeo/test/test_inputs/input_12pts.csv", "r");
    output_test_file = fopen("/home/jwlodek/Documents/ProgrammingWorkspace/libCGeo/test/test_outputs/output_12pts_CH.csv", "r");
    point_set_A = init_point_set(12);
    point_set_B = init_point_set(8);
}


void teardown_general(void){
    free_point_set(point_set_A);
    free_point_set(point_set_B);
    if(point_set_C != NULL)
        free_point_set(point_set_C);
    if(input_test_file != NULL) fclose(input_test_file);
    if(output_test_file != NULL) fclose(output_test_file);
}

Test(asserts, graham_scan_test_12pt, .init = setup_convex_hull_test, .fini = teardown_general){
    CGError_t status_read_A = point_set_from_csv_file(point_set_A, input_test_file, CG_INT);
    CGError_t status_read_B = point_set_from_csv_file(point_set_B, output_test_file, CG_INT);
    CGPointSet_t* convex_hull = compute_graham_scan(point_set_A, CG_W_DEGENERACY);
    int compare = compare_point_sets(point_set_B, point_set_C);
    cr_assert(compare == 0, "Graham Scan not computed correctly");
}

