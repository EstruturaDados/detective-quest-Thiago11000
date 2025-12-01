#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct para Sala (Árvore Binária)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Struct para Pista (BST)
typedef struct Pista {
    char texto[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// Struct para Lista de Pistas associadas a Suspeitos
typedef struct ListaPistas {
    char pista[100];
    struct ListaPistas *prox;
} ListaPistas;

// Struct para Suspeito
typedef struct Suspeito {
    char nome[50];
    ListaPistas *pistas;
    int contador; // Contador de associações
} Suspeito;

// Tabela Hash (array de ponteiros para Suspeitos, tamanho 26 para primeira letra)
#define TAM_HASH 26
Suspeito *hashTable[TAM_HASH];

// Função hash simples: baseada na primeira letra (assumindo maiúsculas)
int hash(char *nome) {
    return nome[0] - 'A';
}

// Inicializar tabela hash
void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        hashTable[i] = NULL;
    }
}

// Criar uma nova sala
Sala* criarSala(char *nome) {
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Conectar salas (esquerda e direita)
void conectarSalas(Sala *pai, Sala *esq, Sala *dir) {
    pai->esquerda = esq;
    pai->direita = dir;
}

// Explorar salas (recursivo para navegação)
void explorarSalas(Sala *atual) {
    if (atual == NULL) return;
    printf("Você está em: %s\n", atual->nome);
}

// Inserir pista na BST
Pista* inserirPista(Pista *raiz, char *texto) {
    if (raiz == NULL) {
        Pista *nova = (Pista*)malloc(sizeof(Pista));
        strcpy(nova->texto, texto);
        nova->esquerda = NULL;
        nova->direita = NULL;
        return nova;
    }
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else {
        raiz->direita = inserirPista(raiz->direita, texto);
    }
    return raiz;
}

// Exibir pistas em ordem alfabética (emOrdem)
void emOrdem(Pista *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("Pista: %s\n", raiz->texto);
        emOrdem(raiz->direita);
    }
}

// Inserir na tabela hash (pista -> suspeito)
void inserirHash(char *pista, char *suspeitoNome) {
    int indice = hash(suspeitoNome);
    if (hashTable[indice] == NULL) {
        hashTable[indice] = (Suspeito*)malloc(sizeof(Suspeito));
        strcpy(hashTable[indice]->nome, suspeitoNome);
        hashTable[indice]->pistas = NULL;
        hashTable[indice]->contador = 0;
    }
    // Adicionar pista à lista
    ListaPistas *nova = (ListaPistas*)malloc(sizeof(ListaPistas));
    strcpy(nova->pista, pista);
    nova->prox = hashTable[indice]->pistas;
    hashTable[indice]->pistas = nova;
    hashTable[indice]->contador++;
}

// Mostrar suspeitos e suas pistas
void mostrarSuspeitos() {
    for (int i = 0; i < TAM_HASH; i++) {
        if (hashTable[i] != NULL) {
            printf("Suspeito: %s (Contador: %d)\n", hashTable[i]->nome, hashTable[i]->contador);
            ListaPistas *atual = hashTable[i]->pistas;
            while (atual != NULL) {
                printf("  Pista: %s\n", atual->pista);
                atual = atual->prox;
            }
        }
    }
}

// Mostrar suspeito mais provável (com maior contador)
void mostrarSuspeitoMaisProvavel() {
    Suspeito *maisProvavel = NULL;
    int maxContador = 0;
    for (int i = 0; i < TAM_HASH; i++) {
        if (hashTable[i] != NULL && hashTable[i]->contador > maxContador) {
            maxContador = hashTable[i]->contador;
            maisProvavel = hashTable[i];
        }
    }
    if (maisProvavel != NULL) {
        printf("Suspeito mais provável: %s (com %d associações)\n", maisProvavel->nome, maxContador);
    } else {
        printf("Nenhum suspeito identificado.\n");
    }
}

int main() {
    // 🌱 Nível Novato: Construir árvore binária da mansão
    Sala *hall = criarSala("Hall de Entrada");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sótão");
    Sala *jardim = criarSala("Jardim");
    Sala *escritorio = criarSala("Escritório");
    Sala *quarto = criarSala("Quarto");
    Sala *porao = criarSala("Porão");

    // Conectar salas (árvore fixa)
    conectarSalas(hall, biblioteca, cozinha);
    conectarSalas(biblioteca, sotao, jardim);
    conectarSalas(cozinha, escritorio, quarto);
    conectarSalas(sotao, NULL, NULL); // Folhas
    conectarSalas(jardim, NULL, NULL);
    conectarSalas(escritorio, NULL, NULL);
    conectarSalas(quarto, porao, NULL);
    conectarSalas(porao, NULL, NULL);

    // 🔍 Nível Aventureiro: Inicializar BST para pistas
    Pista *raizPistas = NULL;

    // 🧠 Nível Mestre: Inicializar tabela hash
    inicializarHash();

    // Loop do jogo
    Sala *atual = hall;
    while (1) {
        explorarSalas(atual);

        // Adicionar pistas automaticamente ao visitar salas (exemplos)
        if (strcmp(atual->nome, "Biblioteca") == 0) {
            raizPistas = inserirPista(raizPistas, "Livro aberto na página 42");
            inserirHash("Livro aberto na página 42", "Professor Plum");
        } else if (strcmp(atual->nome, "Cozinha") == 0) {
            raizPistas = inserirPista(raizPistas, "Faca suja de sangue");
            inserirHash("Faca suja de sangue", "Senhora Peacock");
        } else if (strcmp(atual->nome, "Sótão") == 0) {
            raizPistas = inserirPista(raizPistas, "Corda cortada");
            inserirHash("Corda cortada", "Coronel Mustard");
        } else if (strcmp(atual->nome, "Jardim") == 0) {
            raizPistas = inserirPista(raizPistas, "Pegadas na lama");
            inserirHash("Pegadas na lama", "Senhora Peacock");
        } else if (strcmp(atual->nome, "Escritório") == 0) {
            raizPistas = inserirPista(raizPistas, "Documento rasgado");
            inserirHash("Documento rasgado", "Professor Plum");
        } else if (strcmp(atual->nome, "Quarto") == 0) {
            raizPistas = inserirPista(raizPistas, "Janela quebrada");
            inserirHash("Janela quebrada", "Coronel Mustard");
        } else if (strcmp(atual->nome, "Porão") == 0) {
            raizPistas = inserirPista(raizPistas, "Caixa trancada");
            inserirHash("Caixa trancada", "Professor Plum");
        }

        printf("Escolha: e (esquerda), d (direita), s (sair), r (revisar pistas e suspeitos)\n");
        char op;
        scanf(" %c", &op);

        if (op == 's') break;
        else if (op == 'r') {
            printf("Pistas coletadas (em ordem alfabética):\n");
            emOrdem(raizPistas);
            printf("\nSuspeitos e associações:\n");
            mostrarSuspeitos();
        } else if (op == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (op == 'd' && atual->direita) {
            atual = atual->direita;
        } else {
            printf("Movimento inválido ou caminho bloqueado.\n");
        }
    }

    // Ao final, mostrar suspeito mais provável
    printf("\nAnálise final:\n");
    mostrarSuspeitoMaisProvavel();

    // Liberar memória (simplificado, não implementado completamente para brevidade)
    // Em um código real, seria necessário liberar todas as alocações dinâmicas.

    return 0;
}
