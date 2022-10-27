/**
 * \author Arthur Tavares Quintao
 */

#include "dplist.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>

void setup(void) {
    // Implement pre-test setup
}

void teardown(void) {
    // Implement post-test teardown
}
START_TEST(test_ListFree)
    {
        //t0 Test free NULL
        dplist_t *list = NULL;
        dpl_free(&list);
        ck_assert_msg(list == NULL, "t0 Failure: expected result to be NULL");

        //t1 Test free empty list
        list = dpl_create();
        dpl_free(&list);
        ck_assert_msg(list == NULL, "t1 Failure: expected result to be NULL");

        // TODO : Test free with one element

        // TODO : Test free with multiple element

    }
END_TEST

START_TEST(test_ListInsertAtIndexListNULL)
    {
        // Test inserting at index -1
        dplist_t *result = dpl_insert_at_index(NULL, 'A', -1);
        ck_assert_msg(result == NULL, "Failure: expected list to be NULL");
        // TODO : Test inserting at index 0

        // TODO : Test inserting at index 99
    }
END_TEST

START_TEST(test_ListInsertAtIndexListEmpty)
{
    // Test inserting at index -1
    dplist_t *list = dpl_create();
    dplist_t *result = dpl_insert_at_index(list, 'A', -1);
    ck_assert_msg(dpl_size(result) == 1, "Failure: expected list to have size of 1, got a size of %d",
                                         dpl_size(result));
    dpl_free(&list);
    // TODO : Test inserting at index 0

    // TODO : Test inserting at index 99
}
END_TEST

START_TEST(test_dpl_get_element_at_index)
{
    dplist_t *list = dpl_create();
    dpl_insert_at_index(list, 'B', 1);
    dpl_insert_at_index(list, 'C', 2);
    dplist_t *result = dpl_insert_at_index(list, 'A', 0);
    ck_assert_msg(dpl_get_element_at_index(result,0) == 'A', "t0 Failure: expected list to have 'A' at pos 0");
    ck_assert_msg(dpl_get_element_at_index(result,1) == 'B', "t0 Failure: expected list to have 'B' at pos 0");

    // If 'index' is 0 or negative, the element of the first list node is returned.
    ck_assert_msg(dpl_get_element_at_index(result,-8) == 'A', "t1 Failure: expected to return first element for negative index");
    // If 'index' is bigger than the number of elements in the list, the element of the last list node is returned.
    ck_assert_msg(dpl_get_element_at_index(result,6) == 'C', "t2 Failure: expected to return last element for index out of bounds");
    // TODO : If the list is empty, 0 is returned.
    // TODO : If 'list' is NULL, 0 is returned.
}
END_TEST

START_TEST(test_dpl_size)
{
    dplist_t *list = NULL;
    // TODO: If 'list' is is NULL, -1 is returned.
    ck_assert_msg(dpl_size(list) == -1, "t0 Failure: Empty list - expected to return -1, got a size of %d",
    dpl_size(list));

    list = dpl_create();
    ck_assert_msg(dpl_size(list) == 0, "t1 Failure: expected to return 0, got a size of %d",
    dpl_size(list));

    dpl_insert_at_index(list, 'B', 1);
    ck_assert_msg(dpl_size(list) == 1, "t2 Failure: expected to return 1, got a size of %d",
    dpl_size(list));

    dpl_insert_at_index(list, 'C', 2);
    dplist_t *result = dpl_insert_at_index(list, 'A', 0);

    ck_assert_msg(dpl_size(result) == 3, "t2 Failure: expected to return 3, got a size of %d",
                  dpl_size(result));

    dpl_free(&list);


}
END_TEST

//START_TEST(test_nameOfYourTest)
//  Add other testcases here...
//END_TEST

int main(void) {
    Suite *s1 = suite_create("LIST_EX1");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_checked_fixture(tc1_1, setup, teardown);
    tcase_add_test(tc1_1, test_ListFree);
    tcase_add_test(tc1_1, test_ListInsertAtIndexListNULL);
    tcase_add_test(tc1_1, test_ListInsertAtIndexListEmpty);
    // Add other tests here...
    tcase_add_test(tc1_1, test_dpl_get_element_at_index);
    tcase_add_test(tc1_1, test_dpl_size);

    srunner_run_all(sr, CK_VERBOSE);

    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
