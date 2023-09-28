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

void ExcluirProd(Produtos Tab[TF], int TL)
{
	int i, j, Aux;
	printf("\n**Exclusao de produto**\n");
	printf("Digite o codigo do produto a excluir: ");
	scanf("%d", &Aux);
	while(Aux > 0)
	{
		//j=BuscaExaustiva(Tab[TF], TL, Aux); //copiar o BuscaExaustiva do exemplo na sala
		if(j != -1)
		{
			for(i=j; i<TL-1; i++)
				Tab[i] = Tab[i+1];
			
		} else{
			printf("\nProduto nao encontrado!\n");
		}
		printf("Digite o codigo do produto a excluir: ");
		scanf("%d", &Aux);
	}
}

void AlterarProdCadastrado(Produtos Tab[TF], int &TL){
	int Aux;
	printf("\n**Alterar produto ja cadastrado**\n");
	getch();
	printf("Insira o codigo do produto: \n");
	fflush(stdin);
	scanf("%d",&Aux);
	while(Aux>0){
//		sera que usar a fun��o CadastraProd para fazer uma altera��o de um produto cadastrado, n daria?
//		ai procura a posi��o que est� o produto (verificando o codigo) e altera toda a linha da tabela dele
//		tamb�m faz uma verifica��o para ver se o codigo do produto existe mesmo ou n�o 

// ---- >>>> Eu pensei nessa opção mas é melhor fazer um switch > case e alterar alguma informaçao e validar nessa funcao mesmo
// ---- >>>> Para reutilizar CadastraProd seria interessante apenas se fosse mudar TODAS as infos dentro do produto

// ---- >>>> TL n'precisa ser por referencia


//		finalizar o c�digo aqui...
		
		printf("Insira o codigo do produto: \n");
		fflush(stdin);
		scanf("%d",&Aux);
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
	for(i=LI;i<=LF;i++)
	{
		for(j=CI;j<=CF;j++)
		{
			gotoxy(j,i);
			printf("%c",32);
		}
	}
}

void Menu(void)
{
	int Tab[TF], opc;
	do{
		ClrScrenzona(23,3,23,78);
		gotoxy(3,23);
		printf("Selecione um item:\n");
		ClrScrenzona(8, 3, 20, 24);
		gotoxy(4,9);
		printf("[A] - CLIENTES\n");
		gotoxy(4,10);
		printf("[B] - FORNECEDORES\n");
		gotoxy(4,11);
		printf("[C] - PRODUTOS\n");
		
		opc = toupper(getche());
		
		switch(opc)
		{
			case 'A':
				do
				{
					ClrScrenzona(23,3,23,78);
					gotoxy(3,23);
					printf("*Selecione uma operacao em clientes*\n");
					// a mostrar:
					ClrScrenzona(8,3,20,24);
					gotoxy(4,9);
					printf("[A] - CADASTRO\n");
					gotoxy(4,10);
					printf("[B] - CONSULTA\n");
					gotoxy(4,11);
					printf("[C] - EXCLUSAO\n");
					gotoxy(4,12);
					printf("[D] - ALTERACAO\n");
					gotoxy(4,13);
					printf("[E] - RELATORIO\n");
					gotoxy(4,14);
					opc = toupper(getche());
					
					switch(opc)
					{
						case 'A': CadastraProd(Tab[TF],TL);
							break;
						case 'B':
							break;
						case 'C':
							break;
						case 'D':
							break;
						case 'E':
							break;
						default:
							if(opc != 27)
							{
								ClrScrenzona(23,3,23,78);
								gotoxy(3,23);
								printf("\n##INEXISTENTE!##\nSelecione novamente\n");
								gotoxy(51,23);
								getche();	
							}
						break;	
					}	
				}while(opc != 27);
				break;
			
			case 'B': opc = 0;
				do
				{
					ClrScrenzona(23,3,23,78);
					gotoxy(3,23);
					printf("*Selecione uma operacao em fornecedores*\n");
					// a mostrar:
					ClrScrenzona(8,3,20,24);
					gotoxy(4,9);
					printf("[A] - CADASTRO\n");
					gotoxy(4,10);
					printf("[B] - CONSULTA\n");
					gotoxy(4,11);
					printf("[C] - EXCLUSAO\n");
					gotoxy(4,12);
					printf("[D] - ALTERACAO\n");
					gotoxy(4,13);
					printf("[E] - RELATORIO\n");
					gotoxy(4,14);
					opc = toupper(getche());
					
					switch(opc)
					{
						case 'A': CadastraProd(Tab[TF],TL);
							break;
						case 'B': ConsultaFornecedor(Tab[TF],TL);
							break;
						case 'C': ExcluirProd(Tab[TF],TL);
							break;
						case 'D': AlterarProdCadastrado(Tab[TF],TL);
							break;
						case 'E': Relatorio(Tab[TF],TL);
							break;
						default:
							if(opc != 27)
							{
								ClrScrenzona(23,3,23,78);
								gotoxy(3,23);
								printf("\n##INEXISTENTE!##\nSelecione novamente\n");
								gotoxy(51,23);
								getche();	
							}
						break;	
					}	
				}while(opc != 27);
				break;
				
			case 'C': opc = 0;
				do
				{
					ClrScrenzona(23,3,23,78);
					gotoxy(3,23);
					printf("*Selecione uma operacao em produtos*\n");
					// a mostrar:
					ClrScrenzona(8,3,20,24);
					gotoxy(4,9);
					printf("[A] - CADASTRO\n");
					gotoxy(4,10);
					printf("[B] - CONSULTA\n");
					gotoxy(4,11);
					printf("[C] - EXCLUSAO\n");
					gotoxy(4,12);
					printf("[D] - ALTERACAO\n");
					gotoxy(4,13);
					printf("[E] - RELATORIO\n");
					gotoxy(4,14);
					opc = toupper(getche());
					
					switch(opc)
					{
						case 'A': CadastraProd(Tab[TF],TL);
							break;
						case 'B': 
							break;
						case 'C': ExcluirProd(Tab[TF],TL);
							break;
						case 'D': AlterarProdCadastrado(Tab[TF],TL);
							break;
						case 'E':
							break;
						default:
							if(opc != 27)
							{
								ClrScrenzona(23,3,23,78);
								gotoxy(3,23);
								printf("\n##INEXISTENTE!##\nSelecione novamente\n");
								gotoxy(51,23);
								getche();	
							}
						break;	
					}	
				}while(opc != 27);
				opc = 0;
				break;
			default:
				if(opc!=27)
				{
					ClrScrenzona(23,3,23,78);
					gotoxy(3,23);
					printf("\n##INEXISTENTE!##\nSelecione novamente\n");
					gotoxy(51,23);
					getche();		
				}
			break;	
		}
		if(opc == 27//Sair do loop
		{
			do{
				ClrScrenzona(23,3,23,78);
				gotoxy(3,23);
				printf("CONFIRMACAO: 'Gostaria de sair?'<S/N>");
				gotoxy(41,23);
				
				opc = toupper(getche());
				
				switch(opc)
				{
					case 'S': opc == 27;//para sair do sistema
						break;
					case 'N': opc == 0; //nao sair
						break;
					default:
						if(opc != 83 || opc != 78)
						{
							ClrScrenzona(23,3,23,78);
							gotoxy(3,23);
							printf("\n##INEXISTENTE!##\nSelecione novamente\n");
							gotoxy(52,23);
							getche();
						}
				}
			}while(opc != 27);
		}
		
	}while(opc != 27);
}
//
//void Menu(Fornecedores index_fornecedores[TF], Produtos index_produtos[TF], Clientes index_clientes[TF], Vendas index_vendas[TF], Vendas_Produtos index_vendasprod[TF])
//{
//
//    int TL_fornecedores = 0, TL_produtos = 0, TL_clientes = 0, TL_vendas = 0, TL_cupons = 0, op;
//    char opc;
//    do
//    {
//        printf("[A] - Inserir Elementos\n");
//        printf("[B] - Cadastrar um novo produto\n");
//        printf("[ ] - Alterar produto ja cadastrado\n");
//        printf("[C] - Cadastrar um novo Fornecedor\n");
//        printf("[D] - Cadastrar Cliente\n");
//        printf("[E] - Ordenar Relatorio\n");
//        printf("Digite a opcao a seguir: ");
//        fflush(stdin);
//        opc = toupper(getch());
//        switch (opc)
//        {
//        case 'A':
//            /* code */
//            break;
//        case 'B':
//            CadastraProd(index_produtos, index_fornecedores, TL_produtos, TL_fornecedores);
//            break;
//        case 'C':
//            CadastraFornecedor(index_fornecedores, TL_fornecedores, NULL);
//            break;
//        case 'D':
//            CadastraCliente(index_clientes, TL_clientes);
//            break;
//        case 'F':
//            /* code */
//            break;
//        default:
//        	if(opc != /*colocar as letras*/)
//        		LimparTela(23,3,23,78);
//        		gotoxy(3,23);
//        		
//        		printf("\nOpcao Incorreta\nSelecione novamente\n");
//        		gotoxt(52,23);
//        		getch();
//        }
//    } while (opc != 27);
//}

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
