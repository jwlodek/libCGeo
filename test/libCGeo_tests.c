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


/* Test for parsing from .csv file */
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
    CGError_t status = point_set_from_csv_file(point_set_B, input_test_file, CG_INT);
    cr_assert(status == CG_SUCCESS, "Error in parsing csv file");
    int compare = compare_point_sets(point_set_A, point_set_B);
    cr_assert(compare == 0, "Parsed point set not as expected");
}


/* Test for finding turn type between points */
Test(asserts, turn_type_test, .init = setup_3_points, .fini = teardown_general){
    CGError_t status = point_set_from_csv_file(point_set_A, input_test_file, CG_INT);
    cr_assert(status == CG_SUCCESS, "Error in parsing csv file");
    CGTurn_t turn = find_turn_type(&(point_set_A->points[0]), &(point_set_A->points[1]), &(point_set_A->points[2]));
    cr_assert(turn == CG_TURN_RIGHT, "Right Turn not found correctly");
    turn = find_turn_type(&(point_set_A->points[2]), &(point_set_A->points[1]), &(point_set_A->points[0]));
    cr_assert(turn == CG_TURN_LEFT, "Left Turn not found correctly");
}


/* Test for finding lowest point in set */
Test(asserts, lowest_point_test, .init = setup_3_points, .fini = teardown_general){
    CGError_t status = point_set_from_csv_file(point_set_A, input_test_file, CG_INT);
    cr_assert(status == CG_SUCCESS, "Error in parsing csv file");
    CGPoint_t* temp = (CGPoint_t*) calloc(1, sizeof(CGPoint_t));
    temp->type = CG_INT;
    temp->xcoord = 4;
    temp->ycoord = 3;
    CGPoint_t* point = find_lowest_point_in_set(point_set_A);
    int compare = compare_points(point, temp);
    cr_assert(compare == 0, "Lowest point not found successfully");
    free(temp);
}


/* Test for finding distance between points */
Test(asserts, distance_between_points, .init = setup_3_points, .fini = teardown_general){
    CGError_t status = point_set_from_csv_file(point_set_A, input_test_file, CG_INT);
    cr_assert(status == CG_SUCCESS, "Error in parsing csv file");
    double expected_distance = 0;
    double calculated_distance = distance_between(&(point_set_A->points[0]), &(point_set_A->points[1]));
    cr_assert(expected_distance == calculated_distance, "Distance between points not found correctly");
}
