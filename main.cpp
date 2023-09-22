#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

#define TF 10

typedef struct Fornecedores
{
    int CodForn;
    char NomeForn[20];
    char Cidade[20];
};

typedef struct Produtos {
    int CodProd;
    char Desc[20];
    int Estoque;
    float Preco;
    struct DtValidade {
        int Dia;
        int Mes;
        int Ano;
    };
    Fornecedores CodForn;
};

typedef struct Clientes {
    int CPF[11];
    char NomeCli[20];
    int QtdeCompras[20];
    double ValorTotComprado;
};

typedef struct Vendas {
    int CodVenda;
    Clientes CPF[11];
    struct DtValidade {
        int Dia;
        int Mes;
        int Ano;
    };
    float TotVenda;
};

typedef struct Vendas_Produtos {
    Vendas CodVenda;
    Produtos CodProd;
    int Qtde;
    float ValorUnitario;
};


int main(){

    return 0;
}