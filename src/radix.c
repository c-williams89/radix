/** @file radix.c
 *
 * @brief Radix Trie library for word lookup
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "../include/radix.h"

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
#define INDEX_TO_CHAR(i) ((char)i + (char)'a')

#define NUM_CHARS 26

struct trie_t {
        struct trie_t *children[NUM_CHARS];
        bool b_is_word;
        char *word;
};

trie_t *radix_create(void)
{
        return calloc(1, sizeof(trie_t));
}

int radix_insert_word(trie_t *trie, const char *word)
{
    ;
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

/*
void radix_print_nodes(trie_t *trie)
{
    ;
}
 */

/*** end of file ***/
