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
 * Unit tests for the common functionality for libCGeo used by multiple modules.
 */

#include "libCGeo.h"
#include <criterion/criterion.h>
#include <criterion/assert.h>


CGPointSet_t* point_set_A;
CGPointSet_t* point_set_B;
CGPointSet_t* point_set_C;

FILE* input_test_file;


void setup_3_points(void){
    input_test_file = fopen("/home/jwlodek/Documents/ProgrammingWorkspace/libCGeo/test/test_inputs/test1.csv", "r");
    point_set_A = init_point_set(3);
    point_set_B = init_point_set(3);
    point_set_C = init_point_set(3);
}

void teardown_general(void){
    free_point_set(point_set_A);
    free_point_set(point_set_B);
    free_point_set(point_set_C);
    if(input_test_file != NULL) fclose(input_test_file);
}


Test(asserts, input_parse_test, .init = setup_3_points, .fini = teardown_general){
    point_set_A->points[0].xcoord = -3;
    point_set_A->points[0].ycoord = 7;
    point_set_A->points[1].xcoord = 5;
    point_set_A->points[1].ycoord = 9;
    point_set_A->points[2].xcoord = 4;
    point_set_A->points[2].ycoord = 3;
    int i;
    for(i = 0; i< point_set_A->num_points; i++)
        point_set_A->points[i].type = CG_INT;
    //CGError_t status = point_set_from_csv_file(point_set_B, input_test_file, CG_INT);
    //cr_assert(status == CG_SUCCESS, "Error in parsing csv file");
    print_points(point_set_A);
    print_points(point_set_B);
}



