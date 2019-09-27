#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 40
#define TamVet 500
#define LoadFac 0.7

typedef struct ContaBancaria{
	int codigo;
	char nome[TAM];
	float saldo;
	bool excluido;
	struct ContaBancaria * prox;
}ContaB;

typedef struct Hash{
	ContaB ** vetHash;
	int * vetCol;
	int total;
	int tamHash;
	int totalColisao;
}Hash;

ContaB * novaConta(){
	ContaB * nova = (ContaB*) malloc(sizeof(ContaB));
	nova->excluido = false;
	nova->prox = NULL;
	
	return nova;
}

Hash * novaHash(int tam){
	Hash * hash = (Hash*) malloc (sizeof(Hash));
	int * vetCol = (int*) malloc (sizeof(int));
	hash->vetHash = NULL;
	hash->tamHash = tam;
	hash->total = 0;
	hash->totalColisao = 0;
	
	return hash;
}

Hash * iniciarVet(Hash * hash){
	int i;
	hash->vetHash = (ContaB**) malloc (sizeof(ContaB*) * hash->tamHash);
	hash->vetCol = (int*) malloc (sizeof(int) * hash->tamHash) ;
	
	for(i = 0; i < hash->tamHash; i++){
		hash->vetCol[i] = 0;
		hash->vetHash[i] = NULL;
	}
	
	return hash; 
}

int funcHash(int num, int tam){
	return num%tam;
}

int loadFacAtual(Hash * hash){
	return hash->tamHash * LoadFac;
}

int funcHashDobra(int cod, int tam){
	char codigo[6], num1[2], num2[2], aux[6];
	int int1, int2;

	while(cod > tam){
		itoa(cod, codigo, 10);
		
		num1[1] = codigo[0];
		num1[0] = codigo[1];
		int1 = atoi(num1);

		num2[0] = codigo[2];
		num2[1] = codigo[3];
		int2 = atoi(num2);

		int soma = int1 + int2;
		if(soma >= 100){
			soma -= 100;
		}

		itoa(soma, aux, 10);
		
		aux[0] = '0';
		aux[1] = '0';
		aux[2] = codigo[4];
		aux[3] = codigo[5];

		cod = atoi(aux); 
	}
	return cod;	
}

ContaB * lastCont(ContaB * conta){
	ContaB * nova = conta;

	while(nova->prox){
		nova = nova->prox;
	}
	
	return nova;
}

int qtdColisao(ContaB * conta){
	int col = 1;
	while(conta->prox){
		conta = conta->prox;
		col++;
	}

	return col;
}

Hash * addConta(ContaB * conta, Hash * hash, int tipo){
	int pos, posInicial;
	if(tipo == 1){
		pos = funcHash(conta->codigo, hash->tamHash);
	}else if(tipo == 2){
		pos = funcHashDobra(conta->codigo, hash->tamHash);
	}else{
		printf("Tipo de Hash invalido!!\n");
		return hash;
	}	
	posInicial = pos;
	while(hash->vetHash[pos]){
		pos++;
		hash->totalColisao++;
		hash->vetCol[posInicial]++;
		if(pos >= hash->tamHash){
			pos -= hash->tamHash;
		}
	}
	hash->vetHash[pos] = conta;
	hash->total++;
	
	if(hash->total > loadFacAtual(hash)){
		Hash * aux = novaHash(hash->tamHash * 2);
		aux = iniciarVet(aux);
		aux->totalColisao = hash->totalColisao;
		int i;
		for(i = 0; i < hash->tamHash; i++){
			aux->vetCol[i] = hash->vetCol[i];
			if(hash->vetHash[i])
				aux = addConta(hash->vetHash[i], aux, tipo);
		}
		free(hash);
		hash = aux;
		return hash;
	}
	return hash;
}

Hash * addContaFechada(ContaB * conta, Hash * hash){
	int pos = funcHash(conta->codigo, hash->tamHash);

	if(hash->vetHash[pos]){
		ContaB * auxC1 = lastCont(hash->vetHash[pos]);
		auxC1->prox = conta;
		
		hash->totalColisao += qtdColisao(hash->vetHash[pos]);
		hash->vetCol[pos] += qtdColisao(hash->vetHash[pos]);
	}else{
		hash->vetHash[pos] = conta;
	}
	
	hash->total++;

	if(hash->total > loadFacAtual(hash)){
		Hash * aux = novaHash(hash->tamHash * 2);
		aux = iniciarVet(aux);
		aux->totalColisao = hash->totalColisao;
		ContaB * auxC;
		ContaB * auxC2;
		
		int i;		
		for(i = 0; i < hash->tamHash; i++){
			aux->vetCol[i] = hash->vetCol[i];
			if(hash->vetHash[i]){
				auxC = hash->vetHash[i];
				do {
					aux = addContaFechada(auxC, aux);
					auxC = auxC->prox;					
				} while(auxC);	
			}	
		}
		
		free(hash);
		hash = aux;
		return hash;
	}	
	return hash;
}

ContaB * criarConta(){
	ContaB * conta = novaConta();
	
	do{
		printf("Insira o codigo da conta: \n");
		scanf("%d", &conta->codigo);	
	}while(conta->codigo < 1);
	
	printf("Insira o nome: \n");
	scanf("%s", conta->nome);
	
	printf("Insira o saldo: R$ ");
	scanf("%f", &conta->saldo);

	return conta;
}

Hash * inserirContaFechada(Hash * hash){
	ContaB * conta = criarConta();
	
	hash = addContaFechada(conta, hash);
	
	return hash;
}

void gravarArquivo(Hash * hash, FILE * arqs){
	fprintf(arqs, "Total Elementos: %d;Total Colisões: %d\n", hash->total, hash->totalColisao);
	int i;
	for(i = 0; i < hash->tamHash; i++){
		fprintf(arqs, "Pos: %d;[%d]\n", i, hash->vetCol[i]);
	}
	printf("Arquivo gravado com sucesso!!\n");
}

Hash * carregarArqs(Hash * hash, int op){
	
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
			
			if(op == 1 || op == 2){
				hash = addConta(conta, hash, op);	
			}else if(op == 3){
				hash = addContaFechada(conta, hash);
			}else{
				printf("Opcao nao encontrada!!!\n");
			}
			
		}		
	}
	// FECHANDO O ARQUIVO	
	fclose(arqs);
	
	return hash;
}

Hash * inserirConta(Hash * hash, int tipo){
	ContaB * conta = criarConta();
	
	hash = addConta(conta, hash, tipo);
	
	printf("Conta inserida com sucesso!\n");
	
	return hash;
}

Hash * excluirConta(Hash * hash, ContaB * conta){
	int i;
	for(i = 0; i < hash->tamHash; i++){
		if(hash->vetHash[i] == conta){
			free(hash->vetHash[i]);
			hash->vetHash[i] = novaConta();
			hash->vetHash[i]->excluido = true;
			hash->total--;
		}
	}
	printf("Excluido com sucesso!\n");
	return hash;
}

ContaB * buscarConta(Hash * hash, int tipo){
	int cod, pos;
	do{
		printf("Insira o codigo da conta: ");
		scanf("%d", &cod);
	}while(cod < 1);
	if(tipo == 1){
		pos = funcHash(cod, hash->tamHash);
	}else if(tipo == 2){
		pos = funcHashDobra(cod, hash->tamHash);
	}	
	
	int posInit = pos;
		
	do{
		if(cod == hash->vetHash[pos]->codigo){
			printf("Conta encontrada!!\n");
			return hash->vetHash[pos];
		}else{
			pos++;
			if(pos >= hash->tamHash)
				pos -= hash->tamHash;	
		}
	}while(pos != posInit);
	
	printf("Conta no encontrada!!\n");
	return NULL;
	
}

ContaB * buscarContaFechada(Hash * hash){
	int cod;
	ContaB * aux;
	
	do{
		printf("Insira o codigo da conta: ");
		scanf("%d", &cod);
	}while(cod < 1);
		
	int	pos = funcHash(cod, hash->tamHash);
	aux = hash->vetHash[pos];	
	while(aux){
		if(aux->codigo == cod){
			printf("Conta encontrada!!\n");
			return aux;
		}else{
			aux = aux->prox;
		}
	}
	
	printf("Conta no encontrada!!\n");
	return NULL;
	
}

Hash * excluirContaFechada(Hash * hash){
	ContaB * conta = buscarContaFechada(hash);
	
	if(!conta){
		return hash;
	}
	
	int pos = funcHash(conta->codigo, hash->tamHash);
	ContaB * aux = hash->vetHash[pos];
	
	if(conta == hash->vetHash[pos]){
		hash->vetHash[pos] = hash->vetHash[pos]->prox;
		free(conta);
		hash->total--;
		printf("Total hash: %d", hash->total);
		printf("Conta excluida com sucesso!\n");
		return hash;
	}else{
		while(aux->prox){
			if(aux->prox == conta){
				aux->prox = conta->prox;
				hash->total--;
				printf("Total hash: %d", hash->total);
				free(conta);
				printf("Conta excluida com sucesso!\n");
				return hash;
			}else{
				aux = aux->prox;
			}	
		}
	}
	
	printf("ERRO: Conta nao excluida!\n");
	return hash;
}

void percorrerHash(Hash * hash){
	int i;
	int soma = 0;
	for(i = 0; i < hash->tamHash; i++){
		soma += hash->vetCol[i];
		if(hash->vetHash[i] && !hash->vetHash[i]->excluido){
			printf("Pos: %d  |  Codigo: %d  |  Nome: %s  |  Saldo: %.2f\n", i, hash->vetHash[i]->codigo, hash->vetHash[i]->nome, hash->vetHash[i]->saldo);
		}
	}
	printf("Total contas: %d\nTotal colisoes: %d\n", hash->total, hash->totalColisao);
	printf("Total somado: %d", soma);
}

void percorrerHashFechada(Hash * hash){
	int i;
	ContaB * auxC;
	
	for(i = 0; i < hash->tamHash; i++){
		if(hash->vetHash[i]){
			auxC = hash->vetHash[i];
			do {
				printf("Codigo: %d  |  Nome: %s  |  Saldo: %.2f\n", auxC->codigo, auxC->nome, auxC->saldo);
				auxC = auxC->prox;	
			} while(auxC);				
		}
	}
	printf("Total contas: %d\nTotal colisoes: %d\n", hash->total, hash->totalColisao);
}

void limparTela(){
	system("pause");
	system("cls");
}

int menuPrincipal(){
	printf("\t======= ESCOLHA SUA HASH =======\n");
	printf("\t1 - Hash aberta com MOD\n"\
		   "\t2 - Hash aberta com dobra\n"\
		   "\t3 - Hash fechada\n"\
		   "\t0 - Sair\n");
	printf("Sua escolha: ");
	int op;
	scanf("%d", &op);
	return op;
}

int subMenu(){
	printf("\t======= MENU DA HASH =======\n");
	printf("\t1 - Percorrer hash\n"\
	       "\t2 - Inserir conta\n"\
		   "\t3 - Excluir conta\n"\
		   "\t4 - Buscar conta\n"\
		   "\t0 - Sair\n");
	printf("Sua escolha: ");
	int op;
	scanf("%d", &op);
	return op;
}

int main(){
	FILE * arqMod = fopen("HashAbertaMod.csv", "w");
	Hash * hashMod = novaHash(TamVet);
	hashMod = iniciarVet(hashMod);
	
	FILE * arqDobra = fopen("HashAbertaDobra.csv", "w");
	Hash * hashDobra = novaHash(TamVet);
	hashDobra = iniciarVet(hashDobra);
	
	FILE * arqFechada = fopen("HashFechada.csv", "w");
	Hash * hashFechada = novaHash(TamVet);
	hashFechada = iniciarVet(hashFechada);
	
	int op1, op2;
	while(true){
		limparTela();
		do {
			op1 = menuPrincipal();
			limparTela();
		}while(op1 < 0 || op1 > 3);
		if(op1 == 0)
			exit(1);
		else if(op1 == 1){
			hashMod = carregarArqs(hashMod, 1);
			gravarArquivo(hashMod, arqMod);
		}else if(op1 == 2){
			hashDobra = carregarArqs(hashDobra, 2);
			gravarArquivo(hashDobra, arqDobra);
		}else{
			
			hashFechada = carregarArqs(hashFechada, 3);
			gravarArquivo(hashFechada, arqFechada);
		}
			
		op2 = subMenu();
		switch(op2){
			case 1:
				if(op1 == 1)
					percorrerHash(hashMod);
				else if(op1 == 2)
					percorrerHash(hashDobra);
				else
					percorrerHashFechada(hashFechada);
				limparTela();
				break;
			case 2:
				if (op1 == 1)
					hashMod = inserirConta(hashMod, 1);
				else if (op1 == 2)
					hashDobra = inserirConta(hashDobra, 2);
				else
					hashFechada = inserirContaFechada(hashFechada);
				limparTela();
				break;
			case 3:
				if (op1 == 1){
					ContaB * conta = buscarConta(hashMod, 1);
					hashMod = excluirConta(hashMod, conta);
				}					
				else if (op1 == 2){
					ContaB * conta = buscarConta(hashDobra, 2);
					hashDobra = excluirConta(hashDobra, conta);
				}					
				else
					hashFechada = excluirContaFechada(hashFechada);
				limparTela();
				break;
			case 4:
				if (op1 == 1){
					ContaB * nova = buscarConta(hashMod, 1);
					if(nova)	
						printf("Cod: %d  |  Nome: %s  |  Saldo: %.2f\n", nova->codigo, nova->nome, nova->saldo);
				}
				else if (op1 == 2){
					ContaB * nova = buscarConta(hashDobra, 2);
					if(nova)	
						printf("Cod: %d  |  Nome: %s  |  Saldo: %.2f\n", nova->codigo, nova->nome, nova->saldo);
				}
				else{
					ContaB * nova = buscarContaFechada(hashFechada);
					if(nova)	
						printf("Cod: %d  |  Nome: %s  |  Saldo: %.2f\n", nova->codigo, nova->nome, nova->saldo);
				}
				limparTela();
				break;
			case 0:
				limparTela();
				break;
			default:
				printf("ERRO: Tente novamente!\n");				
		}
	}
	return 0;	
}


