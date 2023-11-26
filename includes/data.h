#include <stdbool.h>
#include <stdio.h>

#define DATA

enum POS_SET {
    SET,
    CUR,
    END
};

enum SIZE_LOGIC {
    LOGIC,
    BYTE
};

#define TF 300
#define QUANT 40
#define BUFFER 50

struct Fornecedores {
    int CodForn;
    char NomeForn[QUANT];
    char Cidade[QUANT];
    bool flag;
};

struct TpData {
    int Dia, Mes, Ano;
};

struct Produtos {
    int CodProd, Estoque;
    char Desc[QUANT];
    float Preco;
    TpData DtValidade;
    int CodForn;
    bool flag;
};

struct Clientes {
    char CPF[12];
    char NomeCli[QUANT];
    int QtdeCompras;
    double ValorTotComprado;
    bool flag;
};

struct Vendas {
    int CodVenda;
    char CPF[12];
    TpData DtVenda;
    float TotVenda;
    bool flag;
};

struct Vendas_Produtos {
    int CodVenda;
    int CodProd;
    int Qtde;
    float ValorUnitario;
    bool flag;
};