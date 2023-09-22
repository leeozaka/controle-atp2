#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

#define TF 10

typedef struct Fornecedores
{
    int CodForn;
    char NomeForn[20];
    char Cidade[20];
};

typedef struct Produtos {
    int CodProd;
    char Desc[20];
    int Estoque;
    float Preco;
    struct DtValidade {
        int Dia;
        int Mes;
        int Ano;
    };
    Fornecedores CodForn; //essa parada eh a chave primaria?
};

typedef struct Clientes {
    int CPF[11];
    char NomeCli[20];
    int QtdeCompras[20];
    double ValorTotComprado;
};

typedef struct Vendas {
    int CodVenda;
    Clientes CPF[11];
    struct DtValidade {
        int Dia;
        int Mes;
        int Ano;
    };
    float TotVenda;
};

typedef struct Vendas_Produtos {
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
		case 'A': printf("[B] - Cadastrar um novo produto\n");
	        /* code */
	        break;
	    case 'A': printf("[C] - Excluir produto\n");
	        /* code */
	        break;
	    case 'A': printf("[D] - Relatorio\n");
	        /* code */
	        break;
	    case 'A': printf("[A] - Inserir Elementos\n");
	        /* code */
	        break;
	    
    }

}


int main(){
    Fornecedores index_fornecedores[TF];
    Produtos index_produtos[TF];
    Clientes index_clientes[TF];
    Vendas index_vendas[TF];
    Vendas_Produtos vendas[TF];


    return 0;
}
