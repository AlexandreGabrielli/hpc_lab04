/* Xavier Ruppen / REDS / HEIG-VD
 * HPC - 17.02.16
 * array_util.h
 *
 * Program entry. Must initialize and sort a list or an array,
 * depending on supplied command line arguments. For example :
 *
 * ./sort list 45000
 * ./sort array 33528
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "array_util.h"
#include "list_util.h"

#define PARAM_COUNT 3
#define PARAM_LIST "list"
#define PARAM_ARRAY "array"

enum sort_enum {
    SORT_ARRAY,
    SORT_LIST
};


int main(int argc, char **argv) {
    enum sort_enum sort_type;
    size_t data_len;

    /* arg parsing */
    if (argc != PARAM_COUNT) {
        fprintf(stderr, "usage: %s list|array data_len\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!strcmp(argv[1], PARAM_ARRAY))
        sort_type = SORT_ARRAY;

    else if (!strcmp(argv[1], PARAM_LIST))
        sort_type = SORT_LIST;

    else {
        fprintf(stderr, "wrong parameter \"%s\"\n", argv[1]);
        return EXIT_FAILURE;
    }

    data_len = atol(argv[2]);

    uint64_t *array;
    struct list_element *liste;
    switch (sort_type) {
        case SORT_ARRAY:
            //initialize the array
            array = array_init(data_len);
            //print_array(array, data_len);
            //sort the array
            array_sort(array, data_len);
            //print_array(array, data_len);
            //clear the array
            array_clear(array);

            break;
        case SORT_LIST:
            //initialize the list
            liste = list_init(data_len);
            //print_list(liste);
            //sort the list
            list_sort(&liste);
            //print_list(liste);
            //clear the list
            list_clear(liste);
            break;
        default:
            printf("something wrong you're not able to reach this part of code");
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
