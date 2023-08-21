#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/radix.h"

#define MAX_CHAR 256

int main(int argc, char *argv[])
{
    // Return value
    int result = 0;

    // Expects two arguments, file to read for adding, file to read for search
    // WARNING: Cannot be same file (double fopen)
    //
    if (argc != 3) {
        fprintf(stderr, "Expected %d args, got %d\n", 2, argc - 1);
        goto SHUTDOWN;
    }

    FILE *fp_read = fopen(argv[argc - 2], "r"); // argv[1]
    if (!fp_read) {
        perror("[ERR!] Read File");
        errno = 0;
        result = 1;
        goto SHUTDOWN;
    }

    trie_t *trie = radix_create();
    if (!trie) {
        fprintf(stderr, "Trie alloc() failure\n");
        result = 1;
        goto FCLOSE_READ;
    }

    char *lineptr = calloc(MAX_CHAR, sizeof(*lineptr));
    if (!lineptr) {
        perror("String allocation");
	errno = 0;
        result = 1;
        goto CLEANUP_TRIE;
    }

    const char *accept = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcedfghijklmnopqrstuvwxyz";
    uint64_t str_len = 0;

    // Adds one word at a time to the trie
    // NOTE: Assumes one word per line, else strspn will reject line
    //
    while (fgets(lineptr, MAX_CHAR, fp_read)) {
        // Cleans line of newline, if it exists
        //
        str_len = strlen(lineptr);
        if ('\n' == lineptr[str_len - 1]) {
            lineptr[--str_len] = '\0';
        }

        // Ignores any string to upper AND lowercase
        // On student to determine how to handle within insert function
        // - Opt 1: Reject entirely
        // - Opt 2: Squash to lowercase/uppercase
        // - Opt 3: Handle Upper and lower values uniquely (not recommended)
        //
        // Determined usage must be included in test plan
        //
        if (str_len == strspn(lineptr, accept)) {
            radix_insert_word(trie, lineptr);
        }
    }

    FILE *fp_search = fopen(argv[argc - 1], "r"); // argv[2]
    if (!fp_search) {
        perror("[ERR!] Search File");
        errno = 0;
        result = 1;
        goto CLEANUP_PTR;
    }

    // Searches trie for one word at a time
    // NOTE: Assumes one word per line, else strspn will reject line
    //
    while (fgets(lineptr, MAX_CHAR, fp_search)) {
        // Cleans line of newline, if it exists
        //
        str_len = strlen(lineptr);
        if ('\n' == lineptr[str_len - 1]) {
            lineptr[--str_len] = '\0';
        }

        // Allows upper/lower/combination words through
        // Students choice on how to handle within find_word() 
        // but should logically match the insertion scenario allowed
        //
        // Determined usage must be included in test plan
        //
        if (str_len == strspn(lineptr, accept)) {
            if (radix_find_word(trie, lineptr)) {
                printf("EXISTS : %s\n", lineptr);
            }
        }
    }

    fclose(fp_search);
CLEANUP_PTR:
    free(lineptr);
CLEANUP_TRIE:
    radix_delete(&trie);
FCLOSE_READ:
    fclose(fp_read);
SHUTDOWN:
    return result;
}
