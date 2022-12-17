/**
 * \author Arthur Tavares Quintao
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"

/*
 * definition of error codes
 * */
#define DPLIST_NO_ERROR 0
#define DPLIST_MEMORY_ERROR 1 // error due to mem alloc failure
#define DPLIST_INVALID_ERROR 2 //error due to a list operation applied on a NULL list

#ifdef DEBUG
#define DEBUG_PRINTF(...) 									                                        \
        do {											                                            \
            fprintf(stderr,"\nIn %s - function %s at line %d: ", __FILE__, __func__, __LINE__);	    \
            fprintf(stderr,__VA_ARGS__);								                            \
            fflush(stderr);                                                                         \
                } while(0)
#else
#define DEBUG_PRINTF(...) (void)0
#endif


#define DPLIST_ERR_HANDLER(condition, err_code)                         \
    do {                                                                \
            if ((condition)) DEBUG_PRINTF(#condition " failed\n");      \
            assert(!(condition));                                       \
        } while(0)


/*
 * The real definition of struct list / struct node
 */

struct dplist_node {
    dplist_node_t *prev, *next;
    void *element;
};

struct dplist {
    dplist_node_t *head;

    void (*element_free)(void **element);

    int (*element_compare)(void *x, void *y);
};


dplist_t *dpl_create(// callback functions
        void *(*element_copy)(void *src_element),
        void (*element_free)(void **element),
        int (*element_compare)(void *x, void *y)
) {
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_MEMORY_ERROR);
    list->head = NULL;
    list->element_free = element_free;
    list->element_compare = element_compare;
    return list;
}

void dpl_free(dplist_t **list, bool free_element) {

    if(*list != NULL){
        if((*list)->head != NULL){
            int size = dpl_size(*list);
            while(size != 0){
                dpl_remove_at_index(*list, size-1, free_element);
                size = dpl_size(*list);
            }
            free(*list);
        }
        *list = NULL;
    }
}

dplist_t *dpl_insert_at_index(dplist_t *list, void *element, int index, bool insert_copy) {

    // If 'list' is NULL, NULL is returned
    if (list == NULL) return NULL;

    dplist_node_t *ref_at_index, *list_node;
    list_node = malloc(sizeof(dplist_node_t));
    DPLIST_ERR_HANDLER(list_node == NULL, DPLIST_MEMORY_ERROR);

    list_node->element = element;

    // pointer drawing breakpoint
    if (list->head == NULL) { // covers case 1: If list doesn't have elements head point to first node
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
        // pointer drawing breakpoint
    }
    else if (index <= 0) { // covers case 2: If 'index' is 0 or negative, the list node is inserted at the start of 'list'.
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
        // pointer drawing breakpoint
    }
    else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        // pointer drawing breakpoint
        if (index < dpl_size(list)) { // covers case 4
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
            // pointer drawing breakpoint
        }
        else { // covers case 3: If 'index' is bigger than the number of elements in the list, the list node is inserted at the end of the list.
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
            // pointer drawing breakpoint
        }
    }

    return list;

}

dplist_t *dpl_remove_at_index(dplist_t *list, int index, bool free_element) {

    if (list == NULL) return NULL;

    if (list->head == NULL) return list;

    dplist_node_t *ref_del_at_index;
    ref_del_at_index = dpl_get_reference_at_index(list, index);

    dplist_node_t *prev_node = ref_del_at_index->prev;
    dplist_node_t *next_node = ref_del_at_index->next;

    if(free_element && ref_del_at_index->element != NULL){
        list->element_free(&(ref_del_at_index->element));
    }

    // index == 0 (deleting the first node)
    if (prev_node == NULL){
        list->head = next_node;
        // for when there's only one element left
        if (next_node != NULL) next_node->prev = NULL;
    }
    else if (next_node == NULL){
        prev_node->next = NULL;
    }
    else{
        prev_node->next = next_node;
        next_node->prev = prev_node;
    }

    free(ref_del_at_index);

    return list;

}

int dpl_size(dplist_t *list) {

    int count = 0;
    if (list == NULL) return -1;

    dplist_node_t *dummy;
    dummy = list->head;

    while(dummy != NULL){
        count++;
        dummy = dummy->next;
    }
    return count;

}

void *dpl_get_element_at_reference(dplist_t *list, dplist_node_t *reference) {
    if (list == NULL || list->head == NULL) return NULL;
    else{
        // If 'reference' is not an existing reference in the list, NULL is returned.
        dplist_node_t *current;
        current = list->head;
        for(int i=0; i< dpl_size(list); i++){
            if(current == reference){
                return reference->element;
            }
            current = current->next;
        }
    }
    return NULL;
}

void *dpl_get_element_at_index(dplist_t *list, int index) {

    dplist_node_t * ref_at_index = dpl_get_reference_at_index(list,index);
    return dpl_get_element_at_reference(list, ref_at_index);
}

int dpl_get_index_of_element(dplist_t *list, void *element) {

    dplist_node_t *current;
    if (list == NULL || list->head == NULL) return -1;
    current = list->head;
    for(int i=0; i< dpl_size(list); i++){
        void *element_at_index = dpl_get_element_at_index(list, i);
        if(element_at_index == NULL) return -1;
            // Use 'element_compare()' to search 'element' in the list, a match is found when 'element_compare()' returns 0
        else if(list->element_compare(element,element_at_index) == 0) return i;
        current = current->next;
    }
    return -1;

}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index) {

    int count;
    dplist_node_t *dummy;

    if (list == NULL || list->head == NULL) return NULL;
    DPLIST_ERR_HANDLER(list == NULL, DPLIST_INVALID_ERROR);

    for (dummy = list->head, count = 0; dummy->next != NULL; dummy = dummy->next, count++) {
        if (count >= index) return dummy;
    }

    return dummy;

}