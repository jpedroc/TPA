#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM 50

typedef struct ContaBancaria{
    int codigo;
    char nome[TAM];
    float saldo;
}ContaB;

typedef struct No{
    int h;
	ContaB * conta;
    struct No * pai;
    struct No * dir;
    struct No * esq;
}No;

No * raiz;

void limparTela();
ContaB * novaConta();
No * iniciarNo(ContaB * conta);
ContaB * preencherConta(ContaB * conta);
int calcAltura (No * raiz);
No * insereNodo(No * raiz, ContaB * conta);
No * removeNodo(No * raiz, ContaB * conta);
No * carregarArqs(No * raiz);
void printaCrescente(No *nodo);
void printaDecrescente(No *nodo);
ContaB * buscaConta(No *nodo);
ContaB * buscar(No * nodo, int codigo);

main(void){
}

void limparTela(){
	system("pause");
	system("cls");
}

ContaB * novaConta(){
	ContaB * nova = (ContaB*) malloc(sizeof(ContaB));
	return nova;
}

No * iniciarNo(ContaB * conta){
    No * novo = (No*) malloc (sizeof(No));
    
    novo->conta = conta;
    novo->pai = NULL;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

ContaB * preencherConta(ContaB * conta){
	printf("\nCodigo: ");
	scanf("%d", &conta->codigo);
	printf("\nNome: ");
	scanf("%s", &conta->nome);
	printf("\nSaldo: ");
	scanf("%f", &conta->saldo);
	
	return conta;
}

int calcAltura (No * raiz) {
   if (!raiz) 
      return -1; // altura da árvore vazia
   else {
      int he = calcAltura(raiz->esq);
      int hd = calcAltura(raiz->dir);
      return hd > he ? (hd +1)  : (he + 1);
   }
}

No * insereNodo(No * raiz, ContaB * conta){
   
    if(raiz == NULL){ // raiz vazia
		raiz = iniciarNo(conta);
		raiz->h = 0;
		
	} 
	else if(conta->codigo < raiz->conta->codigo){ // conta será adicionada a esquerda
		raiz->esq = insereNodo(raiz->esq,conta);
		raiz->esq->pai = raiz;
	} 
	else { // conta será adicionada a direita
	    raiz->dir = insereNodo(raiz->dir, conta);
	    raiz->dir->pai = raiz;
	}
	
	raiz = raiz->pai;	
}

No * removeNodo(No * raiz, ContaB * conta){
    if(raiz == NULL){		// raiz vazia
		return NULL;
	} 
	else if (raiz->conta->codigo > conta->codigo){		// desce pela esquerda
		raiz->esq = removeNodo(raiz->esq, conta);
	} 
	else if (raiz->conta->codigo < conta->codigo){		// desce pela direita		
		raiz->dir = removeNodo(raiz->dir, conta);
	} 
	else {					// Encontrou Nó a ser removido...
		if(raiz->dir == NULL && raiz->esq == NULL){			// Nó a ser removido NÃO tem FILHOS.
			free(raiz);
			raiz = NULL;
		} 
		else if(raiz->esq == NULL){			// Nó a ser removido tem apenas FILHO à DIREITA.
			No *temporario = raiz;
			raiz->dir->pai = raiz->pai;
			raiz = raiz->dir;
			free(temporario);
		} 
		else if(raiz->dir == NULL){			//Nó a ser removido tem apenas FILHO à ESQUERDA.
			No *temporario = raiz;
			raiz->esq->pai = raiz->pai;
			raiz = raiz->esq;
			free(temporario);
		} 
		else {			// Nó tem dois FILHOS.
			No *aux = raiz->esq;
			while (aux->dir){ // maior nodo a direita
				aux = aux->dir;
			}
			//Trocando os dados entre os nodos ...
			raiz->conta = aux->conta;
			aux->conta = conta;
			
			raiz->esq = removeNodo(raiz->esq, conta);
		}
	}
	return raiz;
}

No * carregarArqs(No * raiz){
	
	// ABRINDO O ARQUIVO
	
	FILE * arqs;
	arqs = fopen ("../DadosBancoPulini.txt", "r");
	
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

		if(cod != 0){
			conta->codigo = cod;
			
			strcpy(conta->nome, nome);
			
			aux1 = atof (saldo);
			conta->saldo = aux1;
	
			raiz = insereNodo(raiz, conta);			
		}
	}
	// FECHANDO O ARQUIVO	
	fclose(arqs);

   	return raiz;
   
	
}

void printaCrescente(No *nodo){
	if (nodo != NULL){
		if (nodo->pai != NULL){
			printaCrescente(nodo->esq);
	   		printf("\tPai:%d  -  Codigo:%d  -  Nome:%s  -  Saldo:%.2f\n", nodo->pai->conta->codigo, nodo->conta->codigo, nodo->conta->nome, nodo->conta->saldo);
	   		printaCrescente(nodo->dir);
	   }else{
	   		printaCrescente(nodo->esq);
	   		printf("\tPai:NULL  -  Codigo:%d  -  Nome:%s  -  Saldo:%.2f\n", nodo->conta->codigo, nodo->conta->nome, nodo->conta->saldo);
	   		printaCrescente(nodo->dir);	
	   }   
	}
}

void printaDecrescente(No *nodo){
	if (nodo != NULL){
		if (nodo->pai != NULL){
			printaDecrescente(nodo->dir);
	   		printf("\tPai:%d  -  Codigo:%d  -  Nome:%s  -  Saldo:%.2f\n", nodo->pai->conta->codigo, nodo->conta->codigo, nodo->conta->nome, nodo->conta->saldo);
	   		printaDecrescente(nodo->esq);
	   }else{
	   		printaDecrescente(nodo->dir);
	   		printf("\tPai:NULL  -  Codigo:%d  -  Nome:%s  -  Saldo:%.2f\n", nodo->conta->codigo, nodo->conta->nome, nodo->conta->saldo);
	   		printaDecrescente(nodo->esq);	
	   }   
	}
}

ContaB * buscaConta(No *nodo){
	if(nodo == NULL){
	    printf("Raiz vazia... Impossivel fazer busca!\n");
	    system("PAUSE");
	    return NULL;
	} 
	else {		
		int codigo;
		printf("\nInsira o codigo da conta: ");
		scanf("%d", &codigo);
				
		return buscar(nodo, codigo);			
	}	
}

ContaB * buscar(No *nodo, int codigo){
        	
    if (nodo == NULL){
    	printf("\nConta não encontrada!\n");
    	return NULL;
	} 
	else if (nodo->conta->codigo == codigo){
    	printf("\nConta encontrada!\n");
      	return nodo->conta;
	} 
	else if (nodo->conta->codigo > codigo){
		buscar(nodo->esq, codigo);
	} else {
		buscar(nodo->dir, codigo);
	}
	
}

No * rotRR(No * raiz){
	No * aux = raiz->esq;
	
	raiz->esq = aux->dir;
	
	if(aux->dir)
		aux->dir->pai = raiz;
	
	aux->dir = raiz;
	aux->pai = raiz->pai;
	
	if(raiz->pai){
		if (raiz->pai->esq == raiz)	
			raiz->pai->esq = aux;
		else
			raiz->pai->dir = aux;	
	}
	raiz->pai = aux;
	
	aux->h = calcAltura(aux->esq) - calcAltura(aux->dir);
	raiz->h = calcAltura(raiz->esq) - calcAltura(raiz->dir);
	
	return aux;
}

No * rotLL(No * raiz){
	No * aux = raiz->dir;
	
	raiz->dir = aux->esq;
	
	if(aux->esq)
		aux->esq->pai = raiz;
	
	aux->esq = raiz;
	aux->pai = raiz->pai;
	
	if(raiz->pai){
		if (raiz->pai->dir == raiz)	
			raiz->pai->dir = aux;
		else
			raiz->pai->esq = aux;	
	}
	raiz->pai = aux;
	
	aux->h = calcAltura(aux->esq) - calcAltura(aux->dir);
	raiz->h = calcAltura(raiz->esq) - calcAltura(raiz->dir);
	
	return aux;
}


