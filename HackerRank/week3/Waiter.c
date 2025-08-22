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

// ---------- helper for primes ----------
int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

void generate_primes(int *primes, int q) {
    int count = 0, num = 2;
    while (count < q) {
        if (is_prime(num)) {
            primes[count++] = num;
        }
        num++;
    }
}

// ---------- main waiter logic ----------
int* waiter(int number_count, int* number, int q, int* result_count) {
    int *primes = (int*)malloc(q * sizeof(int));
    generate_primes(primes, q);

    int *result = (int*)malloc(number_count * sizeof(int));
    int res_index = 0;

    int *A = (int*)malloc(number_count * sizeof(int));
    int *nextA = (int*)malloc(number_count * sizeof(int));
    int topA = number_count - 1;

    // copy input into stack A (as given, last element is top)
    for (int i = 0; i < number_count; i++) {
        A[i] = number[i];
    }

    for (int i = 0; i < q; i++) {
        int prime = primes[i];
        int topNextA = -1;
        int *B = (int*)malloc(number_count * sizeof(int));
        int topB = -1;

        for (int j = topA; j >= 0; j--) {
            if (A[j] % prime == 0) {
                B[++topB] = A[j];
            } else {
                nextA[++topNextA] = A[j];
            }
        }

        for (int j = topB; j >= 0; j--) {
            result[res_index++] = B[j];
        }

        free(B);

        for (int j = 0; j <= topNextA; j++) {
            A[j] = nextA[j];
        }
        topA = topNextA;
    }

    for (int j = topA; j >= 0; j--) {
        result[res_index++] = A[j];
    }

    free(A);
    free(nextA);
    free(primes);

    *result_count = res_index;
    return result;
}

// ---------- main provided by HackerRank ----------
int main() {
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));
    int n = parse_int(*(first_multiple_input + 0));
    int q = parse_int(*(first_multiple_input + 1));

    char** number_temp = split_string(rtrim(readline()));
    int* number = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        int number_item = parse_int(*(number_temp + i));
        *(number + i) = number_item;
    }

    int result_count;
    int* result = waiter(n, number, q, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", *(result + i));
        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }
    fprintf(fptr, "\n");

    fclose(fptr);
    return 0;
}

// ---------- helpers given ----------
char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        if (!line) break;
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;
        alloc_length <<= 1;
        data = realloc(data, alloc_length);
        if (!data) return '\0';
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);
        data[data_length] = '\0';
    }
    return data;
}

char* ltrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int spaces = 0;
    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);
        splits[spaces - 1] = token;
        token = strtok(NULL, " ");
    }
    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}
