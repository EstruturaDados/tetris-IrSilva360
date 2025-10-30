// Autor: Iranildo Silva
// ByteBros - Tetris Stack - Aventureiro

#include <stdio.h> // Entrada e saída de dados (printf, scanf)
#include <stdlib.h> // Funções utilitárias: rand, srand, system
#include <time.h> // Manipulação de tempo para gerar números aleatórios
#include <locale.h> // Configuração de idioma e acentuação

#ifdef _WIN32
    #include <windows.h> // Para SetConsoleCP/SetConsoleOutputCP e Sleep
#else
    #include <unistd.h> // Para sleep em sistemas UNIX/Linux
#endif

// Constantes
#define MAX_FILA 5 // Define a capacidade máxima da fila circular
#define MAX_PILHA 3 // Define a capacidade máxima da pilha de reserva

// Tipos / Structs
// Struct que representa cada peça do jogo
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id; // Identificador único sequencial
} Peca;

// Struct da fila circular
typedef struct {
    Peca itens[MAX_FILA]; // Array que armazena as peças
    int inicio; // Índice da peça da frente (próxima a ser jogada)
    int fim; // Índice da próxima posição livre para inserir
    int total; // Quantidade atual de peças na fila
} Fila;

// Struct da pilha (LIFO)
typedef struct {
    Peca itens[MAX_PILHA]; // Array que armazena as peças
    int topo; // Índice do topo da pilha (-1 = vazia)
} Pilha;

// Variáveis Globais
int idGlobal = 0; // Contador global para garantir IDs únicos para cada peça

// Protótipos das Funções

// Fila 
void inicializarFila(Fila *f); // Inicializa a fila e preenche com peças aleatórias
int filaCheia(const Fila *f); // Verifica se a fila está cheia
int filaVazia(const Fila *f); // Verifica se a fila está vazia
int enqueue(Fila *f, Peca p); // Adiciona peça ao fim da fila
int retirarDaFila(Fila *f, Peca *saida); // Remove peça da frente da fila
void mostrarFila(const Fila *f); // Mostra todas as peças da fila

// Pilha 
void inicializarPilha(Pilha *p); // Inicializa a pilha
int pilhaCheia(const Pilha *p); // Verifica se pilha cheia
int pilhaVazia(const Pilha *p); // Verifica se pilha vazia
int pushPilha(Pilha *p, Peca item); // Empilha peça no topo
int popPilha(Pilha *p, Peca *saida); // Desempilha peça do topo
void mostrarPilha(const Pilha *p); // Mostra todas as peças da pilha (Topo -> Base)

// Utilitários 
Peca gerarPeca(); // Gera peça aleatória com ID único
void limpaTela(); // Limpa o terminal (portável)
void limpaBufferEntrada(); // Limpa buffer do teclado
void esperarEnter(); // Pausa aguardando ENTER
void pausaSegundos(int segundos); // Pausa por segundos (portável)

// Implementação das Funções

// Gera peça aleatória com tipo 'I','O','T','L' e ID sequencial
Peca gerarPeca() { 
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4]; // Escolhe aleatoriamente um tipo
    nova.id = idGlobal++; // Atribui ID único e incrementa contador
    return nova;
}

// Fila 

// Inicializa a fila, zera índices e preenche com peças aleatórias
void inicializarFila(Fila *f) { 
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;

    while (!filaCheia(f)) {
        enqueue(f, gerarPeca()); // Preenche fila até MAX_FILA
    }
}

// Retorna 1 se fila cheia, 0 caso contrário
int filaCheia(const Fila *f) {
    return f->total == MAX_FILA;
}

// Retorna 1 se fila vazia, 0 caso contrário
int filaVazia(const Fila *f) {
    return f->total == 0;
}

// Adiciona peça no fim da fila (circular)
int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0; // Não insere se fila cheia
    f->itens[f->fim] = p; // Coloca peça na posição fim
    f->fim = (f->fim + 1) % MAX_FILA; // Incrementa fim de forma circular
    f->total++; // Atualiza total
    return 1;
}

// Remove peça da frente e passa via ponteiro 'saida'
int retirarDaFila(Fila *f, Peca *saida) { 
    if (filaVazia(f)) return 0; // Retorna 0 se fila vazia
    *saida = f->itens[f->inicio]; // Pega peça da frente
    f->inicio = (f->inicio + 1) % MAX_FILA; // Atualiza índice inicial (circular)
    f->total--; // Decrementa total
    return 1;
}

// Mostra todas as peças da fila
void mostrarFila(const Fila *f) { 
    printf("\nFila de Peças: ");
    if (filaVazia(f)) {
        printf("(vazia)\n\n");
        return;
    }
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n\n");
}

// Pilha 

// Inicializa pilha, topo = -1 significa vazia
void inicializarPilha(Pilha *p) { 
    p->topo = -1;
}

// Retorna 1 se pilha cheia
int pilhaCheia(const Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

// Retorna 1 se pilha vazia
int pilhaVazia(const Pilha *p) {
    return p->topo == -1;
}

// Empilha peça no topo da pilha
int pushPilha(Pilha *p, Peca item) {
    if (pilhaCheia(p)) return 0; // Não insere se cheia
    p->itens[++(p->topo)] = item; // Incrementa topo e coloca peça
    return 1;
}

// Desempilha peça do topo
int popPilha(Pilha *p, Peca *saida) {
    if (pilhaVazia(p)) return 0;
    *saida = p->itens[(p->topo)--]; // Passa peça para 'saida' e decrementa topo
    return 1;
}

// Mostra a pilha (Topo -> Base)
void mostrarPilha(const Pilha *p) {
    printf("Pilha de Reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n\n");
}

// Utilitários 

// Limpa a tela 
void limpaTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Limpa buffer do teclado para evitar caracteres residuais
void limpaBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Pausa aguardando o ENTER
void esperarEnter() {
    printf("Pressione ENTER para continuar...");
    getchar();
}

// Pausa por segundos (portável)
void pausaSegundos(int segundos) {
#ifdef _WIN32
    Sleep(segundos * 1000); // Windows Sleep recebe milissegundos
#else
    sleep(segundos);        // UNIX/Linux sleep recebe segundos
#endif
}

// Função Principal - Menu

int main(void) {
    // Configura idioma/acentuação
    setlocale(LC_ALL, "pt_BR.UTF-8");
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);          
    SetConsoleOutputCP(CP_UTF8);    
#endif

    srand((unsigned) time(NULL)); // Inicializa gerador aleatório

    // Estruturas principais
    Fila filaPrincipal;
    Pilha pilhaReserva;

    // Inicializa fila e pilha
    inicializarFila(&filaPrincipal);
    inicializarPilha(&pilhaReserva);

    int opcao;
    do {
        limpaTela();

        // Exibe estado atual
        printf("=========================================\n");
        printf("  ByteBros - Tetris Stack - AVENTUREIRO\n");
        printf("=========================================\n");
        mostrarFila(&filaPrincipal);
        mostrarPilha(&pilhaReserva);

        // Menu de ações
        printf("Opções:\n");
        printf("1 - Jogar Peça\n");
        printf("2 - Reservar Peça\n");
        printf("3 - Usar Peça Reservada\n");
        printf("0 - SAIR\n");
       printf("=========================================\n");
        printf("Opção: ");

        if (scanf("%d", &opcao) != 1) {
            limpaBufferEntrada();
            opcao = -1;
        } else {
            limpaBufferEntrada();
        }

        switch (opcao) {
            case 1: { // Jogar peça
                Peca p;
                if (retirarDaFila(&filaPrincipal, &p)) {
                    printf("\nPeça Jogada: [%c %d]\n\n", p.nome, p.id);
                    enqueue(&filaPrincipal, gerarPeca()); // Repor fila
                } else {
                    printf("\nFila Vazia!\n\n");
                }
                esperarEnter();
                break;
            }

            case 2: { // Reservar peça
                if (pilhaCheia(&pilhaReserva)) {
                    printf("\nPilha Cheia!\n\n");
                } else {
                    Peca p;
                    if (retirarDaFila(&filaPrincipal, &p)) {
                        pushPilha(&pilhaReserva, p);
                        printf("\nPeça Reservada: [%c %d]\n\n", p.nome, p.id);
                        enqueue(&filaPrincipal, gerarPeca()); // Repor fila
                    } else {
                        printf("\nFila Vazia!\n\n");
                    }
                }
                esperarEnter();
                break;
            }

            case 3: { // Usar peça reservada
                Peca p;
                if (popPilha(&pilhaReserva, &p)) {
                    printf("\nPeça Usada da Reserva: [%c %d]\n\n", p.nome, p.id);
                    enqueue(&filaPrincipal, gerarPeca());
                } else {
                    printf("\nPilha Vazia!\n\n");
                }
                esperarEnter();
                break;
            }

            case 0:
                printf("\nSaindo...\n");
                pausaSegundos(2);
                limpaTela();
                break;

            default:
                printf("\nOpção inválida! Tente Novamente!\n\n");
                esperarEnter();
                break;
        }

    } while (opcao != 0);

    return 0;
}
