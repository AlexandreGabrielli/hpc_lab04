#include <list_util.h>

/* Link list node */
struct Node {
    uint64_t data;
    struct Node* next;
};


/* Allocate "len" linked elements and initialize them
 * with random data.
 * Return list head */
struct Node *list_init(size_t len) {
    srand((unsigned) 1991);
    struct Node *head = (struct Node *) malloc(sizeof(struct Node));
    head->data = rand();
    head->next = NULL;
    int i;
    struct Node *current = head;
    for (i = 0; i < len; i++) {
        current->next = (struct Node *) malloc(sizeof(struct Node));
        current = current->next;
        current->next = NULL;
        current->data = rand();
    }
    return head;
}

/* Liberate list memory */
void list_clear(struct Node *head) {
    struct Node *next = head;
    struct Node *nextNext;
    while (next != NULL) {
        nextNext = next->next;
        free(next);
        next = nextNext;
    }
}


/* function prototypes */
struct Node* SortedMerge(struct Node* a, struct Node* b);

void FrontBackSplit(struct Node* source,
                    struct Node** frontRef, struct Node** backRef);

/* sorts the linked list by changing next pointers (not data) */
void list_sort(struct Node** headRef)
{
    struct Node* head = *headRef;
    struct Node* a;
    struct Node* b;

    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);

    /* Recursively sort the sublists */
    list_sort(&a);
    list_sort(&b);

    /* answer = merge the two sorted lists together */
    *headRef = SortedMerge(a, b);
}

/* See https:// www.geeksforgeeks.org/?p=3622 for details of this
function */
struct Node* SortedMerge(struct Node* a, struct Node* b)
{
    struct Node* result = NULL;

    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    /* Pick either a or b, and recur */
    if (a->data <= b->data) {
        result = a;
        result->next = SortedMerge(a->next, b);
    }
    else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}

/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
    and return the two lists using the reference parameters.
    If the length is odd, the extra node should go in the front list.
    Uses the fast/slow pointer strategy. */
void FrontBackSplit(struct Node* source,
                    struct Node** frontRef, struct Node** backRef)
{
    struct Node* fast;
    struct Node* slow;
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
