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

static trie_t *radix_insert_rec(trie_t *node, char *word, int len, int index);
static trie_t *radix_create_node(int len);
static int get_prefix_index(const char *word, const char *new_word); 
static int radix_find_rec(trie_t *root, char *word, bool b_to_remove);
static void radix_delete_rec(trie_t *node);


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
        int return_val = 0;
        trie_t *tmp = trie;
        int len = strlen(word);
        char *word_cpy = calloc(len + 1, sizeof(char));
        memcpy(word_cpy, word, len);
        // strncpy(word_cpy, word, len);
        int index = CHAR_TO_INDEX(word[0]);
        
        // if (radix_find_rec(trie->children[index]), )
        radix_insert_rec(tmp, word_cpy, len, index);
        free(word_cpy);
        return 1;
}

int radix_remove_word(trie_t *trie, const char *word)
{
        if ((!trie) || (!word) || (strlen(word) < 1)) {
                fprintf(stderr, "radix_remove_word: Invalid argument\n");
                return -1;
        }

        for (int i = 0; i < NUM_CHARS; ++i) {
                if (trie->children[i]) {
                        return radix_find_rec(trie->children[i], word, true);
                }
        }


}


int radix_find_word(trie_t *trie, const char *target)
{
        if ((!trie) || (!target) || (strlen(target) < 1)) {
                fprintf(stderr, "radix_find_word: Invalid argument\n");
                return -1;
        }
        
        for (int i = 0; i < NUM_CHARS; ++i) {
                if (trie->children[i]) {
                        return radix_find_rec(trie->children[i], target, false);
                }
        }
}

// int radix_find_prefix(trie_t *trie, const char *prefix)
// {
//         return 0;
// }

void radix_delete(trie_t **trie)
{
        printf("called here\n");
        if ((!trie) || (!*trie)) {
                fprintf(stderr, "radix_delete: Invalid argument - NULL\n");
                return;
        }

        trie_t *tmp = *trie;
        for (int i = 0; i < NUM_CHARS; ++i) {
                if (tmp->children[i]) {
                        printf("%s\n", tmp->children[i]->word);
                        radix_delete(&(tmp->children[i]));
                        // free(tmp->word);

                        // free(tmp->children[i]);
                }
                // free(tmp);
                // free(tmp->children);
        }
        free(tmp->word);
        free(tmp);
}

static trie_t *radix_insert_rec(trie_t *node, char *word, int len, int index) {
        if (!node->children[index]) {
                node->children[index] = radix_create_node(len);
                memcpy(node->children[index]->word, word, len);
                node->children[index]->b_is_word = true;
                return node->children[index];
        }
        int len_to_pass = 0;
        int new_index = 0;
        trie_t *cpy = node->children[index];
        int root_word_len = strlen(cpy->word);
        int new_word_len = strlen(word);
        if (new_word_len == root_word_len) {
                if (0 == (strncmp(word, cpy->word, root_word_len))) {
                        cpy->b_is_word = true;
                        return cpy;
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
                        memcpy(tmp->word, (cpy->word + prefix_idx), new_word_len);
                } else {
                        tmp = radix_create_node(root_word_len);
                        memcpy(tmp->word, (cpy->word + prefix_idx), root_word_len);
                }
                tmp->b_is_word = true;
                
                // Sets non-unique character to null
                // TODO: Doesn't look like this logic is necessary, confirm then delete
                // char *new_string = calloc(root_word_len + 1, sizeof(char));
                // memcpy(new_string, cpy->word + prefix_idx, root_word_len);
                // new_index = CHAR_TO_INDEX(new_string[0]);
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
                        // return radix_insert_rec(cpy, word, strlen(word), new_index);
                } 
                else {
                        return NULL;
                }
        }
        return radix_insert_rec(cpy, word, len_to_pass, new_index); 
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

static int get_prefix_index(const char *word, const char *new_word) {
        int index = 0;
        while ((word[index] == new_word[index]) && (word[index] && new_word[index])) {
                ++index;
        }
        return index;
}

static int radix_find_rec(trie_t *root, char *word, bool b_to_remove) {
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
                                        return_status = 1;
                                        // goto EXIT;
                                        // return 1;
                                }
                                return_status = 1;
                                goto EXIT;
                                // return 1;
                        } else {
                                goto EXIT;
                                // return 0;
                        }
                } else {
                        word += prefix;
                        next_index = CHAR_TO_INDEX(word[0]);
                }

                // if ((root_len == word_len) && (root->b_is_word)) {
                //         if (b_to_remove) {
                //                 root->b_is_word = false;
                //         }
                //         return 1;
                // } else {
                //         word += prefix;
                //         next_index = CHAR_TO_INDEX(word[0]);
                // }
        }
        return radix_find_rec(root->children[next_index], word, b_to_remove);
EXIT:
        return return_status;
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



/*
void radix_print_nodes(trie_t *trie)
{
    ;
}
*/
