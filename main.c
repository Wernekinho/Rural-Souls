#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <math.h>
#include "rpg.h"

#ifdef _WIN32
	#include <windows.h>
	void wait(int ms) { Sleep(ms); }
#else
	#include <unistd.h>
	void wait(int ms) { usleep(ms * 1000); }
#endif

void animacao(){
	char titulo[]= "RURAL SOULS";
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

void pausa() {
	printf("\nPressione ENTER para continuar...\n");
	getchar();
}

int rolarDado() {
	return rand() % 6 + 1;
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

int minigame1() {
    int a, b, resposta, tentativa;
    int tipo = rand() % 3; // 0: +, 1: -, 2: *

    a = rand() % 10 + 1;
    b = rand() % 10 + 1;

    switch (tipo) {
        case 0:
            printf("Resolva: %d + %d = ", a, b);
            resposta = a + b;
            break;
        case 1:
            printf("Resolva: %d - %d = ", a, b);
            resposta = a - b;
            break;
        case 2:
            printf("Resolva: %d * %d = ", a, b);
            resposta = a * b;
            break;
    }

    scanf("%d", &tentativa);

    if (tentativa == resposta) {
        printf(GREEN "Correto! Você ganhou 20 de experiência.\n" RESET);
        return 20;
    } else {
        printf(RED "Errado! A resposta era %d. Nenhuma experiência ganha.\n" RESET, resposta);
        return 0;
    }
}

void batalha(Personagem *p, Marcel m, const char* nome_monstro) {
	printf("\n--- Batalha contra %s ---\n", nome_monstro);
	while (p->vida > 0 && m.vida > 0) {
		int dado = rolarDado();
		int ataque = p->QI + dado;
		printf("Você rolou %d. Ataque total: %d\n", dado, ataque);
		if (ataque > m.QI) {
			printf("Você acertou! %s perdeu 2 de vida.\n", nome_monstro);
			m.vida -= 2;
		} else if (ataque < m.QI) {
			printf("Você errou! Você perdeu 2 de vida.\n");
			p->vida -= 2;
		} else {
			printf("Empate! Ambos perderam 1 de vida.\n");
			p->vida -= 1;
			m.vida -= 1;
		}
		printf("Sua vida: %d | Vida do %s: %d\n", p->vida, nome_monstro, m.vida);
		pausa();
	}

	if (p->vida <= 0) {
		printf("\nVocê foi derrotado. GAME OVER.\n");
		exit(0);
	} else {
		printf("\nVocê derrotou %s!\n", nome_monstro);
		p->exp += 1;
	}
}

void atividades(Personagem *p, int *cont) {
	if (*cont >= 2) {
		printf("\nVocê já realizou todas as suas atividades.\n");
		return;
	}

	printf("\nEscolha uma atividade:\n");
	printf("1 - Ir para a biblioteca\n");
	printf("2 - Ir ao refeitório\n");
	printf("3 - Passear\n");
	int escolha;
	scanf("%d", &escolha);
	getchar();

	switch(escolha){
		case 1:
			printf("Você resolveu uma equação e ganhou +1 QI!\n");
			p->QI += 1;
			break;
		case 2:
			printf("Você ganhou um ticket e recuperou 1 ponto de vida!\n");
			p->vida += 1;
			break;
		case 3:
			printf("Você pegou na pata de um coelho e ganhou +1 sorte!\n");
			p->sorte += 1;
			break;
		default:
			printf("Opção inválida.\n");
			return;
	}
	(*cont)++;
}

void capitulo1(Personagem *p){
	int escolha;
	int caminho[4] = {0, 0, 0, 0};
	printf("\n* DESPERTADOR TOCA *\n 'ah... como eu queria dormir mais... não faz sentido o fim de semana acabar tão rápido assim... enfim, vamos lá, né...'\n");
	wait(1000);
	printf("\n* 1 HORA DEPOIS *\n");
	wait(1000);
	printf("'finalmente! cheguei nessa minha linda faculdade. Bem, mais um dia de luta começando...'\n");
	wait(1000);
	printf("\nVocê chega na faculdade e vai direto para a sala 209...\n");
	wait(1000);
	printf("Personagem: \"Puxa, minha primeira aula é Circuitos.\"\n");
	wait(1000);
	printf("Quer assistir à aula? (1 - Sim, 0 - Não): ");
	int aula;
	scanf("%d", &aula);
	getchar();
	if (aula == 0) {
		printf("Você perdeu 1 de sorte e 1 de QI por não assistir à aula.\n");
		p->sorte -= 1;
		p->QI -= 1;
		printf("\nPara onde deseja ir?\n1 - Biblioteca\n2 - Refeitório\n3 - Passear pela faculdade\n");
		int escolha;
		scanf("%d", &escolha);
		getchar();

		if (escolha == 1) {
			printf("Você foi à biblioteca e resolveu um exercício. +1 QI\n");
			p->QI += 1;
		} else if (escolha == 2) {
			printf("Você foi ao refeitório e comeu ESTROGONOFE! +1 vida\n");
			p->vida += 1;
		} else if (escolha == 3) {
			printf("Você encontrou um coelho e pegou na pata dele. +1 sorte\n");
			p->sorte += 1;
		} 
	
	}else {
			printf("Marcel: \"Bom dia a todos. Como vocês sabem, a prova de Circuitos está chegando.\nHoje darei uma revisão sobre os conteúdos que vão cair.\n");
			wait(1500);
			printf("(2 horas depois)\n");
			wait(1500);
			printf("Marcel: \"Bom pessoal, é isso. Espero ter tirado todas as dúvidas de vocês. Boa sorte na prova!\"\n");
			wait(1500);
			printf("Personagem: \"Ufa, achei que não sobreviveria a essa lista... pelo amor de Deus, e ainda não entendi como funciona um diplexador... enfim, espero que a prova não esteja difícil.\"\n");
	}

	pausa();
	interagirComPosner(p);

	int cont = 0;
	atividadeExtra(p, &cont);
	atividadeExtra(p, &cont);

	printf("\n--- Próximo dia ---\n");
	printf("\"* A PROVA DE CIRCUITOS CHEGOU *\"\n");
	wait(1000);
	wait(1000);

	if (p->exp >= 7) {
		printf("Marcel: \"Somente lápis, borracha e caneta em cima da mesa. Vamos começar a prova.\"\n");
		Marcel prova = {6, 9};
		batalha(p, prova, "PROVA DE CIRCUITOS");
	} else {
		printf("Você não tem experiência suficiente. Vá para a biblioteca treinar.\n");
		p->exp += 2;
		printf("Você treinou e ganhou +1 de habilidade (agora: %d).\n", p->exp);
	}

	printf("\n--- Capítulo 1 concluído ---\n");
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
	Personagem aluno = {10, 5, 3, 7, 0};
	int jogoCriado = 0;
	srand(time(NULL));
	
	animacao();
	
	do{
		Menu();
		scanf("%d", &opcao);

		switch (opcao) {
			case 1:
				criarPersonagem(&aluno);
				jogoCriado = 1;
				capitulo1(&aluno);
				loopJogo(&aluno);
				break;
			case 2:
				carregarGame(&aluno);
				jogoCriado = 1;
				capitulo1(&aluno);
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