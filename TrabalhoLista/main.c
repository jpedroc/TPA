#include <stdio.h>
#include <stdlib.h>
#include "funcoes.c"

int main(){
	int opt;
	LContas * lista = novaLista();
	LNome * listaNome = novaListaNome();
	LNome * listaSaldo = novaListaNome();
	
	FILE * arqNome = fopen("IndexNome.csv", "w");
	FILE * arqSaldo = fopen("IndexSaldo.csv", "w");
	
	carregarArqs(lista);
	
	listaNome = iniciarListaNome(lista, listaNome);
	chamarBubble(listaNome);
	listaSaldo = iniciarListaNome(lista, listaSaldo);
	chamarQuick(listaSaldo, lista->total);	
	limparTela();
	
	
	while (1){
		opt = menu();
	
		switch(opt){
			case 0:
				exit(0);
			case 1: 
				printf("===> LISTA INICIAL <===\n\n");
				percLista(lista);
				limparTela();
				break;
			case 2: 
				printf("===> INDEX NOME <===\n\n");
				percListaN(listaNome);
				limparTela();
				break;
			case 3: 
				printf("===> INDEX SALDO <===\n\n");
				percListaN(listaSaldo);
				limparTela();
				break;
			case 4:
				printf("===> GRAVAR ARQUIVOS <===\n");
				gravarArquivo(listaNome, arqNome);
				printf("Arquivo IndexNome.csv gravado com sucesso!!!\n");
				gravarArquivo(listaSaldo, arqSaldo);
				printf("Arquivo IndexSaldo.csv gravado com sucesso!!!\n");
				limparTela();
				break;				
			default:
				printf("Opcao Invalida!!!");
				limparTela();
				break;
		}	
	}
	return 0;
}