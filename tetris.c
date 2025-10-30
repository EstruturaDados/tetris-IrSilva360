// Autor: Iranildo Silva
// ByteBros - Tetris Stack - MESTRE

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
void inserirNaFrente(Fila *f, Peca x); // Inseri os 3 itens da pilha na frente da fila
int trocaMultipla(Fila *f, Pilha *p);// Executa a trocas multiplas entre a fila e a pilha
void mostrarFila(const Fila *f); // Mostra todas as peças da fila

// Pilha 
void inicializarPilha(Pilha *p); // Inicializa a pilha
int pilhaCheia(const Pilha *p); // Verifica se pilha cheia
int pilhaVazia(const Pilha *p); // Verifica se pilha vazia
int pushPilha(Pilha *p, Peca item); // Empilha peça no topo
int popPilha(Pilha *p, Peca *saida); // Desempilha peça do topo
int trocarFrenteFilaTopoPilha(Fila *f, Pilha *p); // Empilha os 3 itens da fila na pilha
void mostrarPilha(const Pilha *p); // Mostra todas as peças da pilha (Topo -> Base)

// Utilitários 
Peca gerarPeca(); // Gera peça aleatória com ID único
void limpaTela(); // Limpa o terminal (portável)
void limpaBufferEntrada(); // Limpa buffer do teclado
void esperarEnter(); // Pausa aguardando ENTER
void pausaSegundos(int segundos); // Pausa por segundos (portável)

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
        printf("==========================================================\n");
        printf("           ByteBros - Tetris Stack - MESTRE\n");
        printf("==========================================================\n");
        printf("Estatus Atual\n");
        mostrarFila(&filaPrincipal);
        mostrarPilha(&pilhaReserva);
         printf("==========================================================\n");
        // Menu de ações
        printf("Opções:\n");
        printf("1 - Jogar Peça da Frente da Fila\n");
        printf("2 - Enviar Peça da Fila para Reservar (Pilha)\n");
        printf("3 - Usar Peça da Reserva (Pilha)\n");
        printf("4 - Trocar Peça da Frente da Fila com Topo da Pilha\n");
        printf("5 - Trocar os 3 Primeiros da Fila com as 3 Peças da Pilha\n");
        printf("0 - SAIR\n");
        printf("==========================================================\n");
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
                    printf("\nPeça Jogada: [%c %d]\n\nEstatus Atual:\n", p.nome, p.id);
                    enqueue(&filaPrincipal, gerarPeca()); // Repor fila
                } else {
                    printf("\nFila Vazia!\n\n");
                }
                mostrarFila(&filaPrincipal);
                printf("\n");
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
                        printf("\nPeça Reservada: [%c %d]\n\nEstatus Atual:\n", p.nome, p.id);
                        enqueue(&filaPrincipal, gerarPeca()); // Repor fila
                    } else {
                        printf("\nFila Vazia!\n\n");
                    }
                }
                mostrarPilha(&pilhaReserva);
                printf("\n");
                esperarEnter();
                break;
            }

            case 3: { // Usar peça reservada
                Peca p;
                if (popPilha(&pilhaReserva, &p)) {
                    printf("\nPeça Usada da Reserva: [%c %d]\n\nEstatus Atual:\n", p.nome, p.id);
                    enqueue(&filaPrincipal, gerarPeca());
                } else {
                    printf("\nPilha Vazia!\n\n");
                }
                mostrarPilha(&pilhaReserva);
                printf("\n");
                esperarEnter();
                break;
            }

            case 4: { // Usar trocar peça fila X pilha
                if (!filaVazia(&filaPrincipal) && !pilhaVazia(&pilhaReserva)) {
                    Peca pFila = filaPrincipal.itens[filaPrincipal.inicio];
                    Peca pPilha = pilhaReserva.itens[pilhaReserva.topo];

                    trocarFrenteFilaTopoPilha(&filaPrincipal, &pilhaReserva);

                    printf("\nTroca realizada:\n");
                    printf("Fila [%c %d] por Pilha [%c %d]\n\nEstatus Atual:\n", pFila.nome, pFila.id, pPilha.nome, pPilha.id);
                } else {
                    printf("\nNão foi Possível Trocar (VAZIA)!\n\n");
                    
                }
                mostrarFila(&filaPrincipal);
                mostrarPilha(&pilhaReserva); 
                printf("\n");
                esperarEnter();
                break;
            }

            case 5: { // Trocar 3 primeiros da fila com 3 da pilha
                if (filaPrincipal.total >= 3 && pilhaReserva.topo >= 2) {

                    Peca filaTemp[3];
                    Peca pilhaTemp[3];

                    // Guarda o estado ANTES da troca
                    for (int i = 0; i < 3; i++) {
                        int idxFila = (filaPrincipal.inicio + i) % MAX_FILA;
                        filaTemp[i] = filaPrincipal.itens[idxFila];
                        pilhaTemp[i] = pilhaReserva.itens[pilhaReserva.topo - i];
                    }

                    // Executa a troca oficial
                    if (trocaMultipla(&filaPrincipal, &pilhaReserva)) {

                        printf("\nTroca Múltipla Realizada:\n\n");

                        // Exibe o antes e depois
                        for (int i = 0; i < 3; i++) {
                            int idxFila = (filaPrincipal.inicio + i) % MAX_FILA;
                            int idxPilha = pilhaReserva.topo - i;

                            printf("Fila [%c %d] -> [%c %d]\n",
                                filaTemp[i].nome, filaTemp[i].id,
                                filaPrincipal.itens[idxFila].nome,
                                filaPrincipal.itens[idxFila].id);

                            printf("Pilha [%c %d] -> [%c %d]\n\n",
                                pilhaTemp[i].nome, pilhaTemp[i].id,
                                pilhaReserva.itens[idxPilha].nome,
                                pilhaReserva.itens[idxPilha].id);
                        }
                    }

                } else {
                    printf("\nNão foi possível trocar! (Fila ou Pilha insuficientes)\n\n");
                }

                printf("Estatus Atual:\n");
                mostrarFila(&filaPrincipal);
                mostrarPilha(&pilhaReserva);
                printf("\n");
                esperarEnter();
                break;
            }

            case 0:
                printf("\nSaindo...\n");
                pausaSegundos(2);
                limpaTela();
                break;

            default:
                printf("\nOpção inválida! Tente novamente!\n\n");
                esperarEnter();
                break;
        
        }
            
          
     
    } while (opcao != 0);

    return 0;

}

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

// Troca a peça da frente da fila com o topo da pilha
int trocarFrenteFilaTopoPilha(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) return 0; // Falha se algum estiver vazio

    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    return 1;
}

// Troca múltipla: 3 primeiros da fila com 3 da pilha
int trocaMultipla(Fila *f, Pilha *p) {
    if (f->total < 3 || p->topo < 2) return 0;

    Peca tempFila[3];
    Peca tempPilha[3];

    // Retira 3 da fila e guarda em tempFila
    for (int i = 0; i < 3; i++) {
        if (!retirarDaFila(f, &tempFila[i])) {
            for (int j = 0; j < i; j++) inserirNaFrente(f, tempFila[i - 1 - j]); // Em caso de erro restaura o que foi retirado
            return 0;
        }
    }

    // Retira 3 da pilha e guarda em tempPilha
    for (int i = 0; i < 3; i++) {
        if (!popPilha(p, &tempPilha[i])) {
            for (int j = i - 1; j >= 0; j--) pushPilha(p, tempPilha[j]);// Restaura: empilha de volta os que já foram removidos da pilha
            for (int j = 0; j < 3; j++) enqueue(f, tempFila[j]); // e também recoloca os da fila no final da fila
            return 0;
        }
    }

    // Os itens retirados da fila devem entrar na pilha preservando LIFO:
    for (int i = 0; i < 3; i++) {
        if (!pushPilha(p, tempFila[i])) {
            // Tentativa de rollback se falhar (raro, pois verificamos capacidade antes)
            // Não implementamos rollback completo aqui por simplicidade
            return 0;
        }
    }

    // Os itens devem entrar na Fila na ordem FIFO
    for (int i = 2; i >= 0; i--) {
        inserirNaFrente(f, tempPilha[i]);
    }

    return 1;
}

// Inseri os 3 itens da pilha na frente da fila
void inserirNaFrente(Fila *f, Peca x) {
    if (filaCheia(f)) return; // Evita inserir se a fila estiver cheia

    // Decrementa inicio de forma circular
    f->inicio = (f->inicio - 1 + MAX_FILA) % MAX_FILA;

    // Coloca item na nova frente
    f->itens[f->inicio] = x;

    f->total++;
}

// Mostra todas as peças da fila
void mostrarFila(const Fila *f) { 
    printf("Fila de Peças: ");
    if (filaVazia(f)) {

        printf("(VAZIA)\n");
        return;
    }
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
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
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
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
