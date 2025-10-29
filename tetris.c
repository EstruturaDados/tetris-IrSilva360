//Autor: Iranildo Silva

// Bibliotecas padrão
#include <stdio.h> // Entrada e saída de dados (printf, scanf)
#include <stdlib.h> // Funções utilitárias (rand, system)
#include <time.h> // Funções de tempo (time) para aleatoriedade
#include <locale.h> // Configurações de idioma/acentuação

// Inclusão condicional para Windows
#ifdef _WIN32
    #include <windows.h> // Permite alterar codificação de caracteres no Windows
#endif

// Define a capacidade máxima da fila circular de peças
#define MAX_FILA 5

// --- Struct da peça ---
typedef struct {
    char tipo;  // I, O, T, L
    int id;     // Identificador único
} Peca;

// --- Variáveis Globais da Fila ---
Peca fila[MAX_FILA]; // Vetor que armazena a fila circular de peças
int inicio = 0; // Índice do início da fila (posição da próxima peça a ser jogada)
int fim = 0; // Índice do fim da fila (próxima posição livre para inserir nova peça)
int quantidade = 0; // Quantidade atual de peças na fila
int idGlobal = 0; // Garante que cada peça tenha ID único

// --- Protótipos ---
void inicializarFila();
void limpaTela();
void limpaBufferEntrada();
int enqueue(Peca nova);
int dequeue();
int filaCheia();
int filaVazia();
Peca gerarPeca();
void mostrarFila();
void menu();

// --- Função Principal ---
int main() {
    
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Configura idioma/acentuação
// Ajusta codificação de entrada/saída no Windows    
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    srand(time(NULL)); // Inicializa semente de números aleatórios

    limpaTela(); // Limpa a tela ao iniciar

    inicializarFila(); // Preenche a fila inicial com peças aleatórias

    int opcao;
    do {
        menu(); // Exibe menu e fila
        scanf("%d", &opcao);
        limpaBufferEntrada(); // Limpa caracteres residuais no buffer

        switch (opcao) {
        case 1:
            dequeue(); // Jogar peça da frente
            break; 

        case 2:
            enqueue(gerarPeca()); // Inserir nova peça gerada aleatoriamente
            break;
            
        case 0:
            printf("\nSaindo...\n\n"); // Encerra o programa
            break;

        default:
            printf("\nOpção inválida!\n\n");
        }

                
    } while (opcao != 0); // Loop até o jogador escolher sair
    
    return 0;
    
}

// --- Implementação das Funções ---

// Preenche a fila inicial com peças aleatórias até atingir MAX_FILA
void inicializarFila() {
    while (!filaCheia()) {
        enqueue(gerarPeca());
    }
}

// Verifica se fila está cheia
int filaCheia() {
    return quantidade == MAX_FILA;
}

// Verifica se fila está vazia
int filaVazia() {
    return quantidade == 0;
}

// Gera uma peça com tipo aleatório e id sequencial
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.tipo = tipos[rand() % 4];
    nova.id = idGlobal++;
    return nova;
}

// Adiciona peça no final da fila
int enqueue(Peca nova) {
    if (filaCheia()) {
        printf("\nFila cheia! Não é possível adicionar nova peça.\n\n");
        return 0;
    }

    fila[fim] = nova;
    fim = (fim + 1) % MAX_FILA; // Atualiza fim de forma circular
    quantidade++;
    return 1;
}

// Remove peça do início da fila
int dequeue() {
    if (filaVazia()) {
        printf("\nFila vazia! Nenhuma peça para jogar.\n\n");
        return 0;
    }

    Peca jogada = fila[inicio]; // Pega a peça da frente
    printf("\nPeça jogada: [%c %d]\n\n", jogada.tipo, jogada.id);

    inicio = (inicio + 1) % MAX_FILA; // Atualiza início de forma circular
    quantidade--;
    return 1;
}

// Exibe estado atual da fila
void mostrarFila() {
    printf("\nFila de Peças: ");

    if (filaVazia()) {
        printf("(vazia)\n\n");
        return;
    }

    for (int i = 0; i < quantidade; i++) {
        int pos = (inicio + i) % MAX_FILA; // Índice circular
        printf("[%c %d] ", fila[pos].tipo, fila[pos].id);
    }
    printf("\n\n");
}

// Exibe o menu do jogo e mostra a fila atual
void menu() {
    printf("=============================================\n");
    printf("    ByteBros - Tetris Stack (NÍVEL NOVATO)\n");
    printf("=============================================");
    mostrarFila(); // Mostra fila antes das opções
    printf("1 - Jogar Peça\n");
    printf("2 - Inserir Nova Peça\n");
    printf("0 - Sair\n");
    printf("=============================================\n");
    printf("Escolha: ");
}

void limpaTela() { // Limpa o terminal de forma compatível com Windows e Linux
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void limpaBufferEntrada() { // Limpa caracteres residuais do buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.

//int main() {

    // 🧩 Nível Novato: Fila de Peças Futuras
    //
    // - Crie uma struct Peca com os campos: tipo (char) e id (int).
    // - Implemente uma fila circular com capacidade para 5 peças.
    // - Crie funções como inicializarFila(), enqueue(), dequeue(), filaCheia(), filaVazia().
    // - Cada peça deve ser gerada automaticamente com um tipo aleatório e id sequencial.
    // - Exiba a fila após cada ação com uma função mostrarFila().
    // - Use um menu com opções como:
    //      1 - Jogar peça (remover da frente)
    //      0 - Sair
    // - A cada remoção, insira uma nova peça ao final da fila.



    // 🧠 Nível Aventureiro: Adição da Pilha de Reserva
    //
    // - Implemente uma pilha linear com capacidade para 3 peças.
    // - Crie funções como inicializarPilha(), push(), pop(), pilhaCheia(), pilhaVazia().
    // - Permita enviar uma peça da fila para a pilha (reserva).
    // - Crie um menu com opção:
    //      2 - Enviar peça da fila para a reserva (pilha)
    //      3 - Usar peça da reserva (remover do topo da pilha)
    // - Exiba a pilha junto com a fila após cada ação com mostrarPilha().
    // - Mantenha a fila sempre com 5 peças (repondo com gerarPeca()).


    // 🔄 Nível Mestre: Integração Estratégica entre Fila e Pilha
    //
    // - Implemente interações avançadas entre as estruturas:
    //      4 - Trocar a peça da frente da fila com o topo da pilha
    //      5 - Trocar os 3 primeiros da fila com as 3 peças da pilha
    // - Para a opção 4:
    //      Verifique se a fila não está vazia e a pilha tem ao menos 1 peça.
    //      Troque os elementos diretamente nos arrays.
    // - Para a opção 5:
    //      Verifique se a pilha tem exatamente 3 peças e a fila ao menos 3.
    //      Use a lógica de índice circular para acessar os primeiros da fila.
    // - Sempre valide as condições antes da troca e informe mensagens claras ao usuário.
    // - Use funções auxiliares, se quiser, para modularizar a lógica de troca.
    // - O menu deve ficar assim:
    //      4 - Trocar peça da frente com topo da pilha
    //      5 - Trocar 3 primeiros da fila com os 3 da pilha


    //return 0;
//}

