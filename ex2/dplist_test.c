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


        char elementA = 'A';
        char elementB = 'B';
        char elementC = 'C';
        list = dpl_create();
        dpl_insert_at_index(list, &elementB, 1);
        dpl_free(&list);
        ck_assert_msg(list == NULL, "t2 Failure: expected result to be NULL");


        list = dpl_create();
        dpl_insert_at_index(list, &elementA, 0);
        dpl_insert_at_index(list, &elementB, 3);
        dpl_insert_at_index(list, &elementC, 2);
        dpl_free(&list);
        ck_assert_msg(list == NULL, "t3 Failure: expected result to be NULL");

        }
END_TEST

START_TEST(test_ListInsertAtIndexListNULL)
{
    // Test inserting at index -1
    char element = 'A';
    dplist_t *result = dpl_insert_at_index(NULL, &element, -1);
    ck_assert_msg(result == NULL, "Failure: expected list to be NULL");

    result = dpl_insert_at_index(NULL, &element, 0);
    ck_assert_msg(result == NULL, "Failure: expected list to be NULL");


    result = dpl_insert_at_index(NULL, &element, 99);
    ck_assert_msg(result == NULL, "Failure: expected list to be NULL");

    dpl_free(&result);
}
END_TEST

START_TEST(test_ListInsertAtIndexListEmpty)
{
    char elementA = 'A';
    char elementB = 'B';
    char elementC = 'C';
    // Test inserting at index -1
    dplist_t *list = dpl_create();
    dplist_t *result = dpl_insert_at_index(list, &elementA, -1);
    ck_assert_msg(dpl_size(result) == 1, "t0 Failure: expected list to have size of 1, got a size of %d",
                  dpl_size(result));

    dpl_insert_at_index(result, &elementB, 0);
    ck_assert_msg(dpl_size(result) == 2, "t1 Failure: expected list to have size of 2, got a size of %d",
                  dpl_size(result));
    //ck_assert_msg(dpl_get_element_at_index(result,0) == 'B', "t1 Failure: expected list to have 'B' at pos 0");


    dpl_insert_at_index(result, &elementC, 99);
    ck_assert_msg(dpl_size(result) == 3, "t2 Failure: expected list to have size of 3, got a size of %d",
                  dpl_size(result));
    //ck_assert_msg(dpl_get_element_at_index(result,2) == 'C', "t3 Failure: expected list to have 'B' at pos 2");

    dpl_free(&list);

}
END_TEST

START_TEST(test_dpl_get_element_at_index)
{
    char elementA = 'A';
    char elementB = 'B';
    char elementC = 'C';

    dplist_t *list = NULL;
    ck_assert_msg(dpl_get_element_at_index(list,0) == 0, "t0 Failure: 'list' is NULL, expected 0");

    list = dpl_create();
    ck_assert_msg(dpl_get_element_at_index(list,0) == 0, "t1 Failure: 'list' is empty, expected 0");

    dpl_insert_at_index(list, &elementB, 1);
    dpl_insert_at_index(list, &elementC, 2);
    dplist_t *result = dpl_insert_at_index(list, &elementA, 0);
    ck_assert_msg(*dpl_get_element_at_index(result,0) == 'A', "t2 Failure: expected list to have 'A' at pos 0");
    ck_assert_msg(*dpl_get_element_at_index(result,1) == 'B', "t3 Failure: expected list to have 'B' at pos 1");

    // If 'index' is 0 or negative, the element of the first list node is returned.
    ck_assert_msg(*dpl_get_element_at_index(result,-8) == 'A', "t4 Failure: expected to return first element for negative index");
    // If 'index' is bigger than the number of elements in the list, the element of the last list node is returned.
    ck_assert_msg(*dpl_get_element_at_index(result,6) == 'C', "t5 Failure: expected to return last element for index out of bounds");

    dpl_free(&list);
}
END_TEST

START_TEST(test_dpl_size)
{
    char elementA = 'A';
    char elementB = 'B';
    char elementC = 'C';

    dplist_t *list = NULL;

    ck_assert_msg(dpl_size(list) == -1, "t0 Failure: Empty list - expected to return -1, got a size of %d",
                  dpl_size(list));

    list = dpl_create();
    ck_assert_msg(dpl_size(list) == 0, "t1 Failure: expected to return 0, got a size of %d",
                  dpl_size(list));

    dpl_insert_at_index(list, &elementB, 1);
    ck_assert_msg(dpl_size(list) == 1, "t2 Failure: expected to return 1, got a size of %d",
                  dpl_size(list));

    dpl_insert_at_index(list, &elementC, 2);
    dplist_t *result = dpl_insert_at_index(list, &elementA, 0);

    ck_assert_msg(dpl_size(result) == 3, "t2 Failure: expected to return 3, got a size of %d",
                  dpl_size(result));

    dpl_free(&list);
}
END_TEST

START_TEST(test_dpl_get_index_of_element)
{
    char elementA = 'A';
    char elementB = 'B';
    char elementC = 'C';

    dplist_t *list = NULL;

    ck_assert_msg(dpl_get_index_of_element(list,&elementA) == -1, "t0 Failure: NULL list - expected to return -1");

    list = dpl_create();
    ck_assert_msg(dpl_get_index_of_element(list,&elementA) == -1, "t1 Failure: Element 'A' doesn't exist expected to return -1");


    dpl_insert_at_index(list, &elementB, 1);
    dpl_insert_at_index(list, &elementC, 2);
    dplist_t *result = dpl_insert_at_index(list, &elementA, 0);

    ck_assert_msg(dpl_get_index_of_element(list,&elementA) == 0, "t1 Failure: expected 'A' at pos 0, got %d",
                  dpl_get_index_of_element(list,&elementA));

    ck_assert_msg(dpl_get_index_of_element(list,&elementC) == 2, "t1 Failure: expected 'C' at pos 0, got %d",
                  dpl_get_index_of_element(list,&elementC));

    ck_assert_msg(dpl_get_index_of_element(list,&elementB) == 1, "t1 Failure: expected 'B' at pos 0, got %d",
                  dpl_get_index_of_element(list,&elementB));

    dpl_free(&list);

}
END_TEST

START_TEST(test_dpl_remove_at_index)
{

    char elementA = 'A';
    char elementB = 'B';
    char elementC = 'C';
    char elementD = 'D';
    char elementE = 'E';
    char elementF = 'F';

    dplist_t *list = NULL;

    ck_assert_msg(dpl_remove_at_index(list,0) == NULL, "t0 Faliure: expeted NULL");

    list = dpl_create();
    dpl_remove_at_index(list,0);
    ck_assert_msg(dpl_size(dpl_remove_at_index(list,0)) == dpl_size(list), "t0 Faliure: expeted NULL");

    dpl_insert_at_index(list, &elementB, 1);
    dpl_insert_at_index(list, &elementA, 0);
    dpl_insert_at_index(list, &elementD, 3);
    dplist_t *result = dpl_insert_at_index(list, &elementC, 2);

    // 0   1   2   3
    // A = B = C = D

    dpl_remove_at_index(list,-56); // should remove A
    // 0   1   2
    // B = C = D
    ck_assert_msg(*dpl_get_element_at_index(result,0) == 'B', "t0 Failure: expected list to have 'B' at pos 0 but go %c",
                  *dpl_get_element_at_index(result,0));
    ck_assert_msg(*dpl_get_element_at_index(result,2) == 'D', "t1 Failure: expected list to have 'C' at pos 0 but go %c",
                  *dpl_get_element_at_index(result,2));
    dpl_remove_at_index(list,88); // should remove D
    // 0   1
    // B = C
    ck_assert_msg(*dpl_get_element_at_index(result,0) == 'B', "t2 Failure: expected list to have 'B' at pos 0 but go %c",
                  *dpl_get_element_at_index(result,0));
    ck_assert_msg(*dpl_get_element_at_index(result,1) == 'C', "t3 Failure: expected list to have 'C' at pos 0 but go %c",
                  *dpl_get_element_at_index(result,1));

    free(list);
    list = NULL;
    list = dpl_create();

    dpl_insert_at_index(list, &elementA, 0);
    dpl_insert_at_index(list, &elementB, 1);
    dpl_insert_at_index(list, &elementC, 2);
    dpl_insert_at_index(list, &elementD, 3);
    dpl_insert_at_index(list, &elementE, 4);
    dpl_insert_at_index(list, &elementF, 5);

    // 0   1   2   3   4   5
    // A = B = C = D = E = F

    // 0   1   2   3   4   5
    // A =   = C = D = E = F
    result = dpl_remove_at_index(list,1);
    ck_assert_msg(dpl_size(result) == 5, "t6 Faliure: expeted size 5, got size %d",
                  dpl_size(result));
    // 0   1   2   3   4   5
    // A = C = D = E = F
    // A = C = D =  = F
    dpl_remove_at_index(list,3);
    // 0   1   2   3   4   5
    // A = C = D = F
    // A = C = D =
    dpl_remove_at_index(list,3);
    // 0   1   2   3   4   5
    // A = C = D
    //   = C = D
    // C = D
    result = dpl_remove_at_index(list,0);
    ck_assert_msg(*dpl_get_element_at_index(result,0) == 'C', "t4 Failure: expected list to have 'C' at pos 0 but go %c",
                  *dpl_get_element_at_index(result,0));
    ck_assert_msg(*dpl_get_element_at_index(result,1) == 'D', "t5 Failure: expected list to have 'D' at pos 0 but go %c",
                  *dpl_get_element_at_index(result,1));

    ck_assert_msg(dpl_size(result) == 2, "t6 Faliure: expeted size 2, got size %d",
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
    tcase_add_test(tc1_1, test_dpl_get_element_at_index);
    tcase_add_test(tc1_1, test_dpl_size);
    tcase_add_test(tc1_1, test_dpl_get_index_of_element);
    tcase_add_test(tc1_1, test_dpl_remove_at_index);

    srunner_run_all(sr, CK_VERBOSE);

    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
