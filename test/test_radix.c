#include <check.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/radix.h"

// struct trie_t trie_t;
#define NUM_CHARS 26

struct trie_t {
        struct trie_t *children[NUM_CHARS];
        bool b_is_word;
        char *word;
};

START_TEST(test_radix_create) {
        trie_t *root = radix_create();
        ck_assert_ptr_ne(root, NULL);
        ck_assert_ptr_ne(root->children, NULL);
        ck_assert_int_eq(root->b_is_word, 0);
} END_TEST

static TFun core_tests[] = {
        test_radix_create,
        NULL
};

Suite *test_radix(void) {
        Suite *s = suite_create("test_radix");
        TFun *curr = NULL;
        TCase *tc_core = tcase_create("core");
        curr = core_tests;
        while (*curr) {
                tcase_add_test(tc_core, *curr++);
        }
        suite_add_tcase(s, tc_core);
        return s;
}

int main(void) {
        SRunner *sr = srunner_create(NULL);
        srunner_add_suite(sr, test_radix());
#ifdef VALGRIND
        srunner_set_fork_status(sr, CK_NOFORK);
#endif
        srunner_run_all(sr, CK_VERBOSE);
        srunner_free(sr);
}