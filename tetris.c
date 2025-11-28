#include <stdio.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.

// COMENTARIO DO ALUNO: MEU CODIGO COMEÇA AQUI

// #include <stdio.h> RETIREI A LINHA POIS JÁ ESTÁ DECLARADA ACIMA
#include <stdlib.h>
#include <time.h> // usado para a função de criação de numeros aleatorios

// Definições de Constantes 
#define MAX_PECAS 5        // Capacidade máxima da fila de peças futuras
#define NUM_TIPOS_PECA 4   // Número de tipos diferentes de peças

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

//Protótipos das Funções 
blocos gerarBloco();
void exibepecas(pecas *peca);
void preencheFilaInicial(pecas *peca);
void menuacoes(pecas *peca);


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
    blocos blocoRemovido = {'\0', -1}; // Bloco de erro/vazio para retorno em caso de fila vazia
    
    if (filavazia(peca)) { 
        printf("\nFILA VAZIA! Não há peças para jogar (dequeue).\n");
        return blocoRemovido;
    }
    
    // Pega a peça na posição apontada por 'inicio'
    blocoRemovido = peca->bloco[peca->inicio]; 
    
    // Atualiza o índice 'inicio' para o próximo elemento (circulando)
    peca->inicio = (peca->inicio + 1) % MAX_PECAS; 
    
    // Decrementa o tamanho
    peca->total--; 
    
    printf("\nPeça [%c %d] removida (**jogada**) da frente da fila.\n", blocoRemovido.nome, blocoRemovido.id);
    
    return blocoRemovido;
}

/**
 * Exibe o estado atual da fila de peças.
 * * Mostra o tipo e o ID de cada peça na ordem correta (do início para o fim).
 * * Paramteros: peca Ponteiro para a estrutura pecas (a fila).
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
    
    // Percorre a fila a partir do 'inicio' até o 'total' de elementos
    for (i = 0; i < peca->total; i++) {
        // Exibe o elemento na posição atual
        printf("[%c %d] ", peca->bloco[indiceAtual].nome, peca->bloco[indiceAtual].id);
        
        // Move para o próximo índice de forma circular
        indiceAtual = (indiceAtual + 1) % MAX_PECAS;
    }
    
    printf("\n");
}

/**
 * Preenche a fila com o número inicial de peças.
 * Paramteros: peca Ponteiro para a estrutura pecas (a fila).
 */
void preencheFilaInicial(pecas *peca) {
    printf("\nInicialização da Fila de Peças\n");
    for (int i = 0; i < MAX_PECAS; i++) {
        // Gera e insere cada peça
        inserepeca(peca, gerarBloco()); 
    }
    printf("\nFila inicializada com %d peças.\n", MAX_PECAS);
}


/**
 * Loop principal do menu de ações.
 * * Permite ao jogador interagir com a fila (jogar peça, inserir, sair).
 * * Paramteros: peca Ponteiro para a estrutura pecas (a fila).
 */
void menuacoes(pecas *peca) {
    int opcao;
    
    do {
        exibepecas(peca); // Exibe o estado atual da fila antes de cada ação
        
        printf("\nOpções de ação:\n");
        printf("Código | Ação\n");
        printf("-------|-------------------------------\n");
        printf("   1   | Jogar peça\n");
        printf("   2   | Inserir nova peça\n");
        printf("   0   | Sair\n");
        printf("--------------------------------------\n");
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
            case 1:
                removepeca(peca); // Tenta remover uma peça
                break;
            case 2:
                inserepeca(peca, gerarBloco()); // Gera e insere uma nova peça
                break;
            case 0:
                printf("\nSaindo do Tetris Stack. Obrigado!\n");
                break;
            default:
                printf("\nOpção desconhecida. Por favor, escolha 1, 2 ou 0.\n");
        }
    } while (opcao != 0);
}



//Função Principal
int main() {

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

    // COMENTARIO DO ALUNO: MEU CODIGO PARA ESSE NIVEL COMEÇA AQUI
    // Inicializa o gerador de numeros aleatorios para a função rand() para gerar tipos de peças diferentes
    srand(time(NULL));
    
    pecas filaDePeças;
    
    // 1. Inicializa a estrutura da fila
    inicializa(&filaDePeças);
    
    // 2. Preenche a fila com 5 peças iniciais
    preencheFilaInicial(&filaDePeças);
    
    // 3. Inicia o loop do menu
    menuacoes(&filaDePeças);


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


    return 0;
}

