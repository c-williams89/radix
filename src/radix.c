#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "../include/radix.h"

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
#define INDEX_TO_CHAR(i) ((char)i + (char)'a')

#define NUM_CHARS 26

struct trie_t {
        struct trie_t *children[NUM_CHARS];
        char *word;
        bool b_is_word;
};

static trie_t *radix_insert_rec(trie_t *node, char *word, int len, int index);
static trie_t *radix_create_node(int len);
static int get_prefix_index(const char *word, const char *new_word); 


trie_t *radix_create(void)
{
        return calloc(1, sizeof(trie_t));
}

int radix_insert_word(trie_t *trie, const char *word)
{
        if ((!trie) || (!word)) {
                fprintf(stderr, "radix_insert_word: Invalid argument - NULL\n");
                return 0;
        }

        trie_t *tmp = trie;
        int len = strlen(word);
        char *word_cpy = calloc(len + 1, sizeof(char));
        strncpy(word_cpy, word, len);
        int index = CHAR_TO_INDEX(word[0]);
        radix_insert_rec(tmp, word_cpy, len, index);
}

static trie_t *radix_insert_rec(trie_t *node, char *word, int len, int index) {
// TODO: Edge Cases:
//  1. New word is longer - Handled
//  2. New word is shorter
//  3. Bifurcation
        if (!node->children[index]) {
                node->children[index] = radix_create_node(len);
                memcpy(node->children[index]->word, word, len);
                node->children[index]->b_is_word = true;
                return node->children[index];
        }
        
        int new_index = 0;
        trie_t *cpy = node->children[index];
        int root_word_len = strlen(cpy->word);
        if (strlen(word) == root_word_len) {
                if (strncmp(word, cpy->word, root_word_len)) {
                        cpy->b_is_word = true;
                        return;
                }
        }
        // if (strncmp(word, cpy->word, ))
        int prefix_idx = get_prefix_index(word, cpy->word);
        // printf("With word %s, prefix idx is %d\n", word, prefix_idx);
        // printf("len is %d and rootlen is %d\n", len, root_word_len);
        // char *word_cpy = calloc(root_word_len, sizeof(char));

        // Edge case 1: new word diverges at end of root word
        if (prefix_idx == root_word_len) {
                word += prefix_idx;
                printf("True!\n");
                new_index = CHAR_TO_INDEX(word[0]);
                return radix_insert_rec(cpy, word, (len - prefix_idx), new_index);
        // Edge case 2: new word becomes new root word (some bifurcation)
// BUG: Need to compare length of current word vs incoming?

        } else if (prefix_idx < root_word_len) {
                // Creating a new node and copying over bifurcated end of original string
                root_word_len = root_word_len - prefix_idx;
                trie_t *tmp;
                if (root_word_len > strlen(word)) {
                        tmp = radix_create_node(strlen(word));
                        memcpy(tmp->word, (cpy->word + prefix_idx), strlen((word)));
                } else {
                        tmp = radix_create_node(root_word_len);
                        memcpy(tmp->word, (cpy->word + prefix_idx), root_word_len);

                }
                // printf("tmp now: %s\n", tmp->word);
                
                // Sets non-unique character to null
                char *new_string = calloc(root_word_len + 1, sizeof(char));
                memcpy(new_string, cpy->word + prefix_idx, root_word_len);
                new_index = CHAR_TO_INDEX(new_string[0]);
                // new_index = CHAR_TO_INDEX(tmp->word[0]);
                cpy->word[prefix_idx] = '\0';
                // printf("Comparing %s and %s\n", word, cpy->word);
                
                for (int i = 0; i < NUM_CHARS; ++i) {
                        if (cpy->children[i]) {
                                tmp->children[i] = cpy->children[i];
                                cpy->children[i] = NULL;
                        }
                }
                
                
                
                // Handing off new node to original
                cpy->children[new_index] = tmp;
                word += prefix_idx;
                // printf("now: %s\n", cpy->children[new_index]->word);
                new_index = CHAR_TO_INDEX(word[0]);
                return radix_insert_rec(cpy, word, strlen(word), new_index);
                
                // return radix_insert_rec(cpy, new_string, root_word_len, new_index);
        // Edge case 3: complete bifurcation
        // No other case, it either diverges at end of root word, or before
        } else {

        }
}

static trie_t *radix_create_node(int len) {
        trie_t *node = calloc(1, sizeof(*node));
        if (!node) {
                perror("calloc");
                errno = 0;
        }
        node->word = calloc((len + 1), sizeof(char));
        return node;
}


int radix_remove_word(trie_t *trie, const char *word)
{
    ;
}

int radix_find_word(trie_t *trie, const char *target)
{
    ;   
}

int radix_find_prefix(trie_t *trie, const char *prefix)
{
    ;
}

void radix_delete(trie_t **trie)
{
    ;
}

void radix_print(trie_t *root) {
        for (int i = 0; i < NUM_CHARS; ++i) {
                if (root->children[i]) {
                        printf("%d - %s\n", i, root->children[i]->word);
                        for (int j = 0; j < NUM_CHARS; ++j) {
                                if (root->children[i]->children[j]) {
                                        printf("%d:%d - %s\n", i, j, root->children[i]->children[j]->word);
                                        for (int k = 0; k < NUM_CHARS; ++k) {
                                                if (root->children[i]->children[j]->children[k]) {
                                                        printf("%d:%d:%d - %s\n", i, j, k, root->children[i]->children[j]->children[k]->word);
                                                        for (int l = 0; l < NUM_CHARS; ++l) {
                                                                if (root->children[i]->children[j]->children[k]->children[l]) {
                                                                        printf("%d:%d:%d:%d - %s\n", i, j, k, l, root->children[i]->children[j]->children[k]->children[l]->word);
                                                                        for (int m = 0; m < NUM_CHARS; ++m) {
                                                                                if (root->children[i]->children[j]->children[k]->children[l]->children[m]) {
                                                                                        printf("%d:%d:%d:%d:%d - %s\n", i, j, k, l, m, root->children[i]->children[j]->children[k]->children[l]->children[m]->word);
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

static int get_prefix_index(const char *word, const char *new_word) {
        int index = 0;
        // printf("comparing %s and %s\n", word, new_word);
        while (word[index] == new_word[index]) {
                ++index;
        }
        return index;
}

/*
void radix_print_nodes(trie_t *trie)
{
    ;
}
*/
