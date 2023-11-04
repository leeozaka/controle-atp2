// commit history available in
// github.com/leeozaka/controle-atp2;

// Leonardo Neves - 262310406
// Andressa Diniz - 262318644

// issues em README.md

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "conioprintf.h"

#define TF 300
#define QUANT 40

#define BUFFER 50

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
    char CPF[12];
    char NomeCli[QUANT];
    int QtdeCompras;
    double ValorTotComprado;
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
    Moldura(1, 1, 80, 25, 10, 5);
    Moldura(2, 2, 79, 4, 14, 1);
    Moldura(2, 5, 27, 21, 12, 7);
    Moldura(2, 22, 79, 24, 11, 4);
    Moldura(28, 5, 79, 21, 9, 6);
}

int validarCPF(char cpf[11])
{
    int i, digito1 = 0, digito2 = 0, helper, soma;

    ; // verifica se todos caracteres estao entre 0 e 9
    for (i = 0; i < 11; i++)
        if (isalpha(cpf[i]))
            return 0;

    for (soma = 0, helper = 0; helper < 11; helper++)
        soma += (cpf[helper] - '0');
    soma /= 11;
    if (cpf[0] - '0' == soma)
        return 0;
    else
    {
        // digito 1
        for (i = 0; i < strlen(cpf) - 2; i++)
            digito1 += (cpf[i] - '0') * (i + 1);

        digito1 %= 11;
        if (digito1 == 10 || digito1 == 11)
            digito1 = 0;
        if ((cpf[8] - '0') == digito1)
            return 0;

        else
        // digito 2
        {
            for (i = 0; i < strlen(cpf) - 1; i++)
                digito2 += (cpf[i] - '0') * i;
            digito2 %= 11;
            if (digito2 == 10 || digito2 == 11)
                digito2 = 0;
            if ((cpf[10] - '0') != digito2)
                return 0;
        }
    }
    return 1;
}

// void CadastraCliente(Clientes clientes[], int &TL)
int CadastraCliente(FILE *reg_clientes)
{
    if ((reg_clientes = fopen("clientes\\clientes.dat", "rb+")) == NULL)
    {
        reg_clientes = fopen("clientes\\clientes.dat", "ab+");
    }

    Clientes cliente;
    char CPF[11];
    int i, validar = 0;

    conioPrintf(TOPO, ROSA_CLARO, 0, "Cadastro de Clientes!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Digite o CPF: ");

    fflush(stdin);
    fgets(CPF, 12, stdin);

    if (validarCPF(CPF))
    {
        fseek(reg_clientes, 0, SEEK_END);
        if (ftell(reg_clientes) != 0)
        {
            fseek(reg_clientes, 0, SEEK_SET);
            while (!feof(reg_clientes) && validar == 0)
            {
                fread(&cliente, sizeof(Clientes), 1, reg_clientes);
                if (strcmp(CPF, cliente.CPF) == 0)
                    validar = 1;
            }
            if (!!validar)
            {
                conioPrintf(ALERTA, VERMELHO, 0, "CPF ja cadastrado");
                getch();
                return 0;
            }
        }
        strcpy(cliente.CPF, CPF);

        conioPrintf(MENU_RIGHT, BRANCO, 1, "Digite o nome: ");
        fflush(stdin);
        scanf("%s", &cliente.NomeCli);

        cliente.QtdeCompras = 0;
        cliente.ValorTotComprado = 0;

        fseek(reg_clientes, 0, SEEK_END);
        fwrite(&cliente, sizeof(Clientes), 1, reg_clientes);

        conioPrintf(ALERTA, VERDE, 0, "Cliente cadastrado com sucesso!");
        getch();
    }
    else
    {
        conioPrintf(ALERTA, VERMELHO, 0, "O numero de identificacao informado nao esta correto.");
        getch();
        return 0;
    }
    fclose(reg_clientes);
    clearElement(RIGHTSIDE);
    return 1;
}

void ConsultaClientes(Clientes clientes[], int TL)
{
    conioPrintf(TOPO, AZUL_CLARO, 0, "Clientes cadastrados: %d", TL);
    for (int i = 0; i < TL; i++)
    {
        conioPrintf(MENU_RIGHT, BRANCO, i * 4 + 1, "cadastro: %d", i + 1);
        conioPrintf(MENU_RIGHT, BRANCO, i * 4 + 2, "%s", clientes[i].CPF);
        conioPrintf(MENU_RIGHT, BRANCO, i * 4 + 3, "%d compras %f total", clientes[i].QtdeCompras, clientes[i].ValorTotComprado);
        conioPrintf(MENU_RIGHT, BRANCO, i * 4 + 4, "--------------------------------");
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
    conioPrintf(TOPO, BRANCO, 0, "Alterar dados do Fornecedor!");
    if (TL > 0)
    {
        conioPrintf(MENU_RIGHT, BRANCO, 0, "Cod. do fornecedor a ser editado: ");
        fflush(stdin);
        scanf("%d", &cod);
        if (ConsultaFornecedor(fornecedores, TL, cod) >= 0)
        {
            pos = BusFornCod(fornecedores, TL, cod);
            conioPrintf(MENU_RIGHT, BRANCO, 1, "Fornecedor %s", fornecedores[pos].NomeForn);
            conioPrintf(MENU_RIGHT, BRANCO, 2, "Mudar:");
            conioPrintf(MENU_RIGHT, BRANCO, 3, "A - Nome");
            conioPrintf(MENU_RIGHT, BRANCO, 4, "B - Cidade");

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
            conioPrintf(ALERTA, VERMELHO, 0, "Cod. n encontrado!");
            getch();
        }
    }
    else
    {
        conioPrintf(ALERTA, BRANCO, 0, "Lista vazia!");
        getch();
    }
}

// int *cod passa NULL se o codigo do fornecedor ja nao foi inserido em outra funcao, exmeplo no cast do Menu();
void CadastraFornecedor(Fornecedores fornecedores[TF], int &TL, int *cod)
{
    int codforn, busca;

    conioPrintf(TOPO, ROSA, 0, "Cadastro de Fornecedores!");
    if (cod == NULL)
    {
        conioPrintf(MENU_RIGHT, BRANCO, 0, "Digite o cod. do %do fornecedor: ", TL + 1);
        scanf("%d", &codforn);
        clearElement(RIGHTSIDE);
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
            conioPrintf(SWITCHER, VERMELHO, 0, "Cod. Ja existente!");

            conioPrintf(MENU_RIGHT, BRANCO, 1, "Digite o cod. do %do fornecedor: ", TL + 1);
            scanf("%d", &codforn);
            codforn = abs(codforn);
            clearElement(RIGHTSIDE);
        }
    } while (busca != -1);
    fornecedores[TL].CodForn = codforn;
    conioPrintf(MENU_RIGHT, BRANCO, 1, "Nome: ");
    fflush(stdin);
    gets(fornecedores[TL].NomeForn);
    conioPrintf(MENU_RIGHT, BRANCO, 2, "Cidade: ");
    fflush(stdin);
    gets(fornecedores[TL].Cidade);
    TL++;
    conioPrintf(MENU_RIGHT, VERDE, 0, "Fornecedor cod.%d, %s cadastrado!", fornecedores[TL - 1].CodForn, fornecedores[TL - 1].NomeForn);
    getch();
}

void CadastraProd(Produtos produtos[TF], Fornecedores fornecedores[TF], int &TL_Produtos, int &TL_Fornecedores)
{
    int *ptr_codigo;
    int AuxCod, pos, helper, codigo;
    char arg;
    conioPrintf(TOPO, VERDE, 0, "Cadastro de Produtos!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Codigo: ");
    fflush(stdin);
    scanf("%d", &AuxCod);
    while (TL_Produtos < TF && AuxCod > 0)
    {
        pos = BusProdCod(produtos, TL_Produtos, AuxCod);
        if (pos == -1)
        {
            produtos[TL_Produtos].CodProd = AuxCod;
            conioPrintf(MENU_RIGHT, BRANCO, 1, "Descricao: ");
            fflush(stdin);
            gets(produtos[TL_Produtos].Desc);

            conioPrintf(MENU_RIGHT, BRANCO, 2, "Estoque: ");
            fflush(stdin);
            scanf("%d", &produtos[TL_Produtos].Estoque);

            conioPrintf(MENU_RIGHT, BRANCO, 3, "Preco R$: ");
            fflush(stdin);
            scanf("%f", &produtos[TL_Produtos].Preco);

            conioPrintf(MENU_RIGHT, BRANCO, 4, "Data de Validade[dd mm aaaa]: ");
            fflush(stdin);
            scanf("%d %d %d", &produtos[TL_Produtos].DtValidade.Dia, &produtos[TL_Produtos].DtValidade.Mes, &produtos[TL_Produtos].DtValidade.Ano);

            conioPrintf(MENU_RIGHT, BRANCO, 5, "Busca fornecedor, cod: ");
            fflush(stdin);
            scanf("%d", &helper);
            ptr_codigo = &helper;

            codigo = ConsultaFornecedor(fornecedores, TL_Fornecedores, helper);

            if (codigo == -1)
            {
                conioPrintf(MENU_RIGHT, BRANCO, 6, "Fornecedor nao encontrado. Cadastrar fornecedor? S/N");
                arg = toupper(getch());

                if (arg == 'S')
                {
                    clearElement(RIGHTSIDE);
                    CadastraFornecedor(fornecedores, TL_Fornecedores, ptr_codigo);
                    produtos[TL_Produtos].CodForn = codigo;
                    TL_Produtos++;
                    conioPrintf(ALERTA, VERDE, 0, "Fornecedor Cadastrado!");
                    getch();
                }
                else
                {
                    conioPrintf(ALERTA, VERMELHO, 0, "Produto n cadastrado: Fornecedor nao encontrado!");
                    getch();
                }
            }
            else
            {
                clearElement(RIGHTSIDE);
                conioPrintf(MENU_RIGHT, BRANCO, 0, "Fornecedor: %d", codigo);
                produtos[TL_Produtos].CodForn = codigo;
                TL_Produtos++;
                conioPrintf(ALERTA, VERDE, 0, "Produto %s cadastrado!", produtos[TL_Produtos - 1].Desc);
                getch();
            }
        }
        else
        {
            conioPrintf(ALERTA, VERMELHO, 0, "Codigo do Produto ja cadastrado!");
            getch();
        }
        clearElement(ALERTA);
        clearElement(RIGHTSIDE);
        conioPrintf(TOPO, VERDE, 0, "Cadastro de Produtos!");
        conioPrintf(MENU_RIGHT, BRANCO, 0, "Codigo: ");
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
    int pos, i;
    char cpf[11];
    conioPrintf(TOPO, VERMELHO_CLARO, 0, "Exclusao de Clientes!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "CPF do Cliente a ser Deletado: ");
    fflush(stdin);
    gets(cpf);
    if (validarCPF(cpf) == 1)
    {
        for (i = 0, pos = -1; i < TL && pos == -1; i++)
            if ((strcmp(clientes[i].CPF, cpf)) == 0)
                pos = i;
        if (pos >= 0)
        {
            for (i = pos; i < TL - 1; i++)
            {
                clientes[i] = clientes[i + 1];
            }
            TL--;
            conioPrintf(SWITCHER, VERDE, 0, "Cliente removido com sucesso!");
        }
        else
        {
            conioPrintf(SWITCHER, VERMELHO, 0, "Erro ao remover cliente!");
        }
    }
}
void ExcluirProd(Produtos produtos[], int &TL)
{
    int i, Aux, ponto;
    conioPrintf(TOPO, VERDE_CLARO, 0, "Exclusao de produtos!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Digite o cod. do produto a excluir: "); // n ta do lado direito
    scanf("%d", &Aux);
    while (Aux > 0)
    {
        ponto = BusProdCod(produtos, TL, Aux);
        if (ponto != -1)
        {
            for (i = ponto; i < TL - 1; i++)
                produtos[i] = produtos[i + 1];
            TL--;
            conioPrintf(ALERTA, VERDE, 0, "Produto excluido com sucesso!");
        }
        else
        {
            conioPrintf(ALERTA, VERMELHO, 0, "Produto nao encontrado!");
        }
        conioPrintf(MENU_RIGHT, BRANCO, 0, "Digite o cod. do produto a excluir: ");
        scanf("%d", &Aux);
    }
}

void AlterarProdCadastrado(Produtos produtos[], int TL)
{
    TpData aux;
    char resp;
    int Aux, ponto;
    float Aux_Preco;

    conioPrintf(TOPO, VERMELHO, 0, "Alterar produto ja cadastrado!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Insira o codigo do produto: ");
    fflush(stdin);
    scanf("%d", &Aux);
    while (Aux > 0)
    {
        ponto = BusProdCod(produtos, TL, Aux);
        if (ponto == -1)
        {
            conioPrintf(SWITCHER, VERMELHO, 0, "Erro ao procurar pelo Codigo!");
            getch();
        }
        else
        {
            conioPrintf(MENU_RIGHT, BRANCO, 1, "Qual elemento alterar do %s?", produtos[ponto].Desc);
            conioPrintf(MENU_RIGHT, BRANCO, 2, "[A] - Estoque");
            conioPrintf(MENU_RIGHT, BRANCO, 3, "[B] - Preco");
            conioPrintf(MENU_RIGHT, BRANCO, 4, "[C] - Validade");

            fflush(stdin);
            resp = toupper(getch());

            switch (resp)
            {
            case 'A':
                conioPrintf(MENU_RIGHT, BRANCO, 5, "Qual a nova quantidade do %s? ", produtos[ponto].Desc);
                while (scanf(" %d", &Aux) <= 0)
                    conioPrintf(ALERTA, VERMELHO, 0, "Valor invalido");
                produtos[ponto].Estoque = Aux;
                break;

            case 'B':
                conioPrintf(MENU_RIGHT, BRANCO, 5, "Qual o novo preco do %s? R$ ", produtos[ponto].Desc);
                while (scanf(" %f", &Aux_Preco) < 0)
                    conioPrintf(ALERTA, VERMELHO, 0, "Preco invalido");
                produtos[ponto].Preco = Aux_Preco;
                break;

            case 'C':
                conioPrintf(MENU_RIGHT, BRANCO, 0, "Qual a nova validade do %s? ", produtos[ponto].Desc);
                scanf("%d %d %d", &aux.Dia, &aux.Mes, &aux.Ano);
                if (aux.Dia > 0 && aux.Mes > 0 && aux.Ano > 0)
                {
                    produtos[ponto].DtValidade = aux;
                }
                else
                    conioPrintf(ALERTA, VERMELHO, 0, "Data invalida!");
                break;
            }
        }
        clearElement(RIGHTSIDE);
        conioPrintf(MENU_RIGHT, BRANCO, 0, "Insira o codigo do produto: ");
        fflush(stdin);
        scanf("%d", &Aux);
    }
}

void ConsultaProd(Produtos produtos[], int TL)
{
    int ponto, i;
    conioPrintf(TOPO, ROSA, 0, "Consulta de Produto!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Cod. a ser consultado: "); // n ta ficando aqui tbm pro lado direito
    scanf("%d", &ponto);

    i = BusProdCod(produtos, TL, ponto);

    if (i >= 0)
    {
        conioPrintf(MENU_RIGHT, BRANCO, 1, "Codigo: %d", produtos[i].CodProd);
        puts(produtos[i].Desc);
        conioPrintf(MENU_RIGHT, BRANCO, 2, "Quantidade em estoque:%d\n", produtos[i].Estoque);
        conioPrintf(MENU_RIGHT, BRANCO, 3, "%d %d %d", produtos[i].DtValidade.Dia, produtos[i].DtValidade.Mes, produtos[i].DtValidade.Ano);
    }
    else
        conioPrintf(ALERTA, VERMELHO, 0, "Produto nao encontrado!");
}

void produtosPercent(Produtos index_produtos[], Fornecedores index_fornecedores[], int TL_produtos, int TL_fornecedores)
{
    int valor, cod, pos;
    char resp;

    conioPrintf(TOPO, VERMELHO_CLARO, 0, "Percentual  de produtos!");

    /*do - nao funciona
    {
        clearElement(RIGHTSIDE);
        conioPrintf(MENU_RIGHT, BRANCO, 0, "[A] Add - [B] Subtrair");
        fflush(stdin);
        resp = toupper(getch());
    } while (resp != 'A' || resp != 'B');
    */

    clearElement(RIGHTSIDE);
    conioPrintf(MENU_RIGHT, BRANCO, 0, "[A] Add - [B] Subtrair");
    fflush(stdin);
    resp = toupper(getch());

    conioPrintf(MENU_RIGHT, BRANCO, 1, "Valor em porc: ");
    fflush(stdin);
    scanf("%d", &valor);

    conioPrintf(MENU_RIGHT, BRANCO, 2, "Cod do fornecedor para %s: ", resp == 'A' ? "adicionar" : "subtrair");
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
            conioPrintf(SWITCHER, VERDE, 0, "Produtos atualizados!");
            getch();
            break;
        case 'B':
            for (int i = 0; i < TL_produtos; i++)
                if (index_produtos[i].CodForn == index_fornecedores[pos].CodForn)
                    index_produtos[i].Preco -= (index_produtos[i].Preco * (valor / 100));
            conioPrintf(SWITCHER, VERDE, 0, "Produtos atualizados!");
            getch();
            break;
        }
    }
    else
        conioPrintf(SWITCHER, VERMELHO, 0, "Erro! Fornecedor nao cadastrado!");
}

// void EditaClientes(Clientes clientes[], int TL)
int EditaClientes(FILE *reg_clientes)
{
    reg_clientes = fopen("clientes\\clientes.dat", "rb+");
    if (reg_clientes == NULL)
        return 0;

    Clientes cliente;
    int opcao, pos;
    char cpf[12], opc;

    conioPrintf(TOPO, CIANO, 0, "Editar Clientes!");
    clearElement(RIGHTSIDE);

    conioPrintf(MENU_RIGHT, BRANCO, 0, "CPF a ser editado: ");
    fflush(stdin);
    fgets(cpf, 12, stdin);

    if (validarCPF(cpf))
    {
        fseek(reg_clientes, 0, SEEK_SET);
        while (!feof(reg_clientes))
        {
            fread(&cliente, sizeof(Clientes), 1, reg_clientes);
            if (strcmp(cliente.CPF, cpf) == 0)
            {
                pos = ftell(reg_clientes)-sizeof(Clientes);
                break;
            }
        }
        if (ftell(reg_clientes) == EOF)
        {
            conioPrintf(SWITCHER, VERMELHO, 0, "Cliente Inexistente");
            getch();
            return 0;
        }

        conioPrintf(MENU_RIGHT, BRANCO, 2, "Mudar: ");
        conioPrintf(MENU_RIGHT, BRANCO, 3, "A - Nome");
        conioPrintf(MENU_RIGHT, BRANCO, 4, "B - Zerar Compras");

        opc = toupper(getch());

        switch (opc)
        {
        case 'A':
            fseek(reg_clientes, pos, SEEK_SET);
            fflush(stdin);
            conioPrintf(SWITCHER, BRANCO, 0, "Nome: ");
            fgets(cliente.NomeCli,BUFFER,stdin);
            fwrite(&cliente, sizeof(Clientes), 1, reg_clientes);
            fread(&cliente, sizeof(Clientes), 1, reg_clientes);
            conioPrintf(SWITCHER,VERMELHO,0,cliente.NomeCli);
            getch();
            break;
        case 'B':
            cliente.QtdeCompras = 0;
            cliente.ValorTotComprado = 0;
            fseek(reg_clientes, pos, SEEK_SET);
            fwrite(&cliente, sizeof(Clientes), 1, reg_clientes);
            conioPrintf(MENU_RIGHT, BRANCO, 5, "Compras zeradas!");
            // buscar compras no cpf e deletar do index_vendas;
            break;
        }
    }
    else
        conioPrintf(SWITCHER, VERMELHO, 0, "CPF nao encontrado!");
    fclose(reg_clientes);
    getch();
}

void ConsultaFornecedor(Fornecedores fornecedores[], int TL)
{
    int i, consulta;
    conioPrintf(TOPO, VERDE_CLARO, 0, "Consulta de Fornecedores!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Busca fornecedor pelo codigo: ");
    fflush(stdin);
    scanf("%d", &consulta);

    if (consulta >= 0)
    {
        i = BusFornCod(fornecedores, TL, consulta);
        if (i >= 0)
        {
            clearElement(RIGHTSIDE);
            conioPrintf(MENU_RIGHT, BRANCO, 0, "%d", fornecedores[i].CodForn);
            conioPrintf(MENU_RIGHT, BRANCO, 0, "%s", fornecedores[i].NomeForn);
            conioPrintf(MENU_RIGHT, BRANCO, 0, "%s", fornecedores[i].Cidade);
            printf("%d\n%s\n%s", fornecedores[i].CodForn, fornecedores[i].NomeForn, fornecedores[i].Cidade);
        }
        else
            conioPrintf(SWITCHER, VERMELHO, 0, "Fornecedor nao encontrado!");
    }
    else
        conioPrintf(SWITCHER, VERMELHO, 0, "Apenas cod's positivos!");
}

void ExcluirFornecedor(Fornecedores fornecedores[], int &TL)
{
    int i;
    char a;

    conioPrintf(TOPO, CIANO, 0, "Busca fornecedor pelo codigo: ");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Busca fornecedor pelo codigo: ");

    fflush(stdin);
    scanf("%d", &i);

    i = BusFornCod(fornecedores, TL, i);

    if (i >= 0)
    {
        conioPrintf(MENU_RIGHT, BRANCO, 1, "Deletar %s? S/N", fornecedores[i].NomeForn);
        fflush(stdin);
        a = toupper(getch());

        if (a == 'S')
        {
            for (; i < TL - 1; i++)
            {
                fornecedores[i] = fornecedores[i + 1];
            }
            TL--;
            conioPrintf(SWITCHER, VERDE, 0, "Deletado com sucesso!");
        }
    }
    else
        conioPrintf(SWITCHER, VERMELHO, 0, "Fornecedor nao encontrado! ");
}

// funciona como <string.h> strcmp()
int comparaData(int ano1, int mes1, int dia1, int ano2, int mes2, int dia2)
{
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

int BuscaVendaPorCodigo(Vendas vendas[], int TL, int cod)
{
    return cod;
}
int BuscaProdutoPorCodigo(Produtos produtos[], int TL, int cod)
{
    return cod;
}

int novaVenda(Clientes rootClientes[], Fornecedores rootFornecedores[], Produtos rootProdutos[], Vendas rootVendas[], Vendas_Produtos rootVendasProdutos[], int TLclientes, int TLfornecedores, int TLprodutos, int &TLvendas, int &TLvendasprod)
{
    int i, pos_cliente, cod_aux, var, valor_variavel = 0;
    char cpf[11], datahelper;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    TpData construtor_data, input;
    bool pass;
    conioPrintf(TOPO, ROSA, 0, "Nova Venda!!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "CPF do Cliente: ");
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
                clearElement(RIGHTSIDE);
                conioPrintf(MENU_RIGHT, BRANCO, 0, "Cliente: %s", rootClientes[pos_cliente].NomeCli);
                conioPrintf(MENU_RIGHT, BRANCO, 1, "[A] - Usar a data atual para a venda");
                conioPrintf(MENU_RIGHT, BRANCO, 2, "[B] - Data personalizada;");
                fflush(stdin);
                datahelper = getch();

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
                        conioPrintf(SWITCHER, VERMELHO, 0, "Obrigatorio inserir data valida!");
                        getch();
                    }
                } while (pass == false);

                do
                {
                    int prod_pos;
                    clearElement(RIGHTSIDE);
                    conioPrintf(MENU_RIGHT, BRANCO, 0, "Cod do %d prod. a ser adicionado: ", TLvendasprod);
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
                                conioPrintf(MENU_RIGHT, BRANCO, 1, "Produto: %s\nQtde em estoque:%d\n", rootProdutos[prod_pos].Desc, rootProdutos[prod_pos].Estoque);
                                do
                                {
                                    conioPrintf(MENU_RIGHT, BRANCO, 2, "Quantidade vendida: ");
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
                                    conioPrintf(SWITCHER, VERDE, 0, "Item %s adicionado a venda de cod. %d\n", rootProdutos[prod_pos].Desc, TLvendas + 1);
                                    getch();
                                }
                                else
                                {
                                    conioPrintf(SWITCHER, VERMELHO, 0, "Qtde. maior que estoque total!");
                                    getch();
                                }
                            }
                            else
                            {
                                conioPrintf(SWITCHER, AMARELO, 0, "Validade do produto nao bate com a venda");
                                getch();
                            }
                        }
                        else
                        {
                            conioPrintf(SWITCHER, VERMELHO, 0, "Produto n. encontrado, tente novamente");
                            getch();
                        }
                    }
                    clearElement(RIGHTSIDE);
                    conioPrintf(MENU_RIGHT, BRANCO, 0, "Cod do %d prod. a ser adicionado: ", TLvendasprod);
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

                conioPrintf(ALERTA, VERDE, 0, "Venda do cliente %s registrada com sucesso", rootClientes[pos_cliente].NomeCli);
                getch();
            }
            else
            {
                conioPrintf(SWITCHER, VERMELHO, 0, "Cliente nao encontrado!");
                getch();
            }
        }
        else
        {
            conioPrintf(ALERTA, VERMELHO, 0, "Digite um CPF valido!");
            getch();
        }

        clearElement(RIGHTSIDE);
        conioPrintf(MENU_RIGHT, BRANCO, 0, "(0 cancela) CPF do Cliente: ");
        fflush(stdin);
        gets(cpf);
    }
    return 0;
}
void ExcluirVenda(Vendas rootVendas[], Vendas_Produtos rootVendasProdutos[], int &TLvendas, int &TLvendasprod)
{
    int codVenda, i, j;
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Digite o codigo da venda a ser excluida (0 para cancelar): ");
    fflush(stdin);
    scanf("%d", &codVenda);

    if (codVenda > 0)
    {
        for (i = 0; i < TLvendas; ++i)
        {
            if (rootVendas[i].CodVenda == codVenda)
            {
                for (j = 0; j < TLvendasprod; ++j)
                {
                    if (rootVendasProdutos[j].CodVenda == codVenda)
                    {
                        for (int k = j; k < TLvendasprod - 1; ++k)
                        {
                            rootVendasProdutos[k] = rootVendasProdutos[k + 1];
                            // remover o restante
                        }
                        TLvendasprod--;
                        j--;
                    }
                }
                for (int k = i; k < TLvendas - 1; ++k)
                {
                    rootVendas[k] = rootVendas[k + 1];
                }
                TLvendas--;
                conioPrintf(ALERTA, VERDE, 0, "Venda de codigo %d excluida com sucesso", codVenda);
                getch();
                return;
            }
        }
        conioPrintf(ALERTA, VERMELHO, 0, "Venda de codigo %d nao encontrada!", codVenda);
    }
    else
        conioPrintf(ALERTA, CIANO_CLARO, 0, "Operacaoo de exclusao cancelada.");
}

void AlterarVenda(Vendas rootVendas[], Vendas_Produtos rootVendasProdutos[], Produtos rootProdutos[], int TLvendas, int TLvendasprod, int TLprodutos)
{
    int codVenda, novoEstoque, novoQuantidade, codProduto;
    char resp;
    int pontoVenda, pontoProd;

    conioPrintf(TOPO, VERDE, 0, "Alterar Venda Existente!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Insira o codigo da venda: ");
    scanf("%d", &codVenda);

    pontoVenda = BuscaVendaPorCodigo(rootVendas, TLvendas, codVenda);

    if (pontoVenda != -1)
    {
        do
        {
            conioPrintf(MENU_RIGHT, BRANCO, 1, "O que deseja alterar na venda?");
            conioPrintf(MENU_RIGHT, BRANCO, 2, "[A] - Adicionar Produto");
            conioPrintf(MENU_RIGHT, BRANCO, 3, "[B] - Remover Produto");
            conioPrintf(MENU_RIGHT, BRANCO, 4, "[C] - Modificar Produto (Estoque/Quantidade)");
            conioPrintf(MENU_RIGHT, BRANCO, 5, "[D] - Sair");
            fflush(stdin);
            resp = toupper(getch());

            switch (resp)
            {
            case 'A':
                // Adicionar Produto o Venda

                break;

            case 'B':
                // Remover Produto da Venda
                // Colocar o BuscaProdutoPorCodigo pra encontrar o produto na lista de vendas aqui
                break;

            case 'C':
                // Modificar Produto (Estoque/Quantidade) na Venda
                conioPrintf(MENU_RIGHT, BRANCO, 0, "Insira o codigo do produto: ");
                fflush(stdin);
                scanf("%d", &codProduto);
                pontoProd = BuscaProdutoPorCodigo(rootProdutos, TLprodutos, codProduto);

                if (pontoProd != -1)
                {
                    // modif o produto (estoque/quantidade) na venda
                    // e atualizar
                }
                else
                {
                    conioPrintf(ALERTA, VERMELHO, 0, "Produto nao encontrado!");
                    getch();
                }
                break;

            case 'D':
                // Sair da funcao
                break;

            default:
                conioPrintf(ALERTA, VERMELHO, 0, "Opcao invalida!");
                getch();
                break;
            }
        } while (resp != 'D');
    }
    else
    {
        conioPrintf(ALERTA, VERMELHO, 0, "Venda nao encontrada!");
    }
}

int InsereElementos(Fornecedores fornecedores[], Produtos produtos[], Clientes clientes[], Vendas vendas[], Vendas_Produtos vendasprod[], int &tlfornecedores, int &tlprodutos, int &tlclientes, int &tlvendas, int &tlvendasprod)
{
    int i = 0;
    strcpy(fornecedores[tlfornecedores].Cidade, "Presidente Prudente");
    fornecedores[tlfornecedores].CodForn = 300;
    strcpy(fornecedores[tlfornecedores].NomeForn, "Halan");
    tlfornecedores++;
    i++;

    strcpy(fornecedores[tlfornecedores].Cidade, "Assis");
    fornecedores[tlfornecedores].CodForn = 301;
    strcpy(fornecedores[tlfornecedores].NomeForn, "Jaspion");
    tlfornecedores++;
    i++;

    produtos[tlprodutos].CodForn = 300;
    produtos[tlprodutos].CodProd = 100;
    strcpy(produtos[tlprodutos].Desc, "Drink");
    produtos[tlprodutos].DtValidade.Dia = 25;
    produtos[tlprodutos].DtValidade.Mes = 1;
    produtos[tlprodutos].DtValidade.Ano = 2024;
    produtos[tlprodutos].Estoque = 10;
    produtos[tlprodutos].Preco = 25.00;
    tlprodutos++;
    i++;

    produtos[tlprodutos].CodForn = 300;
    produtos[tlprodutos].CodProd = 100;
    strcpy(produtos[tlprodutos].Desc, "Drink");
    produtos[tlprodutos].DtValidade.Dia = 25;
    produtos[tlprodutos].DtValidade.Mes = 1;
    produtos[tlprodutos].DtValidade.Ano = 2024;
    produtos[tlprodutos].Estoque = 10;
    produtos[tlprodutos].Preco = 25.00;
    tlprodutos++;
    i++;

    produtos[tlprodutos].CodForn = 301;
    produtos[tlprodutos].CodProd = 200;
    strcpy(produtos[tlprodutos].Desc, "Comida");
    produtos[tlprodutos].DtValidade.Dia = 30;
    produtos[tlprodutos].DtValidade.Mes = 3;
    produtos[tlprodutos].DtValidade.Ano = 2024;
    produtos[tlprodutos].Estoque = 20;
    produtos[tlprodutos].Preco = 30.00;
    tlprodutos++;
    i++;

    strcpy(clientes[tlclientes].CPF, "12345678909");
    strcpy(clientes[tlclientes].NomeCli, "Michael Jackson");
    clientes[tlclientes].QtdeCompras = 0;
    clientes[tlclientes].ValorTotComprado = 0;
    tlclientes++;
    i++;

    strcpy(clientes[tlclientes].CPF, "11155588894");
    strcpy(clientes[tlclientes].NomeCli, "Michael Schumacker");
    clientes[tlclientes].QtdeCompras = 0;
    clientes[tlclientes].ValorTotComprado = 0;
    tlclientes++;
    i++;
    return i;
}

// void Menu(Fornecedores index_fornecedores[], Produtos index_produtos[], Clientes index_clientes[], Vendas index_vendas[], Vendas_Produtos index_vendasprod[])
void Menu(FILE *fornecedores, FILE *produtos, FILE *clientes, FILE *index_vendas, FILE *vendas)
{
    int op, vendas_size;
    char opc, opc_sub;

    Formulario();

    if ((vendas = fopen("\\vendas\\vendas.dat", "rb+")) == NULL)
        vendas_size = 0;
    else
    {
        fseek(vendas, 0, SEEK_END);
        vendas_size = ftell(vendas) / sizeof(Vendas);
    }

    conioPrintf(TOPO, VERDE, 0, "%s %d", "Vendas:", vendas_size);
    conioPrintf(SWITCHER, VERDE, 0, "Selecione um item:");
    conioPrintf(MENU_LEFT, BRANCO, 0, "[A] - CLIENTES");
    conioPrintf(MENU_LEFT, BRANCO, 1, "[B] - FORNECEDORES");
    conioPrintf(MENU_LEFT, BRANCO, 2, "[C] - PRODUTOS");
    conioPrintf(MENU_LEFT, BRANCO, 3, "[D] - VENDAS");
    conioPrintf(MENU_LEFT, BRANCO, 5, "ESC - Sair");
    gotoxy(4, 14);

    opc = toupper(getche());

    while (opc != 27)
    {
        switch (opc)
        {
        case 'A':
            do
            {
                conioPrintf(TOPO, ROSA, 0, "Controle de Clientes!");
                conioPrintf(SWITCHER, ROSA, 0, "Selecione uma operacao em clientes");
                conioPrintf(MENU_LEFT, BRANCO, 0, "[A] - CADASTRO");
                conioPrintf(MENU_LEFT, BRANCO, 1, "[B] - CONSULTA");
                conioPrintf(MENU_LEFT, BRANCO, 2, "[C] - EXCLUSAO");
                conioPrintf(MENU_LEFT, BRANCO, 3, "[D] - ALTERACAO");
                gotoxy(4, 13);
                opc = toupper(getche());

                switch (opc)
                {
                case 'A':
                    CadastraCliente(clientes);
                    getch();
                    break;
                    /*
                {case 'B':
                    ConsultaClientes(index_clientes, TL_clientes);
                    getch();
                    break;
                case 'C':
                    if (TL_clientes > 0)
                        DeletaClientes(index_clientes, TL_clientes);
                    else
                    {
                        conioPrintf(SWITCHER, VERMELHO, 0, "Erro: Vetor vazio");
                        gotoxy(41, 23);
                    }
                    getch();
                    break;
                    */
                case 'D':
                    EditaClientes(clientes);
                    break;
                    /*
                case 27:
                    break;
                default:
                    conioPrintf(ALERTA, VERMELHO, 0, "##INEXISTENTE!## Selecione novamente");
                    fflush(stdin);
                    getch();
                }
            } while (opc != 27);
            break;

        case 'B':
            do
            {
                conioPrintf(TOPO, MARROM, 0, "Controle de Fornecedores!");
                conioPrintf(SWITCHER, AMARELO, 0, "Selecione uma opcao em fornecedores");
                conioPrintf(MENU_LEFT, BRANCO, 0, "[A] - CADASTRO");
                conioPrintf(MENU_LEFT, BRANCO, 1, "[B] - CONSULTA");
                conioPrintf(MENU_LEFT, BRANCO, 2, "[C] - EXCLUSAO");
                conioPrintf(MENU_LEFT, BRANCO, 3, "[D] - ALTERACAO");
                opc = toupper(getch());

                switch (opc)
                {
                case 'A':
                    if (Compara(TL_fornecedores, TF) == true)
                        CadastraFornecedor(index_fornecedores, TL_fornecedores, NULL);
                    else
                        conioPrintf(SWITCHER, VERMELHO, 0, "Erro: dbCheio");
                    fflush(stdin);
                    getch();
                    break;
                case 'B':
                    ConsultaFornecedor(index_fornecedores, TL_fornecedores);
                    getch();
                    break;
                case 'C':
                    if (TL_fornecedores > 0)
                        ExcluirFornecedor(index_fornecedores, TL_fornecedores);
                    else
                    {
                        conioPrintf(SWITCHER, VERMELHO, 0, "Erro: Vetor vazio");
                        getch();
                    }
                    fflush(stdin);
                    getch();
                    break;
                case 'D':
                    AlterarDadosFornecedor(index_fornecedores, TL_fornecedores);
                    break;
                case 27:
                    break;
                default:
                    conioPrintf(ALERTA, VERMELHO, 0, "##INEXISTENTE!## Selecione novamente");
                    getch();
                }
            }} while (opc != 27);
            break;

        case 'C':
            do
            {
                conioPrintf(TOPO, CINZA_CLARO, 0, "Controle de Produtos!");
                conioPrintf(SWITCHER, MARROM, 0, "Selecione uma operacao em produtos");
                conioPrintf(MENU_LEFT, BRANCO, 0, "[A] - CADASTRO");
                conioPrintf(MENU_LEFT, BRANCO, 1, "[B] - CONSULTA");
                conioPrintf(MENU_LEFT, BRANCO, 2, "[C] - EXCLUSAO");
                conioPrintf(MENU_LEFT, BRANCO, 3, "[D] - ALTERACAO");
                conioPrintf(MENU_LEFT, BRANCO, 4, "[E] - DESC PERCENTUAL");
                fflush(stdin);
                opc = toupper(getch());

                switch (opc)
                {
                case 'A':
                    if (Compara(TL_produtos, TF) == true)
                        CadastraProd(index_produtos, index_fornecedores, TL_produtos, TL_fornecedores);
                    else
                    {
                        conioPrintf(SWITCHER, VERMELHO, 0, "Erro: dbCheio");
                        gotoxy(41, 23);
                    }
                    getch();
                    break;
                case 'B':
                    ConsultaProd(index_produtos, TL_produtos);
                    getch();
                    break;
                case 'C':
                    ExcluirProd(index_produtos, TL_produtos);
                    getch();
                    break;
                case 'D':
                    AlterarProdCadastrado(index_produtos, TL_produtos);
                    getch();
                    break;
                case 'E':
                    if (TL_produtos > 0)
                        produtosPercent(index_produtos, index_fornecedores, TL_produtos, TL_fornecedores);
                    getch();
                    break;
                case 27:
                    break;
                default:
                    conioPrintf(SWITCHER, VERMELHO, 0, "##INEXISTENTE!## Selecione novamente");
                    getch();
                    break;
                }
            } while (opc != 27);
            break;

        case 'D':
            do
            {
                conioPrintf(TOPO, CIANO, 0, "Controle de Vendas!");
                conioPrintf(SWITCHER, CIANO, 0, "Selecione uma operacao em Vendas!");
                conioPrintf(MENU_LEFT, BRANCO, 0, "[A] - NOVA VENDA");
                conioPrintf(MENU_LEFT, BRANCO, 1, "[B] - EXCLUSAO");
                conioPrintf(MENU_LEFT, BRANCO, 2, "[C] - ALTERACAO");
                conioPrintf(MENU_LEFT, BRANCO, 3, "[D] - RELATORIO TOTAL");
                opc = toupper(getch());

                switch (opc)
                {
                case 'A':
                    if (TL_vendas <= TF && TL_cupons <= TF)
                    {
                        novaVenda(index_clientes, index_fornecedores, index_produtos, index_vendas, index_vendasprod, TL_clientes, TL_fornecedores, TL_produtos, TL_vendas, TL_cupons);
                    }
                    else
                    {
                        conioPrintf(SWITCHER, VERMELHO, 0, "Mem. Cheia!");
                        gotoxy(41, 23);
                    }
                    getch();
                    break;
                case 'B':
                    ExcluirVenda(index_vendas, index_vendasprod, TL_cupons, TL_vendas);
                    getch();
                    break;
                case 'C':
                    AlterarVenda(index_vendas, index_vendasprod, index_produtos, TL_cupons, TL_vendas, TL_produtos);
                    getch();
                    break;
                case 'D':
                    // RelatorioTotal();
                    break;
                case 27:
                    break;
                default:
                    conioPrintf(SWITCHER, VERMELHO, 0, "##INEXISTENTE!## Selecione novamente");
                    getch();
                    break;
                }
            } while (opc != 27);
            break;
        case 'E':
            conioPrintf(SWITCHER, VERDE, 0, "%d itens lidos", InsereElementos(index_fornecedores, index_produtos, index_clientes, index_vendas, index_vendasprod, TL_fornecedores, TL_produtos, TL_clientes, TL_vendas, TL_cupons));
            break;
        */
                }
            } while (opc != 27);
            Formulario();
            conioPrintf(TOPO, VERDE, 0, "Vendas realizadas: %d", vendas_size);
            conioPrintf(SWITCHER, VERDE, 0, "Selecione um item:");
            conioPrintf(MENU_LEFT, BRANCO, 0, "[A] - CLIENTES");
            conioPrintf(MENU_LEFT, BRANCO, 1, "[B] - FORNECEDORES");
            conioPrintf(MENU_LEFT, BRANCO, 2, "[C] - PRODUTOS");
            conioPrintf(MENU_LEFT, BRANCO, 3, "[D] - VENDAS");
            conioPrintf(MENU_LEFT, BRANCO, 4, "ESC - Sair");
            opc = toupper(getche());
        }
    }
}

int main(int morteaodevcpp, char **ideruim)
{
    FILE *fornecedores;
    FILE *produtos;
    FILE *clientes;
    FILE *index_vendas;
    FILE *vendas;

    _setcursortype(0);
    Menu(fornecedores, produtos, clientes, index_vendas, vendas);

    return 0;
}
