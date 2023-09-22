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

struct Produtos {
    int CodProd, Estoque;
    char Desc[QUANT];
    float Preco;
    TpData DtValidade;
}; Fornecedores CodForn; //essa parada eh a chave primaria?

struct Clientes {
    int CPF[QUANT];
    char NomeCli[QUANT];
    int QtdeCompras[QUANT];
    double ValorTotComprado;
};

struct Vendas {
    int CodVenda;
    Clientes CPF[11];
    struct DtValidade {
        int Dia;
        int Mes;
        int Ano;
    };
    float TotVenda;
};

struct Vendas_Produtos {
    Vendas CodVenda;
    Produtos CodProd;
    int Qtde;
    float ValorUnitario;
};

int ValidarCPF(Clientes CPF){
	int i, j;
	if (strlen(CPF) == 11){
		//colocar para verificar se todos os caracteres sao numeros?
		//codigo..	
	} else
		printf("\nCPF inválido!\n\n");
	
}

int BusProdCod(Produtos Tab[TF], int TL, int CodProd)
{
	
	printf("**Busca o produto pelo codigo**");
	int i=0;
	while (i<TL && CodProd!=Tab[i].CodProd)
			i++;
	if (i<TL)
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
	scanf("%d",&AuxCod);
	while (TL<TF && AuxCod>0)
	{
		pos = BusProdCod(Tab,TL,AuxCod);
		if (pos == -1)
		{
			Tab[TL].CodProd = AuxCod;
			
			printf("Descricao: ");
			fflush(stdin);
			gets(Tab[TL].Desc);
			
			printf("Estoque: ");
			scanf("%d",&Tab[TL].Estoque);
			
			printf("Preco: R$ ");
			scanf("%f",&Tab[TL].Preco);
			
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
		scanf("%d",&AuxCod);
	}
}

void Relatorio(Produtos Tab[TF], int TL)
{
	int i;
	clrscr();
	printf("\nRelatorio de Produtos\n");
	if (TL==0)
		printf("\nNao ha Produtos Cadastrados!\n");
	else
			for(i=0; i<TL; i++)
			{
				printf("\nCodigo de Venda: %d\n",Tab[i].CodProd);
				printf("\nProdutos: \n%d\n",Tab[i].CodVenda);
				printf("%s\n",Tab[i].Descr);
				printf("%d\n",Tab[i].Estoque);
				printf("R$ %.2f\n",Tab[i].Preco);
				printf("%s\n", Tab[i].NomeForn);
				printf("%f\n",Tab[i].Preco);
			}
	getch();
}

void Menu () {
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
	    case 'A': printf("[A] - Inserir Elementos\n");
	        /* code */
	        break;
		case 'B': printf("[B] - Cadastrar um novo produto\n");
	        /* code */
	        break;
	    case 'C': printf("[C] - Excluir produto\n");
	        /* code */
	        break;
	    case 'D': printf("[D] - Relatorio\n");
	        /* code */
	        break;
	    	    
    }

}


int main(void){
    Fornecedores index_fornecedores[TF];
    Produtos index_produtos[TF];
    Clientes index_clientes[TF];
    Vendas index_vendas[TF];
    Vendas_Produtos vendas[TF];


    return 0;
}
