#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <conio2.h>
#include <time.h>

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
    int QtdeCompras;
    double ValorTotComprado;
};

struct Vendas
{
    int CodVenda;
    char CPF[11];
    TpData DtVenda;
    float TotVenda;
};

struct Vendas_Produtos
{
    int CodVenda;
    int CodProd;
    int Qtde;
    float ValorUnitario;
};
bool Compara(int vet, int size)
{
    return vet < size ? true : false;
}
void Moldura(int CI, int LI, int CF, int LF, int CorT, int CorF)
{
    int i;
    textcolor(CorT);
    textbackground(CorF);
    gotoxy(CI, LI);
    printf("%c", 201);
    gotoxy(CI, LF);
    printf("%c", 200);
    gotoxy(CF, LI);
    printf("%c", 187);
    gotoxy(CF, LF);
    printf("%c", 188);
    for (i = CI + 1; i < CF; i++)
    {
        gotoxy(i, LI);
        printf("%c", 205);
        gotoxy(i, LF);
        printf("%c", 205);
    }
    for (i = LI + 1; i < LF; i++)
    {
        gotoxy(CI, i);
        printf("%c", 186);
        gotoxy(CF, i);
        printf("%c", 186);
    }

    textcolor(7);
    textbackground(0);
}

void Formulario(void)
{
    system("cls");
    Moldura(1, 1, 80, 25, 10, 5);
    gotoxy(27, 3);
    textcolor(15);
    printf(" # # #  T I T U L O  # # #");

    Moldura(2, 2, 79, 4, 14, 1);
    Moldura(2, 5, 27, 21, 12, 7);
    //	Menu();
    Moldura(2, 22, 79, 24, 11, 4);
    gotoxy(4, 23);
    textcolor(10);
    printf("Mensagem: ");

    Moldura(28, 5, 79, 21, 9, 6);
}

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

void CadastraCliente(Clientes clientes[], int &TL)
{
    char CPF[11];
    int i, validar = 0;
    gotoxy(30, 7);
    printf("Digite seu CPF: ");
    fflush(stdin);
    //gets(CPF);
    scanf("%s", &CPF);
    if (validarCPF(CPF) == 1)
    {
        for (i = 0; i < TL && validar != 1; i++)
        {
            if (strcmp(clientes[i].CPF, CPF) == 0)
                validar = 1;
        }
        if (validar == 1)
        {
            gotoxy(30, 8);
            printf("\nCPF ja cadastrado\n!");
            getch();
        }
        else
        {
            gotoxy(30, 8);
            strcpy(clientes[TL].CPF, CPF);
            printf("Digite o nome: ");
            fflush(stdin);
            //gets(clientes[TL].NomeCli);
            scanf("%s", &clientes[TL].NomeCli);
            clientes[TL].QtdeCompras = 0;
            clientes[TL].ValorTotComprado = 0;
            TL++;
            printf("Cliente cadastrado com sucesso!");
            getch();
        }
    }
    else
    {
        //    	ClrScrenzona(8, 3, 20, 24);
        printf("\nO numero de identificacao informado nao esta correto.\n");
        getch();
    }
}

void ConsultaClientes(Clientes clientes[], int TL) // n sei se é pra mostrar todos nessa consulta
{
    printf("Clientes cadastrados:\n");
    for (int i = 0; i < TL; i++)
    {
        printf("cadastro %d\n", i + 1);
        //printf("%s - %s \n", clientes[i].CPF, clientes[i].NomeCli);
        printf("%s \n", clientes[i].CPF);
        printf("%d compras %.2f Total\n", clientes[i].QtdeCompras, clientes[i].ValorTotComprado);
        puts("---------------------------------\n");
    }
    getch();
}
int getPosClientes(Clientes clientes[], int TL, char cpf[])
{
    for (int i = 0; i < TL; i++)
    {
        if (strcmp(clientes[i].CPF, cpf) == 1)
            return i;
    }
    return -1;
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
int BusFornCod(Fornecedores Tab[TF], int TL, int cod)
{
    int i = 0;
    while (i < TL && cod != Tab[i].CodForn)
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

void AlterarDadosFornecedor(Fornecedores fornecedores[], int TL)
{
    int opcao, pos, cod;
    char opc;
    if (TL > 0)
    {
        printf("Cod. fornecedor a ser editado: ");
        fflush(stdin);
        scanf("%d", &cod);
        if (ConsultaFornecedor(fornecedores, TL, cod) >= 0)
        {
            pos = BusFornCod(fornecedores, TL, cod);
            printf("%s %s\n", fornecedores[pos].NomeForn, fornecedores[pos].NomeForn);
            printf("Mudar: \n");
            printf("A-Nome\n");
            printf("B-Cidade\n");
            opc = toupper(getch());
            switch (opc)
            {
            case 'A':
                fflush(stdin);
                gets(fornecedores[pos].NomeForn);
                break;
            case 'B':
                fflush(stdin);
                gets(fornecedores[pos].Cidade);
                // buscar compras no cpf e deletar do index_vendas?;
                break;
            }
        }
        else
        {
            printf("COD n encontrado! \n");
            getch();
        }
    }
    else
    {
        printf("Lista vazia!\n");
        getch();
    }
}

void CadastraFornecedor(Fornecedores fornecedores[TF], int &TL, int *cod)
// int *cod passa NULL se o codigo do fornecedor ja nao foi inserido em outra funcao, exmeplo no cast do Menu();
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
// arrumar a funcao de relatorio, e incrementar uma funcao de nota fiscal apos as vendas
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
            getch();
        }
        else
        {
            printf("Erro ao remover cliente.");
            getch();
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
            printf("A - Estoque\n");
            printf("B - Preco\n");
            printf("C - Validade\n");
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
                printf("Qual o novo preco do %s? R$ ", produtos[ponto].Desc);
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

void ConsultaProd(Produtos produtos[], int TL)
{
    int ponto, i;
    printf("cod. a ser consultado: ");
    scanf("%d", &ponto);
    i = BusProdCod(produtos, TL, ponto);
    if (i >= 0)
    {
        printf("Codigo: %d\n", produtos[i].CodProd);
        puts(produtos[i].Desc);
        printf("Quantidade em estoque:%d\n", produtos[i].Estoque);
        printf("%d %d %d\n", produtos[i].DtValidade.Dia, produtos[i].DtValidade.Mes, produtos[i].DtValidade.Ano);
    }
    else
    {
        printf("Erro! Produto nao encontrado!\n");
    }
}
void produtosPercent(Produtos index_produtos[], Fornecedores index_fornecedores[], int TL_produtos, int TL_fornecedores)
{
    int valor, cod, pos;
    char resp;
    do
    {
        printf("[A] Add - [B] Subtrair\n");
        fflush(stdin);
        resp = toupper(getch());
    } while (resp != 'A' || resp != 'B');
    printf("Valor em porc: ");
    fflush(stdin);
    scanf("%d", &valor);
    printf("Cod do fornecedor para %s: ", resp == 'A' ? "adicionar" : "subtrair");
    fflush(stdin);
    scanf("%d", &cod);
    pos = BusFornCod(index_fornecedores, TL_fornecedores, cod);
    if (pos >= 0)
    {
        switch (resp)
        {
        case 'A':
            for (int i = 0; i < TL_produtos; i++)
                if (index_produtos[i].CodForn == index_fornecedores[pos].CodForn)
                    index_produtos[i].Preco += (index_produtos[i].Preco * (valor / 100));
            gotoxy(30, 7);
            printf("Produtos atualizados\n");
            getch();
            break;
        case 'B':
            for (int i = 0; i < TL_produtos; i++)
                if (index_produtos[i].CodForn == index_fornecedores[pos].CodForn)
                    index_produtos[i].Preco -= (index_produtos[i].Preco * (valor / 100));
            gotoxy(30, 7);
            printf("Produtos atualizados\n");
            getch();
            break;
        }
    }
    else
    {
        printf("\nERRO! Fornecedor nao cadastrado.\n");
    }
}

void EditaClientes(Clientes clientes[], int TL)
{
    int opcao, pos;
    char cpf[11], opc;
    if (TL > 0)
    {
        printf("CPF a ser editado: ");
        fflush(stdin);
        gets(cpf);
        if (validarCPF(cpf) == 1)
        {
            pos = getPosClientes(clientes, TL, cpf);
            printf("%s %s\n", clientes[pos].CPF, clientes[pos].NomeCli);
            printf("Mudar: \n");
            printf("A-Nome\n");
            printf("B-Zerar Compras\n");
            opc = toupper(getch());
            switch (opc)
            {
            case 'A':
                fflush(stdin);
                gets(clientes[pos].NomeCli);
                break;
            case 'B':
                clientes[pos].QtdeCompras = 0;
                clientes[pos].QtdeCompras = 0;
                // buscar compras no cpf e deletar do index_vendas;
                break;
            }
        }
        else
        {
            printf("CPF n encontrado! \n");
            getch();
        }
    }
    else
    {
        printf("Lista vazia!\n");
        getch();
    }
}

void ConsultaFornecedor(Fornecedores fornecedores[], int TL)
{
    int i, xD;
    printf("Busca fornecedor pelo cod. :");
    fflush(stdin);
    scanf("%d", &xD);
    if (xD >= 0)
    {
        i = BusFornCod(fornecedores, TL, xD);
        if (i >= 0)
        {
            printf("%d\n%s\n%s", fornecedores[i].CodForn, fornecedores[i].NomeForn, fornecedores[i].Cidade);
        }
        else
        {
            printf("FORNECEDOR NAO ENCONTRADO!\n");
            getch();
        }
    }
    else
    {
        printf("Apenas cod's positivos\n");
        getch();
    }
}

void ExcluirFornecedor(Fornecedores fornecedores[], int &TL)
{
    int i;
    char a;
    printf("Busca fornecedor pelo cod. : ");
    fflush(stdin);
    scanf("%d", &i);
    i = BusFornCod(fornecedores, TL, i);
    if (i >= 0)
    {
        printf("Deletar %s? S/N", fornecedores[i].NomeForn);
        fflush(stdin);
        a = toupper(getch());
        if (a == 'S')
        {
            for (; i < TL - 1; i++)
            {
                fornecedores[i] = fornecedores[i + 1];
            }
            TL--;
            printf("Deletado com sucesso!\n");
            getch();
        }
    }
    else
    {
        printf("Fornecedor nao encontrado \n");
        getch();
    }
}

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

int novaVenda(Clientes rootClientes[], Fornecedores rootFornecedores[], Produtos rootProdutos[], Vendas rootVendas[], Vendas_Produtos rootVendasProdutos[], int TLclientes, int TLfornecedores, int TLprodutos, int &TLvendas, int &TLvendasprod)
{
    int i, pos_cliente, cod_aux, var, valor_variavel = 0;
    char cpf[11], datahelper;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    TpData construtor_data, input;
    bool pass;
    printf("\nNova venda\n");
    printf("CPF do Cliente: ");
    fflush(stdin);
    gets(cpf);
    while (strcmp(cpf, "0") != 0)
    {
        validarCPF(cpf) == 1 ? pass = true : pass = false;
        if (pass == true)
        {
            pos_cliente = getPosClientes(rootClientes, TLclientes, cpf);
            if (pos_cliente >= 0)
            {
                printf("Cliente: %s\n", rootClientes[pos_cliente].NomeCli);
                printf("[A] - Usar a data atual para a venda\n");
                printf("[B] - Data Personalizada para a venda\n");
                fflush(stdin);
                scanf("%c", &datahelper);
                datahelper = toupper(datahelper);
                pass = false;
                do
                {
                    construtor_data.Dia = tm.tm_mday;
                    construtor_data.Mes = tm.tm_mon + 1;
                    construtor_data.Ano = tm.tm_year + 1900;
                    switch (datahelper)
                    { // bool pass = true quando setar a data
                    case 'A':
                        rootVendas[TLvendas].DtVenda = construtor_data;
                        pass = true;
                        break;
                    case 'B':
                        fflush(stdin);
                        scanf("%d %d %d", &input.Dia, &input.Mes, &input.Ano);
                        if (comparaData(input.Ano, input.Mes, input.Dia, construtor_data.Ano, construtor_data.Mes, construtor_data.Dia) <= 0)
                        {
                            rootVendas[TLvendas].DtVenda = input;
                            pass = true;
                        }
                        break;
                    default:
                        printf("Obrigatorio inserir data valida!\n");
                        getch();
                    }
                } while (pass == false);
                do
                {
                    int prod_pos;
                    printf("Cod do %d prod. a ser adicionado: ");
                    fflush(stdin);
                    scanf("%d", &cod_aux);
                    if (cod_aux > 0)
                    {
                        prod_pos = BusProdCod(rootProdutos, TLprodutos, cod_aux);
                        if (prod_pos >= 0)
                        {
                            pass = false;
                            if (comparaData(rootProdutos[prod_pos].DtValidade.Ano, rootProdutos[prod_pos].DtValidade.Mes, rootProdutos[prod_pos].DtValidade.Dia, construtor_data.Ano, construtor_data.Mes, construtor_data.Dia) <= 0)
                            {
                                printf("Produto: %s\nQtde em estoque:%d\n", rootProdutos[prod_pos].Desc, rootProdutos[prod_pos].Estoque);
                                do
                                {
                                    printf("Quantidade vendida: ");
                                    fflush(stdin);
                                    scanf("%d", &var);
                                } while (var <= 0);
                                if (var <= rootProdutos[prod_pos].Estoque)
                                {
                                    rootProdutos[prod_pos].Estoque -= var;
                                    rootVendasProdutos[TLvendasprod].CodVenda = TLvendas + 1;
                                    rootVendasProdutos[TLvendasprod].CodProd = rootProdutos[prod_pos].CodProd;
                                    rootVendasProdutos[TLvendasprod].Qtde = var;
                                    rootVendasProdutos[TLvendasprod].ValorUnitario = rootProdutos[prod_pos].Preco;
                                    valor_variavel += rootProdutos[prod_pos].Preco * var;
                                    rootClientes[pos_cliente].QtdeCompras++;
                                    TLvendasprod++;
                                    printf("Item %s adicionado a venda de cod. %d\n", rootProdutos[prod_pos].Desc, TLvendas + 1);
                                    getch();
                                }
                                else
                                {

                                    printf("Quantidade maior que estoque total\n");
                                    getch();
                                }
                            }
                            else
                            {
                                printf("Validade do produto nao bate com a venda \n");
                                getch();
                            }
                        }
                        else
                        {
                            // ClrScrenzona(23, 3, 23, 78);
                            gotoxy(3, 23);
                            printf("Produto nao encontrado.\nTente novamente.\n");
                            gotoxy(41, 23);

                            getch();
                        }
                    }
                    printf("Cod do %d prod. a ser adicionado: ");
                    fflush(stdin);
                    scanf("%d", &cod_aux);
                } while (cod_aux > 0);

                rootVendas[TLvendas].CodVenda = TLvendas + 1;
                strcmp(rootVendas[TLvendas].CPF, rootClientes[pos_cliente].CPF);
                rootVendas[TLvendas].DtVenda = construtor_data;
                rootVendas[TLvendas].TotVenda = valor_variavel;
                TLvendas++;
                rootClientes[pos_cliente].ValorTotComprado += valor_variavel;
                valor_variavel = 0;

                //                ClrScrenzona(23, 3, 23, 78); //dando erro n sei pq
                gotoxy(3, 23);
                printf("Venda do cliente %s registrada com sucesso\n", rootClientes[pos_cliente].NomeCli);
                gotoxy(41, 23);

                getch();
            }
            else
            {

                printf("Cliente nao encontrado\n");
                getch();
            }
        }
        else
        {
            printf("Digite um CPF valido!");
            getch();
        }

        printf("(0 cancela) CPF do Cliente:"); // nao esta cancelando
        fflush(stdin);
        gets(cpf);
    }
}

void InsereElementos();

// void Moldura(int CI, int LI, int CF, int LF, int TC, int BG)
//{
// }

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
        gotoxy(4, 12);
        printf("[D] - VENDAS");
        gotoxy(4, 13);
        opc = toupper(getche());

        switch (opc)
        {
        case 'A':
            do
            {
                ClrScrenzona(23, 3, 23, 78);
                gotoxy(3, 23); // parte de baixo
                printf("*Selecione uma operacao em clientes*\n");
                // a mostrar:
                ClrScrenzona(8, 3, 20, 24); // parte de baixo
                gotoxy(4, 9);
                printf("[A] - CADASTRO\n");
                gotoxy(4, 10);
                printf("[B] - CONSULTA\n");
                gotoxy(4, 11);
                printf("[C] - EXCLUSAO\n");
                gotoxy(4, 12);
                printf("[D] - ALTERACAO\n");
                gotoxy(4, 13);
                opc = toupper(getche());

                switch (opc)
                {
                case 'A':
                    if (Compara(TL_clientes, TF) == true)
                        CadastraCliente(index_clientes, TL_clientes);
                    else
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("Erro: dbCheio\n");
                        gotoxy(41, 23);
                    }
                    getch();
                    break;
                case 'B':
                    ConsultaClientes(index_clientes, TL_clientes);
                    break;
                case 'C':
                    if (TL_clientes > 0)
                        DeletaClientes(index_clientes, TL_clientes);
                    else
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("Vetor vazio\n");
                        gotoxy(41, 23);
                        getch();
                    }
                    break;
                case 'D':
                    EditaClientes(index_clientes, TL_clientes);
                    break;
                    // case 'E': q tem
                    //                    break;
                default:
                    if (opc != 27)
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("##INEXISTENTE!## Selecione novamente\n");
                        ClrScrenzona(8, 3, 20, 24);
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
                opc = toupper(getche());

                switch (opc)
                {
                case 'A':
                    if (Compara(TL_fornecedores, TF) == true)
                        CadastraFornecedor(index_fornecedores, TL_fornecedores, NULL);
                    else
                        ClrScrenzona(23, 3, 23, 78);
                    gotoxy(3, 23);
                    printf("Erro: dbCheio\n");
                    gotoxy(41, 23);

                    getch();
                    break;
                case 'B':
                    ConsultaFornecedor(index_fornecedores, TL_fornecedores);
                    break;
                case 'C':
                    if (TL_fornecedores > 0)
                        ExcluirFornecedor(index_fornecedores, TL_fornecedores);
                    else
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("Vetor vazio! \n");
                        gotoxy(52, 23);
                        getch();
                    }
                    break;
                case 'D':
                    AlterarDadosFornecedor(index_fornecedores, TL_fornecedores);
                    break;

                default:
                    if (opc != 27)
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("##INEXISTENTE!## Selecione novamente\n");
                        ClrScrenzona(8, 3, 20, 24);
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
                printf("[E] - Desc Percentual ");
                opc = toupper(getche());

                switch (opc)
                {
                case 'A':
                    if (Compara(TL_produtos, TF) == true)
                        CadastraProd(index_produtos, index_fornecedores, TL_produtos, TL_fornecedores);
                    else
                        ClrScrenzona(23, 3, 23, 78);
                    gotoxy(3, 23);
                    printf("Erro: dbCheio\n");
                    gotoxy(41, 23);

                    getch();
                    break;
                case 'B':
                    ConsultaProd(index_produtos, TL_produtos);
                    break;
                case 'C':
                    ExcluirProd(index_produtos, TL_produtos);
                    break;
                case 'D':
                    AlterarProdCadastrado(index_produtos, TL_produtos);
                    break;
                case 'E':
                    if (TL_produtos > 0)
                        produtosPercent(index_produtos, index_fornecedores, TL_produtos, TL_fornecedores);
                    break;

                default:
                    if (opc != 27)
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("##INEXISTENTE!## Selecione novamente\n");
                        ClrScrenzona(8, 3, 20, 24);
                        gotoxy(51, 23);
                        getche();
                    }
                    break;
                }
            } while (opc != 27);
            opc = 0;
            break;

        case 'D':
            do
            {
                ClrScrenzona(23, 3, 23, 78);
                gotoxy(3, 23);
                printf("*Selecione uma operacao em VENDAS*\n");
                // a mostrar:
                ClrScrenzona(8, 3, 20, 24);
                gotoxy(4, 9);
                printf("[A] - NOVA VENDA\n");
                gotoxy(4, 10);
                printf("[B] - EXCLUSAO DE VENDA\n");
                gotoxy(4, 11);
                printf("[C] - ALTERACAO EM VENDA\n");
                gotoxy(4, 12);
                printf("[D] - RELATORIO DE VENDAS TOTAL");
                opc = toupper(getche());

                switch (opc)
                {
                case 'A':
                    if (TL_vendas <= TF && TL_cupons <= TF)
                    {
                        novaVenda(index_clientes, index_fornecedores, index_produtos, index_vendas, index_vendasprod, TL_clientes, TL_fornecedores, TL_produtos, TL_vendas, TL_cupons);
                    }
                    else
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("memoria cheia! \n");
                        gotoxy(41, 23);
                        getch();
                    }
                    break;
                case 'B':
                    //
                    break;
                case 'C':
                    //
                    break;
                case 'D':
                    //
                    break;

                default:
                    if (opc != 27)
                    {
                        ClrScrenzona(23, 3, 23, 78);
                        gotoxy(3, 23);
                        printf("##INEXISTENTE!## Selecione novamente\n");
                        ClrScrenzona(8, 3, 20, 24);
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
                printf("##INEXISTENTE!## Selecione novamente\n");
                ClrScrenzona(8, 3, 20, 24);
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
    Formulario();
    Fornecedores index_fornecedores[TF];
    Produtos index_produtos[TF];
    Clientes index_clientes[TF];
    Vendas index_vendas[TF];
    Vendas_Produtos vendas[TF];

    Menu(index_fornecedores, index_produtos, index_clientes, index_vendas, vendas);

    return 0;
}
