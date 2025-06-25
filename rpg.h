#ifndef RPG_H
#define RPG_H

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define BOLD    "\x1b[1m"

typedef struct Marcel_ {
    int QI;
    int vida;
} Marcel;

typedef struct Personagem_ {
	char nome[50];
	int vida;
	int exp;
	char itens[5][30];
	int QI;
	int sorte;
} Personagem;

void printascii(const char *filename);
void sorte(Personagem *p);
void criarPersonagem(Personagem *p);

#endif