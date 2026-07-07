#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"

Sensor *criar_sensor(int codigo, const char *nome, int setor) {
    Sensor *s = malloc(sizeof(Sensor));
    if (!s) return NULL;
    s->codigo = codigo;
    strcpy(s->nome, nome);
    s->setor = setor;
    s->prox = NULL;
    return s;
}

Setor *criar_setor(int codigo, const char *nome, const char *desc) {
    Setor *s = malloc(sizeof(Setor));
    if (!s) return NULL;
    s->codigo = codigo;
    strcpy(s->nome, nome);
    strcpy(s->desc, desc);
    s->qtd = 0;
    s->leituras = NULL;
    s->prox = NULL;
    return s;
}

int adicionar_leitura(Setor *listaSetores, int setorCodigo, int sensorCodigo, float l1, float l2, const char *h1, const char *h2) {
    if (!listaSetores) return 0;
    Setor *s = listaSetores;
    while (s) {
        if (s->codigo == setorCodigo) break;
        s = s->prox;
    }
    if (!s) return 0;
    Leitura *lt = malloc(sizeof(Leitura));
    if (!lt) return 0;
    lt->sensorCodigo = sensorCodigo;
    lt->setorCodigo = setorCodigo;
    lt->leitura1 = l1;
    lt->leitura2 = l2;
    strncpy(lt->hora1, h1, sizeof(lt->hora1)-1); lt->hora1[sizeof(lt->hora1)-1] = '\0';
    strncpy(lt->hora2, h2, sizeof(lt->hora2)-1); lt->hora2[sizeof(lt->hora2)-1] = '\0';
    lt->prox = s->leituras;
    s->leituras = lt;
    return 1;
}

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ler_string(const char *prompt, char *buffer, int tamanho) {
    if (!prompt || !buffer || tamanho <= 0) return;
    printf("%s", prompt);
    if (!fgets(buffer, tamanho, stdin)) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
}

void imprimir_espacos(int n) {
    for (int i = 0; i < n; i++) putchar(' ');
}

void imprimir_centralizado(const char *texto, int largura) {
    int len = texto ? strlen(texto) : 0;
    if (largura <= 0) return;
    if (len >= largura) {
        for (int i = 0; i < largura; i++) putchar(texto[i]);
        return;
    }
    int espaco = largura - len;
    int esquerda = espaco / 2;
    int direita = espaco - esquerda;
    imprimir_espacos(esquerda);
    if (texto) fputs(texto, stdout);
    imprimir_espacos(direita);
}

Setor *buscar_setor(Setor *lista, int codigo) {
    while (lista) {
        if (lista->codigo == codigo) return lista;
        lista = lista->prox;
    }
    return NULL;
}

Setor *buscar_setor_rec(Setor *lista, int codigo) {
    if (!lista) return NULL;
    if (lista->codigo == codigo) return lista;
    return buscar_setor_rec(lista->prox, codigo);
}

void adicionar_setor(Setor **lista, Setor *novo) {
    if (!lista || !novo) return;
    if (!*lista) {
        *lista = novo;
        return;
    }
    Setor *aux = *lista;
    while (aux->prox) aux = aux->prox;
    aux->prox = novo;
}

void adicionar_sensor(Sensor **lista, Sensor *novo) {
    if (!lista || !novo) return;
    if (!*lista) {
        *lista = novo;
        return;
    }
    Sensor *aux = *lista;
    while (aux->prox) aux = aux->prox;
    aux->prox = novo;
}

Sensor *buscar_sensor(Sensor *lista, int codigo) {
    while (lista) {
        if (lista->codigo == codigo) return lista;
        lista = lista->prox;
    }
    return NULL;
}

void listar_setores(Setor *lista) {
    if (!lista) {
        printf("Nenhum setor cadastrado.\n");
        return;
    }
    printf("\n===== Lista de Setores =====\n");
    printf("+--------+------------------------------+------------------------------------------+-----+\n");
    printf("| Codigo |            Nome              |               Descricao                | Qtd |\n");
    printf("+--------+------------------------------+------------------------------------------+-----+\n");
    Setor *aux = lista;
    while (aux) {
        char codigo[16];
        char qtd[16];
        snprintf(codigo, sizeof(codigo), "%d", aux->codigo);
        snprintf(qtd, sizeof(qtd), "%d", aux->qtd);
        printf("| "); imprimir_centralizado(codigo, 6); printf(" | ");
        imprimir_centralizado(aux->nome, 28); printf(" | ");
        imprimir_centralizado(aux->desc, 40); printf(" | ");
        imprimir_centralizado(qtd, 3); printf(" |\n");
        aux = aux->prox;
    }
    printf("+--------+------------------------------+------------------------------------------+-----+\n");
}

void listar_sensores(Sensor *lista, Setor *listaSetores) {
    if (!lista) {
        printf("Nenhum sensor cadastrado.\n");
        return;
    }
    printf("\n===== Lista de Sensores =====\n");
    printf("+--------+------------------------------+--------+\n");
    printf("| Codigo |            Nome              | Setor  |\n");
    printf("+--------+------------------------------+--------+\n");
    Sensor *aux = lista;
    while (aux) {
        char codigo[16];
        char setorbuf[16];
        snprintf(codigo, sizeof(codigo), "%d", aux->codigo);
        snprintf(setorbuf, sizeof(setorbuf), "%d", aux->setor);
        printf("| "); imprimir_centralizado(codigo, 6); printf(" | ");
        imprimir_centralizado(aux->nome, 28); printf(" | ");
        imprimir_centralizado(setorbuf, 6); printf(" |\n");

        /* mostrar leituras do sensor (se existirem) */
        if (listaSetores) {
            Setor *s = buscar_setor(listaSetores, aux->setor);
            if (s && s->leituras) {
                Leitura *lt = s->leituras;
                while (lt) {
                    if (lt->sensorCodigo == aux->codigo) {
                        printf("    -> Leitura %s: %.2f | Leitura %s: %.2f\n", lt->hora1, lt->leitura1, lt->hora2, lt->leitura2);
                    }
                    lt = lt->prox;
                }
            }
        }
        aux = aux->prox;
    }
    printf("+--------+------------------------------+--------+\n");
}


int buscar_sensor_rec(Sensor *lista, int codigo) {
    if (!lista) return 0;
    if (lista->codigo == codigo) return 1;
    return buscar_sensor_rec(lista->prox, codigo);
}

void atualizar_qtd(Setor *lista, int codigo, int valor) {
    Setor *aux = lista;
    while (aux) {
        if (aux->codigo == codigo) {
            aux->qtd += valor;
            return;
        }
        aux = aux->prox;
    }
}

int remover_sensor(Sensor **listaSensores, Setor **listaSetores, int codigo) {
    Sensor *ant = NULL, *atual = *listaSensores;
    while (atual && atual->codigo != codigo) {
        ant = atual;
        atual = atual->prox;
    }
    if (!atual) return 0;
    if (ant) ant->prox = atual->prox;
    else *listaSensores = atual->prox;
    atualizar_qtd(*listaSetores, atual->setor, -1);
    free(atual);
    return 1;
}

int salvar_setores(const char *nome, Setor *lista) {
    FILE *arq = fopen(nome, "wb");
    if (!arq) return 0;
    Setor *aux = lista;
    while (aux) {
        struct { int codigo; char nome[30]; char desc[40]; int qtd; } reg;
        reg.codigo = aux->codigo;
        strcpy(reg.nome, aux->nome);
        strcpy(reg.desc, aux->desc);
        reg.qtd = aux->qtd;
        fwrite(&reg, sizeof(reg), 1, arq);
        aux = aux->prox;
    }
    fclose(arq);
    return 1;
}

int carregar_setores(const char *nome, Setor **lista) {
    FILE *arq = fopen(nome, "rb");
    if (!arq) return 0;
    struct { int codigo; char nome[30]; char desc[40]; int qtd; } reg;
    while (fread(&reg, sizeof(reg), 1, arq) == 1) {
        Setor *s = criar_setor(reg.codigo, reg.nome, reg.desc);
        s->qtd = reg.qtd;
        adicionar_setor(lista, s);
    }
    fclose(arq);
    return 1;
}

int salvar_sensores(const char *nome, Sensor *lista) {
    FILE *arq = fopen(nome, "wb");
    if (!arq) return 0;
    Sensor *aux = lista;
    while (aux) {
        struct { int codigo; char nome[30]; int setor; } reg;
        reg.codigo = aux->codigo;
        strcpy(reg.nome, aux->nome);
        reg.setor = aux->setor;
        fwrite(&reg, sizeof(reg), 1, arq);
        aux = aux->prox;
    }
    fclose(arq);
    return 1;
}

int carregar_sensores(const char *nome, Sensor **lista) {
    FILE *arq = fopen(nome, "rb");
    if (!arq) return 0;
    struct { int codigo; char nome[30]; int setor; } reg;
    while (fread(&reg, sizeof(reg), 1, arq) == 1) {
        Sensor *s = criar_sensor(reg.codigo, reg.nome, reg.setor);
        adicionar_sensor(lista, s);
    }
    fclose(arq);
    return 1;
}

void relatorio_sensores(Sensor *lista, int modo, const char *nomeArquivo) {
    FILE *arq = NULL;
    if (modo == 1) {
        arq = fopen(nomeArquivo, "w");
        if (!arq) return;
        fprintf(arq, "codigo,nome,setor\n");
    } else {
        printf("codigo,nome,setor\n");
    }
    Sensor *aux = lista;
    while (aux) {
        if (modo == 1) fprintf(arq, "%d,%s,%d\n", aux->codigo, aux->nome, aux->setor);
        else printf("%d,%s,%d\n", aux->codigo, aux->nome, aux->setor);
        aux = aux->prox;
    }
    if (arq) fclose(arq);
}

void relatorio_setores(Setor *lista, int modo, const char *nomeArquivo) {
    FILE *arq = NULL;
    if (modo == 1) {
        arq = fopen(nomeArquivo, "w");
        if (!arq) return;
        fprintf(arq, "codigo,nome,descricao,qtd\n");
    } else {
        printf("codigo,nome,descricao,qtd\n");
    }
    Setor *aux = lista;
    while (aux) {
        if (modo == 1) fprintf(arq, "%d,%s,%s,%d\n", aux->codigo, aux->nome, aux->desc, aux->qtd);
        else printf("%d,%s,%s,%d\n", aux->codigo, aux->nome, aux->desc, aux->qtd);
        aux = aux->prox;
    }
    if (arq) fclose(arq);
}

void exportar_html(Sensor *listaSensores, Setor *listaSetores, int modo, const char *nomeArquivo) {
    FILE *arq = NULL;
    if (modo == 1) {
        arq = fopen(nomeArquivo, "w");
        if (!arq) return;
    }
    if (modo == 1) {
        fprintf(arq, "<html><body><h1>Relatorio</h1>");
        fprintf(arq, "<h2>Sensores</h2><table><tr><th>Codigo</th><th>Nome</th><th>Setor</th></tr>");
    } else {
        printf("<html><body><h1>Relatorio</h1>");
        printf("<h2>Sensores</h2><table><tr><th>Codigo</th><th>Nome</th><th>Setor</th></tr>");
    }
    Sensor *s = listaSensores;
    while (s) {
        if (modo == 1) fprintf(arq, "<tr><td>%d</td><td>%s</td><td>%d</td></tr>", s->codigo, s->nome, s->setor);
        else printf("<tr><td>%d</td><td>%s</td><td>%d</td></tr>", s->codigo, s->nome, s->setor);
        s = s->prox;
    }
    if (modo == 1) fprintf(arq, "</table><h2>Setores</h2><table><tr><th>Codigo</th><th>Nome</th><th>Descricao</th><th>Qtd</th></tr>");
    else printf("</table><h2>Setores</h2><table><tr><th>Codigo</th><th>Nome</th><th>Descricao</th><th>Qtd</th></tr>");
    Setor *se = listaSetores;
    while (se) {
        if (modo == 1) fprintf(arq, "<tr><td>%d</td><td>%s</td><td>%s</td><td>%d</td></tr>", se->codigo, se->nome, se->desc, se->qtd);
        else printf("<tr><td>%d</td><td>%s</td><td>%s</td><td>%d</td></tr>", se->codigo, se->nome, se->desc, se->qtd);
        se = se->prox;
    }
    if (modo == 1) fprintf(arq, "</table></body></html>\n");
    else printf("</table></body></html>\n");
    if (arq) fclose(arq);
}

void liberar_memoria(Sensor **listaSensores, Setor **listaSetores) {
    Sensor *s;
    while (*listaSensores) {
        s = *listaSensores;
        *listaSensores = (*listaSensores)->prox;
        free(s);
    }
    Setor *se;
    while (*listaSetores) {
        se = *listaSetores;
        *listaSetores = (*listaSetores)->prox;
        free(se);
    }
}
