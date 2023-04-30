#include <math.h>
#include <stdbool.h>
#include <stdio.h>

int main(){
    int a = 10;
    int b = a;
    b |= 0x80000000;
    printf("%d %d", a, b);
    
}
