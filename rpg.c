#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <math.h>
#include "rpg.h"

void printascii(const char *filename) {
    FILE *input;
    char ch;
    input = fopen(filename, "rt");
    while(!feof(input)) {
        fscanf(input,"%c", &ch);
        printf("%c", ch);
    }
    fclose(input);
}

void sorte(Personagem *p){
	srand(time(NULL));
	int dado = (rand() % 6) + 1;
	printf("Sua sorte agora é de: %d", dado);
	p->sorte = dado;
}

void criarPersonagem(Personagem *p){
	printf("\nQual seu nome?\n");
	scanf(" %[^\n]", p->nome);
	
	p->vida = 100;
	p->exp = 0;
	strcpy(p->itens[0], "Caneta Bic azul");
	
	for (int i = 1; i < 5; i++){
		strcpy(p->itens[i], "");
	}
	
	printf(GREEN "\nPersonagem '%s' criado. Bem vindo(a)!\n" RESET, p->nome);
	printf("Vida: %d | Experiencia: %d\n", p->vida, p->exp);
	printf("Item inicial: %s\n", p->itens[0]);
	sorte(p);
}

void interagirComPosner(Personagem *p) {
	printf("\nDesconhecido: \"Olá, vi que você está com dificuldades em Cicuitos...\"\n");
	printf("Desconhecido: \"Prazer, me chamo Daniel Fabio Posner, ou apenas Posner. Pegue essa calculadora!\"\n");
	printf("Você recebeu +2 de QI!\n");
	p->QI += 2;

	int escolha;
	do {
		printf("\nEscolha uma opção:\n");
		printf("1 - Sobre minha vida\n");
		printf("2 - Sobre a sorte\n");
		printf("3 - O que você faz?\n");
		printf("4 - Como passo de desafios/provas?\n");
		printf("5 - Já perguntei o suficiente\n");
		scanf("%d", &escolha);
		getchar();

		switch (escolha){
		case 1:
			printf("Posner: \"Bom, se sua vida zerar, seu ano reinicia. Simples assim.\n");
			break;
		case 2:
			printf("Posner: \"Sorte é um dado. Se o valor for <= sua sorte, deu sorte. Senão, azar. Toda vez que testa sorte, perde 1 ponto.\"\n");
			break;
		case 3:
			printf("Posner: \"Digamos que sou um facilitador de vidas.\"\n");
			break;
		case 4:
			printf("Posner: \"Rola um dado e some ao seu QI. Se for maior que o QI da lista/prova, você acerta. Caso contrário, erra. Empate = dado mútuo.\"\n");
			break;
		case 5:
			printf("Posner: \"Tudo bem, se precisar de mais ajuda, é só chamar.\"\n");
			break;
		default:
			printf("Escolha inválida.\n");
		}
	} while (escolha != 5);
}