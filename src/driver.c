#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errno.h"

#include "../include/radix.h"

int main (void) {
        trie_t *root = radix_create();

        FILE *fp = fopen("./test/p_dict.txt", "r");
        // FILE *fp = fopen("./data.txt", "r");
        if (!fp) {
                perror("fopen");
                errno = 0;
                return 1;
        }
        int nl_loc = 0;
        char *string = calloc(50, sizeof(char));
        while(fgets(string, 50, fp)) {
                nl_loc = strcspn(string, "\n");
                string[nl_loc] = '\0';
                radix_insert_word(root, string);
        }
        radix_print(root);
        fclose(fp);

        char *test_words[] = { "pick", "pla", "play", "banana", "picklings", "p", "placebos", "pic", NULL};

        for (int i = 0; i < 9; ++i) {
                if (radix_find_word(root, test_words[i])) {
                        printf("%s Found!\n", test_words[i]);
                } else {
                        printf("%s Not Found\n", test_words[i]);
                }
        }
        return 1;

}