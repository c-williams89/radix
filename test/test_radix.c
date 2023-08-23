#include <check.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/radix.h"

// TODO: Functions to write:
//  [x] test_radix_create
//  [x] test_radix_insert_word
//  [] test_radix_remove_word
//  [] test_radix_find_word
//  [] test_radix_find_prefix
//  [] test_radix_delete
// struct trie_t trie_t;
#define NUM_CHARS 26

struct trie_t {
	struct trie_t *children[NUM_CHARS];
	bool b_is_word;
	char *word;
}; 

trie_t *root;

void setup(void) {
	root = radix_create();
}

void teardown(void) {
	radix_delete(&root);
}

void populate_trie() {
	setup();
	char *test_words[] = {
		"places",
		"pickling",
		"placebo",
		"play",
		"picture",
		"pickets",
		"panacea",
		"pick",
		"picket",
		"pickles"
	};
	for (int i = 0; i < 10; ++i) {
		radix_insert_word(root, test_words[i]);
	}
}

START_TEST(test_radix_create)
{
	root = radix_create();
	ck_assert_ptr_ne(root, NULL);
	ck_assert_ptr_ne(root->children, NULL);
	ck_assert_int_eq(root->b_is_word, 0);
} END_TEST 

// NOTE: radix_insert returns non-zero on success, 0 on failure
START_TEST(test_radix_insert_valid) {
	setup();
	ck_assert_int_ne(radix_insert_word(root, "pickle"), 0);
	ck_assert_int_ne(radix_insert_word(root, "p"), 0);
	teardown();
} END_TEST

START_TEST(test_radix_insert_invalid) {
	setup();
	ck_assert_int_eq(radix_insert_word(NULL, "p"), 0);
	ck_assert_int_eq(radix_insert_word(root, NULL), 0);
	ck_assert_int_eq(radix_insert_word(root, ""), 0);
	ck_assert_int_eq(radix_insert_word(root, "Pickle"), 0);
	ck_assert_int_eq(radix_insert_word(root, "!ickle"), 0);
	ck_assert_int_ne(radix_insert_word(root, "pickle"), 0);
	ck_assert_int_eq(radix_insert_word(root, "pickle"), 0);
	teardown();
}
END_TEST

// NOTE: radix_remove returns 1 if removed else 0, -1 on error
START_TEST (test_radix_remove_word_valid) {
	populate_trie();
	ck_assert_int_eq(radix_remove_word(root, "pickles"), 1);
	ck_assert_int_ne(root, 0);
	teardown();
} END_TEST

static TFun core_tests[] = {

	test_radix_create,
	test_radix_insert_valid,
	test_radix_insert_invalid,
	test_radix_remove_word_valid,
	NULL
};

Suite *test_radix(void)
{
	Suite *s = suite_create("test_radix");
	TFun *curr = NULL;
	TCase *tc_core = tcase_create("core");
	curr = core_tests;
	// tcase_add_checked_fixture(tc_core, setup, teardown);
	while (*curr) {
		tcase_add_test(tc_core, *curr++);
	}
	suite_add_tcase(s, tc_core);
	return s;
}

int main(void)
{
	SRunner *sr = srunner_create(NULL);
	srunner_add_suite(sr, test_radix());
#ifdef VALGRIND
	srunner_set_fork_status(sr, CK_NOFORK);
#endif
	srunner_run_all(sr, CK_VERBOSE);
	srunner_free(sr);
}
