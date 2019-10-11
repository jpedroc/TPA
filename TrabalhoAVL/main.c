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
void menu();
ContaB * novaConta();
No * iniciarNo(ContaB * conta);
ContaB * preencherConta();
int calcAltura (No * raiz);
No * insereNodo(No * raiz, ContaB * conta);
No * removeNodo(No * raiz, ContaB * conta);
No * carregarArqs(No * raiz);
void printaCrescente(No *nodo);
void printaDecrescente(No *nodo);
ContaB * buscaConta(No *nodo);
ContaB * buscar(No * nodo, int codigo);
No * rotLL(No * nodo);
No * rotRR(No * nodo);
No * balanceia(No * raiz);
void exibirNiveis(No * raiz);
void exibirNiveisB(No * raiz, int atual, int cont);

main(void){
	raiz = carregarArqs(raiz);
	ContaB * conta;
	int opt;
	do{
		int aux1;
		menu();
		scanf("%d", &opt);
		
		switch(opt){
			case 1:
				conta = preencherConta();
				raiz = insereNodo(raiz, conta);
				limparTela();
				break;
			case 2:
				conta = buscaConta(raiz);
				printf("\nCodigo: %d  -  Nome: %s  -  Saldo: %.2f\n", conta->codigo, conta->nome, conta->saldo);
				limparTela();
				break;
			case 3: 
				conta = buscaConta(raiz);
				raiz = removeNodo(raiz, conta);
				limparTela();
				break;
			case 4:
				exibirNiveis(raiz);
				limparTela();
				break;
			case 0:
				exit(1);
			default:
				printf("\nOpcao inválida!!\n");
		}
	}while(opt);
}

void limparTela(){
	system("pause");
	system("cls");
}

void menu(){
	printf("1 - Inserir\n"\
		   "2 - Pesquisar\n"\
		   "3 - Excluir\n"\
		   "4 - Listar por nivel\n"\
		   "0 - Sair\n"\
		   "Escolha sua opcao: ");
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
    novo->h = 0;

    return novo;
}

ContaB * preencherConta(){
	ContaB * conta = novaConta();
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
      return 0; // altura da Ã¡rvore vazia
   else {
      int he = calcAltura(raiz->esq);
      int hd = calcAltura(raiz->dir);
      return hd > he ? (hd +1)  : (he + 1);
   }
}

No * insereNodo(No * raiz, ContaB * conta){
	
	if(!raiz){ // raiz vazia
		raiz = iniciarNo(conta);
		raiz->h = 0;
		No * aux1 = raiz;	
	
		while(aux1){
			aux1->h = calcAltura(aux1->esq) - calcAltura(aux1->dir);
			
			if(aux1->h > 1 || aux1->h < -1){
				aux1 = balanceia(aux1);
			}
	        aux1 = aux1->pai;
		}
	} 
	else if(conta->codigo < raiz->conta->codigo){ // conta serÃ¡ adicionada a esquerda
		raiz->esq = insereNodo(raiz->esq,conta);
		raiz->esq->pai = raiz;
	} 
	else { // conta serÃ¡ adicionada a direita
	    raiz->dir = insereNodo(raiz->dir, conta);
		raiz->dir->pai = raiz;
	}
    
    
	
	

	return raiz;	
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
	else {					// Encontrou NÃ³ a ser removido...
		if(raiz->dir == NULL && raiz->esq == NULL){			// NÃ³ a ser removido NÃƒO tem FILHOS.
			free(raiz);
			raiz = NULL;
		} 
		else if(raiz->esq == NULL){			// NÃ³ a ser removido tem apenas FILHO Ã  DIREITA.
			No *temporario = raiz;
			raiz->dir->pai = raiz->pai;
			raiz = raiz->dir;
			free(temporario);
		} 
		else if(raiz->dir == NULL){			//NÃ³ a ser removido tem apenas FILHO Ã  ESQUERDA.
			No *temporario = raiz;
			raiz->esq->pai = raiz->pai;
			raiz = raiz->esq;
			free(temporario);
		} 
		else {			// NÃ³ tem dois FILHOS.
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
    	printf("\nConta nÃ£o encontrada!\n");
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

No * rotRR(No * nodo){
	No * aux = nodo->esq;
	nodo->esq = aux->dir;
	aux->dir = nodo;
	if(nodo->esq)
		nodo->esq->pai = nodo;
	aux->pai = nodo->pai;
	nodo->pai = aux;
	if (raiz == nodo)
		raiz = aux;
	if(aux->pai){
		if(aux->h < aux->pai->h)
			aux->pai->esq = aux;
		else
			aux->pai->dir = aux;	
	}
	nodo->pai = aux;
	
	aux->h = calcAltura(aux->esq) - calcAltura(aux->dir);
	nodo->h = calcAltura(nodo->esq) - calcAltura(nodo->dir);
	
	return aux;
}

No * rotLL(No * nodo){
	No * dir = nodo->dir;
	
	nodo->dir = dir->esq;
	
	dir->esq = nodo;
	
	if(nodo->dir){
		nodo->dir->pai = nodo;
	}
	dir->pai = nodo->pai;
	nodo->pai = dir;
	if (raiz == nodo){
		raiz = dir;
	}
	if(dir->pai){
		if(dir->h < dir->pai->h){
			dir->pai->esq = dir;
		}
		else{
			dir->pai->dir = dir;	
		}
	}
	nodo->pai = dir;
	
	dir->h = calcAltura(dir->esq) - calcAltura(dir->dir);
	nodo->h = calcAltura(nodo->esq) - calcAltura(nodo->dir);
	
	return dir;
}


No * balanceia(No * aux){
	if(aux->h < -1){
		if(aux->esq && aux->esq->h > 0){
            aux->esq = rotLL(aux->esq);
        }        
        aux = rotRR(aux->dir);
    }
	
    else if(aux->h > 1){
		if(aux->dir && aux->dir->h < 0){
            aux->dir = rotRR(aux->dir);
        }
        aux = rotLL(aux);
    }
    return aux;
}

void exibirNiveis(No * aux){
    int i;

    for(i = 0; i <= calcAltura(raiz); i++){
        printf("Nivel %d\n", i);
        exibirNiveisB(raiz, i, 0);
        printf("\n\n");
    }
}

void exibirNiveisB(No * aux, int atual, int cont){
    if(!raiz){
        printf("A arvore esta vazia!");
        getchar();
        return;
    }

    if(aux){
        exibirNiveisB(aux->esq, atual, cont+1);

        if(atual == cont){
            printf("%d ", aux->conta->codigo);
        }

        exibirNiveisB(aux->dir, atual, cont+1);
    }
}
