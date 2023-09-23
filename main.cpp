#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include <conio.h>
// #include <conio2.h>

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
    int CodForn; // essa parada eh a chave primaria? -> sim
};

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
    char CPF[11];
    TpData DtValidade;
    float TotVenda;
};

struct Vendas_Produtos
{
    int CodVenda;
    int CodProd;
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

void CadastraCliente(Clientes clientes[TF], int &TL)
{
    char CPF[11];
    int i;
    printf("Digite seu CPF: ");
    scanf("%s", &CPF);
    for (i = 0; i < 11 && CPF[i] >= 0 && CPF[i] <= 9; i++)
        ; // verifica se todos caracteres estao entre 0 e 9
    if (i == 11)
    {
        if (validarCPF(CPF) == 1)
        {
            strcpy(clientes[TL].CPF, CPF);
            fflush(stdin);
            printf("Digite o nome: ");
            gets(clientes[TL].NomeCli);
            TL++;
            printf("Cliente cadastrado com sucesso!");
            getch();
        }
        else
        {
            printf("\nO número de identificação informado não está correto.\n");
            getch();
        }
    }
    else
    {
        printf("\nO número de identificação deve conter apenas números!\n");
        getch();
    }
}

int BusProdCod(Produtos Tab[TF], int TL, int CodProd)
{
    int i = 0;
    while (i < TL && CodProd != Tab[i].CodProd)
        i++;
    if (i < TL)
        return i;
    else
        return -1;
}
int ConsultaFornecedor(Fornecedores lista_fornecedores[TF], int TL, int find)
{
    // retorna código do Fornecedor! ñ pos;
    int i;
    for (i = 0; i < TL && find != lista_fornecedores[i].CodForn; i++)
        ;
    return i == TL ? -1 : lista_fornecedores[i].CodForn;
}

void CadastraFornecedor(Fornecedores fornecedores[TF], int &TL)
{
    int codforn, busca;

    printf("Digite o cod. do %do Fornecedor: ", TL + 1);
    scanf("%d", &codforn);
    codforn = abs(codforn);
    do // vai digitar o codigo certo sim
    {
        busca = ConsultaFornecedor(fornecedores, TL, codforn);
        if (busca != -1)
        {
            printf("Cod ja existente\n");
            printf("Digite o cod. do %do Fornecedor: ", TL + 1);
            scanf("%d", &codforn);
            codforn = abs(codforn);
        }
    } while (busca != -1);
    fornecedores[TL].CodForn = codforn;
    printf("Nome: ");
    fflush(stdin);
    gets(fornecedores[TL].NomeForn);
    printf("Cidade:");
    fflush(stdin);
    gets(fornecedores[TL].Cidade);
    TL++;
    printf("Fornecedor cadastrado!\n");
}

void CadastraProd(Produtos produtos[TF], Fornecedores fornecedores[TF], int &TL_Produtos, int &TL_Fornecedores)
{
    int AuxCod, pos, helper, codigo;
    char arg;
    // clrscr();
    printf("\n**Cadastro de Produtos**\n");
    printf("Codigo: ");
    fflush(stdin);
    scanf("%d", &AuxCod);
    while (TL_Produtos < TF && AuxCod > 0)
    {
        pos = BusProdCod(produtos, TL_Produtos, AuxCod);
        if (pos == -1)
        {
            produtos[TL_Produtos].CodProd = AuxCod;

            printf("Descricao: ");
            fflush(stdin);
            gets(produtos[TL_Produtos].Desc);

            printf("Estoque: ");
            fflush(stdin);
            scanf("%d", &produtos[TL_Produtos].Estoque);

            printf("Preco: R$ ");
            fflush(stdin);
            scanf("%f", &produtos[TL_Produtos].Preco);

            printf("Data de Validade [dd mm aaaa]: ");
            fflush(stdin);
            scanf("%d %d %d", &produtos[TL_Produtos].DtValidade.Dia, &produtos[TL_Produtos].DtValidade.Mes, &produtos[TL_Produtos].DtValidade.Ano);

            printf("Busca fornecedor, codigo: ");
            fflush(stdin);
            scanf("%d", &helper);
            codigo = ConsultaFornecedor(fornecedores, TL_Fornecedores, helper);

            if (codigo == -1)
            {
                printf("Fornecedor n encontrado. Cadastrar fornecedor? S/N");
                arg = (getchar());

                if (arg == 's' && arg == 'S')
                {
                    CadastraFornecedor(fornecedores, TL_Fornecedores);
                    produtos[TL_Produtos].CodForn = codigo;
                    TL_Produtos++;
                    printf("Produto e fornecedor cadastrado\n");
                    getch();
                }
                else
                {
                    printf("Produto n/ cadastrado: Fornecedor nao encontrado\n");
                    getch();
                }
            }
            else
            {
                printf("Fornecedor: %d\n", codigo);
                produtos[TL_Produtos].CodForn = codigo;
                TL_Produtos++;
                printf("Produto cadastrado!\n");
                getch();
            }
            printf("\n**Cadastro de Produtos**\n");
            printf("Codigo: ");
            scanf("%d", &AuxCod);
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

/*void Relatorio(Produtos Produtos[TF], Vendas Vendas[TF], int TL)
{
    int i;
    clrscr();
    printf("\nRelatorio de Produtos\n");
    if (TL == 0)
        printf("\nNao ha Produtos Cadastrados!\n");
    else
        for (i = 0; i < TL; i++)
        {
            printf("\nCodigo de Venda: %d\n", Vendas[i].CodVenda);
            printf("Produtos: \n%d\n", );
            printf("%s\n", Produtos[i].Descr);
            printf("%d\n", Produtos[i].Estoque);
            printf("R$ %.2f\n", Produtos[i].Preco);
            printf("%s\n", Produtos[i].NomeForn);
            printf("%f\n", Produtos[i].Preco);
        }
    getch();
}
*/

void InsereElementos();

void Menu(Fornecedores index_fornecedores[TF], Produtos index_produtos[TF], Clientes index_clientes[TF], Vendas index_vendas[TF], Vendas_Produtos index_vendasprod[TF])
{
    // Tamanhos relativos
    int TL_fornecedores = 0, TL_produtos = 0, TL_clientes = 0, TL_vendas = 0, TL_cupons = 0;
    char opc;
    do
    {

        printf("[A] - Inserir Elementos\n");
        printf("[B] - Cadastrar um novo produto\n");
        printf("[C] - Excluir produto\n");
        printf("[D] - Cadastrar Cliente\n");
        printf("[E] - Ordenar Relatorio\n");
        printf("Digite a opcao a seguir: ");
        fflush(stdin);
        scanf("%c", &opc);
        switch (opc)
        {
        case 'A':
            /* code */
            break;
        case 'B':
            CadastraProd(index_produtos, index_fornecedores, TL_produtos, TL_fornecedores);
            break;
        case 'C':
            /* code */
            break;
        case 'D':
            CadastraCliente(index_clientes, TL_clientes);
            break;
        case 'F':
            /* code */
            break;
        }
    } while (opc != 27);
}

int main(void)
{
    Fornecedores index_fornecedores[TF];
    Produtos index_produtos[TF];
    Clientes index_clientes[TF];
    Vendas index_vendas[TF];
    Vendas_Produtos vendas[TF];

    Menu(index_fornecedores, index_produtos, index_clientes, index_vendas, vendas);

    return 0;
}
