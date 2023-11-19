#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "data.h"

#define FCONTROL

static inline bool Compara(int vet, int size)
{
    return vet < size ? true : false;
}

// pega o arquivo, le, posiciona o cursor e retorna o tamanho
//@param ref arquivo aberto para leitura
//@param size Tamanho da estrutura
//@param dir: SET CUR e END Posicao final do ponteiro no arquivo
//@param tipo_retorno LOGIC ou BYTE: tamanho logico ou tamanho em bytes retornados
static int fsizer(FILE *ref, int size, POS_SET dir, SIZE_LOGIC tipo_retorno)
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

//retorna true se encontrar
//@param busca recebe um arquivo aberto
//@param cliente variavel armazenada do cliente atual lido
//@param elemento cpf a ser consultado
//@param pos posicao a retornar na variavel
//@param res BYTE ou LOGIC - retorna a posicao em bytes em pos ou em posicao logica
static bool find_clientes(FILE *busca, Clientes &cliente, char *elemento, int &pos, SIZE_LOGIC res)
{
    int TL = fsizer(busca, sizeof(Clientes), SET, LOGIC);
    for (int i = 0; i < TL; i++)
    {
        fread(&cliente, sizeof(Clientes), 1, busca);
        if (!strcmp(elemento, cliente.CPF) && cliente.flag)
        {
            res == BYTE ? pos = i * sizeof(Clientes) : pos = i;
            return true;
        }
    }
    return false;
}

//retorna true se encontrar
//@param busca recebe um arquivo aberto
//@param fornecedor variavel armazenada do fornecedor atual lido
//@param elemento cod a ser consultado
//@param pos posicao a retornar na variavel
//@param res BYTE ou LOGIC - retorna a posicao em bytes em pos ou em posicao logica
static bool find_fornecedores(FILE *reg_fornecedores, Fornecedores &fornecedor, int elemento, int &pos, SIZE_LOGIC res)
{
    int run = 0;
    int TL = fsizer(reg_fornecedores, sizeof(Fornecedores), SET, LOGIC);

    while (run < TL)
    {
        fread(&fornecedor, sizeof(Fornecedores), 1, reg_fornecedores);
        if (elemento == fornecedor.CodForn && fornecedor.flag)
        {
            res == BYTE ? pos = run * sizeof(Fornecedores) : pos = run;
            return true;
        }
        run++;
    }
    return false;
}

//retorna true se encontrar
//@param busca recebe um arquivo aberto
//@param produto variavel armazenada do produto atual lido
//@param find cod a ser consultado
//@param pos posicao a retornar na variavel
//@param res BYTE ou LOGIC - retorna a posicao em bytes em pos ou em posicao logica
static bool find_produtos(FILE *reg_produtos, Produtos &produto, int find, int &pos, SIZE_LOGIC res)
{
    int run = 0;
    int TL = fsizer(reg_produtos, sizeof(Produtos), SET, LOGIC);
    while (run < TL)
    {
        fread(&produto, sizeof(Produtos), 1, reg_produtos);
        if (find == produto.CodProd && produto.flag)
        {
            res == BYTE ? pos = run * sizeof(Produtos) : pos = run;
            return true;
        }
        run++;
    }
    return false;
}

//retorna true se encontrar
//@param busca recebe um arquivo aberto
//@param venda variavel armazenada da venda atual lido
//@param find cod a ser consultado
//@param pos posicao a retornar na variavel
//@param res BYTE ou LOGIC - retorna a posicao em bytes em pos ou em posicao logica
static bool find_venda(FILE *reg_vendas, Vendas &venda, int find, int &pos, SIZE_LOGIC res)
{
    int run = 0;
    int TL = fsizer(reg_vendas, sizeof(Vendas), SET, LOGIC);
    while (run < TL)
    {
        fread(&venda, sizeof(Vendas), 1, reg_vendas);
        if (find == venda.CodVenda && venda.flag)
        {
            res == BYTE ? pos = run * sizeof(Vendas) : pos = run;
            return true;
        }
        run++;
    }
    return false;
}

//retorna true se encontrar
//@param busca recebe um arquivo aberto
//@param index_venda variavel armazenada da index_venda atual lido
//@param find cod a ser consultado
//@param pos posicao a retornar na variavel
//@param res BYTE ou LOGIC - retorna a posicao em bytes em pos ou em posicao logica
static bool find_index_venda(FILE *reg_index_vendas, Vendas_Produtos &index_venda, int find, int &pos, SIZE_LOGIC res)
{
    int run = 0;
    int TL = fsizer(reg_index_vendas, sizeof(Vendas_Produtos), SET, LOGIC);
    while (run < TL)
    {
        fread(&index_venda, sizeof(Vendas_Produtos), 1, reg_index_vendas);
        if (find == index_venda.CodVenda &&index_venda.flag)
        {
            res == BYTE ? pos = run * sizeof(Vendas_Produtos) : pos = run;
            return true;
        }
        run++;
    }
    return false;
}


//usa getch, em sequencia fecha o arquivo atual e retorna 0
//@param file arquivo a ser fechado
inline int getchclose(FILE *file){
    getch();
    fclose(file);
    return 0;
}