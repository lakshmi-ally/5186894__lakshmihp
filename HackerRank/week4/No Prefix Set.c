#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define ALPHABET_SIZE 10  // only 'a' to 'j'

// ---------------- Trie Node ----------------
typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
} TrieNode;

TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}

// Insert word and check if BAD SET
bool insertAndCheck(TrieNode* root, char* word) {
    TrieNode* curr = root;

    for (int i = 0; word[i] != '\0'; i++) {
        int idx = word[i] - 'a';

        if (curr->isEndOfWord) {
            // Previous word is a prefix of current
            return true;
        }

        if (!curr->children[idx]) {
            curr->children[idx] = createNode();
        }
        curr = curr->children[idx];
    }

    // If current word is identical to an existing one
    if (curr->isEndOfWord) return true;

    // If current word is a prefix of an existing word
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (curr->children[i] != NULL) {
            return true;
        }
    }

    curr->isEndOfWord = true;
    return false;
}

void noPrefix(int words_count, char** words) {
    TrieNode* root = createNode();

    for (int i = 0; i < words_count; i++) {
        if (insertAndCheck(root, words[i])) {
            printf("BAD SET\n%s\n", words[i]);
            return;
        }
    }
    printf("GOOD SET\n");
}

// ---------------- Template Helper Functions ----------------
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
    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    return value;
}

// ---------------- Main ----------------
int main() {
    int n = parse_int(ltrim(rtrim(readline())));
    char** words = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* words_item = readline();
        *(words + i) = words_item;
    }

    noPrefix(n, words);
    return 0;
}
