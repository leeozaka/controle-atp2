#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <conio2.h>

/*
void clrfunc(int LI, int CI, int LF, int CF)
{
    int i;
    int j;
    for (i = LI; i <= LF; i++)
    {
        for (j = CI; j <= CF; j++)
        {
            gotoxy(j, i);
            printf("%c", 32);
        }
    }
} */

// arg p conversao em int
char *conv(unsigned int, int);

/*void clearElement(char *regiao)
{
    if (strcmp(regiao, "topo") == 0)
        clrfunc(3, 3, 3, 70);
    else if (strcmp(regiao, "alerta") == 0)
        clrfunc(23, 3, 23, 78);
    else if (strcmp(regiao, "switcher") == 0)
    {
        clrfunc(23, 3, 23, 78);
        clrfunc(8, 3, 20, 26);
    }
    // switcher gatilho para limpar com printf
    // o menu esquerdo

    // else if (strcmp(regiao, "rightside") == 0)
    //     clrfunc();
} */

char *conv(unsigned int numero, int base)
{
    static char Rep[] = "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = Rep[numero % base];
        numero /= base;
    } while (numero != 0);
    return ptr;
}

/*conioPrintf("string", "posicao", "Cor", somar linha);*/
void conioPrintf(char *str, char *posicao, char *cor, int linha, ...)
{
    unsigned int i;
    char *transc;
    char *s;

    va_list arg;
    va_start(arg, str);
    // setColor
    if (strcmp(cor, "preto") == 0)
        textcolor(0);
    else if (strcmp(cor, "azul") == 0)
        textcolor(1);
    else if (strcmp(cor, "verde") == 0)
        textcolor(2);
    else if (strcmp(cor, "ciano") == 0)
        textcolor(3);
    else if (strcmp(cor, "vermelho") == 0)
        textcolor(4);
    else if (strcmp(cor, "rosa") == 0)
        textcolor(5);
    else if (strcmp(cor, "marrom") == 0)
        textcolor(6);
    else if (strcmp(cor, "branco") == 0)
        textcolor(7);
    else if (strcmp(cor, "cinza_claro") == 0)
        textcolor(8);
    else if (strcmp(cor, "azul_claro") == 0)
        textcolor(9);
    else if (strcmp(cor, "verde_claro") == 0)
        textcolor(10);
    else if (strcmp(cor, "ciano_claro") == 0)
        textcolor(11);
    else if (strcmp(cor, "vermelho_claro") == 0)
        textcolor(12);
    else if (strcmp(cor, "rosa_claro") == 0)
        textcolor(13);
    else if (strcmp(cor, "amarelo") == 0)
        textcolor(14);
    else if (strcmp(cor, "branco") == 0)
        textcolor(15);
    else
        textcolor(15);

    // clearElement(posicao);

    // setPos (definir pos antes)
    if (strcmp(posicao, "topo") == 0)
    {
        int mid = (strlen(str) / 2);
        gotoxy(40 - mid, 3);
    }
    else if (strcmp(posicao, "alerta") == 0)
        gotoxy(3, 23);
    else if (strcmp(posicao, "menu_left") == 0)
        gotoxy(4, 9);
    else if (strcmp(posicao, "menu_right") == 0)
        gotoxy(30, 7);
    else if (strcmp(posicao, "switcher") == 0)
        gotoxy(3, 23);

    // setLinha
    gotoxy(wherex(), wherey() + linha);

    // putStringto - old, sem opcao de formatacao
    // if (*str != NULL)
    //    printf("%s ", str);

    for (transc = str; *transc != '\0'; transc++)
    {
        while (*transc != '%')
        {
            putchar(*transc);
            transc++;
        }
        transc++;
        switch (*transc)
        {
        case 'd':
            i = va_arg(arg, int);
            if (i < 0)
            {
                i = -1;
                putchar('-');
            }
            puts(conv(i, 10));
            break;
        case 's':
            s = va_arg(arg, char *);
            puts(s);
            break;
        }
    }
    va_end(arg);
    textcolor(15);
}

int main()
{
    conioPrintf("Teste de %d numeros\n %s", "topo", "vermelho", 2, 23, "oo");
}

