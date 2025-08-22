
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

/*
 * Helpers
 */
static char** detonate(int rows, int cols, char** grid) {
    char** result = (char**)malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        result[i] = (char*)malloc(cols + 1);
        for (int j = 0; j < cols; j++) result[i][j] = 'O';
        result[i][cols] = '\0';
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 'O') {
                result[i][j] = '.';
                if (i > 0)           result[i - 1][j] = '.';
                if (i < rows - 1)    result[i + 1][j] = '.';
                if (j > 0)           result[i][j - 1] = '.';
                if (j < cols - 1)    result[i][j + 1] = '.';
            }
        }
    }
    return result;
}

/*
 * Complete the 'bomberMan' function below.
 *
 * The function is expected to return a STRING_ARRAY.
 * The function accepts:
 *  1. INTEGER n
 *  2. STRING_ARRAY grid (rows = grid_count, cols inferred)
 */
char** bomberMan(int n, int grid_count, char** grid, int* result_count) {
    *result_count = grid_count;
    int cols = (int)strlen(grid[0]);

    // n == 1 -> initial grid
    if (n == 1) {
        char** out = (char**)malloc(grid_count * sizeof(char*));
        for (int i = 0; i < grid_count; i++) {
            out[i] = (char*)malloc(cols + 1);
            strcpy(out[i], grid[i]);
        }
        return out;
    }

    // Even seconds -> full bombs
    if (n % 2 == 0) {
        char** out = (char**)malloc(grid_count * sizeof(char*));
        for (int i = 0; i < grid_count; i++) {
            out[i] = (char*)malloc(cols + 1);
            for (int j = 0; j < cols; j++) out[i][j] = 'O';
            out[i][cols] = '\0';
        }
        return out;
    }

    // Odd n > 1 cycles:
    // n % 4 == 3 -> first detonation pattern
    // n % 4 == 1 -> second detonation pattern
    char** first = detonate(grid_count, cols, grid);
    if (n % 4 == 3) {
        return first;
    }

    char** second = detonate(grid_count, cols, first);
    // free first
    for (int i = 0; i < grid_count; i++) free(first[i]);
    free(first);

    return second; // covers n % 4 == 1
}

int main() {
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int r = parse_int(*(first_multiple_input + 0));
    int c = parse_int(*(first_multiple_input + 1));
    int n = parse_int(*(first_multiple_input + 2));
    (void)c; // silence unused variable warning (we infer cols from strings)

    char** grid = (char**)malloc(r * sizeof(char*));
    for (int i = 0; i < r; i++) {
        char* grid_item = readline();
        grid[i] = grid_item;
    }

    int result_count = 0; // initialize to silence -Wmaybe-uninitialized
    char** result = bomberMan(n, r, grid, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%s", result[i]);
        if (i != result_count - 1) fprintf(fptr, "\n");
    }
    fprintf(fptr, "\n");

    // (Optional) free allocations made in main
    for (int i = 0; i < r; i++) free(grid[i]);
    free(grid);
    for (int i = 0; i < result_count; i++) free(result[i]);
    free(result);

    fclose(fptr);
    return 0;
}

/* Boilerplate I/O helpers (unchanged) */
char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = (char*)malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) break;

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;

        alloc_length <<= 1;
        data = (char*)realloc(data, alloc_length);
        if (!data) { data = '\0'; break; }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = (char*)realloc(data, data_length);
        if (!data) data = '\0';
    } else {
        data = (char*)realloc(data, data_length + 1);
        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
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
        splits = (char**)realloc(splits, sizeof(char*) * ++spaces);
        if (!splits) return splits;
        splits[spaces - 1] = token;
        token = strtok(NULL, " ");
    }
    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = (int)strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}

