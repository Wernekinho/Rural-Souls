#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#ifdef _WIN32
	#include <windows.h>
	void wait(int ms) { Sleep(ms); }
#else
	#include <unistd.h>
	void wait(int ms) { usleep(ms * 1000); }
#endif

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define BOLD    "\x1b[1m"

typedef struct{
	char nome[50];
	int vida;
	int exp;
	char itens[5][30];
	int QI;
	int sorte;
} Personagem;

void sorte(Personagem *p){
	srand(time(NULL));
	int dado = (rand() % 6) + 1;
	printf("Sua sorte agora é de: %d", dado);
	p->sorte = dado;
}

void animacao(){
	char titulo[]= "AS AVENTURAS DE LUIZIN EM: P.E, A MATERIA DESTRUIDORA";
	int comprimento = strlen(titulo);
	printf("\n" YELLOW BOLD "%s\n" RESET, titulo);
	fflush(stdout);
	
	for (int i = 0; i < comprimento; i++) {
		printf(YELLOW BOLD "-" RESET);
		fflush(stdout);
		wait(100);
	}
	
	printf("\n\n");
}

void Menu(){
	printf(CYAN "Seja Bem-Vindo!.\n" RESET);
	printf(CYAN "1. Novo Jogo\n" RESET);
	printf(CYAN "2. Carregar Jogo\n" RESET);
	printf(CYAN "3. Creditos\n" RESET);
	printf(CYAN "4. Sair\n" RESET);
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
	sorte(&p);
}

void saves(Personagem *p){
	FILE *f = fopen("jogo.sav", "wb");
	if (f == NULL){
		printf(RED "Erro ao salvar o jogo.\n" RESET);
		return;
	}
	fwrite(p, sizeof(Personagem), 1, f);
	fclose(f);
	printf(GREEN "Jogo salvo com sucesso!\n" RESET);
}

void carregarGame(Personagem *p){
	FILE *f = fopen("jogo.sav", "rb");
	if (f == NULL){
		printf(RED "nenhum jogo salvo encontrado.\n" RESET);
		return;
	}
	fread(p, sizeof(Personagem), 1, f);
	fclose(f);
	
	printf(GREEN "\nJogo carregado com sucesso.\n" RESET);
	printf("bem vindo(a) de volta, %s!\n", p->nome);
	printf("Vida: %d | Experiencia: %d\n", p->vida, p->exp);
	printf("Itens:\n");
	for (int i = 0; i < 5; i++){
		if(strlen(p->itens[i]) > 0)
			printf("- %s\n", p->itens[i]);
	}
}

void creditos(){
	printf(BLUE "\n======= CREDITOS =======\n" RESET);
	printf("Jogo criado pelos desenvolvedores: Cauã Wernek, Leonardo Tavares e Luiz Eduardo.\n");
	printf("Inspirado em livros-jogo de RPG e no cotidiano do calouro de CCOMP.\n");
}

void addItem(Personagem *p, const char *item){
	for (int i = 0; i < 5; i++){
		if (strlen(p->itens[i]) == 0){
			strcpy(p->itens[i], item);
			printf(GREEN "'%s' foi adicionado ao seu inventário.\n" RESET, item);
			return;
		}
	}
	printf(RED "Inventário cheio! Venda algum item para adicionar '%s'.\n" RESET, item);
}

void mostrarInventario(Personagem *p){
	printf("Itens:\n");
	for (int i = 0; i < 5; i++){
		if (strlen(p->itens[i]) > 0)
			printf("- %s\n", p->itens[i]);
	}
}

void historia1(Personagem *p){
	int escolha;
	printf("\n* DESPERTADOR TOCA *\n 'ah... como eu queria dormir mais... não faz sentido o fim de semana acabar tão rápido assim... enfim, vamos lá, né...'\n");
	wait(1000);
	printf("\n* 1 HORA DEPOIS *\n");
	wait(1000);
	printf("'finalmente! cheguei nessa minha linda faculdade. Bem, mais um dia de luta começando...'\n");
	wait(1000);
	printf("1. Ir para a biblioteca\n");
	wait(500);
	printf("2. Passar no refeitório para ver qual é o almoço de hoje.\n");
	wait(500);
	printf("3. Ir para o banheiro.\n");
	wait(500);
	printf("4. Ir para a sala de aula.\n");
	
	printf("Escolha seu caminho(1-4): ");
	scanf("%d", &escolha);
	
	switch (escolha){
		case 1:
			printf("\n A prova de Circuitos Digitais está chegando... Você deve estar preparado. Você fez 21 questões sobre circuitos sequênciais.\n -- Você ganhou 20 de experiência.\n");
			p->exp += 20;
			break;
		case 2:
			printf("\n O almoço de hoje é Estrogonofe! Você recebe 1 ticket e mais 20 de vida máxima.\n");
			p->vida += 20;
			addItem(p, "Ticket do R.U");
			break;
		case 3:
			printf("\n Você entrou no banheiro para jogar uma água na na cara, pois estava com muito sono.\n");
			wait(1000);
			printf("Você foi pegar um papel para se enxugar, mas algo chamou sua atenção no chão. ao lado da lixeira, você se deparou com uma calculadora. Essa, a qual ajudará você a realizar cálculos triviais de forma rápida e eficiente.\n");
			addItem(p, "Calculadora");
			break;
		case 4:
			printf("\n Você chegou cedo, só há alguns alunos em sala. Vocês começam a conversar sobre como acham que será a prova de Circuitos.\n");
			wait(500);
			printf("\n Você se lembra de que há uma lista de Circuitos para entregar neste dia, mas você está bem na matéria.\n");
			wait(500);
			printf("Você então decide:\n");
			wait(500);
			printf("1. Entregar a lista e garantir mais uns décimos ao final dessa prova.\n");
			printf("2. Não entregar pois, por mais que o medo da prova ser difícil esteja te acometendo, você se garante(você pelo menos acha que se garante).");
			break;
	}
}

void loopJogo(Personagem *p) {
    int acao;
    while (1) {
        printf("\nO que deseja fazer agora?\n");
        printf("1. Fazer mais uma revisão\n2. Ver Inventário\n3. Salvar Jogo\n4. Sair do Jogo\nEscolha: ");
        scanf("%d", &acao);

        switch (acao) {
            case 1:
                printf("Você acaba de gabaritar todas as questões de uma lista feita por um indiano que você achou no YouTube!\n");
                p->exp += 10;
                break;
            case 2:
                mostrarInventario(p);
                break;
            case 3:
                saves(p);
                break;
            case 4:
                printf("Deseja salvar antes de sair? (s/n): ");
                char sair;
                scanf(" %c", &sair);
                if (sair == 's' || sair == 'S')
                    saves(p);
                printf("Até logo, %s!\n", p->nome);
                exit(0);
            default:
                printf(RED "Ação inválida.\n" RESET);
        }
    }
}
int main(){
	setlocale(LC_ALL, "Portuguese");
	int opcao;
	Personagem aluno;
	int jogoCriado = 0;
	
	animacao();
	
	do{
		Menu();
		scanf("%d", &opcao);
		
		switch (opcao) {
			case 1:
				criarPersonagem(&aluno);
				jogoCriado = 1;
				historia1(&aluno);
				loopJogo(&aluno);
				break;
			case 2:
				carregarGame(&aluno);
				jogoCriado = 1;
				historia1(&aluno);
				loopJogo(&aluno);
				break;
			case 3:
				creditos();
				break;
			case 4:
				if(jogoCriado){
					char salvar;
					printf("Deseja salvar o jogo antes de sair? (s/n): ");
					scanf(" %c", &salvar);
					if (salvar == 's' || salvar == 'S'){
						saves(&aluno);
					}
				}
				printf("Saindo do jogo...\n");
				break;
			default:
				printf(RED "opcao invalida. tente novamente.\n" RESET);
		}
	} while (opcao != 4);
	
	return 0;
}