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
        if (!node->children[index]) {
                node->children[index] = radix_create_node(len);
                memcpy(node->children[index]->word, word, len);
                node->children[index]->b_is_word = true;
                return node->children[index];
        }
        int new_index = 0;
        trie_t *tmp = node->children[index];
        int root_word_len = strlen(tmp->word);
        int prefix_idx = get_prefix_index(word, tmp->word);
        printf("With word %s, prefix idx is %d\n", word, prefix_idx);
        printf("len is %d and rootlen is %d\n", len, root_word_len);
        char *word_cpy = calloc(root_word_len, sizeof(char));
        word += prefix_idx;
        
        if (prefix_idx == root_word_len) {
                printf("True!\n");
                new_index = CHAR_TO_INDEX(word[0]);
                return radix_insert_rec(tmp, word, (len - prefix_idx), new_index);
        } else if (prefix_idx < root_word_len) {
                root_word_len = root_word_len - prefix_idx;
                char *new_string = calloc(root_word_len + 1, sizeof(char));
                memcpy(new_string, tmp->word + prefix_idx, root_word_len);
                new_index = CHAR_TO_INDEX(new_string[0]);
                tmp->word[prefix_idx] = '\0';
                return radix_insert_rec(tmp, new_string, root_word_len, new_index);
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
        printf("%s\n", root->children[15]->word);
        printf("%s\n", root->children[15]->children[18]->word);
}

static int get_prefix_index(const char *word, const char *new_word) {
        int index = 0;
        printf("comparing %s and %s\n", word, new_word);
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
