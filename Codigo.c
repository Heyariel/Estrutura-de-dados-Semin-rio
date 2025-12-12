#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct musica {
    char titulo[100];
    struct musica *prev;
    struct musica *next;
} musica;

typedef struct Playlist {
    char nome[50];
    musica *inicio;
    struct Playlist *next;
} Playlist;

typedef struct NodeFila {
    char titulo[100];
    struct NodeFila *next;
} NodeFila;

NodeFila *filainicio = NULL, *filaFim = NULL;

Playlist *listaPlaylists = NULL;

void enqueue(char titulo[]) {
    NodeFila *novo = malloc(sizeof(NodeFila));
    strcpy(novo->titulo, titulo);
    novo->next = NULL;

    if (!filaFim) {
        filainicio = filaFim = novo;
    } else {
        filaFim->next = novo;
        filaFim = novo;
    }
}

void dequeue() {
    if (!filainicio) {
        printf("Fila vazia!\n");
        return;
    }
    NodeFila *temp = filainicio;
    printf("Tocando agora: %s\n", temp->titulo);
    filainicio = filainicio->next;
    if (!filainicio) filaFim = NULL;
    free(temp);
}

void mostrarFila() {
    NodeFila *aux = filainicio;
    printf("\n--- Fila de Reprodução ---\n");
    while (aux) {
        printf("%s -> ", aux->titulo);
        aux = aux->next;
    }
    printf("NULL\n");
}

Playlist *criarPlaylist(char nome[]) {
    Playlist *p = malloc(sizeof(Playlist));
    strcpy(p->nome, nome);
    p->inicio = NULL;
    p->next = listaPlaylists;
    listaPlaylists = p;
    return p;
}

musica *criarMusica(char titulo[]) {
    musica *m = malloc(sizeof(musica));
    strcpy(m->titulo, titulo);
    m->next = m->prev = NULL;
    return m;
}

void adicionarMusica(Playlist *p, char titulo[]) {
    musica *nova = criarMusica(titulo);

    if (!p->inicio) {
        p->inicio = nova;
        nova->next = nova->prev = nova;
    } else {
        musica *ultimo = p->inicio->prev;
        nova->next = p->inicio;
        nova->prev = ultimo;
        ultimo->next = nova;
        p->inicio->prev = nova;
    }
}

void removerMusica(Playlist *p, char titulo[]) {
    if (!p->inicio) return;

    musica *atual = p->inicio;

    do {
        if (strcmp(atual->titulo, titulo) == 0) {
            if (atual->next == atual) {
                p->inicio = NULL;
            } else {
                atual->prev->next = atual->next;
                atual->next->prev = atual->prev;
                if (atual == p->inicio)
                    p->inicio = atual->next;
            }
            free(atual);
            printf("Musica removida.\n");
            return;
        }
        atual = atual->next;
    } while (atual != p->inicio);

    printf("Musica não encontrada.\n");
}

void mostrarPlaylist(Playlist *p) {
    if (!p->inicio) {
        printf("\nPlaylist vazia.\n");
        return;
    }

    musica *aux = p->inicio;
    printf("\n--- Playlist %s ---\n", p->nome);

    do {
        printf("%s <-> ", aux->titulo);
        aux = aux->next;
    } while (aux != p->inicio);

    printf("(volta ao início)\n");
}

Playlist *buscarPlaylist(char nome[]) {
    Playlist *aux = listaPlaylists;
    while (aux) {
        if (strcmp(aux->nome, nome) == 0)
            return aux;
        aux = aux->next;
    }
    return NULL;
}

int main() {
    int opc;
    char nomeplaylist[50], musica[100];

    while (1) {
        printf("\n=== PLAYER DE MUSICA ===\n");
        printf("1 - Criar playlist\n");
        printf("2 - Adicionar musica\n");
        printf("3 - Remover musica\n");
        printf("4 - Mostrar playlist\n");
        printf("5 - Adicionar à fila\n");
        printf("6 - Tocar próxima\n");
        printf("7 - Mostrar fila\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf(" %d", &opc);

        switch (opc) {
            case 1:
                printf("Nome da playlist: ");
                scanf(" %[^\n]", nomeplaylist);
                criarPlaylist(nomeplaylist);
                break;

            case 2:
                printf("Playlist: ");
                scanf(" %[^\n]", nomeplaylist);
                Playlist *p = buscarPlaylist(nomeplaylist);
                if (!p) { printf("Não encontrada!\n"); break; }
                printf("Título da musica: ");
                scanf(" %[^\n]", musica);
                adicionarMusica(p, musica);
                break;

            case 3:
                printf("Playlist: ");
                scanf(" %[^\n]", nomeplaylist);
                p = buscarPlaylist(nomeplaylist);
                if (!p) { printf("Não encontrada!\n"); break; }
                printf("Título da musica: ");
                scanf(" %[^\n]", musica);
                removerMusica(p, musica);
                break;

            case 4:
                printf("Playlist: ");
                scanf(" %[^\n]", nomeplaylist);
                p = buscarPlaylist(nomeplaylist);
                if (!p) { printf("Não encontrada!\n"); break; }
                mostrarPlaylist(p);
                break;

            case 5:
                printf("Adicionar qual música à fila: ");
                scanf(" %[^\n]", musica);
                enqueue(musica);
                break;

            case 6:
                dequeue();
                break;

            case 7:
                mostrarFila();
                break;

            case 0:
                return 0;

            default:
                printf("Opção inválida.\n");
        }
    }

    return 0;
}
