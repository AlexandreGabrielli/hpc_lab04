#include <list_util.h>

/* A list_element must at least contain a link to the next
 * element, as well as a uint64_t data value */
struct list_element {
    uint64_t data;
    struct list_element *next;
};

/* Allocate "len" linked elements and initialize them
 * with random data.
 * Return list head */
struct list_element *list_init(size_t len) {
    srand((unsigned) 1991);
    struct list_element *head = (struct list_element *) malloc(sizeof(struct list_element));
    head->data = rand();
    head->next = NULL;
    int i;
    struct list_element *current = head;
    for (i = 0; i < len; i++) {
        current->next = (struct list_element *) malloc(sizeof(struct list_element));
        current = current->next;
        current->next = NULL;
        current->data = rand();
    }
    return head;
}

/* Liberate list memory */
void list_clear(struct list_element *head) {
    struct list_element *next = head;
    struct list_element *nextNext;
    while (next != NULL) {
        nextNext = next->next;
        free(next);
        next = nextNext;
    }
}

/*this function print the list*/
void print_list(struct list_element *head) {
    struct list_element *next = head;
    printf("{");
    do {
        printf("%llu", next->data);
        next = next->next;
    } while (next != NULL && printf(","));
    printf("}\n");
}

/* function to swap data of two nodes a and b*/
void swap(struct list_element *a, struct list_element *b) {
    int temp = a->data;
    a->data = b->data;
    b->data = temp;
}

void FrontBackSplit(struct list_element* source,
                    struct list_element** frontRef, struct list_element** backRef)
{
    struct list_element* fast;
    struct list_element* slow;
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

void list_sort(struct list_element *start) {
    struct list_element* head = *start;
    struct list_element* a;
    struct list_element* b;

    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);

    /* Recursively sort the sublists */
    MergeSort(&a);
    MergeSort(&b);

    /* answer = merge the two sorted lists together */
    *start = SortedMerge(a, b);
}