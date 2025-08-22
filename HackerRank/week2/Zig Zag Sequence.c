#include <stdio.h>
#include <stdlib.h>

int cmpfunc(const void *a, const void *b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

void findZigZagSequence(int a[], int n) {
    qsort(a, n, sizeof(int), cmpfunc);

    int mid = n / 2;               // middle index for odd n (0-based)
    int temp = a[mid];
    a[mid] = a[n - 1];
    a[n - 1] = temp;

    int st = mid + 1;
    int ed = n - 2;                // reverse the tail up to n-2
    while (st <= ed) {
        temp = a[st];
        a[st] = a[ed];
        a[ed] = temp;
        st++; ed--;
    }

    for (int i = 0; i < n; i++) {
        if (i) printf(" ");
        printf("%d", a[i]);
    }
    printf("\n");
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        int n; scanf("%d", &n);
        int *a = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) scanf("%d", &a[i]);
        findZigZagSequence(a, n);
        free(a);
    }
    return 0;
}
