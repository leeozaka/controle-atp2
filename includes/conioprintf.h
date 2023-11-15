#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <conio2.h>

#define CONIOPRINTF

enum cores
{
    PRETO,
    AZUL,
    VERDE,
    CIANO,
    VERMELHO,
    ROSA,
    MARROM,
    BRANCO,
    CINZA_CLARO,
    AZUL_CLARO,
    VERDE_CLARO,
    CIANO_CLARO,
    VERMELHO_CLARO,
    ROSA_CLARO,
    AMARELO,
};

enum position
{
    TOPO,
    ALERTA,
    MENU_LEFT,
    MENU_RIGHT,
    SWITCHER,
    RIGHTSIDE
};

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

void clearElement(position pos)
{
    switch (pos)
    {
    case TOPO:
        clrfunc(3, 3, 3, 70);
        break;
    case ALERTA:
        clrfunc(23, 3, 23, 78);
        break;
    case SWITCHER:
        clrfunc(23, 3, 23, 78);
        clrfunc(8, 3, 20, 26);
        clrfunc(6, 29, 20, 78);
        break;
    case RIGHTSIDE:
        clrfunc(6, 29, 20, 78);
        break;
    }
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
// posicao=topo, alerta, menu_left, menu_right, switcher
void conioPrintf(position pos, cores ref, int linha, const char *str, ...)
{
    unsigned int i;
    char *transc;
    char *s;

    va_list arg;
    va_start(arg, str);

    // setColor
    switch (ref)
    {
    case PRETO:
        textcolor(0);
        break;
    case AZUL:
        textcolor(1);
        break;
    case VERDE:
        textcolor(2);
        break;
    case CIANO:
        textcolor(3);
        break;
    case VERMELHO:
        textcolor(4);
        break;
    case ROSA:
        textcolor(5);
        break;
    case MARROM:
        textcolor(6);
        break;
    case BRANCO:
        textcolor(7);
        break;
    case CINZA_CLARO:
        textcolor(8);
        break;
    case AZUL_CLARO:
        textcolor(9);
        break;
    case VERDE_CLARO:
        textcolor(10);
        break;
    case CIANO_CLARO:
        textcolor(11);
        break;
    case VERMELHO_CLARO:
        textcolor(12);
        break;
    case ROSA_CLARO:
        textcolor(13);
        break;
    case AMARELO:
        textcolor(14);
        break;
    default:
        textcolor(7);
    }

    clearElement(pos);

    // setPos (definir pos antes)
    switch (pos)
    {
    case TOPO:
        gotoxy(40 - (strlen(str) / 2), 3);
        break;
    case ALERTA:
        gotoxy(3, 23);
        break;
    case MENU_LEFT:
        gotoxy(4, 9);
        break;
    case MENU_RIGHT:
        gotoxy(30, 7);
        break;
    case SWITCHER:
        gotoxy(3, 23);
    }

    // setLinha
    gotoxy(wherex(), wherey() + linha);

    // print function (ref: stdio, mingw, gnu)
    for (transc = (char*)str; *transc != '\0';)
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
        case 'f':
            i = va_arg(arg, double);
            if (i < 0)
            {
                i = -1;
                putchar('-');
            }
            fputs(conv(i, 10), stdout);
            transc++;
            break;
        }
    }
    textcolor(15);
    va_end(arg);
}