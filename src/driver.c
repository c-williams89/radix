#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errno.h"

#include "../include/radix.h"

int main(void)
{
	trie_t *root = radix_create();

	FILE *fp = fopen("./test/p_dict.txt", "r");
	// FILE *fp = fopen("./data.txt", "r");
	if (!fp) {
		perror("fopen");
		errno = 0;
		return 1;
	}
	// int nl_loc = 0;
	// char *string = calloc(50, sizeof(char));
        // char string[5] = { '\0 '};
	// while (fgets(string, 50, fp)) {
	// 	nl_loc = strcspn(string, "\n");
	// 	string[nl_loc] = '\0';
	// 	radix_insert_word(root, string);
	// }
        // free(string);
	// radix_print(root);
	fclose(fp);

        // Test for the known words:
        char *test_words[] = {
                "places",
                "pickling",
                "placebo",
                "play",
                "picture",
                "pickets",
                "panacea",
                "pick",
                "picket",
                "pickles"
        };

        for (int i = 0; i < 10; ++i) {
                radix_insert_word(root, test_words[i]);
        }
        radix_print(root);
	// char *test_words[] =
	//     { "pick", "pla", "play", "banana", "pickling", "p", "placebos",
        //         "pic", NULL,  };
        
        
        printf("\n\nValid words test:\n");
	for (int i = 0; i < 10; ++i) {
		if (radix_find_word(root, test_words[i])) {
			printf("%s:\tFound!\n", test_words[i]);
		} else {
			printf("%s:\tNot Found\n", test_words[i]);
		}
	}



        // Test for invalid words and prefixes
        char *invalid_words[] = {
                "pla",
                "banana",
                "p",
                "placebos",
                "pic",
                "pan"
        };
        
        // printf("\n\nInvalid words test:\n");
        // for (int i = 0; i < 6; ++i) {
	// 	if (radix_find_word(root, invalid_words[i])) {
	// 		printf("%s:\tFound!\n", invalid_words[i]);
	// 	} else {
	// 		printf("%s:\tNot Found\n", invalid_words[i]);
	// 	}
	// }
	// if (radix_remove_word(root, "pickling")) {
        //         printf("pickling deleted\n");
        // }
	// if (!radix_find_word(root, "pickling")) {
	// 	printf("pickling not found\n");
	// }

        radix_find_prefix(root, "p");
        // radix_find_prefix(root, "picklet");


        radix_delete(&root);
	return 1;

}
