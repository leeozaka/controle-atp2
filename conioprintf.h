#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <conio2.h>

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
}

void clearElement(char regiao[])
{
    if (strcmp(regiao, "topo") == 0)
        clrfunc(3, 3, 3, 70);
    else if (strcmp(regiao, "alerta") == 0)
        clrfunc(23, 3, 23, 78);
    else if (strcmp(regiao, "switcher") == 0)
    {
        clrfunc(23, 3, 23, 78);
        clrfunc(8, 3, 20, 26);
        clrfunc(6,29,20,78);
    }
    else if (strcmp(regiao, "rightside") == 0)
        clrfunc(6,29,20,78);

    // switcher gatilho para limpar com printf
    // o menu esquerdo
}

// arg p conversao em int
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

//>>conioPrintf("posicao", "Cor", somar linha,"string" ...formatacao...);
//
//posicao=topo, alerta, menu_left, menu_right, switcher
void conioPrintf(char *posicao, char *cor, int linha, char *str, ...)
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

    clearElement(posicao);

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

    // putStringto - old, --sem opcao de formatacao--
    // if (*str != NULL)
    //    printf("%s ", str);

    //print function (ref: stdio, mingw, gnu)
    for (transc = str; *transc != '\0';)
    {
        while (*transc != '%' && *transc != '\0')
        {
            putchar(*transc);
            transc++;
        }
        if (*transc == '%')
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
            fputs(conv(i, 10), stdout);
            transc++;
            break;
        case 's':
            s = va_arg(arg, char *);
            fputs(s, stdout);
            transc++;
            break;
        }
        //eu nao consegui fazer um case FLOAT ainda (13-10);
    }
    textcolor(15);
    va_end(arg);
}