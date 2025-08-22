#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    char s[100005];
    if (scanf("%100004s", s) != 1) return 0;

    int n = strlen(s);
    int freq[26] = {0};
    for (int i = 0; i < n; i++) {
        if (s[i] >= 'a' && s[i] <= 'z') freq[s[i] - 'a']++;
        /* if other characters allowed, handle accordingly */
    }

    int maxFreq = 0;
    for (int i = 0; i < 26; i++) if (freq[i] > maxFreq) maxFreq = freq[i];
    if (maxFreq == 0) { /* empty string -> trivially valid */
        printf("YES\n");
        return 0;
    }

    int *countFreq = (int*)calloc(maxFreq + 1, sizeof(int));
    if (!countFreq) return 0;
    for (int i = 0; i < 26; i++) if (freq[i] > 0) countFreq[freq[i]]++;

    int distinct = 0;
    int f1 = 0, c1 = 0, f2 = 0, c2 = 0;
    for (int f = 1; f <= maxFreq; f++) {
        if (countFreq[f] > 0) {
            distinct++;
            if (distinct == 1) { f1 = f; c1 = countFreq[f]; }
            else if (distinct == 2) { f2 = f; c2 = countFreq[f]; }
            else break;
        }
    }
    free(countFreq);

    if (distinct == 1) {
        printf("YES\n");
        return 0;
    }
    if (distinct > 2) {
        printf("NO\n");
        return 0;
    }

    /* distinct == 2, with f1 < f2 by construction */
    if ((f1 == 1 && c1 == 1) || (f2 == f1 + 1 && c2 == 1)) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
    return 0;
}
