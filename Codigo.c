#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct musica{
    char titulo[100];
    struct musica *prev;
    struct musica *proxima;
}musica;

typedef struct playlist{
    char nome[50];
    musica *inicio;
    struct playlist *proxima;
}playlist;

typedef struct numerodafila{
    char titulo[100];
    struct numerodafila *proxima;
} numerodafila;

numerodafila *filainicio = NULL, *finaldafila = NULL;

playlist *listaplaylists = NULL;

void enqueue(char titulo[]){
    numerodafila *novo = malloc(sizeof(numerodafila));
    strcpy(novo->titulo, titulo);
    novo->proxima = NULL;

    if(!finaldafila){
        filainicio = finaldafila = novo;
    }else{
        finaldafila->proxima = novo;
        finaldafila = novo;
    }
}

void dequeue(){
    if(!filainicio){
        printf("Fila vazia!\n");
        return;
    }
    numerodafila *temp = filainicio;
    printf("Tocando agora: %s\n", temp->titulo);
    filainicio = filainicio->proxima;
    if(!filainicio) finaldafila = NULL;
    free(temp);
}

void mostrarfila(){
    numerodafila *l = filainicio;
    printf("\n--- Fila de Reprodução ---\n");
    while(l){
        printf("%s -> ", l->titulo);
        l = l->proxima;
    }
    printf("NULL\n");
}

playlist *criarplaylist(char nome[]){
    playlist *p = malloc(sizeof(playlist));
    strcpy(p->nome, nome);
    p->inicio = NULL;
    p->proxima = listaplaylists;
    listaplaylists = p;
    return p;
}

musica *criarmusica(char titulo[]){
    musica *m = malloc(sizeof(musica));
    strcpy(m->titulo, titulo);
    m->proxima = m->prev = NULL;
    return m;
}

void adicionarmusica(playlist *p, char titulo[]){
    musica *nova = criarmusica(titulo);

    if(!p->inicio){
        p->inicio = nova;
        nova->proxima = nova->prev = nova;
    }else{
        musica *ultimo = p->inicio->prev;
        nova->proxima = p->inicio;
        nova->prev = ultimo;
        ultimo->proxima = nova;
        p->inicio->prev = nova;
    }
}

void removermusica(playlist *p, char titulo[]){
    if(!p->inicio) return;

    musica *atual = p->inicio;

    do{
        if(strcmp(atual->titulo, titulo) == 0){
            if(atual->proxima == atual){
                p->inicio = NULL;
            }else{
                atual->prev->proxima = atual->proxima;
                atual->proxima->prev = atual->prev;
                if(atual == p->inicio)
                    p->inicio = atual->proxima;
            }
            free(atual);
            printf("Musica removida.\n");
            return;
        }
        atual = atual->proxima;
    } while (atual != p->inicio);

    printf("Musica não encontrada.\n");
}

void mstrplaylsit(playlist *p){
    if(!p->inicio){
        printf("\nPlaylist vazia.\n");
        return;
    }

    musica *l = p->inicio;
    printf("\n--- playlist %s ---\n", p->nome);

    do{
        printf("%s <-> ", l->titulo);
        l = l->proxima;
    }while(l != p->inicio);

    printf("(volta ao início)\n");
}

playlist *bscplaylist(char nome[]){
    playlist *l = listaplaylists;
    while(l){
        if(strcmp(l->nome, nome) == 0)
            return l;
        l = l->proxima;
    }
    return NULL;
}

int main(){
    int opc;
    char nomeplaylist[50], musica[100];

    while(1){
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

        switch(opc){
            case 1:
                printf("Nome da playlist: ");
                scanf(" %[^\n]", nomeplaylist);
                criarplaylist(nomeplaylist);
                break;

            case 2:
                printf("playlist: ");
                scanf(" %[^\n]", nomeplaylist);
                playlist *p = bscplaylist(nomeplaylist);
                if (!p) { printf("Não encontrada!\n"); break; }
                printf("Título da musica: ");
                scanf(" %[^\n]", musica);
                adicionarmusica(p, musica);
                break;

            case 3:
                printf("playlist: ");
                scanf(" %[^\n]", nomeplaylist);
                p = bscplaylist(nomeplaylist);
                if (!p) { printf("Não encontrada!\n"); break; }
                printf("Título da musica: ");
                scanf(" %[^\n]", musica);
                removermusica(p, musica);
                break;

            case 4:
                printf("playlist: ");
                scanf(" %[^\n]", nomeplaylist);
                p = bscplaylist(nomeplaylist);
                if (!p) { printf("Não encontrada!\n"); break; }
                mstrplaylsit(p);
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
                mostrarfila();
                break;

            case 0:
                return 0;

            default:
                printf("Opção inválida.\n");
        }
    }
    return 0;
}
