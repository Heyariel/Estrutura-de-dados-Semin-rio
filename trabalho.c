#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Musica {
    char titulo[100];
    char artista[100];
    float duracao;
    struct Musica *prev;
    struct Musica *next;
} Musica;

typedef struct Playlist {
    char nome[100];
    Musica *inicio;
    struct Playlist *next;
} Playlist;

typedef struct Fila {
    Musica *musica;
    struct Fila *next;
} Fila;

Playlist *playlists = NULL;
Fila *fila = NULL;

Musica* criarMusica(char *titulo, char *artista, float duracao) {
    Musica *m = malloc(sizeof(Musica));
    strcpy(m->titulo, titulo);
    strcpy(m->artista, artista);
    m->duracao = duracao;
    m->prev = m;
    m->next = m;
    return m;
}

Playlist* criarplaylist(char *nome) {
    Playlist *p = malloc(sizeof(Playlist));
    strcpy(p->nome, nome);
    p->inicio = NULL;
    p->next = playlists;
    playlists = p;
    return p;
}

Playlist* buscarplaylist(char *nome) {
    Playlist *p = playlists;
    while (p != NULL) {
        if (strcmp(p->nome, nome) == 0) return p;
        p = p->next;
    }
    return NULL;
}

void adicionarMusica(Playlist *p, char *titulo, char *artista, float duracao) {
    Musica *nova = criarMusica(titulo, artista, duracao);
    if (p->inicio == NULL) {
        p->inicio = nova;
    } else {
        Musica *ultimo = p->inicio->prev;
        ultimo->next = nova;
        nova->prev = ultimo;
        nova->next = p->inicio;
        p->inicio->prev = nova;
    }
}

void listarPlaylists() {
    Playlist *p = playlists;
    while (p != NULL) {
        printf("- %s\n", p->nome);
        p = p->next;
    }
}

void listarMusicas(Playlist *p) {
    if (p->inicio == NULL) return;
    Musica *m = p->inicio;
    do {
        printf("%s - %s (%.2f)\n", m->titulo, m->artista, m->duracao);
        m = m->next;
    } while (m != p->inicio);
}

void enfileirar(Musica *m) {
    Fila *novo = malloc(sizeof(Fila));
    novo->musica = m;
    novo->next = NULL;

    if (fila == NULL) {
        fila = novo;
    } else {
        Fila *aux = fila;
        while (aux->next != NULL) aux = aux->next;
        aux->next = novo;
    }
}

void tocarFila() {
    if (fila == NULL) return;

    Fila *aux = fila;
    while (aux != NULL) {
        printf("Tocando: %s - %s\n", aux->musica->titulo, aux->musica->artista);
        aux = aux->next;
    }
}

void musicaatual(Playlist *p){
    if(fila != NULL){
        printf("Tocando agora: %s\n", fila->musica->titulo, fila->musica->artista);
        return;
    }

    if(p != NULL && p->inicio != NULL){
        printf("Tocando agora (da playlist %s): %s\n", p->nome, p->inicio->titulo, p->inicio->artista);
        return;
    }
    printf("Nenhuma música na fila ou playlist.\n");
}

int main() {
    setlocale(LC_ALL, "portuguese");
    char nome[100], titulo[100], artista[100];
    float duracao;

    for (;;) {
        printf("\n1 - Criar playlist\n");
        printf("2 - Listar playlists\n");
        printf("3 - Adicionar música\n");
        printf("4 - Listar músicas\n");
        printf("5 - Enfileirar\n");
        printf("6 - Tocar fila\n");
        printf("7 - Mostrar música atual\n");
        printf("0 - Sair\n> ");

        switch (getchar()) {
            case '0':
                return 0;

            case '1':
                getchar();
                printf("Nome da playlist: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0;
                criarplaylist(nome);
                break;

            case '2':
                getchar();
                listarPlaylists();
                break;

            case '3':
                getchar();
                printf("Playlist: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0;
                {
                    Playlist *p = buscarplaylist(nome);
                    if (!p) break;

                    printf("Título: ");
                    fgets(titulo, 100, stdin);
                    titulo[strcspn(titulo, "\n")] = 0;

                    printf("Artista: ");
                    fgets(artista, 100, stdin);
                    artista[strcspn(artista, "\n")] = 0;

                    printf("Duração: ");
                    scanf("%f", &duracao);
                    getchar();

                    adicionarMusica(p, titulo, artista, duracao);
                }
                break;

            case '4':
                getchar();
                printf("Playlist: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0;
                {
                    Playlist *p = buscarplaylist(nome);
                    if (p) listarMusicas(p);
                }
                break;

            case '5':
                getchar();
                printf("Playlist: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0;
                {
                    Playlist *p = buscarplaylist(nome);
                    if (!p || !p->inicio) break;

                    printf("Título da música: ");
                    fgets(titulo, 100, stdin);
                    titulo[strcspn(titulo, "\n")] = 0;

                    Musica *m = p->inicio;
                    do {
                        if (strcmp(m->titulo, titulo) == 0) {
                            enfileirar(m);
                            break;
                        }
                        m = m->next;
                    } while (m != p->inicio);
                }
                break;

            case '6':
                getchar();
                tocarFila();
                break;

            case '7':
                getchar();
                printf("Playlist (ou vazio para fila): ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0;
                
                musicaatual(buscarplaylist(nome));
                break;

            default:
                getchar();
                break;
        }
    }

    return 0;
}
