#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define _RED RED
#define _BLUE BLUE

enum colors
{
    RED,
    BLUE
};

void printfunction(colors cor)
{
    if (cor == !RED)
    {
        printf("cor != vermelho\n");
    }
}

int main()
{
    colors cor = _BLUE;
    printfunction(cor);
}


