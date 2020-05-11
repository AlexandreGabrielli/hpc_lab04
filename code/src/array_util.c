#include "array_util.h"
#include <stdbool.h>

/* Liberate array memory */
void array_clear(uint64_t *data) {
    free(data);
}


/* Allocate an array of size "len" and fill it
 * with random data.
 * Return the array pointer */
uint64_t *array_init(const size_t len) {
    srand((unsigned) 1991);
    uint64_t *ptr = (uint64_t *) malloc(len * sizeof(uint64_t));
    int i;
    for (i = 0; i < len; i++) {
        ptr[i] = rand();
    }
    return ptr;
}

void array_sort(uint64_t *tableau, int taille) {
    int mur, courant, pivot, tmp;
    if (taille < 2) return;
    // On prend comme pivot l element le plus a droite
    pivot = tableau[taille - 1];
    mur  = courant = 0;
        while (courant<taille) {
        if (tableau[courant] <= pivot) {
            if (mur != courant) {
                tmp=tableau[courant];
                tableau[courant]=tableau[mur];
                tableau[mur]=tmp;              
            }
            mur ++;
        }
        courant ++;
    }
    array_sort(tableau, mur - 1);
    array_sort(tableau + mur - 1, taille - mur + 1);
}

/*this function print the array */
void print_array(uint64_t *data, const size_t len) {
    printf("[");
    int i = 0;
    do {
        printf("%llu", data[i]);
        i++;
    } while (i < len && printf(","));
    printf("]\n");

}
