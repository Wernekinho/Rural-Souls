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
