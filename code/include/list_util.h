/* Xavier Ruppen / REDS / HEIG-VD
 * HPC - 17.02.16
 * array_util.h
 *
 * List API
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


/* A list_element must at least contain a link to the next
 * element, as well as a uint64_t data value */
struct Node;

/* Allocate "len" linked elements and initialize them
 * with random data.
 * Return list head */
struct Node *list_init(size_t len);

/* Liberate list memory */
void list_clear(struct Node *head);

/* Arrange a list in increasing order of value */
void list_sort(struct Node *head);

/*this function print the list*/
void print_list(struct Node *head);
