#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define FCONTROL

enum POS_SET
{
    SET,
    CUR,
    END
};

enum SIZE_LOGIC
{
    LOGIC,
    BYTE
};

#define TF 300
#define QUANT 40
#define BUFFER 50

struct Fornecedores
{
    int CodForn;
    char NomeForn[QUANT];
    char Cidade[QUANT];
    bool flag;
};

struct TpData
{
    int Dia, Mes, Ano;
};

struct Produtos
{
    int CodProd, Estoque;
    char Desc[QUANT];
    float Preco;
    TpData DtValidade;
    int CodForn;
    bool flag;
};

struct Clientes
{
    char CPF[12];
    char NomeCli[QUANT];
    int QtdeCompras;
    double ValorTotComprado;
    bool flag;
};

struct Vendas
{
    int CodVenda;
    char CPF[12];
    TpData DtVenda;
    float TotVenda;
};

struct Vendas_Produtos
{
    int CodVenda;
    int CodProd;
    int Qtde;
    float ValorUnitario;
    bool flag;
};

// pega o arquivo, le, posiciona o cursor e retorna o tamanho
//@param ref arquivo aberto para leitura
//@param size Tamanho da estrutura
//@param dir: SET CUR e END Posicao final do ponteiro no arquivo
//@param tipo_retorno LOGIC ou BYTE: tamanho logico ou tamanho em bytes retornados
static inline int fsizer(FILE *ref, int size, POS_SET dir, SIZE_LOGIC tipo_retorno)
{
    fseek(ref, 0, SEEK_END);
    int contagem = ftell(ref);
    switch (dir)
    {
    case SET:
        fseek(ref, 0, SEEK_SET);
        break;
    case CUR:
        fseek(ref, 0, SEEK_CUR);
        break;
    case END:
        fseek(ref, 0, SEEK_END);
        break;
    }
    switch (tipo_retorno)
    {
    case LOGIC:
        contagem /= size;
        break;
    }
    return contagem;
}

static bool find_clientes(FILE *busca, Clientes &cliente, char *elemento, int &pos)
{
    int TL = fsizer(busca, sizeof(Clientes), SET, LOGIC);
    for (int i = 0; i < TL; i++)
    {
        fread(&cliente, sizeof(Clientes), 1, busca);
        if (!strcmp(elemento, cliente.CPF) && cliente.flag)
        {
            pos = i;
            return true;
        }
    }
    return false;
}

static bool find_fornecedores(FILE *reg_fornecedores, Fornecedores &fornecedor, int elemento, int &pos)
{
    int run = 0;
    int TL = fsizer(reg_fornecedores, sizeof(Fornecedores), SET, LOGIC);

    while (run < TL)
    {
        fread(&fornecedor, sizeof(Fornecedores), 1, reg_fornecedores);
        if (elemento == fornecedor.CodForn)
        {
            pos = run;
            return true;
        }
        run++;
    }
    return false;
}
