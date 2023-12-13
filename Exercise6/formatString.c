#include <stdio.h>

int num1 = 0xdead;

int main(int argc, char **argv){
    int num2 = 0xbeef;
    int *ptr = &num1;
    printf(argv[1]);

    if (num1 == 0xabc){
        printf("Well done");
    }
    if(num2 == 0xdef)
        printf("You are a format string expert");

    printf("\n[DEBUG] num1: 0x%x [%p] num2: 0x%x [%p]\n", num1, &num1, num2, &num2);
    return 0;
}