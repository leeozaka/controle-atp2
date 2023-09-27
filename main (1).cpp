#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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

int validarCPF(char cpf[11]) // corrigido 25-09
{
    int i, digito1 = 0, digito2 = 0, helper;
    if ((strcmp(cpf, "00000000000") == 0) || (strcmp(cpf, "11111111111") == 0) || (strcmp(cpf, "22222222222") == 0) ||
        (strcmp(cpf, "33333333333") == 0) || (strcmp(cpf, "44444444444") == 0) || (strcmp(cpf, "55555555555") == 0) ||
        (strcmp(cpf, "66666666666") == 0) || (strcmp(cpf, "77777777777") == 0) || (strcmp(cpf, "88888888888") == 0) ||
        (strcmp(cpf, "99999999999") == 0))
        return 0; /// se o CPF tiver todos os nÃºmeros iguais ele Ã© invÃ¡lido.
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
            // for (i = 0, j = 11; i < strlen(cpf) - 1; i++, j--)
            // digito2 += (cpf[i] - '0') * j;
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
    int i, validar=0;
    printf("Digite seu CPF: ");
    gets(CPF);
    for (i = 0; i < 11 && CPF[i] >= 48 && CPF[i] <= 57; i++)
        ; // verifica se todos caracteres estao entre 0 e 9
    if (i == 11)
    {
        if (validarCPF(CPF) == 1)
        {
        	for(i=0;i<TL && validar != 1;i++)
        	{
        		if(strcmp(clientes[i].CPF, CPF)==0)
        			validar=1;
			}
			if (validar == 1)
			{
				printf("\nCPF ja cadastrado\n!");
				getch();
			} else{
				strcpy(clientes[TL].CPF, CPF);
	            fflush(stdin);
	            printf("Digite o nome: ");
	            gets(clientes[TL].NomeCli);
	            TL++;
	            printf("Cliente cadastrado com sucesso!");
	            getch();
				
			}
            
        }
        else
        {
            printf("\nO nÃºmero de identificaÃ§Ã£o informado nÃ£o estÃ¡ correto.\n");
            getch();
        }
    }
    else
    {
        printf("\nO nÃºmero de identificaÃ§Ã£o deve conter apenas nÃºmeros!\n");
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
    // retorna cÃ³digo do Fornecedor! Ã± pos;
    int i;
    for (i = 0; i < TL && find != lista_fornecedores[i].CodForn; i++)
        ;
    return i == TL ? -1 : lista_fornecedores[i].CodForn;
}

void CadastraFornecedor(Fornecedores fornecedores[TF], int &TL, int *cod)
// int *cod passa NULL se o cÃ³digo do fornecedor jÃ¡ nao foi inserido em outra funÃ§ao, exmeplo no cast do Menu();
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
void AlterarProdCadastrado(Produtos Tab[TF], int &TL){
	int Aux;
	printf("\n**Alterar produto ja cadastrado**\n");
	getch();
	printf("Insira o codigo do produto: \n");
	fflush(stdin);
	scanf("%d"&Aux);
	while(Aux>0){
//		sera que usar a função CadastraProd para fazer uma alteração de um produto cadastrado, n daria?
//		ai procura a posição que está o produto (verificando o codigo) e altera toda a linha da tabela dele
//		também faz uma verificação para ver se o codigo do produto existe mesmo ou não 

//		finalizar o código aqui...
		
		printf("Insira o codigo do produto: \n");
		fflush(stdin);
		scanf("%d"&Aux);
	}
}

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
        printf("[ ] - Alterar produto ja cadastrado\n");
        printf("[C] - Cadastrar um novo Fornecedor\n");
        printf("[D] - Cadastrar Cliente\n");
        printf("[E] - Ordenar Relatorio\n");
        printf("Digite a opcao a seguir: ");
        fflush(stdin);
        opc = toupper(getch());
        switch (opc)
        {
        case 'A':
            /* code */
            break;
        case 'B':
            CadastraProd(index_produtos, index_fornecedores, TL_produtos, TL_fornecedores);
            break;
        case 'C':
            CadastraFornecedor(index_fornecedores, TL_fornecedores, NULL);
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
