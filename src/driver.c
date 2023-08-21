#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errno.h"

#include "../include/radix.h"

int main (void) {
        trie_t *root = radix_create();

        FILE *fp = fopen("./data.txt", "r");
        if (!fp) {
                perror("fopen");
                errno = 0;
                return;
        }
        int nl_loc = 0;
        char *string = calloc(50, sizeof(char));
        while(fgets(string, 50, fp)) {
                nl_loc = strcspn(string, "\n");
                string[nl_loc] = '\0';
                radix_insert_word(root, string);
        }
        radix_print(root);

}