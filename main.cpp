#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
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
    int CodForn;
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

int validarCPF(char cpf[11]) // corrigido 25-09
{
    int i, digito1 = 0, digito2 = 0, helper;
    for (i = 0; i < 11 && cpf[i] >= 48 && cpf[i] <= 57; i++)
        ;
    if (i < 11)
        return 0;
    ; // verifica se todos caracteres estao entre 0 e 9
    if ((strcmp(cpf, "00000000000") == 0) || (strcmp(cpf, "11111111111") == 0) || (strcmp(cpf, "22222222222") == 0) ||
        (strcmp(cpf, "33333333333") == 0) || (strcmp(cpf, "44444444444") == 0) || (strcmp(cpf, "55555555555") == 0) ||
        (strcmp(cpf, "66666666666") == 0) || (strcmp(cpf, "77777777777") == 0) || (strcmp(cpf, "88888888888") == 0) ||
        (strcmp(cpf, "99999999999") == 0))
        return 0;
    else
    {
        // digito 1
        for (i = 0; i < strlen(cpf) - 2; i++)
            digito1 += (cpf[i] - '0') * (i + 1);

        digito1 %= 11;
        if (digito1 == 10)
            digito1 = 0;
        if ((cpf[8] - '0') == digito1)
            return 0;

        else
        // digito 2
        {
            for (i = 0; i < strlen(cpf) - 2; i++)
                digito2 += (cpf[i] - '0') * i;
            digito2 %= 11;
            if (digito2 == 10)
                digito2 = 0;
            if ((cpf[10] - '0') != digito2)
                return 0;
        }
    }
    return 1;
}

void CadastraCliente(Clientes clientes[TF], int &TL)
{
    char CPF[11];
    int i, validar = 0;
    printf("Digite seu CPF: ");
    gets(CPF);
    if (validarCPF(CPF) == 1)
    {
        for (i = 0; i < TL && validar != 1; i++)
        {
            if (strcmp(clientes[i].CPF, CPF) == 0)
                validar = 1;
        }
        if (validar == 1)
        {
            printf("\nCPF ja cadastrado\n!");
            getch();
        }
        else
        {
            strcpy(clientes[TL].CPF, CPF);
            fflush(stdin);
            printf("Digite o nome: ");
            gets(clientes[TL].NomeCli);
            // clientes[TL].QtdeCompras=0;
            clientes[TL].ValorTotComprado = 0;
            TL++;
            printf("Cliente cadastrado com sucesso!");
            getch();
        }
    }
    else
    {
        printf("\nO número de identificação informado não está correto.\n");
        getch();
    }
}

void ConsultaClientes(Clientes clientes[], int TL) //n sei se é pra mostrar todos nessa consulta
{
    printf("Clientes cadastrados:\n");
    for (int i = 0; i < TL; i++)
    {
        printf("cadastro %i\n", i + 1);
        puts(clientes[i].CPF);
        puts(clientes[i].NomeCli);
        printf("%d compras %.2f Total\n", clientes[i].QtdeCompras, clientes[i].ValorTotComprado);
        puts("---------------------------------\n");
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

void CadastraFornecedor(Fornecedores fornecedores[TF], int &TL, int *cod)
// int *cod passa NULL se o código do fornecedor já nao foi inserido em outra funçao, exmeplo no cast do Menu();
{
    int codforn, busca;

    if (cod == NULL)
    {
        printf("Digite o cod. do %do Fornecedor: ", TL + 1);
        scanf("%d", &codforn);
    }
    else
    {
        codforn = *cod;
    }

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
    printf("\nNome: ");
    fflush(stdin);
    gets(fornecedores[TL].NomeForn);
    printf("Cidade:");
    fflush(stdin);
    gets(fornecedores[TL].Cidade);
    TL++;
    printf("Fornecedor cod.%d, %s cadastrado!\n", fornecedores[TL - 1].CodForn, fornecedores[TL - 1].NomeForn);
}

void CadastraProd(Produtos produtos[TF], Fornecedores fornecedores[TF], int &TL_Produtos, int &TL_Fornecedores)
{
    int *ptr_codigo;
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
            ptr_codigo = &helper;
            codigo = ConsultaFornecedor(fornecedores, TL_Fornecedores, helper);

            if (codigo == -1)
            {
                printf("Fornecedor n encontrado. Cadastrar fornecedor? S/N");
                arg = toupper(getch());

                if (arg == 'S')
                {
                    CadastraFornecedor(fornecedores, TL_Fornecedores, ptr_codigo);
                    produtos[TL_Produtos].CodForn = codigo;
                    TL_Produtos++;
                    printf("\nProduto cadastrado\n");
                    getch();
                }
                else
                {
                    printf("\nProduto n/ cadastrado: Fornecedor nao encontrado\n");
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
void DeletaClientes(Clientes clientes[], int &TL)
{
    int pos;
    char cpf[11];
    printf("Cpf do Cliente a ser deletado: ");
    fflush(stdin);
    gets(cpf);
    if (validarCPF(cpf) == 1)
    {
        for (int i = 0, pos = -1; i < TL && pos == -1; i++)
            if ((strcmp(clientes[i].CPF, cpf)) == 0)
                pos = i;
        if (pos >= 0)
        {
            for (int i = pos; i < TL - 1; i++)
            {
                clientes[i] = clientes[i + 1];
            }
            TL--;
            printf("Cliente removido com sucesso!");
        }
        else
        {
            printf("Erro ao remover cliente.");
        }
    }
}
void ExcluirProd(Produtos produtos[], int &TL)
{
    int i, Aux, ponto;
    printf("\n**Exclusao de produto**\n");
    printf("Digite o codigo do produto a excluir: ");
    scanf("%d", &Aux);
    while (Aux > 0)
    {
        ponto = BusProdCod(produtos, TL, Aux);
        if (ponto != -1)
        {
            for (i = ponto; i < TL - 1; i++)
                produtos[i] = produtos[i + 1];
            TL--;
        }
        else
        {
            printf("\nProduto nao encontrado!\n");
        }
        printf("Digite o codigo do produto a excluir: ");
        scanf("%d", &Aux);
    }
}

void AlterarProdCadastrado(Produtos produtos[], int TL)
{
    TpData aux;
    char resp;
    int Aux, ponto;
    float Aux_Preco;
    printf("\n**Alterar produto ja cadastrado**\n");
    getch();
    printf("Insira o codigo do produto: \n");
    fflush(stdin);
    scanf("%d", &Aux);
    while (Aux > 0)
    {
        ponto = BusProdCod(produtos, TL, Aux);
        if (ponto == -1)
        {
            printf("Erro ao procurar pelo Codigo!\n");
        }
        else
        {
            printf("Qual elemento alterar do %s? ", produtos[ponto].Desc);
            printf("A - Estoque");
            printf("B - Preco");
            printf("C - Validade");
            fflush(stdin);
            resp = toupper(getch());
            switch (resp)
            {
            case 'A':
                printf("Qual a nova quantidade do %s? ", produtos[ponto].Desc);
                while (scanf(" %d", &Aux) <= 0)
                    printf("Valor inválido\n");
                produtos[ponto].Estoque = Aux;
                break;
            case 'B':
                printf("Qual o novo preço do %s? R$ ", produtos[ponto].Desc);
                while (scanf(" %f", &Aux_Preco) < 0)
                    printf("Preco invalido\n");
                produtos[ponto].Preco = Aux_Preco;
                break;
            case 'C':
                printf("Qual a nova validade do %s? ", produtos[ponto].Desc);
                scanf("%d %d %d", &aux.Dia, &aux.Mes, &aux.Ano);
                if (aux.Dia > 0 && aux.Mes > 0 && aux.Ano > 0)
                {
                    produtos[ponto].DtValidade = aux;
                }
                else
                    printf("Data invalida!");
                break;
            }
        }

        printf("Insira o codigo do produto: \n");
        fflush(stdin);
        scanf("%d", &Aux);
    }
}

void InsereElementos();

void Moldura(int CI, int LI, int CF, int LF, int TC, int BG)
{
}

void ClrScrenzona(int LI, int CI, int LF, int CF)
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

void Menu(Fornecedores index_fornecedores[TF], Produtos index_produtos[TF], Clientes index_clientes[TF], Vendas index_vendas[TF], Vendas_Produtos index_vendasprod[TF])
{
    int TL_fornecedores = 0, TL_produtos = 0, TL_clientes = 0, TL_vendas = 0, TL_cupons = 0, op;
    char opc;
    do
    {
        ClrScrenzona(23, 3, 23, 78);
        gotoxy(3, 23);
        printf("Selecione um item:\n");
        ClrScrenzona(8, 3, 20, 24);
        gotoxy(4, 9);
        printf("[A] - CLIENTES\n");
        gotoxy(4, 10);
        printf("[B] - FORNECEDORES\n");
        gotoxy(4, 11);
        printf("[C] - PRODUTOS\n");
        //vai ser necessário a opçao D - Vendas com nova venda, excluir venda, e aqui sim, relatorio geral de vendas
        opc = toupper(getche());

        switch (opc)
        {
        case 'A':
            do
            {
                ClrScrenzona(23, 3, 23, 78);
                gotoxy(3, 23);
                printf("*Selecione uma operacao em clientes*\n");
                // a mostrar:
                ClrScrenzona(8, 3, 20, 24);
                gotoxy(4, 9);
                printf("[A] - CADASTRO\n");
                gotoxy(4, 10);
                printf("[B] - CONSULTA\n");
                gotoxy(4, 11);
                printf("[C] - EXCLUSAO\n");
                gotoxy(4, 12);
                printf("[D] - ALTERACAO\n");
                gotoxy(4, 13);
                printf("[E] - RELATORIO\n");
                gotoxy(4, 14);
                opc = toupper(getche());

                switch (opc)
                {
                case 'A':
                    CadastraCliente(index_clientes, TL_clientes);
                    break;
                case 'B':
                    ConsultaClientes(index_clientes, TL_clientes);
                    break;
                case 'C':
                    DeletaClientes(index_clientes, TL_clientes);
                    break;
                case 'D':
                    // EditaClientes(index_clientes, TL_clientes);
                    break;
                case 'E':
                    break;
                default:
                    if (opc != 27)
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("\n##INEXISTENTE!##\nSelecione novamente\n");
                        gotoxy(51, 23);
                        getche();
                    }
                    break;
                }
            } while (opc != 27);
            break;

        case 'B':
            opc = 0;
            do
            {
                ClrScrenzona(23, 3, 23, 78);
                gotoxy(3, 23);
                printf("*Selecione uma operacao em fornecedores*\n");
                // a mostrar:
                ClrScrenzona(8, 3, 20, 24);
                gotoxy(4, 9);
                printf("[A] - CADASTRO\n");
                gotoxy(4, 10);
                printf("[B] - CONSULTA\n");
                gotoxy(4, 11);
                printf("[C] - EXCLUSAO\n");
                gotoxy(4, 12);
                printf("[D] - ALTERACAO\n");
                gotoxy(4, 13);
                printf("[E] - RELATORIO\n");
                gotoxy(4, 14);
                opc = toupper(getche());

                switch (opc)
                {
                case 'A':
                    CadastraFornecedor(index_fornecedores, TL_fornecedores, NULL);
                    break;
                case 'B':
                    // ConsultaFornecedor(index_fornecedores, TL_fornecedores);
                    break;
                case 'C':
                    // ExcluirFornecedor(index_fornecedores, TL fornecedores);
                    break;
                case 'D':
                    // AlterarDadosFornecedor(index_fornecedores, TL_fornecedores);
                    break;
                case 'E':
                    // Relatorio(index_fornecedores, TL fornecedores);
                    break;
                default:
                    if (opc != 27)
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("\n##INEXISTENTE!##\nSelecione novamente\n");
                        gotoxy(51, 23);
                        getche();
                    }
                    break;
                }
            } while (opc != 27);
            break;

        case 'C':
            opc = 0;
            do
            {
                ClrScrenzona(23, 3, 23, 78);
                gotoxy(3, 23);
                printf("*Selecione uma operacao em produtos*\n");
                // a mostrar:
                ClrScrenzona(8, 3, 20, 24);
                gotoxy(4, 9);
                printf("[A] - CADASTRO\n");
                gotoxy(4, 10);
                printf("[B] - CONSULTA\n");
                gotoxy(4, 11);
                printf("[C] - EXCLUSAO\n");
                gotoxy(4, 12);
                printf("[D] - ALTERACAO\n");
                gotoxy(4, 13);
                printf("[E] - RELATORIO\n");
                gotoxy(4, 14);
                opc = toupper(getche());

                switch (opc)
                {
                case 'A':
                    CadastraProd(index_produtos, index_fornecedores, TL_produtos, TL_fornecedores);
                    break;
                case 'B':
                    break;
                case 'C':
                    ExcluirProd(index_produtos, TL_produtos);
                    break;
                case 'D':
                    AlterarProdCadastrado(index_produtos, TL_produtos);
                    break;
                case 'E':
                    break;
                default:
                    if (opc != 27)
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("\n##INEXISTENTE!##\nSelecione novamente\n");
                        gotoxy(51, 23);
                        getche();
                    }
                    break;
                }
            } while (opc != 27);
            opc = 0;
            break;
        default:
            if (opc != 27)
            {
                ClrScrenzona(23, 3, 23, 78);
                gotoxy(3, 23);
                printf("\n##INEXISTENTE!##\nSelecione novamente\n");
                gotoxy(51, 23);
                getche();
            }
            break;
        }
        if (opc == 27) // Sair do loop
        {
            do
            {
                ClrScrenzona(23, 3, 23, 78);
                gotoxy(3, 23);
                printf("CONFIRMACAO: 'Gostaria de sair?'<S/N>");
                gotoxy(41, 23);

                opc = toupper(getche());

                switch (opc)
                {
                case 'S':
                    opc == 27; // para sair do sistema
                    break;
                case 'N':
                    opc == 0; // nao sair
                    break;
                default:
                    if (opc != 83 || opc != 78)
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("\n##INEXISTENTE!##\nSelecione novamente\n");
                        gotoxy(52, 23);
                        getche();
                    }
                }
            } while (opc != 27);
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
