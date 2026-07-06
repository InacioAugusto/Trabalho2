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
    s->prox = NULL;
    return s;
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

void listar_setores(Setor *lista) {
    Setor *aux = lista;
    while (aux) {
        printf("%d | %s | %s | %d\n", aux->codigo, aux->nome, aux->desc, aux->qtd);
        aux = aux->prox;
    }
}

void listar_sensores(Sensor *lista) {
    Sensor *aux = lista;
    while (aux) {
        printf("%d | %s | setor %d\n", aux->codigo, aux->nome, aux->setor);
        aux = aux->prox;
    }
}

int buscar_setor_rec(Setor *lista, int codigo) {
    if (!lista) return 0;
    if (lista->codigo == codigo) return 1;
    return buscar_setor_rec(lista->prox, codigo);
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
