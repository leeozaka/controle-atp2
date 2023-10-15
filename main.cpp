#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "conioprintf.h"

#define TF 300
#define QUANT 40

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
    Moldura(1, 1, 80, 25, 10, 5);
    Moldura(2, 2, 79, 4, 14, 1);
    Moldura(2, 5, 27, 21, 12, 7);
    Moldura(2, 22, 79, 24, 11, 4);
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

void CadastraCliente(Clientes clientes[], int &TL)
{
    char CPF[11];
    int i, validar = 0;
    conioPrintf("topo", "rosa_claro", 0, "Cadastro de Clientes!");
    conioPrintf("menu_right", "branco", 0, "Digite o CPF: ");
    fflush(stdin);
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
            conioPrintf("alerta", "vermelho", 0, "CPF ja cadastrado");
            getch();
        }
        else
        {
            strcpy(clientes[TL].CPF, CPF);
            conioPrintf("menu_right", "branco", 1, "Digite o nome: ");
            fflush(stdin);
            scanf("%s", &clientes[TL].NomeCli);

            clientes[TL].QtdeCompras = 0;
            clientes[TL].ValorTotComprado = 0;

            TL++;

            conioPrintf("alerta", "verde", 0, "Cliente cadastrado com sucesso! ");
            getch();
        }
    }
    else
    {
        conioPrintf("alerta", "vermelho", 0, "O numero de identificacao informado nao esta correto.");
        getch();
    }
    clearElement("rightside"); //limpar lado direito
}

void ConsultaClientes(Clientes clientes[], int TL)
{
    conioPrintf("topo", "azul_claro", 0, "Clientes cadastrados: %d", TL);
    for (int i = 0; i < TL; i++)
    {
        conioPrintf("menu_right", "branco", i*4 + 1, "cadastro: %d", i + 1);
        conioPrintf("menu_right", "branco", i*4 + 2, clientes[i].CPF);
        conioPrintf("menu_right", "branco", i*4 + 3, "%d compras %f total", clientes[i].QtdeCompras, clientes[i].ValorTotComprado);
        conioPrintf("menu_right", "branco", i*4 + 4, "--------------------------------");
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
    // retorna c√≥digo do Fornecedor! √± pos;
    int i;
    for (i = 0; i < TL && find != lista_fornecedores[i].CodForn; i++)
        ;
    return i == TL ? -1 : lista_fornecedores[i].CodForn;
}

void AlterarDadosFornecedor(Fornecedores fornecedores[], int TL)
{
    int opcao, pos, cod;
    char opc;
    conioPrintf("topo", "azul", 0, "Alterar dados do Fornecedor!");
    if (TL > 0)
    {
        conioPrintf("menu_right", "branco", 0, "Cod. do fornecedor a ser editado: ");
        fflush(stdin);
        scanf("%d", &cod);
        if (ConsultaFornecedor(fornecedores, TL, cod) >= 0)
        {
            pos = BusFornCod(fornecedores, TL, cod);
            conioPrintf("menu_right", "branco", 1, "Fornecedor %s", fornecedores[pos].NomeForn);
            conioPrintf("menu_right", "branco", 2, "Mudar:");
            conioPrintf("menu_right", "branco", 3, "A - Nome");
            conioPrintf("menu_right", "branco", 4, "B - Cidade");

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
            conioPrintf("alerta", "vermelho", 0, "Cod. n encontrado!");
            getch();
        }
    }
    else
    {
        conioPrintf("alerta", "branco", 0, "Lista vazia!");
        getch();
    }
}

void CadastraFornecedor(Fornecedores fornecedores[TF], int &TL, int *cod)
// int *cod passa NULL se o codigo do fornecedor ja nao foi inserido em outra funcao, exmeplo no cast do Menu();
{
    int codforn, busca;

    conioPrintf("topo", "rosa", 0, "Cadastro de Fornecedores!");
    if (cod == NULL)
    {
        conioPrintf("menu_right", "branco", 0, "Digite o cod. do %do fornecedor: ", TL + 1);
        scanf("%d", &codforn);
        clearElement("rightside");
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
            conioPrintf("switcher", "vermelho", 0, "Cod. Ja existente!");

            conioPrintf("menu_right", "branco", 1, "Digite o cod. do %do fornecedor: ", TL + 1);
            scanf("%d", &codforn);
            codforn = abs(codforn);
            clearElement("rightside");
        }
    } while (busca != -1);
    fornecedores[TL].CodForn = codforn;
    conioPrintf("menu_right", "branco", 1, "Nome: ");//n ta indo pro lado direito
    fflush(stdin);
    gets(fornecedores[TL].NomeForn);
    conioPrintf("menu_right", "branco", 2, "Cidade: ");
    fflush(stdin);
    gets(fornecedores[TL].Cidade);
    TL++;
    conioPrintf("menu_rigth", "verde", 0, "Fornecedor cod.%d, %s cadastrado!", fornecedores[TL - 1].CodForn, fornecedores[TL - 1].NomeForn);
    getch();
}

void CadastraProd(Produtos produtos[TF], Fornecedores fornecedores[TF], int &TL_Produtos, int &TL_Fornecedores)
{
    int *ptr_codigo;
    int AuxCod, pos, helper, codigo;
    char arg;
    conioPrintf("topo", "verde", 0, "Cadastro de Produtos!");
    conioPrintf("menu_right", "branco", 0, "Codigo: ");
    fflush(stdin);
    scanf("%d", &AuxCod);
    while (TL_Produtos < TF && AuxCod > 0)
    {
        pos = BusProdCod(produtos, TL_Produtos, AuxCod);
        if (pos == -1)
        {
            produtos[TL_Produtos].CodProd = AuxCod;
            conioPrintf("menu_right", "branco", 1, "Descricao: ");
            fflush(stdin);
            gets(produtos[TL_Produtos].Desc);

            conioPrintf("menu_right", "branco", 2, "Estoque: ");
            fflush(stdin);
            scanf("%d", &produtos[TL_Produtos].Estoque);

            conioPrintf("menu_right", "branco", 3, "Pre√ßo R$: ");
            fflush(stdin);
            scanf("%f", &produtos[TL_Produtos].Preco);

            conioPrintf("menu_right", "branco", 4, "Data de Validade[dd mm aaaa]: ");
            fflush(stdin);
            scanf("%d %d %d", &produtos[TL_Produtos].DtValidade.Dia, &produtos[TL_Produtos].DtValidade.Mes, &produtos[TL_Produtos].DtValidade.Ano);

            conioPrintf("menu_right", "branco", 5, "Busca fornecedor, cod: ");
            fflush(stdin);
            scanf("%d", &helper);
            ptr_codigo = &helper;

            codigo = ConsultaFornecedor(fornecedores, TL_Fornecedores, helper);

            if (codigo == -1)
            {
                conioPrintf("menu_right", "branco", 6, "Fornecedor nao encontrado. Cadastrar fornecedor? S/N");
                arg = toupper(getch());

                if (arg == 'S')
                {
                    clearElement("rightside");
                    CadastraFornecedor(fornecedores, TL_Fornecedores, ptr_codigo);
                    produtos[TL_Produtos].CodForn = codigo;
                    TL_Produtos++;
                    conioPrintf("alerta", "verde", 0, "Fornecedor Cadastrado!");
                    getch();
                }
                else
                {
                    conioPrintf("alerta", "vermelho", 0, "Produto n cadastrado: Fornecedor nao encontrado!");
                    getch();
                }
            }
            else
            {
                clearElement("rightside");
                conioPrintf("menu_right", "branco", 0, "Fornecedor: %d", codigo);
                produtos[TL_Produtos].CodForn = codigo;
                TL_Produtos++;
                conioPrintf("alerta", "verde", 0, "Produto %s cadastrado!", produtos[TL_Produtos - 1].Desc);
                getch();
            }
        }
        else
        {
            conioPrintf("alerta", "vermelho", 0, "Codigo do Produto ja cadastrado!");
            getch();
        }

        clearElement("rightside");
        conioPrintf("menu_left", "branco", 0, "Codigo: ");
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
    conioPrintf("topo", "vermelho_claro", 0, "CPF do Cliente a ser Deletado: ");
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
            conioPrintf("swticher", "verde", 0, "Cliente removido com sucesso!");
            getch();
        }
        else
        {
            conioPrintf("switcher", "vermelho", 0, "Erro ao remover cliente!");
            getch();
        }
    }
}
void ExcluirProd(Produtos produtos[], int &TL)
{
    int i, Aux, ponto;
    conioPrintf("topo", "verde_claro", 0, "Exclusao de produtos!");
    conioPrintf("menu_right", "branco", 0, "Digite o cod. do produto a excluir: ");//n ta do lado direito
    scanf("%d", &Aux);
    while (Aux > 0)
    {
        ponto = BusProdCod(produtos, TL, Aux);
        if (ponto != -1)
        {
            for (i = ponto; i < TL - 1; i++)
                produtos[i] = produtos[i + 1];
            TL--;
            conioPrintf("alerta", "verde", 0, "Produto excluido com sucesso!");
        }
        else
        {
            conioPrintf("alerta", "vermelho", 0, "Produto nao encontrado!");
        }
        conioPrintf("menu_right", "branco", 0, "Digite o cod. do produto a excluir: ");
        scanf("%d", &Aux);
    }
}

void AlterarProdCadastrado(Produtos produtos[], int TL)
{
    TpData aux;
    char resp;
    int Aux, ponto;
    float Aux_Preco;

    conioPrintf("topo", "vermelho", 0, "Alterar produto ja cadastrado!");
    conioPrintf("menu_right", "branco", 0, "Insira o codigo do produto: ");
    fflush(stdin);
    scanf("%d", &Aux);
    while (Aux > 0)
    {
        ponto = BusProdCod(produtos, TL, Aux);
        if (ponto == -1)
        {
            printf("switcher", "vermelho", 0, "Erro ao procurar pelo Codigo!");
            getch();
        }
        else
        {
            conioPrintf("menu_right", "branco", 1, "Qual elemento alterar do %s?", produtos[ponto].Desc);
            conioPrintf("menu_right", "branco", 2, "[A] - Estoque");
            conioPrintf("menu_right", "branco", 3, "[B] - Preco");
            conioPrintf("menu_right", "branco", 4, "[C] - Validade");

            fflush(stdin);
            resp = toupper(getch());

            switch (resp)
            {
            case 'A':
                conioPrintf("menu_right", "branco", 5, "Qual a nova quantidade do %s? ", produtos[ponto].Desc);
                while (scanf(" %d", &Aux) <= 0)
                    conioPrintf("alerta", "vermelho", 0, "Valor invalido");
                produtos[ponto].Estoque = Aux;
                break;

            case 'B':
                conioPrintf("menu_right", "branco", 5, "Qual o novo preco do %s? R$ ", produtos[ponto].Desc);
                while (scanf(" %f", &Aux_Preco) < 0)
                    conioPrintf("alerta", "vermelho", 0, "Preco invalido");
                produtos[ponto].Preco = Aux_Preco;
                break;

            case 'C':
                conioPrintf("menu_right", "branco", 0, "Qual a nova validade do %s? ", produtos[ponto].Desc);
                scanf("%d %d %d", &aux.Dia, &aux.Mes, &aux.Ano);
                if (aux.Dia > 0 && aux.Mes > 0 && aux.Ano > 0)
                {
                    produtos[ponto].DtValidade = aux;
                }
                else
                    conioPrintf("alerta", "vermelho", 0, "Data invalida!");
                break;
            }
        }

        conioPrintf("menu_right", "branco", 0, "Insira o codigo do produto: ");
        fflush(stdin);
        scanf("%d", &Aux);
    }
}

void ConsultaProd(Produtos produtos[], int TL)
{
    int ponto, i;
    conioPrintf("topo", "rosa", 0, "Consulta de Produto!");
    conioPrintf("menu_right", "branco", 0, "Cod. a ser consultado: ");//n ta ficando aqui tbm pro lado direito
    scanf("%d", &ponto);

    i = BusProdCod(produtos, TL, ponto);

    if (i >= 0)
    {
        conioPrintf("menu_right", "branco", 1, "Codigo: %d", produtos[i].CodProd);
        puts(produtos[i].Desc);
        conioPrintf("menu_right", "branco", 2, "Quantidade em estoque:%d\n", produtos[i].Estoque);
        conioPrintf("menu_right", "branco", 3, "%d %d %d", produtos[i].DtValidade.Dia, produtos[i].DtValidade.Mes, produtos[i].DtValidade.Ano);
    }
    else
        conioPrintf("alerta", "vermelho", 0, "Produto nao encontrado!");
    //limpar o final aqui, do lado direito
}
void produtosPercent(Produtos index_produtos[], Fornecedores index_fornecedores[], int TL_produtos, int TL_fornecedores)
{
    int valor, cod, pos;
    char resp;

    conioPrintf("topo", "vermelho_claro", 0, "Percentual  de produtos!");

    do
    {
        clearElement("rightside");
        conioPrintf("menu_right", "branco", 0, "[A] Add - [B] Subtrair");
        fflush(stdin);
        resp = toupper(getch());
    } while (resp != 'A' || resp != 'B');
    conioPrintf("menu_right", "branco", 1, "Valor em porc: ");
    fflush(stdin);
    scanf("%d", &valor);

    printf("menu_right", "branco", 2, "Cod do fornecedor para %s: ", resp == 'A' ? "adicionar" : "subtrair");
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
            conioPrintf("switcher", "verde", 0, "Produtos atualizados!");
            getch();
            break;
        case 'B':
            for (int i = 0; i < TL_produtos; i++)
                if (index_produtos[i].CodForn == index_fornecedores[pos].CodForn)
                    index_produtos[i].Preco -= (index_produtos[i].Preco * (valor / 100));
            conioPrintf("switcher", "verde", 0, "Produtos atualizados!");
            getch();
            break;
        }
    }
    else
        conioPrintf("switcher", "vermelho", 0, "Erro! Fornecedor nao cadastrado!");
}

void EditaClientes(Clientes clientes[], int TL)
{
    int opcao, pos;
    char cpf[11], opc;

    conioPrintf("topo", "ciano", 0, "Editar Clientes!");

    if (TL > 0)
    {
        clearElement("rightside");
        conioPrintf("menu_right", "branco", 0, "CPF a ser editado: ");
        fflush(stdin);
        gets(cpf);

        if (validarCPF(cpf) == 1)
        {
            pos = getPosClientes(clientes, TL, cpf);
            conioPrintf("menu_right", "branco", 1, "%s %s\n", clientes[pos].CPF, clientes[pos].NomeCli);
            conioPrintf("menu_right", "branco", 2, "Mudar: ");
            conioPrintf("menu_right", "branco", 3, "A - Nome");
            conioPrintf("menu_right", "branco", 4, "B - Zerar Compras");

            opc = toupper(getch());

            switch (opc)
            {
            case 'A':
                conioPrintf("menu_right", "branco", 5, "Novo nome do cliente: ");
                fflush(stdin);
                gets(clientes[pos].NomeCli);
                break;
            case 'B':
                clientes[pos].QtdeCompras = 0;
                clientes[pos].QtdeCompras = 0;
                conioPrintf("menu_right", "branco", 5, "Compras zeradas!");
                // buscar compras no cpf e deletar do index_vendas;
                break;
            }
        }
        else
            conioPrintf("switcher", "vermelho", 0, "CPF nao encontrado!");
    }
    else
        conioPrintf("switcher", "amarelo", 0, "Lista vazia!");
    getch();
}

void ConsultaFornecedor(Fornecedores fornecedores[], int TL)
{
    int i, consulta;
    conioPrintf("topo", "verde_claro", 0, "Consulta de Fornecedores!");
    conioPrintf("menu_right", "branco", 0, "Busca fornecedor pelo codigo: ");
    fflush(stdin);
    scanf("%d", &consulta);

    if (consulta >= 0)
    {
        i = BusFornCod(fornecedores, TL, consulta);
        if (i >= 0)
        {
            clearElement("rightside");
            conioPrintf("menu_right", "branco", 0, "%d", fornecedores[i].CodForn);
            conioPrintf("menu_right", "branco", 0, "%s", fornecedores[i].NomeForn);
            conioPrintf("menu_right", "branco", 0, "%s", fornecedores[i].Cidade);
            printf("%d\n%s\n%s", fornecedores[i].CodForn, fornecedores[i].NomeForn, fornecedores[i].Cidade);
        }
        else
            conioPrintf("switcher", "vermelho", 0, "Fornecedor nao encontrado!");
    }
    else
        conioPrintf("switcher", "vermelho", 0, "Apenas cod's positivos!");
    clearElement("rightside");//excl lado direito
    getch();
}

void ExcluirFornecedor(Fornecedores fornecedores[], int &TL)
{
    int i;
    char a;

    conioPrintf("topo", "ciano", 0, "Busca fornecedor pelo codigo: ");
    conioPrintf("menu_right", "branco", 0, "Busca fornecedor pelo codigo: ");

    fflush(stdin);
    scanf("%d", &i);

    i = BusFornCod(fornecedores, TL, i);

    if (i >= 0)
    {
        printf("menu_right", "branco", 1, "Deletar %s? S/N", fornecedores[i].NomeForn);
        fflush(stdin);
        a = toupper(getch());

        if (a == 'S')
        {
            for (; i < TL - 1; i++)
            {
                fornecedores[i] = fornecedores[i + 1];
            }
            TL--;
            conioPrintf("switcher", "verde", 0, "Deletado com sucesso!");
            getch();
        }
    }
    else
        conioPrintf("switcher", "vermelho", 0, "Fornecedor nao encontrado! ");
    getch();
}

//funciona como <string.h> strcmp()
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

int novaVenda(Clientes rootClientes[], Fornecedores rootFornecedores[], Produtos rootProdutos[], Vendas rootVendas[], Vendas_Produtos rootVendasProdutos[], int TLclientes, int TLfornecedores, int TLprodutos, int &TLvendas, int &TLvendasprod)
{
    int i, pos_cliente, cod_aux, var, valor_variavel = 0;
    char cpf[11], datahelper;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    TpData construtor_data, input;
    bool pass;
    conioPrintf("topo", "rosa", 0, "Nova Venda!!");
    conioPrintf("menu_right", "branco", 0, "CPF do Cliente: ");
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
                clearElement("rightside");
                conioPrintf("menu_right", "branco", 0, "Cliente: %s", rootClientes[pos_cliente].NomeCli);
                conioPrintf("menu_right", "branco", 1, "[A] - Usar a data atual para a venda");
                conioPrintf("menu_right", "branco", 2, "[B] - Data personalizada;");
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
                        printf("switcher", "vermelho", 0, "Obrigatorio inserir data valida!");
                        getch();
                    }
                } while (pass == false);

                do
                {
                    int prod_pos;
                    clearElement("rightside");
                    conioPrintf("menu_right", "branco", 0, "Cod do %d prod. a ser adicionado: ", TLvendasprod);
                    fflush(stdin);
                    scanf("%d", &cod_aux);

                    if (cod_aux > 0)
                    {
                        prod_pos = BusProdCod(rootProdutos, TLprodutos, cod_aux);
                        if (prod_pos >= 0)
                        {
                            pass = false;
                            if (comparaData(rootProdutos[prod_pos].DtValidade.Ano, rootProdutos[prod_pos].DtValidade.Mes, rootProdutos[prod_pos].DtValidade.Dia, construtor_data.Ano, construtor_data.Mes, construtor_data.Dia) >= 0)
                            {
                                conioPrintf("menu_right", "branco", 1, "Produto: %s\nQtde em estoque:%d\n", rootProdutos[prod_pos].Desc, rootProdutos[prod_pos].Estoque);
                                do
                                {
                                    conioPrintf("menu_right", "branco", 2, "Quantidade vendida: ");
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
                                    conioPrintf("switcher", "verde", 0, "Item %s adicionado a venda de cod. %d\n", rootProdutos[prod_pos].Desc, TLvendas + 1);
                                    getch();
                                }
                                else
                                {
                                    conioPrintf("switcher", "vermelho", 0, "Qtde. maior que estoque total!");
                                    getch();
                                }
                            }
                            else
                            {
                                conioPrintf("switcher", "amarelo", 0, "Validade do produto nao bate com a venda");
                                getch();
                            }
                        }
                        else
                        {
                            conioPrintf("switcher", "vermelho", 0, "Produto n. encontrado, tente novamente");
                            getch();
                        }
                    }
                    clearElement("rightside");
                    conioPrintf("menu_right", "branco", 0, "Cod do %d prod. a ser adicionado: ", TLvendasprod);
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

                conioPrintf("alerta", "verde", 0, "Venda do cliente %s registrada com sucesso", rootClientes[pos_cliente].NomeCli);
                getch();
            }
            else
            {
                conioPrintf("swticher", "vermelho", 0, "Cliente nao encontrado!");
                getch();
            }
        }
        else
        {
            conioPrintf("alerta", "vermelho", 0, "Digite um CPF valido!");
            getch();
        }

        clearElement("rightside");
        conioPrintf("menu_right", "branco", 0, "(0 cancela) CPF do Cliente: ");
        fflush(stdin);
        gets(cpf);
    }
}
void ExcluirVenda(Vendas rootVendas[], Vendas_Produtos rootVendasProdutos[], int &TLvendas, int &TLvendasprod) {
    int codVenda, i, j;
    conioPrintf("menu_right", "branco", 0, "Digite o cÛdigo da venda a ser excluÌda (0 para cancelar): ");
    fflush(stdin);
    scanf("%d", &codVenda);

    if (codVenda > 0) {
        for (i = 0; i<TLvendas; ++i) {
            if (rootVendas[i].CodVenda == codVenda) {
                for (j=0; j<TLvendasprod; ++j) {
                    if (rootVendasProdutos[j].CodVenda == codVenda) {
                        for(int k = j; k < TLvendasprod - 1; ++k) {
                            rootVendasProdutos[k] = rootVendasProdutos[k + 1];
                            //remover o restante
                        }
                        TLvendasprod--;
                        j--;
                    }
                }
                for(int k=i;k<TLvendas - 1;++k) {
                    rootVendas[k]=rootVendas[k+1];
                }
                TLvendas--;
                conioPrintf("alerta", "verde", 0, "Venda de cÛdigo %d excluÌda com sucesso", codVenda);
                getch();
                return;
            }
        }
        conioPrintf("alerta", "vermelho", 0, "Venda de cÛdigo %d n„o encontrada!", codVenda);
    } else
        conioPrintf("alerta", "ciano_claro", 0, "OperaÁ„o de exclus„o cancelada.");
    getch();
}

void AlterarVenda(Vendas rootVendas[], Vendas_Produtos rootVendasProdutos[], Produtos rootProdutos[], int TLvendas, int TLvendasprod, int TLprodutos)
{
    int codVenda, novoEstoque, novoQuantidade;
    char resp;
    int pontoVenda, pontoProd;

    conioPrintf("topo", "verde", 0, "Alterar Venda Existente!");
    conioPrintf("menu_right", "branco", 0, "Insira o cÛdigo da venda: ");
    scanf("%d", &codVenda);

    pontoVenda = BuscaVendaPorCodigo(rootVendas, TLvendas, codVenda);

    if (pontoVenda != -1)
    {
        do
        {
            conioPrintf("menu_right", "branco", 1, "O que deseja alterar na venda?");
            conioPrintf("menu_right", "branco", 2, "[A] - Adicionar Produto");
            conioPrintf("menu_right", "branco", 3, "[B] - Remover Produto");
            conioPrintf("menu_right", "branco", 4, "[C] - Modificar Produto (Estoque/Quantidade)");
            conioPrintf("menu_right", "branco", 5, "[D] - Sair");
            fflush(stdin);
            resp = toupper(getch());

            switch (resp)
            {
            case 'A':
                // Adicionar Produto ‡ Venda
                
                break;

            case 'B':
                // Remover Produto da Venda
                // Colocar o BuscaProdutoPorCodigo pra encontrar o produto na lista de vendas aqui
                break;

            case 'C':
                // Modificar Produto (Estoque/Quantidade) na Venda
                conioPrintf("menu_right", "branco", 0, "Insira o cÛdigo do produto: ");
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
                    conioPrintf("alerta", "vermelho", 0, "Produto n„o encontrado!");
                    getch();
                }
                break;

            case 'D':
                // Sair da funÁ„o
                break;

            default:
                conioPrintf("alerta", "vermelho", 0, "OpÁ„o inv·lida!");
                getch();
                break;
            }
        } while (resp != 'D');
    }
    else
    {
        conioPrintf("alerta", "vermelho", 0, "Venda n„o encontrada!");
        getch();
    }
}

void InsereElementos();

void Menu(Fornecedores index_fornecedores[TF], Produtos index_produtos[TF], Clientes index_clientes[TF], Vendas index_vendas[TF], Vendas_Produtos index_vendasprod[TF])
{
    int TL_fornecedores = 0, TL_produtos = 0, TL_clientes = 0, TL_vendas = 0, TL_cupons = 0, op;
    char opc, opc_sub;

    Formulario();
    conioPrintf("topo", "verde", 0, "%s %d", "Vendas:", TL_vendas);
    conioPrintf("switcher", "verde", 0, "Selecione um item:");
    conioPrintf("menu_left", "branco", 0, "[A] - CLIENTES");
    conioPrintf("menu_left", "branco", 1, "[B] - FORNECEDORES");
    conioPrintf("menu_left", "branco", 2, "[C] - PRODUTOS");
    conioPrintf("menu_left", "branco", 3, "[D] - VENDAS");
    conioPrintf("menu_left", "branco", 4, "ESC - Sair");
    gotoxy(4, 14);

    opc = toupper(getche());

    while (opc != 27)
    {

        switch (opc)
        {
        case 'A':
            do
            {
                conioPrintf("topo", "rosa", 0, "Controle de Clientes!");
                conioPrintf("switcher", "rosa", 0, "Selecione uma operacao em clientes");
                conioPrintf("menu_left", "branco", 0, "[A] - CADASTRO");
                conioPrintf("menu_left", "branco", 1, "[B] - CONSULTA");
                conioPrintf("menu_left", "branco", 2, "[C] - EXCLUSAO");
                conioPrintf("menu_left", "branco", 3, "[D] - ALTERACAO");
                gotoxy(4, 13);
                opc = toupper(getche());

                switch (opc)
                {
                case 'A':
                    if (Compara(TL_clientes, TF) == true)
                        CadastraCliente(index_clientes, TL_clientes);
                    else
                    {
                        conioPrintf("switcher", "vermelho", 0, "Erro: dbCheio");
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
                        conioPrintf("switcher", "vermelho", 0, "Erro: Vetor vazio");
                        gotoxy(41, 23);
                        getch();
                    }
                    break;
                case 'D':
                    EditaClientes(index_clientes, TL_clientes);
                    break;
                case 27:
                    break;
                default:
                    conioPrintf("alerta", "vermelho", 0, "##INEXISTENTE!## Selecione novamente");
                    fflush(stdin);
                    getch();
                }
            } while (opc != 27);
            break;

        case 'B':
            do
            {
                conioPrintf("topo", "marrom", 0, "Controle de Fornecedores!");
                conioPrintf("switcher", "amarelo", 0, "Selecione uma opcao em fornecedores");
                conioPrintf("menu_left", "branco", 0, "[A] - CADASTRO");
                conioPrintf("menu_left", "branco", 1, "[B] - CONSULTA");
                conioPrintf("menu_left", "branco", 2, "[C] - EXCLUSAO");
                conioPrintf("menu_left", "branco", 3, "[D] - ALTERACAO");
                opc = toupper(getch());

                switch (opc)
                {
                case 'A':
                    if (Compara(TL_fornecedores, TF) == true)
                        CadastraFornecedor(index_fornecedores, TL_fornecedores, NULL);
                    else
                        conioPrintf("switcher", "vermelho", 0, "Erro: dbCheio");
                    fflush(stdin);
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
                        conioPrintf("switcher", "vermelho", 0, "Erro: Vetor vazio");
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
                    conioPrintf("alerta", "vermelho", 0, "##INEXISTENTE!## Selecione novamente");
                    getch();
                }
            } while (opc != 27);
            break;

        case 'C':
            do
            {
                conioPrintf("topo", "cinza_claro", 0, "Controle de Produtos!");
                conioPrintf("switcher", "marrom", 0, "Selecione uma operacao em produtos");
                conioPrintf("menu_left", "branco", 0, "[A] - CADASTRO");
                conioPrintf("menu_left", "branco", 1, "[B] - CONSULTA");
                conioPrintf("menu_left", "branco", 2, "[C] - EXCLUSAO");
                conioPrintf("menu_left", "branco", 3, "[D] - ALTERACAO");
                conioPrintf("menu_left", "branco", 4, "[E] - DESC PERCENTUAL");
                fflush(stdin);
                opc = toupper(getch());
                //em Produtos nd esta do lado direito :(

                switch (opc)
                {
                case 'A':
                    if (Compara(TL_produtos, TF) == true)
                        CadastraProd(index_produtos, index_fornecedores, TL_produtos, TL_fornecedores);
                    else
                    {
                        conioPrintf("switcher", "vermelho", 0, "Erro: dbCheio");
                        gotoxy(41, 23);
                        getch();
                    }
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
                case 27:
                    break;
                default:
                    conioPrintf("switcher", "vermelho", 0, "##INEXISTENTE!## Selecione novamente");
                    getch();
                    break;
                }
            } while (opc != 27);
            break;

        case 'D':
            do
            {
                conioPrintf("topo", "ciano", 0, "Controle de Vendas!");
                conioPrintf("switcher", "ciano", 0, "Selecione uma operacao em Vendas!");
                conioPrintf("menu_left", "branco", 0, "[A] - NOVA VENDA");
                conioPrintf("menu_left", "branco", 1, "[B] - EXCLUSAO");
                conioPrintf("menu_left", "branco", 2, "[C] - ALTERACAO");
                conioPrintf("menu_left", "branco", 3, "[D] - RELATORIO TOTAL");
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
                        conioPrintf("switcher", "vermelho", 0, "Mem. Cheia!");
                        gotoxy(41, 23);
                    }
                    fflush(stdin);
                    getch();
                    break;
                case 'B':
                	ExcluirVenda(index_produtos, TL_produtos);
                    break;
                case 'C':
                	AlterarVenda(index_produtos, TL_produtos);
                    break;
                case 'D':
                	RelatorioTotal();
                    break;
                case 27:
                    break;
                default:
                    conioPrintf("switcher", "vermelho", 0, "##INEXISTENTE!## Selecione novamente");
                    getch();
                    break;
                }
            } while (opc != 27);
            break;
        }
        Formulario();
        conioPrintf("topo", "verde", 0, "Vendas realizadas: %d", TL_vendas);
        conioPrintf("switcher", "verde", 0, "Selecione um item:");
        conioPrintf("menu_left", "branco", 0, "[A] - CLIENTES");
        conioPrintf("menu_left", "branco", 1, "[B] - FORNECEDORES");
        conioPrintf("menu_left", "branco", 2, "[C] - PRODUTOS");
        conioPrintf("menu_left", "branco", 3, "[D] - VENDAS");
        conioPrintf("menu_left", "branco", 4, "ESC - Sair");
        opc = toupper(getche());
    }
}

int main(void)
{
    Fornecedores index_fornecedores[TF];
    Produtos index_produtos[TF];
    Clientes index_clientes[TF];
    Vendas index_vendas[TF];
    Vendas_Produtos vendas[TF];

    _setcursortype(0);

    Menu(index_fornecedores, index_produtos, index_clientes, index_vendas, vendas);

    return 0;
}
