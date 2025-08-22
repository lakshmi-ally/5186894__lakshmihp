#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char data;             // character (only set at leaves)
    struct Node *left;
    struct Node *right;
} Node;

Node* newNode(char data) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) exit(EXIT_FAILURE);
    n->data = data;
    n->left = n->right = NULL;
    return n;
}

void insertCode(Node *root, const char *code, char ch) {
    Node *cur = root;
    for (size_t i = 0; code[i] != '\0'; ++i) {
        if (code[i] == '0') {
            if (!cur->left) cur->left = newNode('\0');
            cur = cur->left;
        } else if (code[i] == '1') {
            if (!cur->right) cur->right = newNode('\0');
            cur = cur->right;
        } else {
            // ignore any other characters (shouldn't happen)
        }
    }
    // place the character at the leaf
    cur->data = ch;
}

void decode_and_print(Node *root, const char *enc) {
    if (!root) return;

    // Special case: tree with only one node (single unique character)
    if (root->left == NULL && root->right == NULL) {
        // the entire encoded string corresponds to repeated occurrences of this single char
        size_t len = strlen(enc);
        for (size_t i = 0; i < len; ++i) putchar(root->data);
        return;
    }

    Node *cur = root;
    for (size_t i = 0; enc[i] != '\0'; ++i) {
        if (enc[i] == '0') {
            cur = cur->left;
        } else if (enc[i] == '1') {
            cur = cur->right;
        } else {
            // skip invalid characters
            continue;
        }

        if (!cur) {
            // invalid code path — stop to avoid segfault
            break;
        }

        // If leaf, output and reset to root
        if (cur->left == NULL && cur->right == NULL) {
            putchar(cur->data);
            cur = root;
        }
    }
}

void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Failed to read number of codes\n");
        return 1;
    }

    // allocate buffers for reading tokens
    // code lengths are usually small; give a generous buffer
    const size_t CODE_BUF = 200000;
    const size_t ENC_BUF  = 2000000;
    char *codebuf = (char*)malloc(CODE_BUF);
    char *encbuf  = (char*)malloc(ENC_BUF);
    if (!codebuf || !encbuf) exit(EXIT_FAILURE);

    Node *root = newNode('\0');

    for (int i = 0; i < n; ++i) {
        char ch;
        // read a non-whitespace char and its code
        if (scanf(" %c %199999s", &ch, codebuf) != 2) {
            fprintf(stderr, "Invalid codebook input at line %d\n", i+1);
            free_tree(root);
            free(codebuf);
            free(encbuf);
            return 1;
        }
        insertCode(root, codebuf, ch);
    }

    // read encoded string (single token of 0/1)
    if (scanf(" %1999999s", encbuf) != 1) {
        fprintf(stderr, "Failed to read encoded string\n");
        free_tree(root);
        free(codebuf);
        free(encbuf);
        return 1;
    }

    // decode and print
    decode_and_print(root, encbuf);
    putchar('\n');

    // cleanup
    free_tree(root);
    free(codebuf);
    free(encbuf);

    return 0;
}
