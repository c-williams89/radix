#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "../include/radix.h"

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

#define NUM_CHARS 26

struct trie_t {
	struct trie_t *children[NUM_CHARS];
	char *word;
	bool b_is_word;
};

static bool validate_input(const char *word);
static int radix_find_rec(trie_t * root, const char *word, bool b_to_remove);
static trie_t *radix_insert_rec(trie_t * node, char *word, int len, int index);

static trie_t *radix_create_node(int len);
static int get_prefix_index(const char *word, const char *new_word);
static void print_word_by_prefix(trie_t * node, char *word, int len);
static trie_t *get_prefix_node(trie_t * node, const char *word);

// TODO: Go through each function and do NULL checks on any calloc

trie_t *radix_create(void)
{
	return calloc(1, sizeof(trie_t));
}

int radix_insert_word(trie_t * trie, const char *word)
{
	int return_val = 0;
	if ((!trie) || (!word)) {
		fprintf(stderr, "radix_insert_word: Invalid argument - NULL\n");
		goto EXIT;
	}

	if (strlen(word) < 1) {
		fprintf(stderr,
			"radix_insert_word: Invalid argument - empty string\n");
		goto EXIT;
	}

	if (!validate_input(word)) {
		fprintf(stderr,
			"radix_insert_word: Invalid argument - must be ASCII lower case\n");
		goto EXIT;
	}

	trie_t *tmp = trie;
	int len = strlen(word);
	char *word_cpy = calloc(len + 1, sizeof(char));
        if (!word_cpy) {
                perror("radix_insert_word");
                errno = 0;
                goto EXIT;
        }

	memcpy(word_cpy, word, len);
	int index = CHAR_TO_INDEX(word[0]);

	if (radix_find_rec(tmp->children[index], word, false)) {
		free(word_cpy);
		goto EXIT;
	}

	if (radix_insert_rec(tmp, word_cpy, len, index)) {
		return_val = 1;
	}

	free(word_cpy);
 EXIT:
	return return_val;
}

int radix_remove_word(trie_t * trie, const char *word)
{
	int return_val = -1;
	if ((!trie) || (!word)) {
		fprintf(stderr, "radix_remove_word: Invalid argument - NULL\n");
		goto EXIT;
	}

	if (strlen(word) < 1) {
		fprintf(stderr,
			"radix_remove_word: Invalid argument - empty string\n");
		goto EXIT;
	}

	if (!validate_input(word)) {
		fprintf(stderr,
			"radix_remove_word: Invalid argument - must be ASCII lower case\n");
		goto EXIT;
	}

	return_val = 0;
	int index = CHAR_TO_INDEX(word[0]);
	if (trie->children[index]) {
		return_val = radix_find_rec(trie->children[index], word, true);
	}
 EXIT:
	return return_val;
}

int radix_find_word(trie_t * trie, const char *target)
{
	int return_val = -1;

	if ((!trie) || (!target)) {
		fprintf(stderr, "radix_find_word: Invalid argument - NULL\n");
		goto EXIT;
	}

	if (strlen(target) < 1) {
		fprintf(stderr,
			"radix_find_word: Invalid argument - empty string\n");
		goto EXIT;
	}

	if (!validate_input(target)) {
		fprintf(stderr,
			"radix_find_word: Invalid argument - must be ASCII lower case\n");
		goto EXIT;
	}

	return_val = 0;
	int index = CHAR_TO_INDEX(target[0]);
	if (trie->children[index]) {
		return_val =
		    radix_find_rec(trie->children[index], target, false);
	}
 EXIT:
	return return_val;
}

int radix_find_prefix(trie_t * trie, const char *prefix)
{
	int return_status = -1;

	if ((!trie) || (!prefix)) {
		fprintf(stderr, "radix_find_prefix: Invalid argument - NULL\n");
		goto EXIT;
	}

	if (strlen(prefix) < 1) {
		fprintf(stderr,
			"radix_find_prefix: Invalid argument - empty string\n");
		goto EXIT;
	}

	if (!validate_input(prefix)) {
		fprintf(stderr,
			"radix_find_prefix: Invalid argument - must be ASCII lower case\n");
		goto EXIT;
	}

	int index = CHAR_TO_INDEX(prefix[0]);
	if (!trie->children[index]) {
		fprintf(stderr, "radix_find_prefix: Prefix '%s' not found\n",
			prefix);
		return_status = 0;
		goto EXIT;
	}
	int len = strlen(prefix);
	char *word = calloc(len, sizeof(char));
	memcpy(word, prefix, len);
	trie_t *tmp = trie->children[index];
	trie_t *node = get_prefix_node(tmp, prefix);
	if (!node) {
		fprintf(stderr, "radix_find_prefix: Prefix '%s' not found\n",
			prefix);
		free(word);
		return_status = 0;
		goto EXIT;
	}
	return_status = 1;
	printf("Prefix: %s\n", prefix);
	char *new_prefix = calloc(50, sizeof(char));
	while (tmp != node) {
		if (0 == strncmp(tmp->word, prefix, strlen(tmp->word))) {
			strncat(new_prefix, tmp->word, strlen((tmp->word)));
			prefix += strlen(tmp->word);
			tmp = tmp->children[CHAR_TO_INDEX(prefix[0])];
		}
	}
	print_word_by_prefix(tmp, new_prefix, len);
	free(word);
	free(new_prefix);
 EXIT:
	return return_status;
}

void radix_delete(trie_t ** trie)
{
	if ((!trie) || (!*trie)) {
		fprintf(stderr, "radix_delete: Invalid argument - NULL\n");
		return;
	}

	trie_t *tmp = *trie;
	for (int i = 0; i < NUM_CHARS; ++i) {
		if (tmp->children[i]) {
			radix_delete(&(tmp->children[i]));
		}
	}
	free(tmp->word);
	free(tmp);
	*trie = NULL;
}

static trie_t *radix_insert_rec(trie_t * node, char *word, int len, int index)
{
	trie_t *cpy = NULL;
	if (!node->children[index]) {
		node->children[index] = radix_create_node(len);
		memcpy(node->children[index]->word, word, len);
		node->children[index]->b_is_word = true;
		cpy = node->children[index];
		goto EXIT;
		// return node->children[index];
	}
	int len_to_pass = 0;
	int new_index = 0;
	cpy = node->children[index];
	int root_word_len = strlen(cpy->word);
	int new_word_len = strlen(word);
	if (new_word_len == root_word_len) {
		if (0 == (strncmp(word, cpy->word, root_word_len))) {
			if (!cpy->b_is_word) {
				cpy->b_is_word = true;
				goto EXIT;
			}
			cpy = NULL;
			goto EXIT;
			// return cpy;
		}
	}
	int prefix_idx = get_prefix_index(word, cpy->word);

	// Edge case 1: new word diverges at end of root word
	if (prefix_idx == root_word_len) {
		word += prefix_idx;
		new_index = CHAR_TO_INDEX(word[0]);
		len_to_pass = len - prefix_idx;
		// return radix_insert_rec(cpy, word, (len - prefix_idx), new_index);
		// Edge case 2: new word becomes new root word (some bifurcation)
	} else {
		// Creating a new node and copying over bifurcated end of original string
		root_word_len = root_word_len - prefix_idx;
		trie_t *tmp;
		if (root_word_len > new_word_len) {
			tmp = radix_create_node(new_word_len);
			memcpy(tmp->word, (cpy->word + prefix_idx),
			       root_word_len);
		} else {
			tmp = radix_create_node(root_word_len);
			memcpy(tmp->word, (cpy->word + prefix_idx),
			       root_word_len);
		}
                if (cpy->b_is_word) {
		        tmp->b_is_word = true;
                }
		new_index = CHAR_TO_INDEX((cpy->word + prefix_idx)[0]);
		cpy->word[prefix_idx] = '\0';
		if (new_word_len == prefix_idx) {
			cpy->b_is_word = true;
		} else {
			cpy->b_is_word = false;
		}

		for (int i = 0; i < NUM_CHARS; ++i) {
			if (cpy->children[i]) {
				tmp->children[i] = cpy->children[i];
				cpy->children[i] = NULL;
			}
		}
		// Handing off new node to original
		cpy->children[new_index] = tmp;
		word += prefix_idx;
		if (strlen(word) > 0) {
			new_index = CHAR_TO_INDEX(word[0]);
			len_to_pass = strlen(word);
		} else {
			goto EXIT;
		}
	}
	return radix_insert_rec(cpy, word, len_to_pass, new_index);
 EXIT:
	return cpy;
}

static trie_t *radix_create_node(int len)
{
	trie_t *node = calloc(1, sizeof(*node));
	if (!node) {
		perror("calloc");
		errno = 0;
	}
	node->word = calloc((len + 1), sizeof(char));
	return node;
}

static int get_prefix_index(const char *word, const char *new_word)
{
	int index = 0;
	while ((word[index] == new_word[index])
	       && (word[index] && new_word[index])) {
		++index;
	}
	return index;
}

static int radix_find_rec(trie_t * root, const char *word, bool b_to_remove)
{
	int return_status = 0;
	if ((!root) || (strlen(word) < 1)) {
		goto EXIT;
	}
	int root_len = strlen(root->word);
	int word_len = strlen(word);
	int prefix = get_prefix_index(root->word, word);
	int next_index = 0;

	if (0 == strncmp(root->word, word, root_len)) {
		if (root_len == word_len) {
			if (root->b_is_word) {
				if (b_to_remove) {
					root->b_is_word = false;
				}
				return_status = 1;
				goto EXIT;
			} else {
				goto EXIT;
			}
		} else {
			word += prefix;
			next_index = CHAR_TO_INDEX(word[0]);
		}
	}
	return radix_find_rec(root->children[next_index], word, b_to_remove);
 EXIT:
	return return_status;
}

void radix_print(trie_t * root)
{
	for (int i = 0; i < NUM_CHARS; ++i) {
		if (root->children[i]) {
			printf("%d - %s\n", i, root->children[i]->word);
			for (int j = 0; j < NUM_CHARS; ++j) {
				if (root->children[i]->children[j]) {
					printf("%d:%d - %s\n", i, j,
					       root->children[i]->children[j]->
					       word);
					for (int k = 0; k < NUM_CHARS; ++k) {
						if (root->children[i]->
						    children[j]->children[k]) {
							printf
							    ("%d:%d:%d - %s\n",
							     i, j, k,
							     root->children[i]->
							     children[j]->
							     children[k]->word);
							for (int l = 0;
							     l < NUM_CHARS;
							     ++l) {
								if (root->
								    children
								    [i]->
								    children
								    [j]->
								    children
								    [k]->
								    children[l])
								{
									printf
									    ("%d:%d:%d:%d - %s\n",
									     i,
									     j,
									     k,
									     l,
									     root->
									     children
									     [i]->
									     children
									     [j]->
									     children
									     [k]->
									     children
									     [l]->
									     word);
									for (int
									     m =
									     0;
									     m <
									     NUM_CHARS;
									     ++m)
									{
										if (root->children[i]->children[j]->children[k]->children[l]->children[m]) {
											printf
											    ("%d:%d:%d:%d:%d - %s\n",
											     i,
											     j,
											     k,
											     l,
											     m,
											     root->
											     children
											     [i]->
											     children
											     [j]->
											     children
											     [k]->
											     children
											     [l]->
											     children
											     [m]->
											     word);
										}
									}
								}
							}
						}

					}
				}
			}
		}
	}
}

static trie_t *get_prefix_node(trie_t * node, const char *word)
{
	trie_t *tmp = NULL;
	if (!node || strlen(word) < 1) {
		goto EXIT;
	}

	int root_word_len = strlen(node->word);
	int word_len = strlen(word);
	int prefix = get_prefix_index(node->word, word);
	int next_index = 0;

	if (root_word_len >= word_len) {
		if (0 == strncmp(node->word, word, word_len)) {
			tmp = node;
			goto EXIT;
		}
	} else {
		if (0 == strncmp(node->word, word, root_word_len)) {
			word += prefix;
			next_index = CHAR_TO_INDEX(word[0]);
			return get_prefix_node(node->children[next_index],
					       word);
		}
	}
 EXIT:
	return tmp;
}

static void print_word_by_prefix(trie_t * node, char *prefix, int len)
{
	char *word = calloc(len, sizeof(char));
	strncpy(word, prefix, len);
	int new_len = strlen(node->word);
	char *tmp = reallocarray(word, len + new_len, sizeof(char));
	word = tmp;
	strncat(word, node->word, new_len);
	if (node->b_is_word) {
		printf("Word: %s\n", word);
	}
	for (int i = 0; i < NUM_CHARS; ++i) {
		if (node->children[i]) {
			print_word_by_prefix(node->children[i], word,
					     len + new_len);
		}
	}
	free(word);
}

/*
void radix_print_nodes(trie_t *trie)
{
    ;
}
*/

static bool validate_input(const char *word)
{
	int len = strlen(word);
	for (int i = 0; i < len; ++i) {
		if ((word[i] < 'a') || (word[i] > 'z')) {
			return false;
		}
	}
	return true;
}
