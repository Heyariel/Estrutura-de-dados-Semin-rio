#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Musica {
    char titulo[100];
    char artista[100];
    float duracao;
    struct Musica *anterior;
    struct Musica *proximo;
} Musica;

typedef struct Playlist {
    char nome[100];
    Musica *inicio;
    struct Playlist *proximo;
} Playlist;

typedef struct Fila {
    Musica *musica;
    struct Fila *proximo;
} Fila;

Playlist *playlists = NULL;
Fila *fila = NULL;

Musica* criarMusica(char *titulo, char *artista, float duracao) {
    Musica *m = malloc(sizeof(Musica));
    if (!m) return NULL;
    strncpy(m->titulo, titulo, sizeof(m->titulo)-1); m->titulo[sizeof(m->titulo)-1] = '\0';
    strncpy(m->artista, artista, sizeof(m->artista)-1); m->artista[sizeof(m->artista)-1] = '\0';
    m->duracao = duracao;
    m->anterior = m;
    m->proximo = m;
    return m;
}

Playlist* criarplaylist(char *nome) {
    Playlist *p = malloc(sizeof(Playlist));
    if (!p) return NULL;
    strncpy(p->nome, nome, sizeof(p->nome)-1); p->nome[sizeof(p->nome)-1] = '\0';
    p->inicio = NULL;
    p->proximo = playlists;
    playlists = p;
    return p;
}

Playlist* buscarplaylist(char *nome) {
    Playlist *p = playlists;
    while (p != NULL) {
        if (strcmp(p->nome, nome) == 0) return p;
        p = p->proximo;
    }
    return NULL;
}

void adicionarMusica(Playlist *p, char *titulo, char *artista, float duracao) {
    if (!p) return;
    Musica *nova = criarMusica(titulo, artista, duracao);
    if (!nova) return;
    if (p->inicio == NULL) {
        p->inicio = nova;
    } else {
        Musica *ultimo = p->inicio->anterior;
        ultimo->proximo = nova;
        nova->anterior = ultimo;
        nova->proximo = p->inicio;
        p->inicio->anterior = nova;
    }
}

void listarPlaylists() {
    Playlist *p = playlists;
    while (p != NULL) {
        printf("- %s\n", p->nome);
        p = p->proximo;
    }
}

void listarMusicas(Playlist *p) {
    if (!p || p->inicio == NULL) return;
    Musica *m = p->inicio;
    do {
        printf("%s - %s (%.2f)\n", m->titulo, m->artista, m->duracao);
        m = m->proximo;
    } while (m != p->inicio);
}

void enfileirar(Musica *m) {
    if (!m) return;
    Fila *novo = malloc(sizeof(Fila));
    if (!novo) return;
    novo->musica = m;
    novo->proximo = NULL;

    if (fila == NULL) {
        fila = novo;
    } else {
        Fila *aux = fila;
        while (aux->proximo != NULL) aux = aux->proximo;
        aux->proximo = novo;
    }
}

void tocarFila() {
    if (fila == NULL) return;

    Fila *aux = fila;
    while (aux != NULL) {
        printf("Tocando: %s - %s\n", aux->musica->titulo, aux->musica->artista);
        aux = aux->proximo;
    }
}

void musicaatual(Playlist *p){
    if(fila != NULL){
        printf("Tocando agora: %s\n", fila->musica->titulo);
        printf(" - %s\n", fila->musica->artista);
        printf("Duracao: %.2f minutos\n", fila->musica->duracao);
        return;
    }

    if(p != NULL && p->inicio != NULL){
        printf("Tocando agora (da playlist %s): %s\n", p->nome, p->inicio->titulo);
        printf(" - %s\n", p->inicio->artista);
        printf("Duracao: %.2f minutos\n", p->inicio->duracao);
        return;
    }
    printf("Nenhuma musica na fila ou playlist.\n");
}

void limparbuff(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int main() {
    setlocale(LC_ALL, "");
    char nome[100], titulo[100], artista[100];
    float duracao;

    for (;;) {
        printf("\n1 - Criar playlist\n");
        printf("2 - Listar playlists\n");
        printf("3 - Adicionar musica\n");
        printf("4 - Listar musicas\n");
        printf("5 - Enfileirar\n");
        printf("6 - Tocar fila\n");
        printf("7 - Mostrar musica atual\n");
        printf("0 - Sair\n> ");

        int opcao;
        if (scanf("%d", &opcao) != 1) {
            limparbuff();
            continue;
        }
        limparbuff();

        switch (opcao){
            case 0:
                return 0;

            case 1:
                printf("Nome da playlist: ");
                if (!fgets(nome, sizeof(nome), stdin)) break;
                nome[strcspn(nome, "\n")] = 0;
                criarplaylist(nome);
                break;

            case 2:
                listarPlaylists();
                break;

            case 3:
                printf("Playlist: ");
                if (!fgets(nome, sizeof(nome), stdin)) break;
                nome[strcspn(nome, "\n")] = 0;
                {
                    Playlist *p = buscarplaylist(nome);
                    if (!p){
                        printf("Playlist nao encontrada.\n");
                        break;
                    }
                    printf("Titulo: ");
                    if (!fgets(titulo, sizeof(titulo), stdin)) break;
                    titulo[strcspn(titulo, "\n")] = 0;

                    printf("Artista: ");
                    if (!fgets(artista, sizeof(artista), stdin)) break;
                    artista[strcspn(artista, "\n")] = 0;

                    printf("Duracao: ");
                    if (scanf("%f", &duracao) != 1) { limparbuff(); break; }
                    limparbuff();

                    adicionarMusica(p, titulo, artista, duracao);
                }
                break;

            case 4:
                printf("Playlist: ");
                if (!fgets(nome, sizeof(nome), stdin)) break;
                nome[strcspn(nome, "\n")] = 0;
                {
                    Playlist *p = buscarplaylist(nome);
                    if (p) listarMusicas(p);
                }
                break;

            case 5:
                printf("Playlist: ");
                if (!fgets(nome, sizeof(nome), stdin)) break;
                nome[strcspn(nome, "\n")] = 0;
                {
                    Playlist *p = buscarplaylist(nome);
                    if (!p || !p->inicio) break;

                    printf("Titulo da musica: ");
                    if (!fgets(titulo, sizeof(titulo), stdin)) break;
                    titulo[strcspn(titulo, "\n")] = 0;

                    Musica *m = p->inicio;
                    do {
                        if (strcmp(m->titulo, titulo) == 0) {
                            enfileirar(m);
                            break;
                        }
                        m = m->proximo;
                    } while (m != p->inicio);
                }
                break;

            case 6:
                tocarFila();
                break;

            case 7:
                printf("Playlist (Aperte Enter para usar a fila): ");
                if (!fgets(nome, sizeof(nome), stdin)) break;
                nome[strcspn(nome, "\n")] = 0;

                if(strlen(nome) == 0)
                    musicaatual(NULL);
                else
                    musicaatual(buscarplaylist(nome));
                break;

            default:
                break;
        }
    }
    return 0;
}
