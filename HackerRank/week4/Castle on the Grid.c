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

typedef struct {
    int x, y, moves;
} Node;

int minimumMoves(int grid_count, char** grid, int startX, int startY, int goalX, int goalY) {
    int n = grid_count;
    bool visited[n][n];
    memset(visited, false, sizeof(visited));

    Node queue[n * n];
    int front = 0, rear = 0;

    queue[rear++] = (Node){startX, startY, 0};
    visited[startX][startY] = true;

    while (front < rear) {
        Node curr = queue[front++];
        int x = curr.x;
        int y = curr.y;
        int moves = curr.moves;

        if (x == goalX && y == goalY) {
            return moves;
        }

        // UP
        for (int i = x - 1; i >= 0 && grid[i][y] != 'X'; i--) {
            if (!visited[i][y]) {
                visited[i][y] = true;
                queue[rear++] = (Node){i, y, moves + 1};
            }
        }
        // DOWN
        for (int i = x + 1; i < n && grid[i][y] != 'X'; i++) {
            if (!visited[i][y]) {
                visited[i][y] = true;
                queue[rear++] = (Node){i, y, moves + 1};
            }
        }
        // LEFT
        for (int j = y - 1; j >= 0 && grid[x][j] != 'X'; j--) {
            if (!visited[x][j]) {
                visited[x][j] = true;
                queue[rear++] = (Node){x, j, moves + 1};
            }
        }
        // RIGHT
        for (int j = y + 1; j < n && grid[x][j] != 'X'; j++) {
            if (!visited[x][j]) {
                visited[x][j] = true;
                queue[rear++] = (Node){x, j, moves + 1};
            }
        }
    }

    return -1;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));

    char** grid = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* grid_item = readline();
        *(grid + i) = grid_item;
    }

    char** first_multiple_input = split_string(rtrim(readline()));

    int startX = parse_int(*(first_multiple_input + 0));
    int startY = parse_int(*(first_multiple_input + 1));
    int goalX = parse_int(*(first_multiple_input + 2));
    int goalY = parse_int(*(first_multiple_input + 3));

    int result = minimumMoves(n, grid, startX, startY, goalX, goalY);

    fprintf(fptr, "%d\n", result);

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
        if (!line) break;
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;
        alloc_length <<= 1;
        data = realloc(data, alloc_length);
        if (!data) break;
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
