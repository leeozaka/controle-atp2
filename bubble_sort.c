#include <stdio.h>
#include <stdlib.h>

void bubblesort(int numeros[], int size) {
    int swap = 1;
    while (swap) {
        swap = 0;
        for (int i = 0; i < size - 1; ++i) {
            if (numeros[i] > numeros[i + 1]) {
                int temp = numeros[i];
                numeros[i] = numeros[i + 1];
                numeros[i + 1] = temp;
                ++swap;
            }
        }
    }
}

int _buscabinaria(int numeros[], int x, int start, int stop) {
    if (stop >= start) {
        int mid  = start + (stop-start) / 2;
        if (numeros[mid] == x) return mid;
        else if (numeros[mid] > x) return _buscabinaria(numeros, x, start, mid-1);
        else return _buscabinaria(numeros, x, mid+1, stop);
    }
    return -1;
}

int main() {
    int numeros[] = {1, 2, 4, 3, 5, 6, 7, 9, 8};
    bubblesort(numeros, 9);
    for (int i = 0; i < 9; i++) {
        printf("%d ", numeros[i]);
    }
    printf("num 3 pos %d", _buscabinaria(numeros, 3, 0, 9));
}