// commit history available in
// github.com/leeozaka/controle-atp2;

// Leonardo Neves - 262310406

// issues em README.md

#include "includes\\conioprintf.h"
#include "includes\\fcontrol.h"
#include "includes\\draw.h"

int validarCPF(char cpf[11])
{
    int i, digito1 = 0, digito2 = 0, helper, soma;
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
        for (i = 0; i < strlen(cpf) - 2; i++)
            digito1 += (cpf[i] - '0') * (i + 1);

        digito1 %= 11;
        if (digito1 == 10 || digito1 == 11)
            digito1 = 0;
        if ((cpf[8] - '0') == digito1)
            return 0;

        else
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

// funciona como <string.h> strcmp()
double comparaData(int ano1, int mes1, int dia1, int ano2, int mes2, int dia2)
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

int CadastraCliente(FILE *reg_clientes)
{
    if ((reg_clientes = fopen("clientes\\clientes.dat", "rb+")) == NULL)
        reg_clientes = fopen("clientes\\clientes.dat", "ab+");

    Clientes cliente;
    char CPF[11];
    int i, validar = 0, pos = 0;

    conioPrintf(TOPO, ROSA_CLARO, 0, "Cadastro de Clientes!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Digite o CPF: ");

    fflush(stdin);
    fgets(CPF, 12, stdin);

    if (validarCPF(CPF))
    {
        if (find_clientes(reg_clientes, cliente, CPF, pos, BYTE))
        {
            conioPrintf(ALERTA, VERMELHO, 0, "CPF ja cadastrado");
            return getchclose(reg_clientes);
        }
        strcpy(cliente.CPF, CPF);

        conioPrintf(MENU_RIGHT, BRANCO, 1, "Digite o nome: ");
        fflush(stdin);
        scanf("%s", &cliente.NomeCli);

        cliente.QtdeCompras = 0;
        cliente.ValorTotComprado = 0;
        cliente.flag = true;

        fseek(reg_clientes, 0, SEEK_END);
        fwrite(&cliente, sizeof(Clientes), 1, reg_clientes);

        conioPrintf(ALERTA, VERDE, 0, "Cliente cadastrado com sucesso!");
        return getchclose(reg_clientes);
    }
    else
    {
        conioPrintf(ALERTA, VERMELHO, 0, "O numero de identificacao informado nao esta correto.");
        return getchclose(reg_clientes);
    }
}
int ConsultaClientes(FILE *reg_clientes)
{
    reg_clientes = fopen("clientes\\clientes.dat", "rb");
    if (reg_clientes == NULL || feof(reg_clientes))
        return 1;

    char cpf[12];
    int pos = 0;

    conioPrintf(SWITCHER, AMARELO, 0, "CPF a procurar: ");
    fflush(stdin);
    fgets(cpf, 12, stdin);

    Clientes cliente;

    if (find_clientes(reg_clientes, cliente, cpf, pos, BYTE))
    {
        fseek(reg_clientes, pos, SEEK_SET);
        conioPrintf(MENU_RIGHT, BRANCO, 1, "Cliente: %s", cliente.NomeCli);
        conioPrintf(MENU_RIGHT, BRANCO, 2, "%s", cliente.CPF);
        conioPrintf(MENU_RIGHT, BRANCO, 3, "%d compras %f total", cliente.QtdeCompras, cliente.ValorTotComprado);
    }
    else
        conioPrintf(ALERTA, VERMELHO, 0, "Cliente not found");

    return getchclose(reg_clientes);
}

int EditaClientes(FILE *reg_clientes)
{
    reg_clientes = fopen("clientes\\clientes.dat", "rb+");
    if (reg_clientes == NULL)
        return 1;

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
        if (!find_clientes(reg_clientes, cliente, cpf, pos, BYTE))
        {
            conioPrintf(SWITCHER, VERMELHO, 0, "Cliente Inexistente");
            getch();
            fclose(reg_clientes);
            return 0;
        }

        conioPrintf(MENU_RIGHT, BRANCO, 2, "Mudar: ");
        conioPrintf(MENU_RIGHT, BRANCO, 3, "A - Nome");
        conioPrintf(MENU_RIGHT, BRANCO, 4, "B - Zerar Compras");

        opc = toupper(getch());

        switch (toupper(getch()))
        {
        case 'A':
            fseek(reg_clientes, pos, SEEK_SET);
            fflush(stdin);
            conioPrintf(SWITCHER, BRANCO, 0, "Nome: ");
            fgets(cliente.NomeCli, BUFFER, stdin);
            fwrite(&cliente, sizeof(Clientes), 1, reg_clientes);
            conioPrintf(SWITCHER, VERDE, 0, "Nome atualizado!!");
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
        conioPrintf(SWITCHER, VERMELHO, 0, "CPF invalido!");
    return getchclose(reg_clientes);
}

int DeletaClientes(FILE *reg_clientes)
{
    if ((reg_clientes = fopen("clientes\\clientes.dat", "rb+")) == NULL || feof(reg_clientes))
        return 1;

    Clientes cliente;

    char cpf[12];
    int pos;

    conioPrintf(TOPO, AZUL, 0, "Deletar Cliente");
    conioPrintf(MENU_RIGHT, VERMELHO, 0, "Cpf a ser cancelado: ");
    fflush(stdin);
    fgets(cpf, 12, stdin);

    int documentsize = fsizer(reg_clientes, sizeof(Clientes), SET, LOGIC);

    if (find_clientes(reg_clientes, cliente, cpf, pos, BYTE))
    {
        conioPrintf(MENU_RIGHT, VERMELHO, 1, "Deletar %s?  S/N", cliente.NomeCli);
        if ((toupper(getch())) == 'S')
        {
            cliente.flag = false;
            fseek(reg_clientes, pos, SEEK_SET);
            fwrite(&cliente, sizeof(Clientes), 1, reg_clientes);
            fclose(reg_clientes);
            conioPrintf(ALERTA, VERDE, 0, "Cliente removido!");
            getch();
        }
    }
    else
    {
        conioPrintf(ALERTA, VERMELHO, 0, "Cliente nao encontrado!");
        return getchclose(reg_clientes);
    }
}

int RelatorioClientes(FILE *reg_clientes)
{
    if ((reg_clientes = fopen("clientes\\clientes.dat", "rb")) == NULL)
        return 1;

    int documentsize = fsizer(reg_clientes, sizeof(Clientes), SET, LOGIC);
    int quantidade = 0, run = 0;
    float valorcompratotal = 0, maiorcompra = 0;
    Clientes cliente, highscore;

    conioPrintf(TOPO, AMARELO, 0, "Relatorio de Clientes!");

    while (run < documentsize)
    {
        fread(&cliente, sizeof(Clientes), 1, reg_clientes);
        if (validarCPF(cliente.CPF) && cliente.flag)
        {
            quantidade++;
            valorcompratotal += cliente.ValorTotComprado;
            if (maiorcompra < cliente.ValorTotComprado)
            {
                maiorcompra = cliente.ValorTotComprado;
                highscore = cliente;
            }
        }
        run++;
    }
    if (quantidade > 0)
    {
        conioPrintf(SWITCHER, VERDE, 0, "Detalhes dos Clientes");
        conioPrintf(MENU_RIGHT, BRANCO, 0, "Quantidade de clientes cad.: %d", quantidade);
        conioPrintf(MENU_RIGHT, BRANCO, 1, "Valor total comprado: %f", valorcompratotal);
        if (maiorcompra > 0)
        {
            conioPrintf(MENU_RIGHT, VERDE, 3, "-Cliente que mais comprou!-");
            conioPrintf(MENU_RIGHT, BRANCO, 4, "Nome: %s", highscore.NomeCli);
            conioPrintf(MENU_RIGHT, BRANCO, 5, "CPF: %s", highscore.CPF);
            conioPrintf(MENU_RIGHT, BRANCO, 6, "Total de Compras: %f", highscore.ValorTotComprado);
            conioPrintf(MENU_RIGHT, BRANCO, 6, "Quantidade de compras: %f", highscore.QtdeCompras);
        }
    }
    else
        conioPrintf(SWITCHER, VERMELHO, 0, "Sem dados a serem exibidos");

    return getchclose(reg_clientes);
}

int ConsultaFornecedor(FILE *reg_fornecedores)
{
    if ((reg_fornecedores = fopen("fornecedores\\fornecedores.dat", "rb+")) == NULL)
        return 1;

    int i = 0, cod, pos;
    Fornecedores fornecedor;

    conioPrintf(TOPO, AZUL_CLARO, 0, "Consultar Fornecedor!");
    conioPrintf(MENU_RIGHT, VERDE, 0, "Codigo do Fornecedor a ser Consultado: ");
    fflush(stdin);
    scanf("%d", &cod);

    int fornecedores_size = fsizer(reg_fornecedores, sizeof(Fornecedores), SET, LOGIC);

    if (find_fornecedores(reg_fornecedores, fornecedor, cod, pos, BYTE))
    {
        conioPrintf(SWITCHER, AMARELO, 0, "Fornecedor Encontrado!");
        conioPrintf(MENU_RIGHT, BRANCO, 0, "Nome: %s", fornecedor.NomeForn);
        conioPrintf(MENU_RIGHT, BRANCO, 1, "Cidade: %s", fornecedor.Cidade);

        return getchclose(reg_fornecedores);
    }
    else
    {
        conioPrintf(ALERTA, ROSA, 0, "Cod. N Encontrado!");
        return getchclose(reg_fornecedores);
    }
}

int AlterarDadosFornecedor(FILE *reg_fornecedores)
{
    if ((reg_fornecedores = fopen("fornecedores\\fornecedores.dat", "rb+")) == NULL)
        return 1;

    int opcao, cod, pos, fornecedores_size;
    char opc;

    Fornecedores fornecedor;

    conioPrintf(TOPO, BRANCO, 0, "Alterar dados do Fornecedor!");

    if ((fornecedores_size = fsizer(reg_fornecedores, sizeof(Fornecedores), SET, LOGIC)))
    {
        conioPrintf(MENU_RIGHT, BRANCO, 0, "Cod. do fornecedor a ser editado: ");
        fflush(stdin);
        scanf("%d", &cod);

        if (find_fornecedores(reg_fornecedores, fornecedor, cod, pos, BYTE))
        {
            conioPrintf(MENU_RIGHT, BRANCO, 1, "Fornecedor %s", fornecedor.NomeForn);
            conioPrintf(MENU_RIGHT, BRANCO, 2, "Mudar:");
            conioPrintf(MENU_RIGHT, BRANCO, 3, "A - Nome");
            conioPrintf(MENU_RIGHT, BRANCO, 4, "B - Cidade");

            fflush(stdin);
            opc = toupper(getch());
            switch (opc)
            {
            case 'A':
                conioPrintf(MENU_RIGHT, VERDE, 5, "Digite o novo nome: ");
                fflush(stdin);
                gets(fornecedor.NomeForn);
                break;
            case 'B':
                conioPrintf(MENU_RIGHT, VERDE, 5, "Digite a nova cidade: ");
                fflush(stdin);
                gets(fornecedor.Cidade);
                // buscar compras no cpf e deletar do index_vendas?;
                break;
            default:
                conioPrintf(ALERTA, VERMELHO, 0, "Cond. errada!");
                fclose(reg_fornecedores);
                getch();
                return 1;
            }

            fseek(reg_fornecedores, pos, SEEK_SET);
            fwrite(&fornecedor, sizeof(Fornecedores), 1, reg_fornecedores);
            fclose(reg_fornecedores);

            conioPrintf(SWITCHER, VERDE, 0, "Fornecedor %s atualizado!", fornecedor.NomeForn);
            return getchclose(reg_fornecedores);
        }
        else
        {
            conioPrintf(ALERTA, VERMELHO, 0, "Cod. n encontrado!");
            return getchclose(reg_fornecedores);
        }
    }
    else
    {
        conioPrintf(ALERTA, VERMELHO, 0, "Arquivo vazio!");
        return getchclose(reg_fornecedores);
    }
}

int CadastraFornecedor(FILE *reg_fornecedores, int *cod)
{
    if ((reg_fornecedores = fopen("fornecedores\\fornecedores.dat", "rb+")) == NULL)
        reg_fornecedores = fopen("fornecedores\\fornecedores.dat", "ab+");

    int i = 0, codforn, busca;
    Fornecedores fornecedor;

    int fornecedores_size = fsizer(reg_fornecedores, sizeof(Fornecedores), SET, LOGIC);

    conioPrintf(TOPO, ROSA, 0, "Cadastro de Fornecedores!");
    if (cod == NULL)
    {
        conioPrintf(MENU_RIGHT, BRANCO, 0, "Digite o cod. do %do fornecedor: ", fornecedores_size + 1);
        scanf("%d", &codforn);
    }
    else
        codforn = *cod;

    codforn = abs(codforn);
    do
    {
        if (find_fornecedores(reg_fornecedores, fornecedor, codforn, busca, LOGIC) || codforn == 0)
        {
            conioPrintf(SWITCHER, VERMELHO, 0, "Cod. Ja existente!");

            conioPrintf(MENU_RIGHT, BRANCO, 1, "Digite o cod. do %do fornecedor: ", fornecedores_size + 1);
            scanf("%d", &codforn);
            codforn = abs(codforn);
        }
    } while (find_fornecedores(reg_fornecedores, fornecedor, codforn, busca, LOGIC));

    fornecedor.CodForn = codforn;
    conioPrintf(MENU_RIGHT, BRANCO, 1, "Nome: ");
    fflush(stdin);
    gets(fornecedor.NomeForn);
    conioPrintf(MENU_RIGHT, BRANCO, 2, "Cidade: ");
    fflush(stdin);
    gets(fornecedor.Cidade);
    fornecedor.flag = true;

    fseek(reg_fornecedores, 0, SEEK_END);
    fwrite(&fornecedor, sizeof(Fornecedores), 1, reg_fornecedores);

    conioPrintf(MENU_RIGHT, VERDE, 3, "Fornecedor cod.%d, %s cadastrado!", fornecedor.CodForn, fornecedor.NomeForn);

    return getchclose(reg_fornecedores);
}

int ExcluirFornecedor(FILE *reg_fornecedores, FILE *reg_produtos)
{
    if ((reg_fornecedores = fopen("fornecedores\\fornecedores.dat", "rb+")) == NULL)
        return 1;
    reg_produtos = fopen("produtos\\produtos.dat", "rb+");

    Fornecedores fornecedor;
    Produtos produto;

    int cod, pos;
    int fornecedores_size = fsizer(reg_fornecedores, sizeof(Fornecedores), SET, BYTE);

    conioPrintf(TOPO, CIANO, 0, "Excluir Fornecedor!!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Busca fornecedor pelo codigo: ");

    fflush(stdin);
    scanf("%d", &cod);

    if (find_fornecedores(reg_fornecedores, fornecedor, cod, pos, BYTE))
    {
        conioPrintf(MENU_RIGHT, VERMELHO, 1, "Deletar %s? S/N", fornecedor.NomeForn);
        fflush(stdin);
        if (toupper(getch()) == 'S')
        {
            fornecedor.flag = false;
            fseek(reg_fornecedores, pos, SEEK_SET);
            fwrite(&fornecedor, sizeof(Fornecedores), 1, reg_fornecedores);

            int produtos_size = fsizer(reg_produtos, sizeof(Produtos), SET, LOGIC);

            // busca e deleta logicamente produtos com o cod. do forn.
            for (int i = 0; i < produtos_size; i++)
            {
                if (find_produtos(reg_produtos, produto, cod, pos, BYTE))
                {
                    produto.flag = false;
                    fseek(reg_produtos, pos, SEEK_SET);
                    fwrite(&produto, sizeof(Produtos), 1, reg_produtos);
                }
            }

            conioPrintf(SWITCHER, VERDE, 0, "Fornecedor/produtos removido(s)!");
            fclose(reg_produtos);
            fclose(reg_fornecedores);
            getch();
        }
        else
        {
            fclose(reg_fornecedores);
            return 0;
        }
    }
    else
    {
        conioPrintf(SWITCHER, VERMELHO, 0, "Fornecedor nao encontrado! ");
        fclose(reg_fornecedores);
        getch();
        return 1;
    }
}

int RelatorioFornecedores(FILE *reg_fornecedores)
{
    if ((reg_fornecedores = fopen("fornecedores\\fornecedores.dat", "rb+")) == NULL)
        return 1;

    conioPrintf(TOPO, CIANO_CLARO, 0, "Relatorio de Fornecedores!");

    Fornecedores fornecedor;
    char cidades[20][300], cidade[20];
    int sizeof_cidades = 0;

    int documentsize = fsizer(reg_fornecedores, sizeof(Fornecedores), SET, LOGIC);

    int i = 0;
    while (i < documentsize)
    {
        fread(&fornecedor, sizeof(Fornecedores), 1, reg_fornecedores);
        strcpy(cidade, fornecedor.Cidade);

        int j = 0;
        bool found = false;
        while (!found && j < i)
            !strcmp(cidade, cidades[j]) ? found = true : j++;

        if (!found)
        {
            strcpy(cidades[sizeof_cidades], cidade);
            sizeof_cidades++;
        }
        i++;
    }

    conioPrintf(MENU_RIGHT, BRANCO, 0, "Fornecedores cadastrados: %d", documentsize);
    conioPrintf(MENU_RIGHT, BRANCO, 1, "Quantidade de cidades diferentes: %d", sizeof_cidades);

    return getchclose(reg_fornecedores);
}

void CadastraProd(FILE *reg_produtos, FILE *reg_fornecedores)
{
    if ((reg_produtos = fopen("produtos\\produtos.dat", "rb+")) == NULL)
        reg_produtos = fopen("produtos\\produtos.dat", "ab+");
    if ((reg_fornecedores = fopen("fornecedores\\fornecedores.dat", "rb+")) == NULL)
        reg_fornecedores = fopen("fornecedores\\fornecedores.dat", "ab+");

    Produtos produto;
    Fornecedores fornecedor;

    int AuxCod, pos, helper, codigo;
    char arg;

    int prodsize = fsizer(reg_produtos, sizeof(Produtos), SET, LOGIC);
    int fornsize = fsizer(reg_fornecedores, sizeof(Fornecedores), SET, LOGIC);

    conioPrintf(TOPO, VERDE, 0, "Cadastro de Produtos!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Codigo: ");

    fflush(stdin);
    scanf("%d", &AuxCod);

    while (AuxCod > 0)
    {
        if (!find_produtos(reg_produtos, produto, AuxCod, pos, LOGIC))
        {
            produto.CodProd = AuxCod;
            conioPrintf(MENU_RIGHT, BRANCO, 1, "Descricao: ");
            fflush(stdin);
            gets(produto.Desc);

            conioPrintf(MENU_RIGHT, BRANCO, 2, "Estoque: ");
            fflush(stdin);
            scanf("%d", &produto.Estoque);

            conioPrintf(MENU_RIGHT, BRANCO, 3, "Preco R$: ");
            fflush(stdin);
            scanf("%f", &produto.Preco);

            conioPrintf(MENU_RIGHT, BRANCO, 4, "Data de Validade[dd mm aaaa]: ");
            fflush(stdin);
            scanf("%d %d %d", &produto.DtValidade.Dia, &produto.DtValidade.Mes, &produto.DtValidade.Ano);

            conioPrintf(MENU_RIGHT, BRANCO, 5, "Busca fornecedor, cod: ");
            fflush(stdin);
            scanf(" %d", &helper);

            produto.flag = true;

            if (!find_fornecedores(reg_fornecedores, fornecedor, helper, codigo, LOGIC))
            {
                conioPrintf(MENU_RIGHT, BRANCO, 6, "Fornecedor nao encontrado. Cadastrar fornecedor? S/N");
                fflush(stdin);
                arg = toupper(getch());

                if (arg == 'S')
                {
                    clearElement(RIGHTSIDE);
                    fclose(reg_fornecedores);
                    CadastraFornecedor(reg_fornecedores, &helper);
                    produto.CodForn = helper;

                    fseek(reg_produtos, 0, SEEK_END);
                    fwrite(&produto, sizeof(Produtos), 1, reg_produtos);
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
                produto.CodForn = helper;
                fseek(reg_produtos, 0, SEEK_END);
                fwrite(&produto, sizeof(Produtos), 1, reg_produtos);

                conioPrintf(ALERTA, VERDE, 0, "Produto %s cadastrado!", produto.Desc);
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
    fclose(reg_fornecedores);
    fclose(reg_produtos);
}

int ExcluirProd(FILE *reg_produtos)
{
    if ((reg_produtos = fopen("produtos\\produtos.dat", "rb+")) == NULL)
        return 1;

    Produtos produto;

    int Aux, pos;
    int documentsize = fsizer(reg_produtos, sizeof(Produtos), SET, BYTE);

    conioPrintf(TOPO, VERDE_CLARO, 0, "Exclusao de produtos!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Digite o cod. do produto a excluir: ");
    scanf("%d", &Aux);

    if (find_produtos(reg_produtos, produto, Aux, pos, BYTE))
    {
        conioPrintf(SWITCHER, VERMELHO, 0, "Deletar produto %s? S/N", produto.Desc);
        fflush(stdin);
        if (toupper(getch()) == 'S')
        {
            produto.flag = false;
            fseek(reg_produtos, pos, SEEK_SET);
            fwrite(&produto, sizeof(Produtos), 1, reg_produtos);
            conioPrintf(ALERTA, VERDE, 0, "Produto excluido com sucesso!");
        }
    }
    else
        conioPrintf(ALERTA, VERMELHO, 0, "Produto nao encontrado!");

    return getchclose(reg_produtos);
    // fclose(reg_produtos);
    // getch();
    // return 0;
}

int ConsultaProd(FILE *reg_produtos)
{
    if ((reg_produtos = fopen("produtos\\produtos.dat", "rb+")) == NULL)
        return 1;

    int ponto, i;
    Produtos produto;

    conioPrintf(TOPO, ROSA, 0, "Consulta de Produto!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Cod. a ser consultado: ");
    scanf("%d", &ponto);

    int documentsize = fsizer(reg_produtos, sizeof(Produtos), SET, LOGIC);

    if (find_produtos(reg_produtos, produto, ponto, i, LOGIC))
    {
        fseek(reg_produtos, i * sizeof(Produtos), SEEK_SET);
        fread(&produto, sizeof(Produtos), 1, reg_produtos);

        conioPrintf(MENU_RIGHT, BRANCO, 1, "Codigo: %d", produto.CodProd);
        conioPrintf(MENU_RIGHT, BRANCO, 2, "Descricao: %s", produto.Desc);
        conioPrintf(MENU_RIGHT, BRANCO, 3, "Quantidade em estoque: %d", produto.Estoque);
        conioPrintf(MENU_RIGHT, BRANCO, 4, "Validade: %d/%d/%d", produto.DtValidade.Dia, produto.DtValidade.Mes, produto.DtValidade.Ano);
        conioPrintf(MENU_RIGHT, BRANCO, 5, "Cod. Fornecedor: %d", produto.CodForn);
        conioPrintf(MENU_RIGHT, BRANCO, 6, "Preco: %f", produto.Preco);
    }
    else
        conioPrintf(ALERTA, VERMELHO, 0, "Produto nao encontrado!");

    return getchclose(reg_produtos);
    // getch();
    // fclose(reg_produtos);
    // return 0;
}

int AlterarProdCadastrado(FILE *reg_produtos)
{
    if ((reg_produtos = fopen("produtos\\produtos.dat", "rb+")) == NULL)
        return 1;

    TpData aux;
    Produtos produto;

    char resp;
    int Aux, pos;
    float Aux_Preco;

    int documentsize = fsizer(reg_produtos, sizeof(Produtos), SET, BYTE);

    conioPrintf(TOPO, VERMELHO, 0, "Alterar produto ja cadastrado!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Insira o codigo do produto: ");
    fflush(stdin);
    scanf("%d", &Aux);

    while (Aux > 0)
    {
        if (!find_produtos(reg_produtos, produto, Aux, pos, BYTE))
        {
            conioPrintf(SWITCHER, VERMELHO, 0, "Erro ao procurar pelo Codigo!");
            getch();
        }
        else
        {
            fseek(reg_produtos, pos, SEEK_SET);
            fread(&produto, sizeof(Produtos), 1, reg_produtos);

            conioPrintf(MENU_RIGHT, BRANCO, 1, "Qual elemento alterar do %s?", produto.Desc);
            conioPrintf(MENU_RIGHT, BRANCO, 2, "[A] - Estoque");
            conioPrintf(MENU_RIGHT, BRANCO, 3, "[B] - Preco");
            conioPrintf(MENU_RIGHT, BRANCO, 4, "[C] - Validade");

            fflush(stdin);
            resp = toupper(getch());

            switch (resp)
            {
            case 'A':
                conioPrintf(MENU_RIGHT, BRANCO, 5, "Qual a nova quantidade do %s? ", produto.Desc);
                while ((scanf(" %d", &Aux)) <= 0)
                    conioPrintf(ALERTA, VERMELHO, 0, "Valor invalido");
                produto.Estoque = Aux;

                fseek(reg_produtos, pos, SEEK_SET);
                fwrite(&produto, sizeof(Produtos), 1, reg_produtos);
                conioPrintf(ALERTA, VERDE, 0, "Quantidade atualizada!");
                break;

            case 'B':
                conioPrintf(MENU_RIGHT, BRANCO, 5, "Qual o novo preco do %s? R$ ", produto.Desc);
                while (scanf(" %f", &Aux_Preco) < 0)
                    conioPrintf(ALERTA, VERMELHO, 0, "Preco invalido");
                produto.Preco = Aux_Preco;

                fseek(reg_produtos, pos, SEEK_SET);
                fwrite(&produto, sizeof(Produtos), 1, reg_produtos);
                conioPrintf(ALERTA, VERDE, 0, "Preco atualizado!");
                break;

            case 'C':
                conioPrintf(MENU_RIGHT, BRANCO, 0, "Qual a nova validade do %s? ", produto.Desc);
                scanf("%d %d %d", &aux.Dia, &aux.Mes, &aux.Ano);
                if (aux.Dia > 0 && aux.Mes > 0 && aux.Ano > 0)
                {
                    produto.DtValidade = aux;

                    fseek(reg_produtos, pos, SEEK_SET);
                    fwrite(&produto, sizeof(Produtos), 1, reg_produtos);
                    conioPrintf(ALERTA, VERDE, 0, "Validade Atualizada!");
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
    fclose(reg_produtos);
}

int produtosPercent(FILE *reg_produtos, FILE *reg_fornecedores)
{
    if ((reg_produtos = fopen("produtos\\produtos.dat", "rb+")) == NULL)
        return 1;
    if ((reg_fornecedores = fopen("fornecedores\\fornecedores.dat", "rb")) == NULL)
        return 1;

    int documentsize = fsizer(reg_produtos, sizeof(Produtos), SET, LOGIC);
    int fornsize = fsizer(reg_fornecedores, sizeof(Produtos), SET, LOGIC);

    int valor, cod, pos;
    char resp;

    Produtos produto;
    Fornecedores fornecedor;

    conioPrintf(TOPO, VERMELHO_CLARO, 0, "Percentual  de produtos!");

    clearElement(RIGHTSIDE);
    conioPrintf(MENU_RIGHT, BRANCO, 0, "[A] Add - [B] Subtrair");
    fflush(stdin);
    resp = toupper(getch());

    conioPrintf(MENU_RIGHT, BRANCO, 1, "Valor em porcentagem: ");
    fflush(stdin);
    scanf("%d", &valor);

    conioPrintf(MENU_RIGHT, BRANCO, 2, "Cod do fornecedor para %s: ", resp == 'A' ? "adicionar" : "subtrair");
    fflush(stdin);
    scanf("%d", &cod);

    if (find_fornecedores(reg_fornecedores, fornecedor, cod, pos, LOGIC))
    {
        switch (resp)
        {
        case 'A':
            for (int i = 0; i < documentsize; i++)
            {
                fread(&produto, sizeof(Produtos), 1, reg_produtos);
                if (produto.CodForn == cod)
                {
                    fseek(reg_produtos, 0 - sizeof(Produtos), SEEK_CUR);
                    produto.Preco += produto.Preco * valor / 100;
                    fwrite(&produto, sizeof(Produtos), 1, reg_produtos);
                }
            }
            conioPrintf(SWITCHER, VERDE, 0, "Produtos atualizados!");
            getch();
            break;

        case 'B':
            for (int i = 0; i < documentsize; i++)
            {
                fread(&produto, sizeof(Produtos), 1, reg_produtos);
                if (produto.CodForn == cod)
                {
                    fseek(reg_produtos, 0 - sizeof(Produtos), SEEK_CUR);
                    produto.Preco -= produto.Preco * valor / 100;
                    fwrite(&produto, sizeof(Produtos), 1, reg_produtos);
                }
            }
            conioPrintf(SWITCHER, VERDE, 0, "Produtos atualizados!");
            getch();
            break;
        }
    }
    else
        conioPrintf(SWITCHER, VERMELHO, 0, "Erro! Fornecedor nao cadastrado!");

    fclose(reg_fornecedores);
    fclose(reg_produtos);
    return 0;
}

int RelatorioProdutos(FILE *reg_produtos)
{
    if ((reg_produtos = fopen("produtos\\produtos.dat", "rb")) == NULL)
        return 1;

    conioPrintf(TOPO, ROSA_CLARO, 0, "Relatorio de Produtos!");
    Produtos produto, highscore = {0};

    // Relatorio de produtos:
    // Quantidade total de produtos cadastrados, estoque total, preço total da mercadoria

    int total_produtos = 0, total_estoque = 0, produtos_size = 0;
    float media_precos = 0;
    double total_preco = 0;

    if ((produtos_size = fsizer(reg_produtos, sizeof(Produtos), SET, LOGIC)) == 0)
    {
        fclose(reg_produtos);
        return 0;
    }

    for (int i = 0; i < produtos_size; i++)
    {
        fread(&produto, sizeof(Produtos), 1, reg_produtos);
        if (produto.flag)
        {
            total_produtos++;
            total_estoque += produto.Estoque;
            total_preco += produto.Preco;
            media_precos = (media_precos + produto.Preco) / 2;
            if (produto.Preco > highscore.Preco)
            {
                highscore = produto;
            }
        }
    }

    conioPrintf(MENU_RIGHT, BRANCO, 0, "Total de produtos cadastrados: %d", total_produtos);
    conioPrintf(MENU_RIGHT, BRANCO, 1, "Qte. produtos em estoque: %d", total_estoque);
    conioPrintf(MENU_RIGHT, BRANCO, 2, "Valor tot. em mercadorias: %f", total_preco);
    conioPrintf(MENU_RIGHT, BRANCO, 3, "Media de precos: %f", media_precos);
    conioPrintf(MENU_RIGHT, AMARELO, 4, "--------------");
    conioPrintf(MENU_RIGHT, AMARELO, 5, "Produto mais caro: %s", highscore.Desc);
    conioPrintf(MENU_RIGHT, AMARELO, 6, "Valor: R$%f", highscore.Preco);

    return getchclose(reg_produtos);
}

int _getactualvendascod(FILE *vendas, int vendas_size)
{
    Vendas venda;
    if (vendas_size == 0)
        return 1;
    fseek(vendas, sizeof(Vendas) - (vendas_size * sizeof(Vendas)), SEEK_SET);
    fread(&venda, sizeof(Vendas), 1, vendas);
    rewind(vendas);
    return venda.CodVenda + 1;
}

int novaVenda(FILE *reg_clientes, FILE *reg_fornecedores, FILE *reg_produtos, FILE *reg_index_vendas, FILE *reg_vendas)
{
    reg_clientes = fopen("clientes\\clientes.dat", "rb+");
    reg_fornecedores = fopen("fornecedores\\fornecedores.dat", "rb+");
    reg_produtos = fopen("produtos\\produtos.dat", "rb+");
    reg_index_vendas = fopen("vendas\\index_vendas.dat", "ab+");
    reg_vendas = reg_vendas = fopen("vendas\\vendas.dat", "ab+");

    if (!reg_clientes || !reg_fornecedores || !reg_produtos || !reg_index_vendas || !reg_vendas)
    {
        return 0;
    }

    Clientes cliente;
    Fornecedores fornecedor;
    Produtos produto;
    Vendas_Produtos venda_produto;
    Vendas venda;
    int pos_cliente, pos_fornecedor, pos_produto, pos_venda_produto, pos_venda;

    static int clientes_size = fsizer(reg_clientes, sizeof(Clientes), SET, LOGIC);
    static int fornecedores_size = fsizer(reg_fornecedores, sizeof(Fornecedores), SET, LOGIC);
    static int produtos_size = fsizer(reg_produtos, sizeof(Produtos), SET, LOGIC);
    int index_vendas_size = fsizer(reg_index_vendas, sizeof(Vendas_Produtos), SET, LOGIC);
    int vendas_size = fsizer(reg_vendas, sizeof(Vendas), SET, LOGIC);

    int i, var, cod_aux;
    float valor_variavel = 0;
    char cpf[11], datahelper;

    time_t t = time(NULL);
    tm tm = *localtime(&t);
    TpData construtor_data, input;

    bool pass;

    int codigo_venda_atual = _getactualvendascod(reg_vendas, vendas_size);
    conioPrintf(TOPO, ROSA, 0, "Nova Venda!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "CPF do Cliente: ");
    fflush(stdin);
    gets(cpf);

    if (validarCPF(cpf))
    {
        if (find_clientes(reg_clientes, cliente, cpf, pos_cliente, BYTE))
        {
            fseek(reg_clientes, pos_cliente, SEEK_SET);

            conioPrintf(TOPO, ROSA_CLARO, 0, "Cliente: %s", cliente.NomeCli);
            pass = false;
            construtor_data.Dia = tm.tm_mday;
            construtor_data.Mes = tm.tm_mon + 1;
            construtor_data.Ano = tm.tm_year + 1900;
            do
            {
                clearElement(RIGHTSIDE);
                conioPrintf(MENU_RIGHT, BRANCO, 1, "[A] - Usar a data atual para a venda");
                conioPrintf(MENU_RIGHT, BRANCO, 2, "[B] - Data personalizada;");
                fflush(stdin);
                datahelper = toupper(getch());
                switch (datahelper)
                { // bool pass = true quando setar a data
                case 'A':
                    venda.DtVenda = construtor_data;
                    pass = true;
                    break;
                case 'B':
                    conioPrintf(MENU_RIGHT, AMARELO, 4, "Dia: ");
                    fflush(stdin);
                    scanf("%d", &input.Dia);
                    conioPrintf(MENU_RIGHT, AMARELO, 5, "Mês: ");
                    fflush(stdin);
                    scanf("%d", &input.Mes);
                    conioPrintf(MENU_RIGHT, AMARELO, 6, "Ano: ");
                    fflush(stdin);
                    scanf("%d", &input.Ano);

                    if (input.Dia > 0 && input.Dia < 31 && input.Mes > 0 && input.Mes < 13 && input.Ano > 2000 && input.Ano < 2024)
                    {
                        if (comparaData(input.Ano, input.Mes, input.Dia, construtor_data.Ano, construtor_data.Mes, construtor_data.Dia) >= 0)
                        {
                            venda.DtVenda = input;
                            pass = true;
                        }
                        else
                        {
                            conioPrintf(ALERTA, VERMELHO, 0, "Data invalida!!");
                            getch();
                        }
                    }
                    else
                    {
                        conioPrintf(ALERTA, VERMELHO, 0, "Data invalida!!");
                        getch();
                    }
                    break;
                }
            } while (pass == false);

            conioPrintf(TOPO, AZUL, 0, "Cliente: %s - %d/%d/%d - cod = %d", cliente.NomeCli, venda.DtVenda.Dia, venda.DtVenda.Mes, venda.DtVenda.Ano, codigo_venda_atual);

            clearElement(RIGHTSIDE);
            conioPrintf(MENU_RIGHT, BRANCO, 0, "Cod do prod. a ser adicionado: ");
            fflush(stdin);
            scanf("%d", &cod_aux);

            while (cod_aux > 0)
            {
                if (find_produtos(reg_produtos, produto, cod_aux, pos_produto, BYTE) && produto.Estoque > 0)
                {
                    fseek(reg_produtos, pos_produto, SEEK_SET);

                    pass = false;
                    if (comparaData(produto.DtValidade.Ano, produto.DtValidade.Mes, produto.DtValidade.Dia, construtor_data.Ano, construtor_data.Mes, construtor_data.Dia) <= 0)
                    {
                        conioPrintf(MENU_RIGHT, BRANCO, 1, "Produto: %s", produto.Desc, produto.Estoque);
                        do
                        {
                            conioPrintf(MENU_RIGHT, BRANCO, 3, "Quantidade vendida: ");
                            int x = wherex(), y = wherey();
                            conioPrintf(MENU_RIGHT, CINZA_CLARO, 4, "Quantidade disponivel: %d", produto.Estoque);
                            gotoxy(x, y);
                            fflush(stdin);
                            scanf("%d", &var);

                        } while (var <= 0);
                        if (var <= produto.Estoque)
                        {
                            produto.Estoque -= var;
                            venda_produto.CodVenda = codigo_venda_atual;
                            venda_produto.CodProd = produto.CodProd;
                            venda_produto.Qtde = var;
                            venda_produto.ValorUnitario = produto.Preco;
                            venda_produto.flag = true;
                            valor_variavel += produto.Preco * var;
                            fwrite(&venda_produto, sizeof(Vendas_Produtos), 1, reg_index_vendas);
                            fwrite(&produto, sizeof(Produtos), 1, reg_produtos);
                            conioPrintf(SWITCHER, VERDE, 0, "Item %s adicionado a venda de cod. %d", produto.Desc, codigo_venda_atual);
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
                    conioPrintf(SWITCHER, VERMELHO, 0, "-- Erro na venda do produto");
                    getch();
                }
                clearElement(RIGHTSIDE);
                conioPrintf(MENU_RIGHT, BRANCO, 0, "Cod do prod. a ser adicionado: ");
                fflush(stdin);
                scanf("%d", &cod_aux);
            }

            venda.CodVenda = codigo_venda_atual;
            strcmp(venda.CPF, cliente.CPF);
            venda.DtVenda = construtor_data;
            venda.TotVenda = valor_variavel;
            venda.flag = true;
            cliente.QtdeCompras++;
            cliente.ValorTotComprado += valor_variavel;
            fwrite(&cliente, sizeof(Clientes), 1, reg_clientes);
            fwrite(&venda, sizeof(Vendas), 1, reg_vendas);

            conioPrintf(ALERTA, VERDE, 0, "Venda do cliente %s registrada com sucesso", cliente.NomeCli);
        }
        else
            conioPrintf(SWITCHER, VERMELHO, 0, "Cliente nao encontrado!");
    }
    else
        conioPrintf(ALERTA, VERMELHO, 0, "Digite um CPF valido!");

    fclose(reg_fornecedores);
    fclose(reg_index_vendas);
    fclose(reg_vendas);
    fclose(reg_produtos);
    return getchclose(reg_clientes);
}
int ExcluirVenda(FILE *reg_vendas, FILE *reg_index_vendas, FILE *reg_clientes, FILE *reg_produtos)
{
    reg_clientes = fopen("clientes\\clientes.dat", "rb+");
    reg_produtos = fopen("produtos\\produtos.dat", "rb+");
    reg_index_vendas = fopen("vendas\\index_vendas.dat", "rb+");
    reg_vendas = fopen("vendas\\vendas.dat", "rb+");

    if (!reg_clientes || !reg_produtos || !reg_index_vendas || !reg_vendas)
    {
        return 0;
    }

    Clientes cliente;
    Produtos produto;
    Vendas_Produtos venda_produto;
    Vendas venda;
    int pos_cliente, pos_produto, pos_venda_produto, pos_venda;

    int index_vendas_size = fsizer(reg_index_vendas, sizeof(Vendas_Produtos), SET, LOGIC);

    int codVenda, i, j;
    conioPrintf(TOPO, CIANO, 0, "Exclusao de venda!");
    conioPrintf(MENU_RIGHT, BRANCO, 0, "Digite o codigo da venda a ser excluida: ");
    fflush(stdin);
    scanf("%d", &codVenda);

    if (find_venda(reg_vendas, venda, codVenda, pos_venda, BYTE))
    {
        conioPrintf(ALERTA, VERMELHO, 0, "Apagar venda %d? ", venda.CodVenda);
        fflush(stdin);
        if ((toupper(getch())) == 'S')
        {
            for (i = 0; i < index_vendas_size; i++)
            {
                fread(&venda_produto, sizeof(Vendas_Produtos), 1, reg_index_vendas);
                if (venda_produto.CodVenda == codVenda)
                {
                    venda_produto.flag = false;
                    fseek(reg_index_vendas, i * sizeof(Vendas_Produtos), SEEK_SET);

                    find_produtos(reg_produtos, produto, venda_produto.CodProd, pos_produto, BYTE);
                    produto.Estoque += venda_produto.Qtde;
                    fseek(reg_produtos, pos_produto, SEEK_SET);

                    fwrite(&venda_produto, sizeof(Vendas_Produtos), 1, reg_index_vendas);
                    fwrite(&produto, sizeof(Produtos), 1, reg_produtos);
                }
            }
            find_clientes(reg_clientes, cliente, venda.CPF, pos_cliente, BYTE);
            cliente.ValorTotComprado -= venda.TotVenda;
            --cliente.QtdeCompras;
            fseek(reg_clientes, pos_cliente, SEEK_SET);

            venda.flag = false;
            fseek(reg_vendas, pos_venda, SEEK_SET);

            fwrite(&cliente, sizeof(Clientes), 1, reg_clientes);
            fwrite(&venda, sizeof(Vendas), 1, reg_vendas);
            conioPrintf(ALERTA, VERDE, 0, "Venda de codigo %d excluida com sucesso", codVenda);
        }
        else
            conioPrintf(ALERTA, CIANO_CLARO, 0, "Operacaoo de exclusao cancelada.");
    }
    else
        conioPrintf(ALERTA, VERMELHO, 0, "Venda de codigo %d nao encontrada!", codVenda);

    fclose(reg_vendas);
    fclose(reg_index_vendas);
    fclose(reg_clientes);
    return getchclose(reg_produtos);
}

// void AlterarVenda(Vendas rootVendas[], Vendas_Produtos rootVendasProdutos[], Produtos rootProdutos[], int TLvendas, int TLvendasprod, int TLprodutos)
// {
//     int codVenda, novoEstoque, novoQuantidade, codProduto;
//     char resp;
//     int pontoVenda, pontoProd;

//     conioPrintf(TOPO, VERDE, 0, "Alterar Venda Existente!");
//     conioPrintf(MENU_RIGHT, BRANCO, 0, "Insira o codigo da venda: ");
//     scanf("%d", &codVenda);

//     pontoVenda = BuscaVendaPorCodigo(rootVendas, TLvendas, codVenda);

//     if (pontoVenda != -1)
//     {
//         do
//         {
//             conioPrintf(MENU_RIGHT, BRANCO, 1, "O que deseja alterar na venda?");
//             conioPrintf(MENU_RIGHT, BRANCO, 2, "[A] - Adicionar Produto");
//             conioPrintf(MENU_RIGHT, BRANCO, 3, "[B] - Remover Produto");
//             conioPrintf(MENU_RIGHT, BRANCO, 4, "[C] - Modificar Produto (Estoque/Quantidade)");
//             conioPrintf(MENU_RIGHT, BRANCO, 5, "[D] - Sair");
//             fflush(stdin);
//             resp = toupper(getch());

//             switch (resp)
//             {
//             case 'A':
//                 // Adicionar Produto o Venda

//                 break;

//             case 'B':
//                 // Remover Produto da Venda
//                 // Colocar o BuscaProdutoPorCodigo pra encontrar o produto na lista de vendas aqui
//                 break;

//             case 'C':
//                 // Modificar Produto (Estoque/Quantidade) na Venda
//                 conioPrintf(MENU_RIGHT, BRANCO, 0, "Insira o codigo do produto: ");
//                 fflush(stdin);
//                 scanf("%d", &codProduto);
//                 pontoProd = BuscaProdutoPorCodigo(rootProdutos, TLprodutos, codProduto);

//                 if (pontoProd != -1)
//                 {
//                     // modif o produto (estoque/quantidade) na venda
//                     // e atualizar
//                 }
//                 else
//                 {
//                     conioPrintf(ALERTA, VERMELHO, 0, "Produto nao encontrado!");
//                     getch();
//                 }
//                 break;

//             case 'D':
//                 // Sair da funcao
//                 break;

//             default:
//                 conioPrintf(ALERTA, VERMELHO, 0, "Opcao invalida!");
//                 getch();
//                 break;
//             }
//         } while (resp != 'D');
//     }
//     else
//     {
//         conioPrintf(ALERTA, VERMELHO, 0, "Venda nao encontrada!");
//     }
// }

void listaClientes(FILE *clientes) {
    clrscr();
    clientes = fopen ("clientes\\clientes.dat", "rb");
    Clientes cliente;
    int TL = fsizer (clientes, sizeof(Clientes), SET, LOGIC);
    for (int i=0; i<TL && cliente.flag; i++) {
        fread(&cliente,sizeof(Clientes),1,clientes);
        gotoxy(1,i+1);
        printf("                          |           |            |        ");
        gotoxy(1,i+1);
        printf("%s",cliente.NomeCli);
        gotoxy(28,i+1);
        printf("%s",cliente.CPF);
        gotoxy(40,i+1);
        printf("%d compras", cliente.QtdeCompras);
        gotoxy(53,i+1);
        printf("R$%.2f total", cliente.ValorTotComprado);
    }
    fclose(clientes);
    getch();
    clrscr();
}
void Menu(FILE *fornecedores, FILE *produtos, FILE *clientes, FILE *index_vendas, FILE *vendas)
{
    int op, vendas_size;
    char opc, opc_sub;

    Formulario();

    if ((vendas = fopen("vendas\\vendas.dat", "rb+")) == NULL)
        vendas_size = 0;
    else
        vendas_size = fsizer(vendas, sizeof(Vendas), SET, LOGIC);
    fclose(vendas);

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
                Formulario();
                conioPrintf(TOPO, ROSA, 0, "Controle de Clientes!");
                conioPrintf(SWITCHER, ROSA, 0, "Selecione uma operacao em clientes");
                conioPrintf(MENU_LEFT, BRANCO, 0, "[A] - CADASTRO");
                conioPrintf(MENU_LEFT, BRANCO, 1, "[B] - CONSULTA");
                conioPrintf(MENU_LEFT, BRANCO, 2, "[C] - EXCLUSAO");
                conioPrintf(MENU_LEFT, BRANCO, 3, "[D] - ALTERACAO");
                conioPrintf(MENU_LEFT, BRANCO, 4, "[E] - RELATORIO");
                conioPrintf(MENU_LEFT, VERDE, 6, "F - Lista");
                gotoxy(4, 14);
                opc = toupper(getche());

                switch (opc)
                {
                case 'A':
                    CadastraCliente(clientes);
                    break;
                case 'B':
                    ConsultaClientes(clientes);
                    break;
                case 'C':
                    DeletaClientes(clientes);
                    break;
                case 'D':
                    EditaClientes(clientes);
                    break;
                case 'E':
                    RelatorioClientes(clientes);
                    break;
                case 'F':
                    listaClientes(clientes);
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
                conioPrintf(MENU_LEFT, BRANCO, 4, "[E] - RELATORIO");
                opc = toupper(getch());

                switch (opc)
                {
                case 'A':
                    CadastraFornecedor(fornecedores, NULL);
                    break;
                case 'B':
                    ConsultaFornecedor(fornecedores);
                    break;
                case 'C':
                    ExcluirFornecedor(fornecedores, produtos);
                    break;
                case 'D':
                    AlterarDadosFornecedor(fornecedores);
                    break;
                case 'E':
                    RelatorioFornecedores(fornecedores);
                    break;
                case 27:
                    break;
                default:
                    conioPrintf(ALERTA, VERMELHO, 0, "INEXISTENTE! Selecione novamente");
                    getch();
                }
            } while (opc != 27);
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
                conioPrintf(MENU_LEFT, BRANCO, 4, "[F] - RELATORIO");
                fflush(stdin);
                opc = toupper(getch());

                switch (opc)
                {
                case 'A':
                    CadastraProd(produtos, fornecedores);
                    break;
                case 'B':
                    ConsultaProd(produtos);
                    break;
                case 'C':
                    ExcluirProd(produtos);
                    break;
                case 'D':
                    AlterarProdCadastrado(produtos);
                    break;
                case 'E':
                    produtosPercent(produtos, fornecedores);
                    break;
                case 'F':
                    RelatorioProdutos(produtos);
                    break;
                case 27:
                    break;
                default:
                    conioPrintf(SWITCHER, VERMELHO, 0, "INEXISTENTE! Selecione novamente");
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
                conioPrintf(MENU_LEFT, BRANCO, 3, "[D] - RELATORIO VENDAS");
                opc = toupper(getch());
                switch (opc)
                {
                case 'A':
                    novaVenda(clientes, fornecedores, produtos, index_vendas, vendas);
                    break;
                case 'B':
                    ExcluirVenda(vendas, index_vendas, clientes, produtos);
                    break;
                    //     case 'C':
                    //         AlterarVenda(index_vendas, index_vendasprod, index_produtos, TL_cupons, TL_vendas, TL_produtos);
                    //         getch();
                    //         break;
                    //     case 'D':
                    //         // RelatorioTotal();
                    //         break;
                    //     case 27:
                    //         break;
                    //     default:
                    //         conioPrintf(SWITCHER, VERMELHO, 0, "INEXISTENTE! Selecione novamente");
                    //         getch();
                    //         break;
                    //     }
                }
            } while (opc != 27);
        }

        if ((vendas = fopen("vendas\\vendas.dat", "rb+")) == NULL)
            vendas_size = 0;
        else
            vendas_size = fsizer(vendas, sizeof(Vendas), SET, LOGIC);
        fclose(vendas);

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

void file_clear(FILE *fornecedores, FILE *produtos, FILE *clientes, FILE *index_vendas, FILE *vendas)
{
    fornecedores = fopen("fornecedores\\fornecedores.dat", "rb+");
    clientes = fopen("clientes\\clientes.dat", "rb+");
    produtos = fopen("produtos\\produtos.dat", "rb+");
    index_vendas = fopen("vendas\\index_vendas.dat", "rb+");
    vendas = fopen("vendas\\vendas.dat", "rb+");

    Fornecedores fornecedor;
    Clientes cliente;
    Produtos produto;
    Vendas_Produtos index_venda;
    Vendas venda;

    if (fornecedores != NULL)
    {
        int fornecedores_size = fsizer(fornecedores, sizeof(Fornecedores), SET, LOGIC);
        FILE *fornecedores_temp = fopen("fornecedores\\fornecedores_temp.dat", "ab+");

        for (int i = 0; i < fornecedores_size; i++)
        {
            fread(&fornecedor, sizeof(Fornecedores), 1, fornecedores);
            if (fornecedor.flag)
                fwrite(&fornecedor, sizeof(Fornecedores), 1, fornecedores_temp);
        }
        fclose(fornecedores);
        fclose(fornecedores_temp);
        remove("fornecedores\\fornecedores.dat");
        rename("fornecedores\\fornecedores_temp.dat", "fornecedores\\fornecedores.dat");
    }

    if (clientes != NULL)
    {
        int clientes_size = fsizer(clientes, sizeof(Clientes), SET, LOGIC);
        FILE *clientes_temp = fopen("clientes\\clientes_temp.dat", "ab+");

        for (int i = 0; i < clientes_size; i++)
        {
            fread(&cliente, sizeof(Clientes), 1, clientes);
            if (cliente.flag)
                fwrite(&cliente, sizeof(Clientes), 1, clientes_temp);
        }

        fclose(clientes);
        fclose(clientes_temp);
        remove("clientes\\clientes.dat");
        rename("clientes\\clientes_temp.dat", "clientes\\clientes.dat");
    }

    if (produtos != NULL)
    {
        int produtos_size = fsizer(produtos, sizeof(Produtos), SET, LOGIC);
        FILE *produtos_temp = fopen("produtos\\produtos_temp.dat", "ab+");

        for (int i = 0; i < produtos_size; i++)
        {
            fread(&produto, sizeof(Produtos), 1, produtos);
            if (produto.flag)
                fwrite(&produto, sizeof(Produtos), 1, produtos_temp);
        }

        fclose(produtos);
        fclose(produtos_temp);
        remove("produtos\\produtos.dat");
        rename("produtos\\produtos_temp.dat", "produtos\\produtos.dat");
    }

    if (vendas != NULL)
    {
        int vendas_size = fsizer(vendas, sizeof(Vendas), SET, LOGIC);
        FILE *vendas_temp = fopen("vendas\\vendas_temp.dat", "ab+");

        for (int i = 0; i < vendas_size; i++)
        {
            fread(&venda, sizeof(Vendas), 1, vendas);
            if (venda.flag)
                fwrite(&venda, sizeof(Vendas), 1, vendas_temp);
        }

        fclose(vendas);
        fclose(vendas_temp);
        remove("vendas\\vendas.dat");
        rename("vendas\\vendas_temp.dat", "vendas\\vendas.dat");
    }

    if (index_vendas != NULL)
    {
        int index_vendas_size = fsizer(index_vendas, sizeof(Vendas_Produtos), SET, LOGIC);
        FILE *index_vendas_temp = fopen("vendas\\index_vendas_temp.dat", "ab+");

        for (int i = 0; i < index_vendas_size; i++)
        {
            fread(&index_venda, sizeof(Vendas_Produtos), 1, index_vendas);
            if (index_venda.flag)
                fwrite(&index_venda, sizeof(Vendas_Produtos), 1, index_vendas_temp);
        }

        fclose(index_vendas);
        fclose(index_vendas_temp);
        remove("vendas\\index_vendas.dat");
        rename("vendas\\index_vendas_temp.dat", "vendas\\index_vendas.dat");
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
    file_clear(fornecedores, produtos, clientes, index_vendas, vendas);

    return 0;
}
