#include <stdio.h>
#include <stdlib.h>


typedef struct {
    char nome[50];
    char autor[50];
    int tempo;
} Musica;

typedef struct Musicas {
    Musica dados;
    struct Musicas* proximo;
        struct Musicas* anterior;
} Musicas;

Musicas* criarMusica(Musica p) {
    Musicas* novo = (Musicas*)malloc(sizeof(Musicas));
    if (novo) {
        novo->dados = p;
        novo->proximo = NULL;
        novo->anterior = NULL;
    }
    return novo;
}

void exibirMusica(Musicas* cabeca) {
    Musicas* atual = cabeca;
    while (atual) {
        printf("Nome: %s, Autor: %s, Tempo: %d\n", atual->dados.nome, atual->dados.autor, atual->dados.tempo);
        atual = atual->proximo;
    }
}    
void InserirMusica(Musicas** cabeca, Musica p) {
    Musicas* novo = criarMusica(p);
    
    if (!*cabeca) {
        *cabeca = novo;
        return;
    }
    
    Musicas* atual = *cabeca;
    while (atual->proximo) {
        atual = atual->proximo;
    }
    atual->proximo = novo;
}