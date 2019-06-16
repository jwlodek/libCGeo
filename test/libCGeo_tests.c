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

#include "libCGeo/libCGeo.h"
#include <criterion/criterion.h>
#include <criterion/assert.h>


CGPointSet_t* point_set_A;
CGPointSet_t* point_set_B;
CGPointSet_t* point_set_C;

FILE* input_test_file;
FILE* output_test_file1;
FILE* output_test_file2;


/* Setup function that initializes test file and 3 point sets */
void setup_3_points(void){
    input_test_file = fopen("/home/jwlodek/Documents/ProgrammingWorkspace/libCGeo/test/test_inputs/test1.csv", "r");
    point_set_A = init_point_set();
    point_set_B = init_point_set();
    point_set_C = init_point_set();
}


/* Setup function that initializes in/out test file and 3 point sets with 12 points each */
void setup_12_points(void){
    input_test_file = fopen("/home/jwlodek/Documents/ProgrammingWorkspace/libCGeo/test/test_inputs/input_12pts.csv", "r");
    output_test_file1 = fopen("/home/jwlodek/Documents/ProgrammingWorkspace/libCGeo/test/test_outputs/output_12pts_sortX.csv", "r");
    output_test_file2 = fopen("/home/jwlodek/Documents/ProgrammingWorkspace/libCGeo/test/test_outputs/output_12pts_sortD.csv", "r");
    point_set_A = init_point_set();
    point_set_B = init_point_set();
    point_set_C = init_point_set();
}


/* Function that performs memory cleanup after every test */
void teardown_general(void){
    free_point_set(point_set_A);
    free_point_set(point_set_B);
    free_point_set(point_set_C);
    if(input_test_file != NULL) fclose(input_test_file);
    if(output_test_file1 != NULL) fclose(output_test_file1);
    if(output_test_file2 != NULL) fclose(output_test_file2);
}


/* Test for parsing from .csv file */
Test(asserts, input_parse_test, .init = setup_3_points, .fini = teardown_general){
    add_coords_to_set(point_set_A, -3, 7);
    add_coords_to_set(point_set_A, 5, 9);
    add_coords_to_set(point_set_A, 4, 3);
    CGError_t status = point_set_from_csv_file(point_set_B, input_test_file);
    cr_assert(status == CG_SUCCESS, "Error in parsing csv file");
    int compare = compare_point_sets(point_set_A, point_set_B);
    cr_assert(compare == 0, "Parsed point set not as expected");
}


/* Test for finding turn type between points */
Test(asserts, turn_type_test, .init = setup_3_points, .fini = teardown_general){
    CGError_t status = point_set_from_csv_file(point_set_A, input_test_file);
    cr_assert(status == CG_SUCCESS, "Error in parsing csv file");
    CGTurn_t turn = find_turn_type(point_set_A->head->point, point_set_A->head->next->point, point_set_A->head->next->next->point);
    cr_assert(turn == CG_TURN_RIGHT, "Right Turn not found correctly");
    turn = find_turn_type(point_set_A->head->next->next->point, point_set_A->head->next->point, point_set_A->head->point);
    cr_assert(turn == CG_TURN_LEFT, "Left Turn not found correctly");
}


/* Test for finding lowest point in set */
Test(asserts, lowest_point_test, .init = setup_3_points, .fini = teardown_general){
    CGError_t status = point_set_from_csv_file(point_set_A, input_test_file);
    cr_assert(status == CG_SUCCESS, "Error in parsing csv file");
    CGPoint_t temp;
    temp.xcoord = 4;
    temp.ycoord = 3;
    CGPoint_t* point = find_lowest_point_in_set(point_set_A);
    int compare = compare_points(point, &temp);
    cr_assert(compare == 0, "Lowest point not found successfully");
}


/* Test for finding distance between points */
Test(asserts, distance_between_points, .init = setup_3_points, .fini = teardown_general){
    CGError_t status = point_set_from_csv_file(point_set_A, input_test_file);
    cr_assert(status == CG_SUCCESS, "Error in parsing csv file");
    double expected_distance = 8.246211;
    double calculated_distance = distance_between(point_set_A->head->point, point_set_A->head->next->point);
    cr_assert(fabs(expected_distance - calculated_distance) < FLOAT_TOLERANCE, "Distance between points not found correctly");
}


/* Test for sorting points by x/y-coord */
Test(asserts, sort_coord_x, .init = setup_12_points, .fini = teardown_general){
    CGError_t status_in = point_set_from_csv_file(point_set_A, input_test_file);
    CGError_t status_out = point_set_from_csv_file(point_set_B, output_test_file1);
    CGPointNode_t* current_node = point_set_A->head;
    while(current_node != NULL){
        current_node->point->sort_val = current_node->point->xcoord;
        current_node->point->sort_val_desc = "x-coord";
        current_node = current_node->next;
    }
    CGError_t status_sort = sort_point_set(point_set_A, NULL);
    int compare = compare_point_sets(point_set_A, point_set_B);
    cr_assert(compare == 0, "Points not sorted correctly");
}


/* Test for sorting points by distance to lowest point */
Test(asserts, sort_coord_dist, .init = setup_12_points, .fini = teardown_general){
    CGError_t status_in = point_set_from_csv_file(point_set_A, input_test_file);
    CGError_t status_out = point_set_from_csv_file(point_set_B, output_test_file2);
    CGPoint_t* point = find_lowest_point_in_set(point_set_A);
    CGPointNode_t* current_node = point_set_A->head;
    while(current_node != NULL){
        current_node->point->sort_val = distance_between(current_node->point, point);
        current_node->point->sort_val_desc = "distance to lowest";
        current_node = current_node->next;
    }
    CGError_t status_sort = sort_point_set(point_set_A, NULL);
    int compare = compare_point_sets(point_set_A, point_set_B);
    cr_assert(compare == 0, "Points not sorted correctly");
}
