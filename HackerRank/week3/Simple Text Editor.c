#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1000000

// Stack node for storing previous states
typedef struct {
    char *state;
} History;

int main() {
    int Q;
    scanf("%d", &Q);

    char S[MAX_LEN + 1] = ""; // Current string

    // History stack
    History history[Q];
    int top = -1;

    for (int i = 0; i < Q; i++) {
        int type;
        scanf("%d", &type);

        if (type == 1) { // append
            char w[MAX_LEN + 1];
            scanf("%s", w);

            // Save current state
            top++;
            history[top].state = strdup(S);

            // Append
            strcat(S, w);

        } else if (type == 2) { // delete
            int k;
            scanf("%d", &k);

            // Save current state
            top++;
            history[top].state = strdup(S);

            int len = strlen(S);
            if (k <= len) {
                S[len - k] = '\0';
            }

        } else if (type == 3) { // print
            int k;
            scanf("%d", &k);
            if (k > 0 && k <= strlen(S)) {
                printf("%c\n", S[k - 1]);
            }

        } else if (type == 4) { // undo
            if (top >= 0) {
                strcpy(S, history[top].state);
                free(history[top].state);
                top--;
            }
        }
    }

    // Free remaining states (in case of memory leaks)
    while (top >= 0) {
        free(history[top].state);
        top--;
    }

    return 0;
}
