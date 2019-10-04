#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TAM 50 
#define TAMVET 500000

/*

	EM CONSTRUÇÃO

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

// ASSINATURAS DAS FUNÇÕES
void limparTela();
int menu();
ContaB * novaConta();
No ** iniciarHash();
No * iniciarArvore();
No * iniciarNo(ContaB * conta);
No * insereNodo(No * raiz, ContaB * conta);
No * removeNodo(No * raiz, ContaB * conta);
No * carregarArqs(No * raiz);
void printaCrescente(No *nodo);
void printaDecrescente(No *nodo);
ContaB * buscaConta(No *nodo);
ContaB * buscar(No * nodo, int codigo);
int calcAltura(No * raiz);
int verificarEstBinaria(No * raiz);
int nivelConta(No * raiz, ContaB * conta);
int contaNos(No * raiz);
ContaB * preencherConta(ContaB * conta);
int verificarCompleta(No * raiz);
ContaB ** inserirVetor(ContaB ** vetor, ContaB * conta);
ContaB ** carregarVetor(ContaB ** vetor);
ContaB ** iniciarVetor();
int verificarEspelho(ContaB ** vetor, No * raiz, int pos);
No ** inserirHash(No ** hash, ContaB * conta);
No ** carregarHash(No ** hash, No * raiz);
int buscaVetor(ContaB ** vetor, int codigo);
int buscaHash (No ** hash, int codigo);
void compararBusca(No ** hash, ContaB ** vetor);

// MAIN

main(void){
	
	
	raiz = iniciarArvore();
	No ** hash = iniciarHash();
	raiz = carregarArqs(raiz);
	ContaB * conta = novaConta();
	ContaB * contaAux = novaConta();
	int eb, cp, opt;
	ContaB ** vetConta = iniciarVetor();
	
	vetConta = carregarVetor(vetConta);
	hash = carregarHash(hash, raiz);
	
	do{
		system("cls");
		opt = menu();
		
		switch(opt){
			case 0:
				limparTela();
				break;
			case 1: 
				printf("\nA altura da arvore eh: %d\n", calcAltura(raiz));
				limparTela();
				break;
			case 2: 
				conta = buscaConta(raiz);
				printf("\nO nivel da conta eh: %d\n", nivelConta(raiz, conta));
				limparTela();
				break;
			case 3:
				eb = verificarEstBinaria(raiz);
				if (eb)	
					printf("\nArvore estritamente binaria!!\n");
				else
					printf("\nArvore nao eh estritamente binaria!!\n");
				limparTela();
				break;
			case 4:
				eb = verificarEstBinaria(raiz);
				cp  = verificarCompleta(raiz);
				if (eb && cp)
					printf("\nArvore estritamente binaria e completa!\n");
				else
					printf("\nArvore nao eh estritamente binaria e compelta!\n");
				limparTela();
				break;
			case 5:
				printf("\nO numero de nos nas arvore eh: %d\n", contaNos(raiz));
				limparTela();
				break;
			case 6:
				printaDecrescente(raiz);
				limparTela();
				break;
			case 7:
				printaCrescente(raiz);
				limparTela();
				break;
			case 8:
				conta = buscaConta(raiz);
				printf("\nCodigo: %d  -  Nome: %s  -  Saldo: %.2f\n", conta->codigo, conta->nome, conta->saldo);
				limparTela();
				break;
			case 9:
				contaAux = preencherConta(conta);
				raiz = insereNodo(raiz, contaAux);
				limparTela();
				break;
			case 10:
				conta = buscaConta(raiz);
				raiz = removeNodo(raiz, conta);
				limparTela();
				break;
			case 11: 
				if(verificarEspelho(vetConta, raiz, 0))
					printf("Arvores sao similares!\n");
				else
					printf("Arvores nao sao similares!\n");
				limparTela();
				break;
			case 12:
				// funcao faltando
				compararBusca(hash, vetConta);
				limparTela();
				break;
			default:
				printf("\nOpcao invalida... Tente novamente!\n");
		}	
	}while(opt != 0);
}

// FUNÇÕES

void limparTela(){
	system("pause");
	system("cls");
}

int menu(){
	printf("1 - Exibir altura da arvore\n"\
		   "2 - Exibir nivel de um no\n"\
		   "3 - Verificar se arvore eh binaria\n"\
		   "4 - Verificar se arvore eh binaria e completa\n"\
		   "5 - Contar nos\n"\
		   "6 - Imprimir(Descrescente)\n"\
		   "7 - Imprimir(Crescente)\n"\
		   "8 - Buscar conta\n"\
		   "9 - Inserir conta\n"\
		   "10 - Excluir conta\n"\
		   "11 - Arvore Similar\n"\
		   "12 - Comparar busca vetTree - hashTree\n"\
		   "0 - Sair\n"\
		   "Escolha sua opcao: ");
	int opt;
	do{
		scanf("%d", &opt);
	}while(opt < 0 || opt > 12);
	
	return opt;
}

ContaB * novaConta(){
	ContaB * nova = (ContaB*) malloc(sizeof(ContaB));
	return nova;
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

ContaB ** iniciarVetor(){
	int i;
	ContaB ** vetor = (ContaB**) malloc(TAMVET * sizeof(ContaB*));
	for(i = 0; i < TAMVET; i++)
		vetor[i] = NULL;
	return vetor;	
}

No ** iniciarHash(){
	int i;
	No ** hash = (No**) malloc(TAM * sizeof(No*));
	for(i = 0; i < TAM; i++){
		hash[i] = NULL;
	}
	return hash;
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

No * insereNodo(No * raiz, ContaB * conta){
   
    if(raiz == NULL){ // raiz vazia
		raiz = iniciarNo(conta);
	} 
	else if(conta->codigo < raiz->conta->codigo){ // conta será adicionada a esquerda
		raiz->esq = insereNodo(raiz->esq,conta);
		raiz->esq->pai = raiz;
	} 
	else { // conta será adicionada a direita
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

int calcAltura (No * raiz) {
   if (!raiz) 
      return -1; // altura da árvore vazia
   else {
      int he = calcAltura(raiz->esq);
      int hd = calcAltura(raiz->dir);
      return hd > he ? (hd +1)  : (he + 1);
   }
}

int nivelConta(No * raiz, ContaB * conta){
	if (raiz == NULL) {
	  printf("Árvore vazia!!!\n");
	  return -1;
	}
	else {
		No * aux = raiz;
		int h = 0;
		while(aux){
			if(aux->conta->codigo > conta->codigo){
				aux = aux->esq;
				h++;
			}
			else if (aux->conta->codigo < conta->codigo){
				aux = aux->dir;
				h++;
			}
			else {
				return h+1;
			}
		}
   }
   return -1;
}

int verificarEstBinaria(No * raiz){
	if (raiz){
		if((!raiz->dir && raiz->esq) || (raiz->dir && !raiz->esq)){ // se possui apenas um filho, pare.
			return 0;
		}else{ // se possui dois filhos, continue..
			return verificarEstBinaria(raiz->esq) && verificarEstBinaria(raiz->dir);
		}
	}
	else
		return 1;
}

int contaNos(No * raiz){
	if (!raiz){
		return 0;
	}
	else {
		int nEsq = contaNos(raiz->esq);
		int nDir = contaNos(raiz->dir);
		return nDir + nEsq + 1;
	}
}

int verificarCompleta(No * raiz){
	if (!raiz) return 1;
	else{
		int tamIgual = 0;
		int hEsq = calcAltura(raiz->esq); // verifica subarvores esquerda
		int hDir = calcAltura(raiz->dir); // verifica subarvores direita
		
		if (hEsq == hDir || hEsq == hDir+1) tamIgual=1;
		
		return tamIgual && verificarCompleta(raiz->esq) && verificarCompleta(raiz->dir); // retorna 0 ou 1
	}
}

ContaB ** inserirVetor(ContaB ** vetor, ContaB * conta){
	int i = 0;
	while(i < TAMVET){
		if (!vetor[i]) {
			vetor[i] = conta;
			return vetor;
		}	
		else if (conta->codigo >= vetor[i]->codigo){
			i = 2*i+1;
		}
		else if (conta->codigo < vetor[i]->codigo){
			i = 2*i+2;
		}
	}
	return vetor;
}

ContaB ** carregarVetor(ContaB ** vetor){
	// ABRINDO O ARQUIVO
	
	FILE * arqs;
	arqs = fopen ("../DadosBancoPulini2.txt", "r");
	
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
			
			vetor = inserirVetor(vetor, conta);
		}
		
	}

	fclose(arqs);

	return vetor;
}

int verificarEspelho(ContaB ** vetor, No * raiz, int pos){
		
	if ((!vetor[pos] && !raiz) || (vetor[pos] == raiz->conta)){
		return 1;
	}
	if (!vetor[pos] || !raiz){
		return 0;
	}
	
	
	if (raiz && vetor[pos]){
		return (verificarEspelho(vetor, raiz->dir, 2*pos+1) && verificarEspelho(vetor, raiz->esq, 2*pos+2));
	}	
}

int funcHash(int codigo){
	return codigo % TAM;
}

No ** inserirHash(No ** hash, ContaB * conta){
	int pos = funcHash(conta->codigo);
	hash[pos] = insereNodo(hash[pos], conta);
		
	return hash;
}

No ** carregarHash(No ** hash, No * raiz){
	
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
			
			hash = inserirHash(hash, conta);
			
		}
	}
	// FECHANDO O ARQUIVO	
	fclose(arqs);
	
    return hash;
}

int buscaHash(No ** hash, int codigo){
	double temp;
	int pos = funcHash(codigo);
	printf("\nBuscando na hash...\n");
	clock_t time1 = clock();
	No * aux = hash[pos];
	while(aux){
		if(aux->conta->codigo == codigo){
			clock_t time2 = clock();
			printf("Conta encontrada!!\n");
			temp =  (time2 - time1) / (double)CLOCKS_PER_SEC;
			printf("Tempo de busca: %gs\n", temp);
			return 1;
		}
		else if (codigo > aux->conta->codigo){
			aux = aux->dir;
		}
		else {
			aux = aux->esq;
		}
	}
	printf("Conta inexistente!!!\n");
	return 0;
	}

int buscaVetor(ContaB ** vetor, int codigo){
	int i=0;
	double temp;
	printf("\nBuscando no vetor...\n");
	clock_t time1 = clock();	
	while(vetor[i]){
		if (vetor[i]->codigo == codigo) {
			clock_t time2 = clock();
			printf("Conta encontrada!!\n");
			temp =  (time2 - time1) / (double)CLOCKS_PER_SEC;
			printf("Tempo de busca: %gs\n", temp);
			printf("Codigo: %d  -  Nome: %s  -  Saldo: %.2f", vetor[i]->codigo, vetor[i]->nome, vetor[i]->saldo);
			return 1;
		}	
		else if (codigo > vetor[i]->codigo){
			i = 2*i+1;
		}
		else if (codigo < vetor[i]->codigo){
			i = 2*i+2;
		}
	}
	printf("Conta inexistente!\n");
	return 0;
}

void compararBusca(No ** hash, ContaB ** vetor){
	int codigo, th, tv;
	printf("\nCodigo: ");
	scanf("%d", &codigo);
	
	// buscando na hash
	th = buscaHash(hash, codigo);
	system("pause");
	// buscando no vetor
	tv = buscaVetor(vetor, codigo);
	
}


