#include <stdio.h>

int mod(int b, int a) {
    return a == 0 ? 0 : b % a;
}

int fux(int b, int a) {
    printf("fux:::: ");
    printf("b : %d, a : %d\n", b, a);
    int ret = a ? b % a : 0;
    printf("b : %d, a : %d\n", b, a);
    return ret;
}


int main() {
    printf("mod b, a\n");
    printf("mod 2, 3: %d\n", mod(2, 3));
    printf("mod 0, 3: %d\n", mod(0, 3));
    printf("mod 2, 0: %d\n", mod(2, 0));
    printf("fux 2, 3: %d\n", fux(2, 3));
    printf("fux 0, 3: %d\n", fux(0, 3));
    printf("fux 2, 0: %d\n", fux(2, 0));

    printf("finished!\n");

    return 0;
}
