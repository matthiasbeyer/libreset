#include <check.h>

#include <stdlib.h>
#include <errno.h>

#include "avl/avl.h"
#include "set_cfg.h"

START_TEST (test_avl_alloc_destroy) {
    struct avl* avl = calloc(1, sizeof(*avl));

    ck_assert(avl != NULL);
    ck_assert(avl->root == NULL);

    ck_assert(-EEXIST == avl_destroy(avl, &cfg_int)); // root node is NULL
}
END_TEST

START_TEST (test_avl_insert) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data = 1;
    r_hash hash = 1;

    avl_insert(avl, hash, &data, &cfg_int);

    ck_assert(&data == avl_find(avl, hash, &data, &cfg_int));

    avl_destroy(avl, &cfg_int);
}
END_TEST

START_TEST (test_avl_insert_destroy) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data = 1;
    r_hash hash = 1;

    avl_insert(avl, hash, &data, &cfg_int);

    ck_assert(0 == avl_destroy(avl, &cfg_int));
}
END_TEST

START_TEST (test_avl_insert_multiple) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    r_hash hash[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };


    int i;
    for (i = 0; i < 10; i++) {
        ck_assert(0 == avl_insert(avl, hash[i], &data[i], &cfg_int));
    }
    ck_assert(avl_node_cnt(avl->root) == 10);

    for (i = 0; i < 10; i++) {
        ck_assert(&data[i] == avl_find(avl, hash[i], &data[i], &cfg_int));
    }

    avl_destroy(avl, &cfg_int);
}
END_TEST

#define MANY_INTS_CNT 10000

START_TEST (test_avl_insert_many_distinct) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data[MANY_INTS_CNT];

    int i;
    for (i = 0; i < MANY_INTS_CNT; i++) {
        data[i] = i;
        ck_assert(0 == avl_insert(avl, data[i], &data[i], &cfg_int));
    }

    for (i = 0; i < MANY_INTS_CNT; i++) {
        ck_assert(&data[i] == avl_find(avl, data[i], &data[i], &cfg_int));
    }

    ck_assert(0 == avl_destroy(avl, &cfg_int));
}
END_TEST

START_TEST (test_avl_insert_multiple_destroy) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    r_hash hash[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    int i;
    for (i = 0; i < 10; i++) {
        avl_insert(avl, hash[i], &data[i], &cfg_int);
    }

    ck_assert(0 == avl_destroy(avl, &cfg_int));
}
END_TEST

START_TEST (test_avl_insert_collisions) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    r_hash hash = 0;

    int i;
    for (i = 0; i < 10; i++) {
        ck_assert(0 == avl_insert(avl, hash, &data[i], &cfg_int));
    }

    for (i = 0; i < 10; i++) {
        ck_assert(&data[i] == avl_find(avl, hash, &data[i], &cfg_int));
    }

    ck_assert(0 == avl_destroy(avl, &cfg_int));
}
END_TEST

START_TEST (test_avl_delete) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data = 1;
    r_hash hash = 1;

    avl_insert(avl, hash, &data, &cfg_int);

    ck_assert(&data == avl_find(avl, hash, &data, &cfg_int));
    ck_assert(0 == avl_del(avl, hash, &data, &cfg_int));
    ck_assert(-EEXIST == avl_del(avl, hash, &data, &cfg_int));

    avl_destroy(avl, &cfg_int);
}
END_TEST

START_TEST (test_avl_delete_multiple) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    r_hash hash[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };


    int i;
    for (i = 0; i < 10; i++) {
        ck_assert(0 == avl_insert(avl, hash[i], &data[i], &cfg_int));
    }
    ck_assert(avl_node_cnt(avl->root) == 10);

    for (i = 0; i < 10; i++) {
        ck_assert(0 == avl_del(avl, hash[i], &data[i], &cfg_int));
    }
    ck_assert(avl_node_cnt(avl->root) == 0);

    for (i = 0; i < 10; i++) {
        ck_assert(NULL == avl_find(avl, hash[i], &data[i], &cfg_int));
    }

    avl_destroy(avl, &cfg_int);
}
END_TEST

START_TEST (test_avl_find_single) {
    struct avl* avl = calloc(1, sizeof(*avl));
    int data        = 1;
    r_hash hash    = 1;
    int* found;

    ck_assert(0 == avl_insert(avl, hash, &data, &cfg_int));
    found = avl_find(avl, hash, &data, &cfg_int);

    ck_assert(*found == data);
    ck_assert(0 == avl_destroy(avl, &cfg_int));
}
END_TEST


START_TEST (test_avl_find_multiple) {
    struct avl* avl = calloc(1, sizeof(*avl));
    int data[]      = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    r_hash hash[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int* found[10] = { NULL };

    int i;
    int j;

    for (i = 0; i < 10; i++) {
        ck_assert(0 == avl_insert(avl, hash[i], &data[i], &cfg_int));
    }

    for (i = 0; i < 10; i++) {
        found[i] = (int*) avl_find(avl, hash[i], &data[i], &cfg_int);
        ck_assert(found[i] != NULL);
    }

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (i != j) {
                ck_assert_msg(found[i] != found[j], "Expected %x to not equal %x", found[i],
                              found[j]);
            }
        }
    }

    avl_destroy(avl, &cfg_int);
}
END_TEST

START_TEST (test_avl_cardinality) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    r_hash hash[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    int i;
    for (i = 0; i < 10; i++) {
        avl_insert(avl, hash[i], &data[i], &cfg_int);
    }

    ck_assert(10 == avl_cardinality(avl));
    avl_destroy(avl, &cfg_int);
}
END_TEST

START_TEST (test_avl_cardinality_continuous) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    r_hash hash[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    int i;
    for (i = 0; i < 10; i++) {
        avl_insert(avl, hash[i], &data[i], &cfg_int);
        ck_assert(i + 1 == avl_cardinality(avl));
    }

    ck_assert(10 == avl_cardinality(avl));
    avl_destroy(avl, &cfg_int);
}
END_TEST

START_TEST (test_avl_subset) {
    struct avl* avl = calloc(1, sizeof(*avl));

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    r_hash hash[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    int i;
    for (i = 0; i < 10; i++) {
        avl_insert(avl, hash[i], &data[i], &cfg_int);
    }

    ck_assert(avl_is_subset(avl, avl, &cfg_int) == 1);
    avl_destroy(avl, &cfg_int);
}
END_TEST

START_TEST (test_avl_subset_distinct) {
    struct avl* avl = calloc(1, sizeof(*avl));
    struct avl* avl2 = calloc(1, sizeof(*avl2));

    int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int data2[] = { 0, 1, 2, 3 };
    r_hash hash[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    int i;
    for (i = 0; i < 10; i++) {
        avl_insert(avl, hash[i], &data[i], &cfg_int);
    }
    for (i = 0; i < 4; i++) {
        avl_insert(avl2, hash[i], &data2[i], &cfg_int);
    }

    ck_assert(avl_is_subset(avl2, avl, &cfg_int) == 1);
    ck_assert(avl_is_subset(avl, avl2, &cfg_int) != 1);
    avl_destroy(avl, &cfg_int);
}
END_TEST

Suite*
suite_avl_create(void) {
    Suite* s;
    TCase* case_allocfree;
    TCase* case_adding;
    TCase* case_deleting;
    TCase* case_finding;
    TCase* case_subset;

    s = suite_create("AVL");

    /* Test case creation */
    case_allocfree  = tcase_create("Allocating and deleting");
    case_adding     = tcase_create("Adding");
    case_deleting   = tcase_create("Deleting");
    case_finding    = tcase_create("Finding");
    case_subset     = tcase_create("Finding");

    /* test adding to test cases */
    tcase_add_test(case_allocfree, test_avl_alloc_destroy);

    tcase_add_test(case_adding, test_avl_insert);
    tcase_add_test(case_adding, test_avl_insert_destroy);
    tcase_add_test(case_adding, test_avl_insert_multiple);
    tcase_add_test(case_adding, test_avl_insert_many_distinct);
    tcase_add_test(case_adding, test_avl_insert_multiple_destroy);
    tcase_add_test(case_adding, test_avl_insert_collisions);

    tcase_add_test(case_deleting, test_avl_delete);
    tcase_add_test(case_deleting, test_avl_delete_multiple);

    tcase_add_test(case_finding, test_avl_find_single);
    tcase_add_test(case_finding, test_avl_find_multiple);

    tcase_add_test(case_finding, test_avl_cardinality);
    tcase_add_test(case_finding, test_avl_cardinality_continuous);

    tcase_add_test(case_subset, test_avl_subset);
    tcase_add_test(case_subset, test_avl_subset_distinct);

    /* Adding test cases to suite */
    suite_add_tcase(s, case_allocfree);
    suite_add_tcase(s, case_adding);
    suite_add_tcase(s, case_deleting);
    suite_add_tcase(s, case_finding);
    suite_add_tcase(s, case_subset);

    return s;
}

