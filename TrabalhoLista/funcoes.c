#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#define TAM 40

typedef struct ContaBancaria{
	int codigo;
	char nome[TAM];
	float saldo;
	struct ContaBancaria * prox;
}ContaB;

typedef struct ListaContas{
	ContaB * primeiraConta;
	int total;	
}LContas;

typedef struct ListaNome{
	ContaB * conta;
	struct ListaNome * prox;
}LNome;


bool listaVazia(LContas * listaC){
	return !(listaC->primeiraConta);
}

LNome * novaListaNome(){
	LNome * novo = (LNome*) malloc (sizeof(LNome));
	
	novo->conta = NULL;
	novo->prox = NULL;
	
	return novo;
}

LNome * iniciarListaNome(LContas * listaC, LNome * listaN){
	ContaB * aux = listaC->primeiraConta;
	listaN->conta = aux;
	LNome * auxN = listaN;
	int i = 0;
	while(aux->prox){
		auxN->prox = novaListaNome();
		auxN = auxN->prox;
		aux = aux->prox;
		auxN->conta = aux;
	}
	
	return listaN;
}

LContas * novaLista(){
	LContas * lista = (LContas*) malloc (sizeof(LContas));
	
	lista->primeiraConta = NULL;
	lista->total = 0;
	
	return lista;
}

ContaB * novaConta(){
	ContaB * conta = (ContaB*) malloc (sizeof(ContaB));
	
	conta->prox = NULL;
	
	return conta;	
}

ContaB * lastElem(LContas * lista){
	ContaB * aux = novaConta();
	
	if(listaVazia(lista)){
		return;
	}else{
		aux = lista->primeiraConta;
		while (aux->prox){
			aux = aux->prox;
		}	
	}
	
	return aux;
}

void carregarArqs(LContas * listaC){
	
	// ABRINDO O ARQUIVO
	
	FILE * arqs;
	arqs = fopen ("DadosBancoPulini.txt", "r");
	
	// TESTANDO O ARQUIVO
	
	if(!arqs){
		printf("ERRO AO ABRIR O ARQUIVO!!!");
		exit(1);
	}else{
		printf("ARQUIVO ABERTO COM SUCESSO!!!\n ");
	}
	
	// PREENCHENDO A LISTA

	while(!feof(arqs)){
		
		int i = 0, cod, x = 0;
		char lixo[100] = "", nome[TAM] = "", saldo[TAM] = "", codigo[6] = "";
		float aux1;
				
		ContaB * conta = novaConta();
		fgets(lixo, sizeof(ContaB), arqs);
				
		while(lixo[i] != '|' && i < strlen(lixo)){
			codigo[i] = lixo[i];
			i++;
		}
		++i;
		while(lixo[i] != '|' && i < strlen(lixo)){
			nome[x] = lixo[i];
			x++;
			i++;
		}
		
		++i;
		int j = 0;
		while(i < strlen(lixo)){
			for (i; i < strlen(lixo); i++){
				if(lixo[i] == '.'){
					// apenas ignora o .
				}else if (lixo[i] == ','){
					saldo[j] = '.';
					j++;
				}else{
					saldo[j] = lixo[i];
					j++;	
				}					
			}
		}
		
		cod = atoi(codigo);
		conta->codigo = cod;
		strcpy(conta->nome, nome);
		aux1 = atof(saldo);
		conta->saldo = aux1;
		
		if(conta->codigo != 0){
			if(listaVazia(listaC)){
				listaC->primeiraConta = conta;
				listaC->total++;
			}else{
				ContaB * aux = novaConta();
				aux = lastElem(listaC);
				aux->prox = conta;
				listaC->total++;
			}
		}
	}
	// FECHANDO O ARQUIVO	
	fclose(arqs);
}

void percLista(LContas * listaC){
	ContaB * aux = novaConta();
	
	aux = listaC->primeiraConta;
	
	while(aux){
		printf("Codigo: %d | Nome: %s | Saldo: %.2f\n", aux->codigo, aux->nome, aux->saldo);
		aux = aux->prox;
	}
}

void percListaN(LNome * listaN){
	LNome * aux = novaListaNome();
	
	aux = listaN;
	
	while(aux){
		printf("Codigo: %d | Nome: %s | Saldo: %.2f\n", aux->conta->codigo, aux->conta->nome, aux->conta->saldo);
		aux = aux->prox;
	}
}

LNome * posLista(LNome * listaN, int pos){
	int i = 0;
	LNome * aux = listaN;
	
	while (i < pos){
		aux = aux->prox;
		i++;
	}
	
	return aux;
}

void quickSort(LNome * listaN, int tamVet){
	int pEsq = 0;
	int pDir = tamVet-1;
	float pivo = posLista(listaN, tamVet / 2)->conta->saldo;
	
	do{	
		while(posLista(listaN, pEsq)->conta->saldo < pivo ){
			pEsq++;
		}
		while(posLista(listaN, pDir)->conta->saldo > pivo){
			pDir--;
		}
	
		if(pDir >= pEsq){
			ContaB * aux = posLista(listaN, pEsq)->conta;
			posLista(listaN, pEsq)->conta = posLista(listaN, pDir)->conta;
			posLista(listaN, pDir)->conta = aux;
			pDir--;
			pEsq++;
		}	
	}while(pDir >= pEsq);
	
	if (pDir){
		quickSort( listaN, pDir+1);
	}
	
	if (pEsq < tamVet - 1){
		quickSort( posLista(listaN, pEsq), tamVet - pEsq);
	}
}

void bubbleSort (LNome * listaN) {
	LNome * aux1 = listaN;
	LNome * aux2;
	ContaB * aux3;
	
	while(aux1){
		aux2 = aux1->prox;
		while(aux2){
			if(strcmp(aux1->conta->nome, aux2->conta->nome) > 0){
				aux3 = aux1->conta;
				aux1->conta = aux2->conta;
				aux2->conta = aux3;			
			}
			aux2 = aux2->prox;
		}
		aux1 = aux1->prox;
	}
}

int menu(){
	int opt;
	
	printf("\t\t===> MENU <===\n\n");
	printf("1 - Mostrar Lista Inicial\n"\
		   "2 - Mostrar Index Nome\n"\
		   "3 - Mostrar Index Saldo\n"\
		   "4 - Gravar Arquivos\n"\
		   "0 - Sair");
	printf("\nOpcao: ");
	scanf("%d", &opt);	
	system("cls");
	
	return opt;
		
}

void gravarArquivo(LNome * lista, FILE * arqs){
	while(lista){
		fprintf(arqs, "%d;%s;%.2f\n", lista->conta->codigo, lista->conta->nome, lista->conta->saldo);
		lista = lista->prox;
	}
	
}

void chamarBubble(LNome * listaNome){
	printf("===> BUBBLESORT <===\n\n");
	clock_t clock1 = clock();
	printf("Tempo inicial: %g\n", (double) clock1);
	bubbleSort(listaNome);
	clock_t clock2 = clock();
	printf("Tempo final: %g\n", (double) clock2);
	double temp = (clock2 - clock1) / (double)CLOCKS_PER_SEC;
	printf("Tempo gasto: %g s\n", temp);
}

void chamarQuick(LNome * listaSaldo, int tamVet){
	printf("===> QUICKSORT <===\n\n");
	clock_t clock1 = clock();
	printf("Tempo inicial: %g s\n", clock1);
	quickSort(listaSaldo, tamVet);
	clock_t clock2 = clock();
	printf("Tempo final: %g\n", clock2);
	double temp = (clock2 - clock1) / (double)CLOCKS_PER_SEC;
	printf("Tempo gasto: %g s\n", temp);

}

void limparTela(){
	system("pause");
	system("cls");
}
