#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <conio2.h>

#define TF 10
#define QUANT 20

struct Fornecedores
{
    int CodForn;
    char NomeForn[QUANT];
    char Cidade[QUANT];
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
};
Fornecedores CodForn; // essa parada eh a chave primaria? -> sim

struct Clientes
{
    char CPF[11];
    char NomeCli[QUANT];
    int QtdeCompras[QUANT];
    double ValorTotComprado;
};

struct Vendas
{
    int CodVenda;
    Clientes CPF[11];
    struct DtValidade
    {
        int Dia;
        int Mes;
        int Ano;
    };
    float TotVenda;
};

struct Vendas_Produtos
{
    Vendas CodVenda;
    Produtos CodProd;
    int Qtde;
    float ValorUnitario;
};

/*int ValidarCPF(Clientes CPF)
int ValidarCPF(int CPF[11]){{
    int i, j;
    // if (strlen(CPF) == 11){


        O vetor já tem 11 posicoes (alocaçao estática de memória)

        //colocar para verificar se todos os caracteres sao numeros?
        //codigo..

    } else
        printf("\nCPF inv�lido!\n\n");

} */

int validarCPF(char cpf[11])
{
    int i, j, digito1 = 0, digito2 = 0;
    if ((strcmp(cpf, "00000000000") == 0) || (strcmp(cpf, "11111111111") == 0) || (strcmp(cpf, "22222222222") == 0) ||
        (strcmp(cpf, "33333333333") == 0) || (strcmp(cpf, "44444444444") == 0) || (strcmp(cpf, "55555555555") == 0) ||
        (strcmp(cpf, "66666666666") == 0) || (strcmp(cpf, "77777777777") == 0) || (strcmp(cpf, "88888888888") == 0) ||
        (strcmp(cpf, "99999999999") == 0))
        return 0; /// se o CPF tiver todos os números iguais ele é inválido.
    else
    {
        /// digito 1---------------------------------------------------
        for (i = 0, j = 10; i < strlen(cpf) - 2; i++, j--) /// multiplica os números de 10 a 2 e soma os resultados dentro de digito1
            digito1 += (cpf[i] - 48) * j;
        digito1 %= 11;
        if (digito1 < 2)
            digito1 = 0;
        else
            digito1 = 11 - digito1;
        if ((cpf[9] - 48) != digito1)
            return 0;
        else
        /// digito 2--------------------------------------------------
        {
            for (i = 0, j = 11; i < strlen(cpf) - 1; i++, j--)
                digito2 += (cpf[i] - 48) * j;
            digito2 %= 11;
            if (digito2 < 2)
                digito2 = 0;
            else
                digito2 = 11 - digito2;
            if ((cpf[10] - 48) != digito2)
                return 0;
        }
    }
    return 1;
}

int BusProdCod(Produtos Tab[TF], int TL, int CodProd)
{

    printf("**Busca o produto pelo codigo**");
    int i = 0;
    while (i < TL && CodProd != Tab[i].CodProd)
        i++;
    if (i < TL)
        return i;
    else
        return -1;
}

void CadastProd(Produtos Tab[TF], int &TL)
{
    int AuxCod, pos;
    clrscr();
    printf("\n**Cadastro de Produtos**\n");
    printf("Codigo: ");
    scanf("%d", &AuxCod);
    while (TL < TF && AuxCod > 0)
    {
        pos = BusProdCod(Tab, TL, AuxCod);
        if (pos == -1)
        {
            Tab[TL].CodProd = AuxCod;

            printf("Descricao: ");
            fflush(stdin);
            gets(Tab[TL].Desc);

            printf("Estoque: ");
            scanf("%d", &Tab[TL].Estoque);

            printf("Preco: R$ ");
            scanf("%f", &Tab[TL].Preco);

            printf("Data de Validade [dd mm aaaa]: ");
            scanf("%d %d %d", &Tab[TL].DtValidade.Dia, &Tab[TL].DtValidade.Mes, &Tab[TL].DtValidade.Ano);
            TL++;
        }
        else
        {
            printf("\nCodigo do Produto ja Cadastrado!\n");
            getch();
        }

        printf("\nCodigo: ");
        scanf("%d", &AuxCod);
    }
}

void Relatorio(Produtos Tab[TF], int TL)
{
    int i;
    clrscr();
    printf("\nRelatorio de Produtos\n");
    if (TL == 0)
        printf("\nNao ha Produtos Cadastrados!\n");
    else
        for (i = 0; i < TL; i++)
        {
            printf("\nCodigo de Venda: %d\n", Tab[i].CodProd);
            printf("\nProdutos: \n%d\n", Tab[i].CodVenda);
            printf("%s\n", Tab[i].Descr);
            printf("%d\n", Tab[i].Estoque);
            printf("R$ %.2f\n", Tab[i].Preco);
            printf("%s\n", Tab[i].NomeForn);
            printf("%f\n", Tab[i].Preco);
        }
    getch();
}

void Menu()
{
    char opc;
    printf("[A] - Inserir Elementos\n");
    printf("[B] - Cadastrar um novo produto\n");
    printf("[C] - Excluir produto\n");
    printf("[D] - Relatorio\n");
    printf("[E] - Ordenar Relatorio\n")
        printf("Digite a opcao a seguir: ");
    fflush(stdin);
    scanf("%c", &opc);
    switch (opc)
    {
    case 'A':
        printf("[A] - Inserir Elementos\n");
        /* code */
        break;
    case 'B':
        printf("[B] - Cadastrar um novo produto\n");
        /* code */
        break;
    case 'C':
        printf("[C] - Excluir produto\n");
        /* code */
        break;
    case 'D':
        printf("[D] - Relatorio\n");
        /* code */
        break;
    }
}

int main(void)
{
    Fornecedores index_fornecedores[TF];
    Produtos index_produtos[TF];
    Clientes index_clientes[TF];
    Vendas index_vendas[TF];
    Vendas_Produtos vendas[TF];

    return 0;
}
