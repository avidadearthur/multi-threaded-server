#define _GNU_SOURCE

#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifndef _TESTING_
#define TEST_ELEMENTS \
    my_element_t *element0 = (my_element_t *) malloc(sizeof(my_element_t)); \
    char *name0; \
    asprintf(&name0,"%s","Arthur"); \
    element0->id = 0; \
    element0->name = name0; \
    my_element_t *element1 = (my_element_t *) malloc(sizeof(my_element_t)); \
    char *name1; \
    asprintf(&name1,"%s","Marie"); \
    element1->id = 1; \
    element1->name = name1; \
    my_element_t *element2 = (my_element_t *) malloc(sizeof(my_element_t)); \
    char *name2; \
    asprintf(&name2,"%s","Paula"); \
    element2->id = 2; \
    element2->name = name2; \
    my_element_t *element3 = (my_element_t *) malloc(sizeof(my_element_t)); \
    char *name3; \
    asprintf(&name3,"%s","Davide"); \
    element3->id = 3; \
    element3->name = name3;
#endif

typedef struct {
    int id;
    char* name;
} my_element_t;


void* element_copy(void * element);
void element_free(void ** element);
int element_compare(void * x, void * y);

void * element_copy(void * element) {
    my_element_t* copy = malloc(sizeof (my_element_t));
    char* new_name;
    asprintf(&new_name,"%s",((my_element_t*)element)->name); //asprintf requires _GNU_SOURCE
    assert(copy != NULL);
    copy->id = ((my_element_t*)element)->id;
    copy->name = new_name;
    return (void *) copy;
}

void element_free(void ** element) {
    free((((my_element_t*)*element))->name);
    free(*element);
    *element = NULL;
}

int element_compare(void * x, void * y) {
    return ((((my_element_t*)x)->id < ((my_element_t*)y)->id) ? -1 : (((my_element_t*)x)->id == ((my_element_t*)y)->id) ? 0 : 1);
}

void setup(void) {
    // Implement pre-test setup
}

void teardown(void) {
    // Implement post-test teardown
}

START_TEST(test_ListFree)
    {
        // Test free NULL, don't use callback
        dplist_t *list = NULL;
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free NULL, use callback
        list = NULL;
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, don't use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // TODO : Test free with one element, also test if inserted elements are set to NULL

        // TODO : Test free with multiple element, also test if inserted elements are set to NULL

    }
END_TEST

START_TEST(test_dpl_insert_at_index) // tests get_element_at_index indirectely
{
    // TODO: Test Insert at zero in null list
    dplist_t *list = NULL;

    // TODO: Element creation
    TEST_ELEMENTS

    // TODO: Test insert at index 0
    // In this test we're creating deep copies but the element content should still be the same.
    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, element0, 0, true); // Arthur
    int result = element_compare(dpl_get_element_at_index(list, 0), element0);
    ck_assert_msg(result == 0, "t0: Failure: expected result to be 0");

    // TODO: Test insert at size-1 > index > 0
    dpl_insert_at_index(list, element1, 1, true); // Arthur Marie
    result = element_compare(dpl_get_element_at_index(list, 1), element1);
    ck_assert_msg(result == 0, "t1: Failure: expected result to be 0");

    // TODO: Test insert at index -1
    dpl_insert_at_index(list, element2, -1, true); // Paula Arthur Marie
    result = element_compare(dpl_get_element_at_index(list, -1), element2);
    ck_assert_msg(result == 0, "t1: Failure: expected result to be 0");

    // TODO: Test insert at index  99
    dpl_insert_at_index(list, element3, 99, true); // Paula Arthur Marie Davide
    result = element_compare(dpl_get_element_at_index(list, 99), element3);
    ck_assert_msg(result == 0, "t1: Failure: expected result to be 0");


}
END_TEST

START_TEST(test_dpl_size) // tests insert_at_index indirectely
{
    // TODO: Test get size of null list
    dplist_t *list = NULL;
    ck_assert_msg(dpl_size(list) == -1, "t0: Failure: expected result to be -1");

    // TODO: Test get size of empty list
    list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_size(list) == 0, "t1: Failure: expected result to be 0");

    // TODO: Element creation
    TEST_ELEMENTS

    // TODO: Test get size of on element list
    dpl_insert_at_index(list, element0, 0, true); // Arthur
    ck_assert_msg(dpl_size(list) == 1, "t2: Failure: expected result to be 1");

    // TODO: Test insert at size-1 > index > 0
    dpl_insert_at_index(list, element1, 1, true); // Arthur Marie
    ck_assert_msg(dpl_size(list) == 2, "t3: Failure: expected result to be 2");

    // TODO: Test insert at index -1
    dpl_insert_at_index(list, element2, -1, true); // Paula Arthur Marie
    ck_assert_msg(dpl_size(list) == 3, "t4: Failure: expected result to be 3");

    // TODO: Test insert at index  99
    dpl_insert_at_index(list, element3, 99, true); // Paula Arthur Marie Davide
    ck_assert_msg(dpl_size(list) == 4, "t5: Failure: expected result to be 4");


}
END_TEST

START_TEST(test_dpl_get_reference_at_index) // tests get_element_at_ref and
                                            // dpl_get_reference_at_index indirectely
{
    // TODO: Test get reference of null list
    dplist_t *list = NULL;
    ck_assert_msg(dpl_get_reference_at_index(list,0) == NULL, "t0: Failure: expected result to be NULL");

    // TODO: Test get reference of empty list
    list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_get_reference_at_index(list,0) == NULL, "t1: Failure: expected result to be NULL");

    // TODO: Element creation
    TEST_ELEMENTS

    // TODO: Test get referece of element at index 0
    dpl_insert_at_index(list, element0, 0, false); // Arthur
    dplist_node_t * ref_at0 = dpl_get_reference_at_index(list,0);
    ck_assert_msg(dpl_get_element_at_reference(list, ref_at0) == element0, "t2: Failure: expected result to be element0");

    // TODO: Test get ref at size-1 > index > 0
    dpl_insert_at_index(list, element1, 1, false); // Arthur Marie
    dplist_node_t * ref_at1 = dpl_get_reference_at_index(list,1);
    ck_assert_msg(dpl_get_element_at_reference(list, ref_at1) == element1, "t3: Failure: expected result to be element1");

    // TODO: Test get ref at index -1
    dpl_insert_at_index(list, element2, -1, false); // Paula Arthur Marie
    dplist_node_t * ref_at2 = dpl_get_reference_at_index(list,-1);
    ck_assert_msg(dpl_get_element_at_reference(list, ref_at2) == element2, "t4: Failure: expected result to be element2");

    // TODO: Test insert at index  99
    dpl_insert_at_index(list, element3, 99, false); // Paula Arthur Marie Davide
    dplist_node_t * ref_at3 = dpl_get_reference_at_index(list,99);
    ck_assert_msg(dpl_get_element_at_reference(list, ref_at3) == element3, "t5: Failure: expected result to be element3");

}
END_TEST


START_TEST(test_dpl_get_element_at_reference) // tests dpl_get_reference_at_index indirectely

{
    // TODO: Element creation
    TEST_ELEMENTS

    // TODO: Test get element at reference of null list
    dplist_t *list_A = NULL;
    list_A = dpl_create(element_copy, element_free, element_compare);
    dplist_t *list_B = NULL;

    dpl_insert_at_index(list_A, element0, 0, false); // Arthur
    dplist_node_t * ref_at0_A = dpl_get_reference_at_index(list_A,0);
    ck_assert_msg(dpl_get_element_at_reference(list_B,ref_at0_A) == NULL, "t0: Failure: expected result to be NULL");

    // TODO: Test get element at reference of empty list
    list_B = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_get_element_at_reference(list_B,ref_at0_A) == NULL, "t1: Failure: expected result to be NULL");

    // TODO: Test get element at reference that doesn't belong to the list
    dpl_insert_at_index(list_B, element1, 0, false); // Marie
    ref_at0_A = dpl_get_reference_at_index(list_A,0);
    ck_assert_msg(dpl_get_element_at_reference(list_B,ref_at0_A ) == NULL, "t2: Failure: expected result to be NULL");

    // TODO: Test get element at reference that belong to the list
    dpl_insert_at_index(list_A, element3, 1, false); // Arthur Davide (deep copy)
    dplist_node_t * ref_at1_A = dpl_get_reference_at_index(list_A,1);
    ck_assert_msg(dpl_get_element_at_reference(list_A,ref_at1_A) == element3, "t3: Failure: expected result to be  element3");

}
END_TEST

START_TEST(test_dpl_get_index_of_element) // tests dpl_insert_at_index indirectely

{
    // TODO: Element creation
    TEST_ELEMENTS

    // TODO: Test get index of element of null list
    dplist_t *list = NULL;
    ck_assert_msg(dpl_get_index_of_element(list, element0) == -1, "t0: Failure: expected result to be -1");

    // TODO: Test get index of element of empty list
    list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_get_index_of_element(list, element0) == -1, "t1: Failure: expected result to be -1");

    // TODO: Test get index of element inserted at index 0
    dpl_insert_at_index(list, element0, 0, true); // Arthur
    ck_assert_msg(dpl_get_index_of_element(list, element0) == 0, "t2: Failure: expected result to be 0");
    ck_assert_msg(dpl_get_index_of_element(list, element1) == -1, "t3: Failure: expected result to be -1");
    ck_assert_msg(dpl_get_index_of_element(list, element2) == -1, "t4: Failure: expected result to be -1");
    ck_assert_msg(dpl_get_index_of_element(list, element3) == -1, "t5: Failure: expected result to be -1");

    // TODO: Test get index of element inserted at size-1 > index > 0
    dpl_insert_at_index(list, element1, 1, false); // Arthur Marie
    ck_assert_msg(dpl_get_index_of_element(list, element0) == 0, "t6: Failure: expected result to be 0");
    ck_assert_msg(dpl_get_index_of_element(list, element1) == 1, "t7: Failure: expected result to be 1");
    ck_assert_msg(dpl_get_index_of_element(list, element2) == -1, "t8: Failure: expected result to be -1 but result was %d",
                  dpl_get_index_of_element(list, element2));
    ck_assert_msg(dpl_get_index_of_element(list, element3) == -1, "t9: Failure: expected result to be -1");

    // TODO: Test get index of element inserted at index -1
    dpl_insert_at_index(list, element2, -1, true); // Paula Arthur Marie
    ck_assert_msg(dpl_get_index_of_element(list, element0) == 1, "t10: Failure: expected result to be 1");
    ck_assert_msg(dpl_get_index_of_element(list, element1) == 2, "t11: Failure: expected result to be 2");
    ck_assert_msg(dpl_get_index_of_element(list, element2) == 0, "t12: Failure: expected result to be 0");
    ck_assert_msg(dpl_get_index_of_element(list, element3) == -1, "t13: Failure: expected result to be -1");

    // TODO: Test get index of element inserted at  index  99
    dpl_insert_at_index(list, element3, 99, true); // Paula Arthur Marie Davide
    ck_assert_msg(dpl_get_index_of_element(list, element0) == 1, "t14: Failure: expected result to be 1");
    ck_assert_msg(dpl_get_index_of_element(list, element1) == 2, "t15: Failure: expected result to be 2");
    ck_assert_msg(dpl_get_index_of_element(list, element2) == 0, "t16: Failure: expected result to be 0");
    ck_assert_msg(dpl_get_index_of_element(list, element3) == 3, "t17: Failure: expected result to be 3");


}
END_TEST

START_TEST(test_dpl_remove_at_index) // tests  indirectely

{
    // TODO: remove_at_index from NULL list
    dplist_t *list = NULL;
    dpl_remove_at_index(list, 0, false);

    // TODO: remove_at_index from empty list
    list = dpl_create(element_copy, element_free, element_compare);
    dpl_remove_at_index(list, 0, false);

    // TODO: Element creation
    TEST_ELEMENTS
    //   0      1      2    3
    // Paula Arthur Marie Davide
    // TODO: populate list with elements (not deep copies)
    dpl_insert_at_index(list, element0, 0, false); // Arthur
    dpl_insert_at_index(list, element1, 1, false); // Arthur Marie
    dpl_insert_at_index(list, element2, -1, false); // Paula Arthur Marie
    dpl_insert_at_index(list, element3, 99, false); // Paula Arthur Marie Davide

    // Test remove_at_index, element at index 0
    dplist_node_t * old_ref_at0 = dpl_get_reference_at_index(list,0);
    dpl_remove_at_index(list, 0, false); // Arthur Marie Davide --- & element2 != NULL
    dplist_node_t * ref_at0 = dpl_get_reference_at_index(list,0);
    ck_assert_msg(dpl_get_element_at_reference(list,ref_at0) == element0, "t0: Failure: expected result to be  element0");
    ck_assert_msg(element2 != NULL, "t1: Failure: expected result to be != NULL");
    // Test to see if old ref doesn't belong anymore
    ck_assert_msg(dpl_get_element_at_reference(list, old_ref_at0) == NULL, "t2: Failure: expected old_ref_at0 to be NULL");


    // TODO: Test remove_at_index, element at index -1
    dpl_insert_at_index(list, element2, 0, false); // Paula Arthur Marie Davide
    dpl_remove_at_index(list, -1, false); // Arthur Marie Davide --- & element2 != NULL
    ref_at0 = dpl_get_reference_at_index(list,-1);
    ck_assert_msg(dpl_get_element_at_reference(list,ref_at0) == element0, "t3: Failure: expected result to be  element0");
    ck_assert_msg(element2 != NULL, "t4: Failure: expected result to be != NULL");

    dpl_remove_at_index(list, -1, false); // Marie Davide --- & element2 != NULL, element0 != NULL
    ref_at0 = dpl_get_reference_at_index(list,0);
    ck_assert_msg(dpl_get_element_at_reference(list,ref_at0) == element1, "t5: Failure: expected result to be  element1");
    ck_assert_msg(element0 != NULL, "t6: Failure: expected result to be != NULL");

    // TODO: Test remove_at_index, element at index  99
    dpl_remove_at_index(list, 99, false); // Marie --- & element2 != NULL, element0 != NULL , element3 != NULL
    ref_at0 = dpl_get_reference_at_index(list,0);
    ck_assert_msg(dpl_get_element_at_reference(list,ref_at0) == element1, "t7: Failure: expected result to be  element1");
    ck_assert_msg(element3 != NULL, "t8: Failure: expected result to be != NULL");

    dpl_remove_at_index(list, 0, false);
    // TODO: perv tests but with free_elements
    dpl_insert_at_index(list, element0, 0, false); // Arthur
    dpl_insert_at_index(list, element1, 1, false); // Arthur Marie
    dpl_insert_at_index(list, element2, -1, false); // Paula Arthur Marie
    dpl_insert_at_index(list, element3, 99, false); // Paula Arthur Marie Davide

    // Test remove_at_index, element at index 0
    dpl_remove_at_index(list, 0, true); // Arthur Marie Davide --- & element2 != NULL
    dpl_remove_at_index(list, -1, true); // Marie Davide --- & element2 != NULL, element0 != NULL
    // Test remove_at_index, element at index  99
    dpl_remove_at_index(list, 99, true); // Marie --- & element2 != NULL, element0 != NULL , element3 != NULL
    dpl_remove_at_index(list, 0, false);

}
END_TEST

//START_TEST(test_nameOfYourTest)
//  Add other testcases here...
//END_TEST

int main(void) {
    Suite *s1 = suite_create("LIST_EX3");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_checked_fixture(tc1_1, setup, teardown);
    tcase_add_test(tc1_1, test_ListFree);
    // Add other tests here...
    tcase_add_test(tc1_1, test_dpl_insert_at_index);
    tcase_add_test(tc1_1, test_dpl_size);
    tcase_add_test(tc1_1, test_dpl_get_reference_at_index);
    tcase_add_test(tc1_1, test_dpl_get_element_at_reference);
    tcase_add_test(tc1_1, test_dpl_get_index_of_element);
    tcase_add_test(tc1_1, test_dpl_remove_at_index);

    srunner_run_all(sr, CK_VERBOSE);

    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
