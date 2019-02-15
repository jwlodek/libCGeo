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
 * This example program demonstrates how to read and write to .csv (Comma Separated Value)
 * files. This can be useful for integration with other software such as Excel. It also allows for 
 * much easier input of data over initializing point sets manually.
 */

#include <stdio.h>
#include <stdlib.h>
#include "libCGeo/libCGeo.h"

int main(int argc, char** argv){

    if(argc != 2){
        print_cg_error(CG_INVALID_INPUT, main);
        return -1;
    }
    const char* input_file_path = argv[1];

    FILE* input_file = fopen(input_file_path, "r");
    FILE* output_file = fopen("output_A.csv", "w");

    if(input_file == NULL || output_file == NULL){
        print_cg_error(CG_NO_FILE, "main");
        return -1;
    }
    
    CGPointSet_t* point_set = init_point_set(7);
    if(point_set_from_csv_file(point_set, input_file, CG_INT) != CG_SUCCESS){
        print_cg_error(CG_INVALID_INPUT, "point_set_from_csv_file");
        return -1;
    }

    print_points(point_set);

    CGPoint_t* lowest = find_lowest_point_in_set(point_set);
    fprintf("The lowest point in the set is:\n", stdout);
    print_point_to_file(lowest, stdout);
    int i;
    for(i = 0; i < point_set->num_points; i++){
        point_set->points[i].sort_val = distance_between(&(point_set->points[i]), lowest);
        point_set->points[i].sort_val_desc = "Distance to lowest point";
    }

    if(sort_point_set(point_set, NULL) != CG_SUCCESS){
        print_cg_error(CG_INVALID_INPUT, "sort_point_set");
    }

    if(csv_file_from_point_set(point_set, output_file) != CG_SUCCESS){
        print_cg_error(CG_NO_FILE, "csv_file_from_point_set");
        return -1;
    }

    free_point_set(point_set);
    fclose(input_file);
    fclose(output_file);
    return 0;
}