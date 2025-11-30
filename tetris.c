#include <stdio.h>

// **** REMOVI TODOS OS COMENTÁRIOS ORIGINAIS DO ARQUIVO 
// COLOCADO COMO BASE POIS JÁ NÃO TINHAM SENTIDO E ESTAVAM 
// TUMULTUANDO MEU CODIGO, ME FAZENDO PERDER A NOÇAO DE O 
// QUE PERTENCE A QUAL NÍVEL.

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.


// Coloquei funções como int para retornar 1 ou 0 (true ou false) e eu poder 
// checar se a ação funcionou ou não parar avisar ao usuário

// COMENTARIO DO ALUNO: MEU CODIGO COMEÇA AQUI

// #include <stdio.h> RETIREI A LINHA POIS JÁ ESTÁ DECLARADA ACIMA
#include <stdlib.h>
#include <time.h> // usado para a função de criação de numeros aleatorios

// Definições de Constantes 
#define MAX_PECAS 5        // Capacidade máxima da fila de peças futuras
#define NUM_TIPOS_PECA 4   // Número de tipos diferentes de peças

// ACRESCIDO: Capacidade da pilha de reserva (nível intermediário)
#define MAX_PILHA 3        // ACRESCIDO - capacidade máxima da pilha de reserva

//Variável Global para Gerar IDs Únicos
// Usada em gerarBloco para garantir que cada peça tenha um ID diferente.
int proximo_id = 0; 

// Definição da Estrutura de Blocos
// As peças são tipo de blocos
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L', etc.)
    int id;    // Identificador único da ordem de criação
} blocos;

//Definição da Estrutura de Peças (a Fila Circular)
typedef struct {
    blocos bloco[MAX_PECAS]; // O array que armazena os elementos
    int inicio;              // Índice do primeiro elemento (frente)
    int fim;                 // Índice da próxima posição livre (trás)
    int total;               // Contador de elementos atuais na fila
} pecas;

// ACRESCIDO: Definição da Estrutura da Pilha de Reserva (NÍVEL INTERMEDIÁRIO)
typedef struct {
    blocos item[MAX_PILHA];  // array que armazena as peças da pilha
    int topo;                // índice do topo (-1 se vazia)
    int total;               // contador de elementos na pilha. Era desnecessário mas fiz assim para eu 
                             // manter o mesmo estilo e entendimento na comparação com as filas
} pilhaReserva;

//Protótipos das Funções 
blocos gerarBloco();
void exibepecas(pecas *peca);
void preencheFilaInicial(pecas *peca);
void menuacoes(pecas *peca);

// ACRESCIDO: protótipos da pilha (NÍVEL INTERMEDIÁRIO)
void inicializaPilha(pilhaReserva *p);
int pilhaVazia(pilhaReserva *p);
int pilhaCheia(pilhaReserva *p);
int pushPilha(pilhaReserva *p, blocos b);
int popPilha(pilhaReserva *p, blocos *b);
void exibePilha(pilhaReserva *p);

// ACRESCIDO: protótipos das novas operações (trocas)
int trocaFrenteComTopo(pecas *peca, pilhaReserva *p);        // ACRESCIDO
int trocaTresPrimeiros(pecas *peca, pilhaReserva *p);       // ACRESCIDO

//Implementação das Funções

/**
 * Função que inicializa a fila de peças.
 * Parametros: peca Ponteiro para a estrutura pecas (a fila).
 */
void inicializa (pecas *peca) {
    peca->inicio = 0;
    peca->fim = 0;
    peca->total = 0;
}

/**
 * Função que retorna se fila está vazia.
 * Paramteros: peca Ponteiro para a estrutura pecas (a fila).
 * Retorna:int 1 se vazia, 0 caso contrário.
 */
int filavazia( pecas *peca) {
    return (peca->total == 0) ? 1 : 0; 
}

/**
 * Função que retorna se fila está cheia.
 * Paramteros: peca Ponteiro para a estrutura pecas (a fila).
 * Retorna:int 1 se cheia, 0 caso contrário.
 */
int filacheia( pecas *peca) {
    return (peca->total == MAX_PECAS) ? 1 : 0; 
}

/**
 * Gera um novo bloco (peça) com um tipo aleatório e um ID único.
 * Retorna:blocos O novo bloco (peça) gerado.
 */
blocos gerarBloco() {
    blocos novoBloco;
    
    // Tipos de peças disponíveis: 'I', 'O', 'T', 'L'
    char tipos[NUM_TIPOS_PECA] = {'I', 'O', 'T', 'L'};
    
    // Gera um índice aleatório para escolher o tipo da peça
    int indiceAleatorio = rand() % NUM_TIPOS_PECA;
    
    novoBloco.nome = tipos[indiceAleatorio];
    novoBloco.id = proximo_id++; // Atribui o ID atual e incrementa para o próximo
    
    return novoBloco;
}

/**
 * Insere peças ao final da fila (enqueue).
 * Paramteros: peca Ponteiro para a estrutura pecas (a fila).
 * Paramteros: novoBloco Bloco (peça) a ser inserido.
 */
void inserepeca (pecas *peca, blocos novoBloco) {
    
    if (filacheia(peca)) { // confere se a fila já está cheia e avisa se for o caso
        printf("\nFILA CHEIA. Impossível adicionar nova peça.\n");
        return;
    }
    
    peca->bloco[peca->fim] = novoBloco;      // coloco o elemento na ultima posição do vetor
    peca->fim = (peca->fim + 1) % MAX_PECAS; // calculo o novo fim atraves da formula de fim+1 modulo de MAX_PECAS 
    peca->total++;                           // incremento o total de peças
    
    printf("\nPeça [%c %d] inserida com sucesso ao final da fila.\n", novoBloco.nome, novoBloco.id);
    return;
}

/**
 * Remove a peça da frente da fila (dequeue) e a retorna.
 * Paramteros: peca Ponteiro para a estrutura pecas (a fila).
 * Retorna:blocos A peça removida ou um bloco vazio em caso de erro.
 */
blocos removepeca (pecas *peca) {
    blocos blocoRemovido = {'\0', -1}; // Bloco de erro/vazio
    
    if (filavazia(peca)) { 
        printf("\nFILA VAZIA! Não há peças para jogar (dequeue).\n");
        return blocoRemovido;
    }
    
    blocoRemovido = peca->bloco[peca->inicio];
    peca->inicio = (peca->inicio + 1) % MAX_PECAS;
    peca->total--;
    
    printf("\nPeça [%c %d] removida (**jogada**) da frente da fila.\n", blocoRemovido.nome, blocoRemovido.id);
    
    return blocoRemovido;
}

/**
 * Exibe o estado atual da fila de peças.
 */
void exibepecas(pecas *peca) {
    printf("\nFila de Peças (Total: %d/%d)\n", peca->total, MAX_PECAS);
    
    if (filavazia(peca)) {
        printf("A fila está vazia.\n");
        return;
    }
    
    printf("Fila de peças: ");
    
    int i;
    int indiceAtual = peca->inicio;
    
    for (i = 0; i < peca->total; i++) {
        printf("[%c %d] ", peca->bloco[indiceAtual].nome, peca->bloco[indiceAtual].id);
        indiceAtual = (indiceAtual + 1) % MAX_PECAS;
    }
    
    printf("\n");
}

/**
 * Preenche a fila com o número inicial de peças.
 */
void preencheFilaInicial(pecas *peca) {
    printf("\nInicialização da Fila de Peças\n");
    for (int i = 0; i < MAX_PECAS; i++) {
        inserepeca(peca, gerarBloco()); 
    }
    printf("\nFila inicializada com %d peças.\n", MAX_PECAS);
}


// ===============================
// ACRESCIDO: IMPLEMENTAÇÃO DA PILHA (NÍVEL MESTRE)
// ===============================

/**
 * Inicializa a pilha de reserva.
 * Parametros: p Ponteiro para pilhaReserva.
 */
void inicializaPilha(pilhaReserva *p) {
    p->topo = -1;
    p->total = 0;
}

/**
 * Verifica se a pilha está vazia.
 * Retorna 1 se vazia, 0 caso contrário.
 */
int pilhaVazia(pilhaReserva *p) {
    return (p->total == 0) ? 1 : 0;
}

/**
 * Verifica se a pilha está cheia.
 * Retorna 1 se cheia, 0 caso contrário.
 */
int pilhaCheia(pilhaReserva *p) {
    return (p->total == MAX_PILHA) ? 1 : 0;
}

/**
 * Insere um bloco no topo da pilha.
 * Retorna 1 em sucesso, 0 se falhar (pilha cheia).
 */
int pushPilha(pilhaReserva *p, blocos b) {
    if (pilhaCheia(p)) {
        printf("\nPILHA CHEIA. Não é possível reservar a peça.\n");
        return 0;
    }
    p->topo++;
    p->item[p->topo] = b;
    p->total++;
    printf("\nPeça [%c %d] empilhada na reserva (topo).\n", b.nome, b.id);
    return 1;
}

/**
 * Remove o bloco do topo da pilha e copia para *b.
 * Retorna 1 em sucesso, 0 se falhar (pilha vazia).
 */
int popPilha(pilhaReserva *p, blocos *b) {
    if (pilhaVazia(p)) {
        printf("\nPILHA VAZIA. Não há peças reservadas para usar.\n");
        return 0;
    }
    *b = p->item[p->topo];
    printf("\nPeça [%c %d] removida da pilha de reserva (usada).\n", b->nome, b->id);
    p->topo--;
    p->total--;
    return 1;
}

/**
 * Exibe a pilha de reserva do topo para a base.
 */
void exibePilha(pilhaReserva *p) {
    printf("\nPilha de reserva (Topo -> Base) (Total: %d/%d)\n", p->total, MAX_PILHA);
    if (pilhaVazia(p)) {
        printf("A pilha de reserva está vazia.\n");
        return;
    }
    printf("Pilha de reserva: ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->item[i].nome, p->item[i].id);
    }
    printf("\n");
}

// ===============================
// ACRESCIDO: Função para trocar a peça da frente da fila com o topo da pilha
// Retorna 1 se trocou com sucesso, 0 caso contrário
// ===============================
int trocaFrenteComTopo(pecas *peca, pilhaReserva *p) {
    if (filavazia(peca)) {
        printf("\nNão é possível trocar: a fila está vazia.\n");
        return 0;
    }
    if (pilhaVazia(p)) {
        printf("\nNão é possível trocar: a pilha está vazia.\n");
        return 0;
    }
    // índice da frente na fila
    int idxFila = peca->inicio;
    // índice do topo na pilha
    int idxPilha = p->topo;
    // realiza a troca
    blocos temp = peca->bloco[idxFila];
    peca->bloco[idxFila] = p->item[idxPilha];
    p->item[idxPilha] = temp;

    printf("\nTroca realizada entre a peça da frente da fila e o topo da pilha.\n");
    return 1;
}

// ===============================
// ACRESCIDO: Função para trocar os 3 primeiros da fila com as 3 peças da pilha
// Retorna 1 se trocou com sucesso, 0 caso contrário
// ===============================
int trocaTresPrimeiros(pecas *peca, pilhaReserva *p) {
    if (peca->total < 3) {
        printf("\nNão é possível trocar: a fila não tem 3 peças.\n");
        return 0;
    }
    if (p->total < 3) {
        printf("\nNão é possível trocar: a pilha não tem 3 peças.\n");
        return 0;
    }

    // Para i = 0..2:
    // fila indices: inicio + i (mod MAX_PECAS)
    // pilha indices: topo - i
    for (int i = 0; i < 3; i++) {
        int idxFila = (peca->inicio + i) % MAX_PECAS;
        int idxPilha = p->topo - i;
        blocos temp = peca->bloco[idxFila];
        peca->bloco[idxFila] = p->item[idxPilha];
        p->item[idxPilha] = temp;
    }

    printf("\nTroca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
    return 1;
}


// ===============================
// FIM DA IMPLEMENTAÇÃO DA PILHA (NÍVEL MESTRE)
// ===============================


// ===============================
// MENU DE AÇÕES (MODIFICADO PARA INCLUIR TROCAS ACRESCIDAS)
// ===============================
void menuacoes(pecas *peca) {
    int opcao;
    // ACRESCIDO: instância da pilha de reserva
    pilhaReserva reserva;
    inicializaPilha(&reserva);

    do {
        exibepecas(peca); // Exibe o estado atual da fila antes de cada ação
        exibePilha(&reserva); // ACRESCIDO: mostra a pilha de reserva junto

        printf("\nOpções de ação:\n");
        printf("Código | Ação\n");
        printf("-------|------------------------------------------\n");
        printf("   1   | Jogar peça (remover da frente da fila)\n");
        printf("   2   | Reservar peça (mover frente da fila -> pilha)\n");
        printf("   3   | Usar peça reservada (remover do topo da pilha)\n");
        printf("   4   | Trocar peça da frente com o topo da pilha\n");
        printf("   5   | Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("   6   | Inserir nova peça manualmente na fila\n");
        printf("   0   | Sair\n");
        printf("-----------------------------------------------\n");
        printf("Escolha uma opção: ");
        
        // Garante que apenas um número inteiro válido é lido
        if (scanf("%d", &opcao) != 1) {
            printf("\nOpção inválida. Por favor, digite um número.\n");
            // Limpa o buffer de entrada
            while (getchar() != '\n');
            continue; 
        }
        
        // Executa a ação baseada na opção
        switch (opcao) {
            case 1: {
                // Jogar peça: remove da frente da fila (dequeue)
                blocos jogada = removepeca(peca);
                if (jogada.id != -1) {
                    // ACRESCIDO: gerar e inserir nova peça para manter fila cheia quando possível
                    if (!filacheia(peca)) {
                        inserepeca(peca, gerarBloco());
                    }
                }
                break;
            }
            case 2: {
                // Reservar peça: mover da frente da fila para a pilha, se houver espaço
                if (pilhaCheia(&reserva)) {
                    printf("\nNão foi possível reservar: A pilha de reserva está cheia.\n");
                } else if (filavazia(peca)) {
                    printf("\nNão foi possível reservar: A fila está vazia.\n");
                } else {
                    // Remove da fila
                    blocos aReservar = removepeca(peca);
                    // Tenta empilhar na pilha de reserva
                    if (!pushPilha(&reserva, aReservar)) {
                        // Se por algum motivo falhar ao empilhar (não deve ocorrer pois checamos),
                        // podemos re-inserir na fila (opcional). Aqui vamos re-inserir para consistência.
                        inserepeca(peca, aReservar);
                    } else {
                        // ACRESCIDO: gerar e inserir nova peça para manter fila cheia quando possível
                        if (!filacheia(peca)) {
                            inserepeca(peca, gerarBloco());
                        }
                    }
                }
                break;
            }
            case 3: {
                // Usar peça reservada: pop da pilha
                blocos usada;
                if (popPilha(&reserva, &usada)) {
                    // ACRESCIDO: peças usadas não voltam; mas gerar nova peça para fila se houver espaço
                    if (!filacheia(peca)) {
                        inserepeca(peca, gerarBloco());
                    }
                }
                break;
            }
            case 4: {
                // Trocar peça da frente da fila com o topo da pilha
                if (trocaFrenteComTopo(peca, &reserva)) {
                    // nenhuma mudança de contagem; fila permanece com mesmo total
                }
                break;
            }
            case 5: {
                // Trocar 3 primeiros da fila com os 3 da pilha
                if (trocaTresPrimeiros(peca, &reserva)) {
                    // troca efetuada
                }
                break;
            }
            case 6:
                // Inserir nova peça manualmente (mantido do Nível Novato) - ACRESCIDO como opção 6
                inserepeca(peca, gerarBloco());
                break;
            case 0:
                printf("\nSaindo do Tetris Stack. Obrigado!\n");
                break;
            default:
                printf("\nOpção desconhecida. Por favor, escolha 1, 2, 3, 4, 5, 6 ou 0.\n");
        }
    } while (opcao != 0);
}



//Função Principal
int main() {


    // COMENTARIO DO ALUNO: MEU CODIGO COMEÇA AQUI
    // Inicializa o gerador de numeros aleatorios para a função rand() para gerar tipos de peças diferentes
    srand(time(NULL));
    
    pecas filaDePeças;
    
    // 1. Inicializa a estrutura da fila
    inicializa(&filaDePeças);
    
    // 2. Preenche a fila com 5 peças iniciais
    preencheFilaInicial(&filaDePeças);
    
    // 3. Inicia o loop do menu (agora com opções do Nível Intermediário ACRESCIDO)
    menuacoes(&filaDePeças);



    return 0;
}
