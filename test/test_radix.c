#include <check.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/radix.h"

// TODO: Functions to write:
//  [x] test_radix_create
//  [x] test_radix_insert_word
//  [x] test_radix_remove_word
//  [x] test_radix_find_word
//  [x] test_radix_find_prefix
//  [x] test_radix_delete

#define NUM_CHARS 26

struct trie_t {
	struct trie_t *children[NUM_CHARS];
	bool b_is_word;
	char *word;
};

trie_t *root;

const char *valid_words[] = {
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

const char *invalid_args[] = {
	NULL,
	"",
	" ",
	"Pickle",
	"!ickle",
	"pickl!"
};

const char *invalid_words[] = {
	"p",
	"a",
	"laces",
	"anacea",
	"pic",
	"pickl",
	"pla",
	"placeb",
	"pi"
};

const char *valid_prefixes[] = {
	"p",
	"pan",
	"pi",
	"pick",
	"pl",
	"pla",
	"placeb",
	"picke",
	"pickl",
	"pickle",
	"picklin"
};

const char *invalid_prefixes[] = {
	"ic",
	"pand",
	"pice",
	"ply",
	"placb",
	"lay",
	"anacea",
	"plce"
};

void setup(void)
{
	root = radix_create();
}

void teardown(void)
{
	radix_delete(&root);
}

void populate_trie()
{
	setup();
	for (int i = 0; i < 10; ++i) {
		radix_insert_word(root, valid_words[i]);
	}
}

START_TEST(test_radix_create)
{
	root = radix_create();
	ck_assert_ptr_ne(root, NULL);
	ck_assert_ptr_ne(root->children, NULL);
	ck_assert_int_eq(root->b_is_word, 0);
	teardown();
} END_TEST

// NOTE: radix_insert returns non-zero on success, 0 on failure
START_TEST(test_radix_insert_valid)
{
	setup();
	ck_assert_int_ne(radix_insert_word(root, "pickle"), 0);
	ck_assert_int_ne(radix_insert_word(root, "p"), 0);
	teardown();
} END_TEST START_TEST(test_radix_insert_invalid)
{
	setup();

	// TEST: aginst known errors, NULL, empty string, and invalid chars
	ck_assert_int_eq(radix_insert_word(NULL, "p"), 0);
	for (int i = 0; i < 6; ++i) {
		ck_assert_int_eq(radix_insert_word(root, invalid_args[i]), 0);
	}

	// TEST: against already added word
	ck_assert_int_ne(radix_insert_word(root, "pickle"), 0);
	ck_assert_int_eq(radix_insert_word(root, "pickle"), 0);
	teardown();
}

END_TEST
// NOTE: radix_remove returns 1 if removed else 0, -1 on error
START_TEST(test_radix_remove_word_valid)
{
	populate_trie();
	for (int i = 0; i < 10; ++i) {
		ck_assert_int_eq(radix_remove_word(root, valid_words[i]), 1);
	}
	teardown();
} END_TEST START_TEST(test_radix_remove_word_invalid)
{
	populate_trie();

	// TEST against known errors, NULL, empty string and invalid chars
	ck_assert_int_eq(radix_remove_word(NULL, "pickles"), -1);
	for (int i = 0; i < 6; ++i) {
		ck_assert_int_eq(radix_remove_word(root, invalid_args[i]), -1);
	}

	// TEST: against words not in trie
	for (int i = 0; i < 9; ++i) {
		ck_assert_int_eq(radix_remove_word(root, invalid_words[i]), 0);
	}

	// TEST: against word previously removed
	ck_assert_int_eq(radix_remove_word(root, "pickles"), 1);
	ck_assert_int_eq(radix_remove_word(root, "pickles"), 0);
	teardown();

}

END_TEST
// NOTE: radix_find_word returns 1 if found, else 0. -1 on error
START_TEST(test_radix_find_word_valid)
{
	populate_trie();
	for (int i = 0; i < 10; ++i) {
		ck_assert_int_eq(radix_find_word(root, valid_words[i]), 1);
	}

	teardown();
}

END_TEST START_TEST(test_radix_find_word_invalid)
{
	populate_trie();

	// TEST: against known errors, NULL, empty string and invalid chars
	for (int i = 0; i < 6; ++i) {
		ck_assert_int_eq(radix_find_word(root, invalid_args[i]), -1);
	}

	// TEST: against node strings, or partial strings in trie but not marked as word
	for (int i = 0; i < 9; ++i) {
		ck_assert_int_eq(radix_find_word(root, invalid_words[i]), 0);
	}

	teardown();
}

END_TEST
// NOTE: radix_find_prefix returns 1 if found, else 0, -1 on error
START_TEST(test_radix_find_prefix_valid)
{
	populate_trie();

	for (int i = 0; i < 11; ++i) {
		ck_assert_int_eq(radix_find_prefix(root, valid_prefixes[i]), 1);
	}

	teardown();
}

END_TEST START_TEST(test_radix_find_prefix_invalid)
{
	populate_trie();

	//TEST: against known errors, NULL, empty string and invalid chars
	for (int i = 0; i < 6; ++i) {
		ck_assert_int_eq(radix_find_prefix(root, invalid_args[i]), -1);
	}

	// TEST: against prefixes not in trie or that start at level > 1
	for (int i = 0; i < 8; ++i) {
		ck_assert_int_eq(radix_find_prefix(root, invalid_prefixes[i]),
				 0);
	}
}

END_TEST START_TEST(test_radix_delete_valid)
{
	populate_trie();
	radix_delete(&root);

	// TEST: children all NULL after delete
	for (int i = 0; i < NUM_CHARS; ++i) {
		ck_assert_ptr_eq(root->children, NULL);
	}

	// TEST: root set to NULL after delete
	ck_assert_ptr_eq(root, NULL);

	// TEST: call delete again to ensure delete does not segfaul on NULL ptr
	radix_delete(&root);
} END_TEST static TFun core_tests[] = {

	test_radix_create,
	test_radix_insert_valid,
	test_radix_insert_invalid,
	test_radix_remove_word_valid,
	test_radix_remove_word_invalid,
	test_radix_find_word_valid,
	test_radix_find_word_invalid,
	test_radix_find_prefix_valid,
	test_radix_find_prefix_invalid,
	test_radix_delete_valid,
	NULL
};

Suite *test_radix(void)
{
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
