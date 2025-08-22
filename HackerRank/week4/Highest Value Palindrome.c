#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);
char* highestValuePalindrome(char* s, int n, int k) {
    char* str = malloc((n + 1) * sizeof(char));
    strcpy(str, s);

    int* changed = calloc(n, sizeof(int)); // track which positions were changed
    int left = 0, right = n - 1;
    int changes_needed = 0;

    // Step 1: Make it a palindrome
    while (left < right) {
        if (str[left] != str[right]) {
            char max_digit = (str[left] > str[right]) ? str[left] : str[right];
            str[left] = str[right] = max_digit;
            changed[left] = changed[right] = 1;
            k--;
            changes_needed++;
            if (k < 0) {
                free(changed);
                free(str);
                return "-1";
            }
        }
        left++;
        right--;
    }

    // Step 2: Maximize value by turning digits into '9'
    left = 0;
    right = n - 1;
    while (left <= right) {
        if (left == right) { // middle digit in odd length number
            if (k > 0 && str[left] != '9') {
                str[left] = '9';
                k--;
            }
        } else {
            if (str[left] != '9') {
                if (changed[left] && k > 0) {
                    // already changed once in step 1, so only 1 more change needed
                    str[left] = str[right] = '9';
                    k--;
                } else if (!changed[left] && k > 1) {
                    // not changed in step 1, so need 2 changes to make both '9'
                    str[left] = str[right] = '9';
                    k -= 2;
                }
            }
        }
        left++;
        right--;
    }

    free(changed);
    return str;


}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int k = parse_int(*(first_multiple_input + 1));

    char* s = readline();

    char* result = highestValuePalindrome(s, n, k);

    fprintf(fptr, "%s\n", result);

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
