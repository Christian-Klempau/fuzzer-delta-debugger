#include <stdlib.h>
#include <stdio.h>

int main()
{
    for (int i = 0; i < 10; i++)
        printf("%f\n", random() / (double)((1L << 31) - 1));
    return 0;
}