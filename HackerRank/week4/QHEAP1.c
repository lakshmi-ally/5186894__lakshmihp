#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define MAX 100000

int heap[MAX]; // store the elements
int heap_size = 0;

void insert(int x) {
    heap[heap_size++] = x;
}

void delete_elem(int x) {
    for (int i = 0; i < heap_size; i++) {
        if (heap[i] == x) {
            heap[i] = heap[--heap_size]; // replace with last element
            return;
        }
    }
}

int get_min() {
    int min = heap[0];
    for (int i = 1; i < heap_size; i++) {
        if (heap[i] < min)
            min = heap[i];
    }
    return min;
}

int main() {
    int Q;
    scanf("%d", &Q);

    for (int i = 0; i < Q; i++) {
        int type, val;
        scanf("%d", &type);

        if (type == 1) {
            scanf("%d", &val);
            insert(val);
        } 
        else if (type == 2) {
            scanf("%d", &val);
            delete_elem(val);
        } 
        else if (type == 3) {
            printf("%d\n", get_min());
        }
    }
    return 0;
}
