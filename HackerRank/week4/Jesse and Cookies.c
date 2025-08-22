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

/* ---------- Min-heap (long long) helpers ---------- */
static inline void ll_swap(long long* a, long long* b) {
    long long t = *a; *a = *b; *b = t;
}

void heapify_down_ll(long long* heap, int size, int i) {
    while (1) {
        int smallest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        if (l < size && heap[l] < heap[smallest]) smallest = l;
        if (r < size && heap[r] < heap[smallest]) smallest = r;
        if (smallest == i) break;
        ll_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

void heapify_up_ll(long long* heap, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[i] < heap[p]) {
            ll_swap(&heap[i], &heap[p]);
            i = p;
        } else break;
    }
}

long long heap_pop_ll(long long* heap, int* size) {
    long long root = heap[0];
    heap[0] = heap[--(*size)];
    heapify_down_ll(heap, *size, 0);
    return root;
}

void heap_push_ll(long long* heap, int* size, long long val) {
    heap[(*size)++] = val;
    heapify_up_ll(heap, (*size) - 1);
}

/*
 * Complete the 'cookies' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER k
 *  2. INTEGER_ARRAY A
 */
int cookies(int k, int A_count, int* A) {
    if (A_count == 0) return -1;

    // Build heap from A (use long long for safety)
    long long* heap = (long long*)malloc((size_t)A_count * sizeof(long long));
    if (!heap) exit(EXIT_FAILURE);

    int size = A_count;
    for (int i = 0; i < A_count; i++) heap[i] = (long long)A[i];
    for (int i = (size / 2) - 1; i >= 0; i--) heapify_down_ll(heap, size, i);

    int ops = 0;

    while (size > 0 && heap[0] < (long long)k) {
        if (size < 2) { // cannot combine further
            free(heap);
            return -1;
        }
        long long least = heap_pop_ll(heap, &size);
        long long second = heap_pop_ll(heap, &size);
        long long combined = least + 2LL * second;
        heap_push_ll(heap, &size, combined);
        ops++;
    }

    // Now smallest is >= k or no elements left
    int result = (size > 0 && heap[0] >= (long long)k) ? ops : -1;
    free(heap);
    return result;
}

/* ------------ I/O helpers and main (as given) ------------ */

int main() {
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));
    int n = parse_int(*(first_multiple_input + 0));
    int k = parse_int(*(first_multiple_input + 1));

    char** A_temp = split_string(rtrim(readline()));
    int* A = (int*)malloc((size_t)n * sizeof(int));
    if (!A) exit(EXIT_FAILURE);

    for (int i = 0; i < n; i++) {
        int A_item = parse_int(*(A_temp + i));
        A[i] = A_item;
    }

    int result = cookies(k, n, A);
    fprintf(fptr, "%d\n", result);

    fclose(fptr);
    free(A);
    return 0;
}

char* readline() {
    size_t alloc_length = 1024, data_length = 0;
    char* data = (char*)malloc(alloc_length);
    if (!data) exit(EXIT_FAILURE);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, (int)(alloc_length - data_length), stdin);
        if (!line) break;
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;
        alloc_length <<= 1;
        data = (char*)realloc(data, alloc_length);
        if (!data) exit(EXIT_FAILURE);
    }

    if (data_length > 0 && data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = (char*)realloc(data, data_length);
        if (!data) exit(EXIT_FAILURE);
    } else {
        data = (char*)realloc(data, data_length + 1);
        if (!data) exit(EXIT_FAILURE);
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
        splits = (char**)realloc(splits, sizeof(char*) * ++spaces);
        if (!splits) exit(EXIT_FAILURE);
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
