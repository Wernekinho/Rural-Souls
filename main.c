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

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

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
	char nomeArquivo[50];
	strcpy(nomeArquivo, p->nome);
	strcat(nomeArquivo, ".sav");
	FILE *f = fopen(nomeArquivo, "wb");
	if (f == NULL){
		printf(RED "Erro ao salvar o jogo.\n" RESET);
		return;
	}
	fwrite(p, sizeof(Personagem), 1, f);
	fclose(f);
	printf(GREEN "Jogo salvo com sucesso!\n" RESET);
}

void carregarGame(Personagem *p){
	char nomeArquivo[50];
	printf("Qual o nome do personagem que deseja carregar? (Escreva exatamente o nome do personagem)\n: ");
	scanf(" %[^\n]", nomeArquivo);
	strcat(nomeArquivo, ".sav");
	FILE *f = fopen(nomeArquivo, "rb");
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
	wait(1500);
	limparTela();

	pausa();
	interagirComPosner(p);

	int cont = 0;
	atividades(p, &cont);
	atividades(p, &cont);

	printf("\n--- Próximo dia ---\n");
	printf("\"* A PROVA DE CIRCUITOS CHEGOU *\"\n");
	wait(1000);

	while (p->exp < 7) {
		printf("Você não tem experiência suficiente. Vá para a biblioteca treinar.\n");
		p->exp += 2;
		printf("Você treinou e ganhou +1 de habilidade (agora: %d).\n", p->exp);
		wait(1000);
	}

	printf("Marcel: \"Somente lápis, borracha e caneta em cima da mesa. Vamos começar a prova.\"\n");
	Marcel prova;
	prova.QI = 6;
	prova.vida = 9;
	batalha(p, prova, "PROVA DE CIRCUITOS");

	printf("\n--- Capítulo 1 concluído ---\n");
	p->capituloAtual = 1;
	saves(p);

	limparTela(); 
}

void ProvaMD(Personagem *p) {
	Fernanda provaMD = {"Matemática Discreta", 8, 10};
    printf("\nVocê enfrentará a prova de %s!\n", provaMD.nome);
    printf("QI da prova: %d | Vida da prova: %d\n", provaMD.QI, provaMD.vida);
    wait(1000);

    while (p->vida > 0 && provaMD.vida > 0) {
        printf("\n--- Sua vez ---\n");
        int dado = (rand() % 6) + 1;
        int ataque = dado + p->QI;
        printf("Você rolou um dado: %d + QI (%d) = %d\n", dado, p->QI, ataque);

        if (ataque > provaMD.QI) {
            printf(GREEN "Você acertou! A prova perde 2 de vida.\n" RESET);
            provaMD.vida -= 2;
        } else if (ataque < provaMD.QI) {
            printf(RED "Você errou! Você perde 2 de vida.\n" RESET);
            p->vida -= 2;
        } else {
            printf("Empate! Ambos perdem 1 de vida.\n");
            p->vida -= 1;
            provaMD.vida -= 1;
        }

        printf("Sua vida: %d | Vida da Prova: %d\n", p->vida, provaMD.vida);
        wait(1000);
    }

    if (p->vida <= 0) {
        printf(RED "\nVocê foi derrotado pela prova de Matemática Discreta...\n" RESET);
        printf(RED "GAME OVER\n" RESET);
        exit(0);
    } else {
        printf(GREEN "\nParabéns! Você passou na prova de %s!\n" RESET, provaMD.nome);
        p->exp += 30;
    }
}

void capitulo2(Personagem *p){
	printf("\nVocê: Uou, consegui passar dessa prova... foi difícil. Bem, menos uma matéria agora.\n");
	wait(1000);
	printf("Posner: Vejo que conseguiu passar pelo primeiro desafio. Daqui pra frente será mais difícil, se prepare.\n ");
	wait(1000);
	printf("Você: Oi, é você de novo? Você sempre aparece nesses momentos. Bom... até mais.\n");
	wait(1000);
	limparTela();

	printf("\n--- 1 MÊS DEPOIS ---\n");
	wait(1000);
	printf("Você: Hoje começa minha jornada pela Matemática Discreta.\n");
	wait(1000);
	printf("Você: Todos estão falando pra mim que é bem mais difícil que Circuitos...\n");
	wait(500);
	printf("Você: Somente 10 pessoas passam... É o que vamos ver.\n");
	wait(1500);
	limparTela();

	printf("\nProfessora: Boa tarde chuchus! Meu nome é Fernanda e irei lecionar Matemática Discreta a vocês nesse período.\n");
	wait(2000);
	printf("\nAo final da aula...\n");
	printf("Você: Cara, essa aula foi demais! Porém, preciso estudar cada vez mais.\n");
	wait(1000);
	limparTela();

	int acao;
	printf("\nFernanda: Oi chuchu. Você poderia encher minha garrafa enquanto eu guardo as coisas aqui, por favor?\n1. Sim\n2. Não\nEscolha: ");
	scanf("%d", &acao);
	if (acao == 1){
		printf("Você: Claro, professora!\n");
		wait(300);
		printf("Você encheu a garrafa dela. Ganhou 2 de QI e 2 de vida máxima. Além disso, Fernanda te deu um presente. Creatina!\n");
		p->QI += 2;
		p->vida += 2;
		addItem(p, "Creatina");
	} else {
		printf("Você recusou. Fernanda te olhou com julgamento... Nenhum ganho.\n");
	}

	wait(1000);
	printf("\nPosner: E então, como está indo MD?\n");
	printf("Você: Estou fazendo o meu maximo.\n");
	printf("Posner: Irei te dar um presente: uma lista de exercícios de MD.\n");
	limparTela();

	printf("\nResolva: 45 + 24 = ");
	int resposta;
	scanf("%d", &resposta);
	if (resposta == 69) {
		printf(GREEN "Você fez a lista de exercícios e ganhou +1 de QI e +1 de sorte!\n" RESET);
		p->QI += 1;
		p->sorte += 1;
	} else {
		printf(RED "Errado.. A resposta era 69. Nenhum ganho\n" RESET);
	}

	wait(1000);
	printf("\n---- NA SEMANA SEGUINTE ----\n");
	wait(1000);
	printf("\nVocê: Hoje é o dia da prova. Não sei se estou pronto...\n");
	if (p->QI < 6) {
        printf(RED "Seu QI é insuficiente. Vá estudar mais na biblioteca.\n" RESET);
        printf("Você estudou bastante e ganhou +2 de QI.\n");
        p->QI += 2;
    }

	printf("\n--- INICIANDO PROVA DE MD ---\n");
    wait(1000);
    
	ProvaMD(p);

    printf(GREEN "\nVocê passou na prova de Matemática Discreta!\n" RESET); 

    wait(1500);
    printf("\nPosner: Geralmente essas são as provas mais difíceis do período.\n");
    printf("Você: Você de novo? Está me perseguindo?\n");
    wait(1000);
    printf("Você: De qualquer forma, falta uma matéria apenas para eu me formar.\n");
    printf("Posner: Bom, essa é a mais difícil... até semestre que vem, aluno.\n");

    wait(1500);
    printf("Você: Espera... ALUNO? ME CHAMOU DE ALUNO?\n");
    wait(1000);

    printf(GREEN "\nFIM DO CAPÍTULO 2.\n" RESET);
	p->capituloAtual = 2;
	saves(p);

	limparTela();
}

void ProvaPE(Personagem *p) {
    Posner provaPE = {"Prova de PE", 12, 16};

    printf("\nVocê enfrentará a %s!\n", provaPE.nome);
    printf("QI da prova: %d | Vida da prova: %d\n", provaPE.QI, provaPE.vida);
    wait(1000);

    while (p->vida > 0 && provaPE.vida > 0) {
        printf("\n--- Sua vez ---\n");
        int dado = (rand() % 6) + 1;
        int ataque = dado + p->QI;
        printf("Você rolou: %d + QI (%d) = %d\n", dado, p->QI, ataque);

        if (ataque > provaPE.QI) {
            printf(GREEN "Você acertou! A prova perde 2 de vida.\n" RESET);
            provaPE.vida -= 2;
        } else if (ataque < provaPE.QI) {
            printf(RED "Você errou! Você perde 2 de vida.\n" RESET);
            p->vida -= 2;
        } else {
            printf(YELLOW "Empate! Ambos perdem 1 de vida.\n" RESET);
            p->vida -= 1;
            provaPE.vida -= 1;
        }

        printf("Sua vida: %d | Vida da Prova: %d\n", p->vida, provaPE.vida);
        wait(1000);
    }

	if (p->vida <= 0) {
        printf(RED "\nVocê foi derrotado pela Prova de PE...\nGAME OVER.\n" RESET);
        exit(0);
    } else {
        printf(GREEN "\nVocê venceu a %s!\n" RESET, provaPE.nome);
        p->exp += 40;
    }
}

void capitulo3(Personagem *p) {
	printf("\n1 mês depois...\n");
    wait(1000);
    printf("Você: Aquele cara me chamou de aluno, mas ele também não era aluno?\n");
    wait(1500);

    printf("\n* Chegando na sala *\n");
    wait(1000);
    printf("Posner: Bom dia, sou o professor Posner. Aqueles que não se consideram dignos, por favor, saiam.\n");
    wait(2000);
    printf("Posner: Já aviso de antemão que ninguém nunca passou em minha disciplina. Boa sorte.\n");
    wait(2000);

    printf("\n* Ao fim da primeira aula *\n");
    wait(1000);
    printf("Você: Esse cara não tá pra brincadeira. Tenho que estudar o dobro... não, o triplo pra passar.\n");
    wait(1500);

    printf("\n* Nos dias seguintes, andando pelos corredores... *\n");
    wait(1500);
    printf("Desconhecido: Ei, psiu!\n");
    wait(1000);
    printf("Você: Ahn? Eu?\n");
    wait(1000);
    printf("Desconhecido: É você mesmo. Venha cá.\n");
    wait(1000);
    printf("Desconhecido: Prazer, sou Bruno, o único aluno a superar as 3 disciplinas.\n");
    wait(2000);
    printf("Você: Mas o Posner disse...\n");
    wait(1000);
    printf("Bruno: ELE MENTE! Você é a única esperança para destruir essa matéria. Quanto mais ele reprova, mais ele se fortalece.\n");
    wait(2500);
    printf("Bruno: Irei te dar uma dica. Na biblioteca, na área de História, existe um livro escondido de PROGRAMAÇÃO EM C.\n");
    wait(2500);
    printf("Bruno: Caso consiga pegá-lo, você passará. É tudo que posso dizer. Até.\n");
    wait(2000);
    printf("* E, mais rápido que um raio, Bruno some *\n");

    int acreditar;
    printf("\nVocê acredita em Bruno? (1 - Sim, 0 - Não): ");
    scanf("%d", &acreditar);

    if (acreditar == 1) {
        printf("Você encontra o livro escondido de PROGRAMAÇÃO EM C na biblioteca!\n");
        p->QI += 5;
        p->vida += 5;
        printf(GREEN "Você ganhou +5 de QI e +5 de Vida!\n" RESET);
    } else {
        printf("Você ignorou a dica de Bruno.\n");
    }

    wait(1500);
    printf("\n* Próxima aula *\n");
    wait(1000);
    printf("Posner: Boa tarde caros alunos. Hoje falaremos de Matrizes.\n");
    wait(1500);

    int tirarDuvida;
    printf("Deseja tirar uma dúvida com o Posner? (1 - Sim, 0 - Não): ");
    scanf("%d", &tirarDuvida);

    if (tirarDuvida == 1) {
        p->QI -= 1;
        p->vida -= 1;
        printf(RED "Você perdeu 1 ponto de QI e 1 de Vida...\n" RESET);
    }

    wait(1500);
    printf("\nVocê: Finalmente chegou o dia da prova.\n");
    wait(1500);
    printf("* PROVA DE PE se aproxima *\n");

	ProvaPE(p);

    printf(GREEN "\nPosner: Bom, você conseguiu. Agora que perdi, posso deixar meu cargo para você.\n" RESET);
    wait(2000);
    printf("Você: Espera, eu não quero!\n");
    wait(1000);
    printf("Posner: É assim que funciona, garoto.\n");
    wait(1500);
    printf("Você: Mas e o Bruno? Ele passou antes de mim! Por que ele não é o atual professor?\n");
    wait(2000);
    printf("Posner: Bruno? Não existe nenhum Bruno.\n");
    wait(2000);

    printf("\n* 1 ano depois... *\n");
    wait(1500);
    printf("Você: Bom dia alunos, sou %s, e irei lecionar a matéria de PE. Vamos começar dizendo os nomes. Você, por favor, seu nome?\n", p->nome);
    wait(3000);
    printf("Aluno: Muito prazer, meu nome é Bruno.\n");
    wait(2000);
    printf(GREEN "\nFIM DO CAPÍTULO 3\n" RESET);
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
				capitulo2(&aluno);
				capitulo3(&aluno);
				break;
			case 2:
				carregarGame(&aluno);
				if (aluno.capituloAtual == 1) {
					capitulo1(&aluno);
					capitulo2(&aluno);
				} else if (aluno.capituloAtual == 2) {
					capitulo2(&aluno);
				} else if (aluno.capituloAtual == 3) {
					capitulo3(&aluno);
				} else {
					printf("Capítulo salvo não reconhecido.\n");
				}

				jogoCriado = 1;
				capitulo1(&aluno);
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