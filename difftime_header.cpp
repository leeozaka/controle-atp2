#include <stdio.h>
#include <conio2.h>
#include <stdlib.h>
#include <time.h>

int comparaData(int ano1, int mes1, int dia1, int ano2, int mes2, int dia2)
{
    // nao testado
    // output esperado: como strcmp(str1,str2);
    struct tm data1 = {0};
    data1.tm_year = ano1 - 1900;
    data1.tm_mon = mes1 - 1;
    data1.tm_mday = dia1;

    struct tm data2 = {0};
    data2.tm_year = ano2 - 1900;
    data2.tm_mon = mes2 - 1;
    data2.tm_mday = dia2;
    return difftime((mktime(&data2)), mktime(&data1));
}

int main() {
	printf("%d", comparaData(2023,01,01,2022,12,31));
	printf("%d", comparaData(2022,12,31,2023,01,01));
}
