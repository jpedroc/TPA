#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM 50 

/*

	EM CONSTRU��O
	
*/

typedef struct ContaBancaria{
    int codigo;
    char nome[50];
    float saldo;
}ContaB;

typedef struct No{
    ContaB * conta;
    struct No * pai;
    struct No * dir;
    struct No * esq;
}No;

No * raiz;

// ASSINATURAS DAS FUN��ES
ContaB * novaConta();
No * iniciarArvore();
No * iniciarNo(ContaB * conta);
No * insereNodo(No ** raiz, ContaB * conta);
No * removeNodo(No * raiz, ContaB * conta);
No * carregarArqs(No * raiz);
void caminhamento_em_ordem(No *nodo);
ContaB * buscaConta(No *nodo);
ContaB * buscar(No *nodo, int codigo);


// MAIN

main(void){
	
	raiz = iniciarArvore();
	raiz = carregarArqs(raiz);
	
	caminhamento_em_ordem(raiz);
	
	ContaB * conta = novaConta();
	conta = buscaConta(raiz);
	
	if(conta)
		printf("Codigo: %d  -  Nome: %s  -  Saldo: %.2f", conta->codigo, conta->nome, conta->saldo);
	    
	raiz = removeNodo(raiz, conta);
	
	
	caminhamento_em_ordem(raiz);   
}

// FUNÇÕES

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

No * iniciarArvore(){
    return NULL;
}

No * insereNodo(No ** raiz, ContaB * conta){
   
    if((*raiz) == NULL){ // raiz vazia
		(*raiz) = iniciarNo(conta);
	} 
	else if(conta->codigo < (*raiz)->conta->codigo){ // conta ser� adicionada a esquerda
		(*raiz)->esq = insereNodo(&(*raiz)->esq,conta);
		(*raiz)->esq->pai = (*raiz);
	} 
	else { // conta ser� adicionada a direita
	    (*raiz)->dir = insereNodo(&(*raiz)->dir, conta);
	    (*raiz)->dir->pai = (*raiz);
	}
	
	return (*raiz);	
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
	else {					// Encontrou N� a ser removido...
		if(raiz->dir == NULL && raiz->esq == NULL){			// N� a ser removido N�O tem FILHOS.
			free(raiz);
			raiz = NULL;
		} 
		else if(raiz->esq == NULL){			// N� a ser removido tem apenas FILHO � DIREITA.
			No *temporario = raiz;
			raiz->dir->pai = raiz->pai;
			raiz = raiz->dir;
			free(temporario);
		} 
		else if(raiz->dir == NULL){			//N� a ser removido tem apenas FILHO � ESQUERDA.
			No *temporario = raiz;
			raiz->esq->pai = raiz->pai;
			raiz = raiz->esq;
			free(temporario);
		} 
		else {			// N� tem dois FILHOS.
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

		if(cod != 0){
			conta->codigo = cod;
			
			strcpy(conta->nome, nome);
			
			aux1 = atof (saldo);
			conta->saldo = aux1;

			raiz = insereNodo(&raiz, conta);
			
		}
	}
	// FECHANDO O ARQUIVO	
	fclose(arqs);

    return raiz;
}

void caminhamento_em_ordem(No *nodo){
	if (nodo != NULL){
		if (nodo->pai != NULL){
			caminhamento_em_ordem(nodo->esq);
	   		printf("\n\tNodo:(%d) |--| Pai:(%d).", nodo->conta->codigo, nodo->pai->conta->codigo);
	   		caminhamento_em_ordem(nodo->dir);
	   }else{
	   		caminhamento_em_ordem(nodo->esq);
	   		printf("\n\tNodo:(%d) |--| Pai:(NULL).", nodo->conta->codigo);
	   		caminhamento_em_ordem(nodo->dir);	
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
		printf("Insira o codigo da conta: ");
		scanf("%d", &codigo);
		
		return buscar(nodo, codigo);			
	}	
}

ContaB * buscar(No *nodo, int codigo){
        	
    if (nodo == NULL){
    	printf("\nConta n�o encontrada!\n");
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

