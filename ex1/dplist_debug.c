/**
 * \author Arthur Tavares Quintao
 */

#include "dplist.h"
#include <stdlib.h>
#include <stdio.h>


void debug_dpl_get_index_of_element(){

    printf( "*** Debugging dpl_get_index_of_element ***\n");

    dplist_t *list = NULL;
    // TODO: If 'list' is is NULL, -1 is returned.
    dpl_get_index_of_element(list,'A');

    //TODO:  If 'element' is not found in the list, -1 is returned.
    list = dpl_create();
    dpl_get_index_of_element(list,'A');

    //TODO: the first list node has index 0.
    dpl_insert_at_index(list, 'B', 1);
    dpl_insert_at_index(list, 'C', 2);
    dplist_t *result = dpl_insert_at_index(list, 'A', 0);

    dpl_get_index_of_element(list,'A');
    dpl_get_index_of_element(list,'B');
    dpl_get_index_of_element(list,'C');

}

void debug_dpl_remove_at_index(){
    printf( "*** Debugging dpl_remove_at_index ***\n");

    dplist_t *list = NULL;
    //TODO: If 'list' is is NULL, NULL is returned.
    dpl_remove_at_index(list,0);

    //TODO: If the list is empty, return the unmodified list.
    list = dpl_create();
    dpl_remove_at_index(list,0);

    //TODO: If 'index' is 0 or negative, the first list node is removed.
    //TODO: If 'index' is bigger than the number of elements in the list, the last list node is removed.

    dpl_insert_at_index(list, 'B', 1);

    dpl_insert_at_index(list, 'A', 0);
    dpl_insert_at_index(list, 'C', 2);
    dplist_t *result = dpl_insert_at_index(list, 'D', 3);

    // A = B = C = D

    dpl_remove_at_index(list,-56); // should remove A
    dpl_remove_at_index(list,88); // should remove D

    // B = C
    dpl_remove_at_index(list,0); // should remove B
    dpl_remove_at_index(list,1); // should remove C


}

void debug_dpl_free(){
    // TODO : Test free with one element
    dplist_t *list = NULL;
    list = dpl_create();
    dpl_insert_at_index(list, 'B', 1);
    dpl_free(&list);

    // TODO : Test free with multiple element
    list = dpl_create();
    dpl_insert_at_index(list, 'A', 0);
    dpl_insert_at_index(list, 'D', 3);
    dpl_insert_at_index(list, 'C', 2);
    dpl_free(&list);

}

int main(void) {

    //debug_dpl_get_index_of_element;
    //debug_dpl_remove_at_index();
    debug_dpl_free();

    return 0;
}
