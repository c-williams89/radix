# radix

## About
`radix.h` is a custom data structure library that provides an API for creating and interacting with a radix trie data structure in C through public-facing functions.<br>
It has been developed with C18 and compiled with gcc-9.

## Installation and Building
`radix` can be cloned directly from this repo.<br>
A Makefile is included to compile source code files and run tests against it.<br>
The only external dependecies are the `check.h` library for running unit tests. `check.h` can be pulled [here](https://github.com/libcheck/check/tree/master)

## Functions
`radix.h` has 6 public functions for interacting with the trie data structure:
```c
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
```

## Testing
Automated unit tests have been written for the `radix` library and can be run using make.
```sh
$ make check
```

will build and run automated unit testing.
```sh
Running suite(s): test_radix
100%: Checks: 10, Failures: 0, Errors: 0
test/test_radix.c:106:P:core:*curr++:0: Passed
test/test_radix.c:115:P:core:*curr++:0: Passed
test/test_radix.c:129:P:core:*curr++:0: Passed
test/test_radix.c:139:P:core:*curr++:0: Passed
test/test_radix.c:159:P:core:*curr++:0: Passed
test/test_radix.c:170:P:core:*curr++:0: Passed
test/test_radix.c:187:P:core:*curr++:0: Passed
test/test_radix.c:200:P:core:*curr++:0: Passed
test/test_radix.c:217:P:core:*curr++:0: Passed
test/test_radix.c:233:P:core:*curr++:0: Passed
```

## Support
Both the library and README are a work in progress. For questions regarding the use of the library, its funnctions, or to submit bug tickets, email: clayton.e.williams15.mil@army.mil<br>
Additional support can be found in the man page:
```sh
$ man ./doc/radix.h.3
```
