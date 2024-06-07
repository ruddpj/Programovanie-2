#include <stdio.h>

void primeFactors(int a, int m) {
    if (a < 2) {
        printf("ERROR\n");
        return;
    }

    int x = 2;
    int keep_a = a;
    int k = 0;
    while ((x <= keep_a) && (k < m)) {
        if (a % x) {
            x++;
        } else {
            printf("%d\n", x);
            while (!(a % x)) {
                a = a / x;
            }
            k++;
        }
    }
}

int main() {
    int a, b;
    scanf("%d %d", &a, &b);
    primeFactors(a, b);
    return 0;
}
