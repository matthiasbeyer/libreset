#include <check.h>

#include <stdlib.h>

#include "ht/ht.h"
#include "set_cfg.h"

#define LEN(x) (sizeof((x)) / sizeof((x)[0]))

START_TEST (test_ht_init) {
    struct ht ht;
    ht_init(&ht, 1); /* allocate 2^1 */

    ck_assert(ht.buckets != NULL);
    ck_assert(ht.sizeexp == 1);

    ht_destroy(&ht, &cfg_int);
}
END_TEST

START_TEST (test_ht_init_big) {
    size_t exp;

    for (exp = 2; exp < 10; exp++) {
        struct ht ht;
        ht_init(&ht, exp); /* allocate 2^exp */

        ck_assert(ht.buckets != NULL);
        ck_assert(ht_nbuckets(&ht) == (1 << exp)); /* (1 << exp) == 2^exp */

        ht_destroy(&ht, &cfg_int);
    }
}
END_TEST

static const struct {
    size_t exp;
    unsigned int nvals;
} map_exp_nvals[] = {
    { .exp = 2,     .nvals = 10 },
    { .exp = 10,    .nvals = 10 },
    { .exp = 2,     .nvals = 1000 },
    { .exp = 10,    .nvals = 1000 },
};

START_TEST (test_ht_insertion) {
    size_t exp          = map_exp_nvals[_i].exp;
    unsigned int nvals  = map_exp_nvals[_i].nvals;

    struct ht ht;
    ht_init(&ht, exp);

    /* allocating on not-cleared stack, assuming to be random inside */
    int vals[nvals];

    unsigned int i;
    for (i = nvals; i; --i) {
        vals[i - 1] = i;
        ck_assert(1 == ht_insert(&ht, vals + i - 1, &cfg_int));
    }

    ht_destroy(&ht, &cfg_int);
}
END_TEST

Suite*
suite_ht_create(void) {
    Suite* s;
    TCase* case_allocfree;
    TCase* case_adding;
    TCase* case_finding;

    s = suite_create("HT");

    /* Test case creation */
    case_allocfree  = tcase_create("Allocating and deleting");
    case_adding     = tcase_create("Adding");
    case_finding    = tcase_create("Finding");

    /* test adding to test cases */
    tcase_add_test(case_allocfree, test_ht_init);
    tcase_add_test(case_allocfree, test_ht_init_big);

    tcase_add_loop_test(case_adding,
                        test_ht_insertion,
                        0,
                        LEN(map_exp_nvals));

    /* Adding test cases to suite */
    suite_add_tcase(s, case_allocfree);
    suite_add_tcase(s, case_adding);

    return s;
}

