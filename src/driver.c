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
	int nl_loc = 0;
	char *string = calloc(50, sizeof(char));
	while (fgets(string, 50, fp)) {
		nl_loc = strcspn(string, "\n");
		string[nl_loc] = '\0';
		radix_insert_word(root, string);
	}
        free(string);
	radix_print(root);
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
	// char *test_words[] =
	//     { "pick", "pla", "play", "banana", "pickling", "p", "placebos",
        //         "pic", NULL,  };
        printf("\n\nValid words test:\n");
	for (int i = 0; i < 10; ++i) {
		if (radix_find_word(root, test_words[i])) {
			printf("%s:Found!\n", test_words[i]);
		} else {
			printf("%s:\tNot Found\n", test_words[i]);
		}
	}



        // Test for invalid words and prefixes
        char *invalid_words[] = {
                NULL,
                "pla",
                "banana",
                "p",
                "placebos",
                "pic",
                "pan"
        };
        printf("\n\nInvalid words test:\n");
        for (int i = 0; i < 7; ++i) {
		if (radix_find_word(root, invalid_words[i])) {
			printf("%s:\tFound!\n", invalid_words[i]);
		} else {
			printf("%s:\tNot Found\n", invalid_words[i]);
		}
	}
	radix_remove_word(root, "pickling");
	if (!radix_find_word(root, "pickling")) {
		printf("pickling not found\n");
	}
        radix_delete(&root);
	return 1;

}
