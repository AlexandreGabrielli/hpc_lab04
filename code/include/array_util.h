/* Xavier Ruppen / REDS / HEIG-VD
 * HPC - 17.02.16
 * array_util.h
 *
 * Array API
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/* Allocate an array of size "len" and fill it
 * with random data.
 * Return the array pointer */
uint64_t *array_init(size_t len);

/* Liberate array memory */
void array_clear(uint64_t *data);

/* Arrange a array in increasing order of value */
void array_sort(int *tableau, int taille) ;

/*this function print the array */
void print_array(uint64_t *data, size_t len);
