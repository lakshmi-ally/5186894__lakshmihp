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

// DFS to explore connected cities
void dfs(int node, bool* visited, int** adj, int* adjSize) {
    visited[node] = true;
    for (int i = 0; i < adjSize[node]; i++) {
        int next = adj[node][i];
        if (!visited[next]) {
            dfs(next, visited, adj, adjSize);
        }
    }
}

long roadsAndLibraries(int n, int c_lib, int c_road, int cities_rows, int cities_columns, int** cities) {
    // If building a library is cheaper or equal to building a road,
    // just build a library in each city
    if (c_lib <= c_road) {
        return (long)c_lib * n;
    }

    // Create adjacency list
    int** adj = (int**)malloc((n + 1) * sizeof(int*));
    int* adjSize = (int*)calloc(n + 1, sizeof(int));

    for (int i = 0; i <= n; i++) {
        adj[i] = NULL;
        adjSize[i] = 0;
    }

    for (int i = 0; i < cities_rows; i++) {
        int u = cities[i][0];
        int v = cities[i][1];
        adj[u] = realloc(adj[u], (adjSize[u] + 1) * sizeof(int));
        adj[v] = realloc(adj[v], (adjSize[v] + 1) * sizeof(int));
        adj[u][adjSize[u]++] = v;
        adj[v][adjSize[v]++] = u;
    }

    bool* visited = (bool*)calloc(n + 1, sizeof(bool));
    long total_cost = 0;

    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            // New connected component found
            dfs(i, visited, adj, adjSize);

            // One library + (component_size - 1) roads
            // Since DFS just marks visited, count roads later
            // We'll simulate: for each connected component, build 1 library + (size-1)*road cost
            // But DFS doesn't return size here, so let's adjust:
        }
    }

    // Better DFS that counts component size
    memset(visited, false, (n + 1) * sizeof(bool));
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            int stackSize = 0;
            int* stack = (int*)malloc(n * sizeof(int));
            stack[stackSize++] = i;
            visited[i] = true;
            int component_size = 0;

            while (stackSize > 0) {
                int node = stack[--stackSize];
                component_size++;
                for (int j = 0; j < adjSize[node]; j++) {
                    int next = adj[node][j];
                    if (!visited[next]) {
                        visited[next] = true;
                        stack[stackSize++] = next;
                    }
                }
            }

            total_cost += c_lib + (long)(component_size - 1) * c_road;
            free(stack);
        }
    }

    // Free adjacency list
    for (int i = 0; i <= n; i++) {
        free(adj[i]);
    }
    free(adj);
    free(adjSize);
    free(visited);

    return total_cost;
}

int main() {
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int q = parse_int(ltrim(rtrim(readline())));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        char** first_multiple_input = split_string(rtrim(readline()));

        int n = parse_int(*(first_multiple_input + 0));
        int m = parse_int(*(first_multiple_input + 1));
        int c_lib = parse_int(*(first_multiple_input + 2));
        int c_road = parse_int(*(first_multiple_input + 3));

        int** cities = malloc(m * sizeof(int*));

        for (int i = 0; i < m; i++) {
            *(cities + i) = malloc(2 * (sizeof(int)));
            char** cities_item_temp = split_string(rtrim(readline()));

            for (int j = 0; j < 2; j++) {
                int cities_item = parse_int(*(cities_item_temp + j));
                *(*(cities + i) + j) = cities_item;
            }
        }

        long result = roadsAndLibraries(n, c_lib, c_road, m, 2, cities);

        fprintf(fptr, "%ld\n", result);

        for (int i = 0; i < m; i++) {
            free(*(cities + i));
        }
        free(cities);
    }

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
    if (!str) return '\0';
    if (!*str) return str;
    while (*str != '\0' && isspace(*str)) {
        str++;
    }
    return str;
}

char* rtrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
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
        if (!splits) return splits;
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

