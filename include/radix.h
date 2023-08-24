/** @file radix.h
 *
 * @brief A shortened trie for word handling.
 *
 * 170D WOBC Module-M (DSAII) PE 1B
 */
#ifndef RADIX_H
#define RADIX_H

typedef struct trie_t trie_t;

/**
 * @brief Creates a prefixing trie
 *
 * @return A new radix trie or NULL
 */
trie_t *radix_create(void);

/**
 * @brief Adds word to trie
 *
 * @param[in] trie Trie to add the data to
 * @param[in] word Word to be added
 * 
 * @return Non-zero on success, zero on failure
 */
int radix_insert_word(trie_t * trie, const char *word);

/**
 * @brief Removes word from trie
 *
 * @param[in] trie Trie to remove word from
 * @param[in] word Word to be removed
 * 
 * @return 1 if removed, else 0. -1 on error.
 */
int radix_remove_word(trie_t * trie, const char *word);

/**
 * @brief Searches trie for word and prints word if found
 *
 * @param[in] trie Trie to search
 * @param[in] word "word" to search for 
 *
 * @return 1 if found, else 0. -1 on error.
 */
int radix_find_word(trie_t * trie, const char *target);

/**
 * @brief Searches trie for prefix and prints all words starting with prefix
 *
 * @param[in] trie Trie to search
 * @param[in] prefix The prefix to display results of
 *
 * @return 1 if found, else 0. -1 on error.
 */
int radix_find_prefix(trie_t * trie, const char *prefix);

/**
 * @brief Deletes tree, free() memory, set calling functions pointer to NULL
 *
 * @param[in/out] trie The trie to delete -> NULL
 */
void radix_delete(trie_t ** trie);

/**
 * @brief Extra Credit: Print function supporting the following rules
 *
 * Each existing complete word must be displayed on their own line.
 * Each node traversed will print with one (1) space separating
 *
 * @param[in] trie The trie to display
 *
 * void radix_print_nodes(trie_t *trie);
 */

void radix_print(trie_t * root);

#endif				/* RADIX_H */
/*** end of file ***/
