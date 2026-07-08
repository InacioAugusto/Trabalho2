#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"

/* ============================================================
   FUNCOES AUXILIARES DE IMPRESSAO
   Usadas para montar as tabelas bonitinhas no terminal,
   centralizando o texto dentro de uma largura fixa.
   ============================================================ */

/* Imprime "n" espacos em branco seguidos */
void imprimir_espacos(int n) {
    for (int i = 0; i < n; i++) {
        printf(" ");
    }
}

/* Centraliza um texto dentro de uma largura fixa (para as tabelas) */
void imprimir_centralizado(const char *texto, int largura) {
    int len = texto ? (int)strlen(texto) : 0;
    if (largura <= 0) return;

    /* Se o texto for maior que a largura, corta e imprime só o que cabe */
    if (len >= largura) {
        printf("%.*s", largura, texto);
        return;
    }

    /* Calcula quantos espacos colocar de cada lado do texto */
    int espaco = largura - len;
    int esquerda = espaco / 2;
    int direita = espaco - esquerda;

    imprimir_espacos(esquerda);
    if (texto) printf("%s", texto);
    imprimir_espacos(direita);
}

/* ============================================================
   CRIACAO DE ELEMENTOS
   Alocam memoria (malloc) para um novo Sensor ou Setor e
   preenchem os campos com strncpy (copia segura de strings).
   ============================================================ */

Sensor *criar_sensor(int codigo, const char *nome, int setor) {
    Sensor *s = malloc(sizeof(Sensor));
    if (!s) return NULL;
    s->codigo = codigo;
    strncpy(s->nome, nome, sizeof(s->nome) - 1);
    s->nome[sizeof(s->nome) - 1] = '\0';
    s->setor = setor;
    s->prox = NULL;
    return s;
}

Setor *criar_setor(int codigo, const char *nome, const char *desc) {
    Setor *s = malloc(sizeof(Setor));
    if (!s) return NULL;
    s->codigo = codigo;
    strncpy(s->nome, nome, sizeof(s->nome) - 1);
    s->nome[sizeof(s->nome) - 1] = '\0';
    strncpy(s->desc, desc, sizeof(s->desc) - 1);
    s->desc[sizeof(s->desc) - 1] = '\0';
    s->qtd = 0;
    s->leituras = NULL;
    s->prox = NULL;
    return s;
}

/* Cria uma Leitura e a encaixa no INICIO da lista de leituras do setor */
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
    strncpy(lt->hora1, h1, sizeof(lt->hora1) - 1);
    lt->hora1[sizeof(lt->hora1) - 1] = '\0';
    strncpy(lt->hora2, h2, sizeof(lt->hora2) - 1);
    lt->hora2[sizeof(lt->hora2) - 1] = '\0';
    lt->prox = s->leituras;
    s->leituras = lt;
    return 1;
}

/* ============================================================
   ENTRADA DE DADOS (leitura segura do teclado)
   ============================================================ */

/* Descarta o resto do que estiver no buffer do teclado ate a quebra de linha */
void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Le uma linha de texto do teclado, removendo o '\n' do final */
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

/* ============================================================
   BUSCA
   Cada busca tem duas versoes: iterativa (com while) e
   recursiva (a funcao chama ela mesma) - dois jeitos de
   resolver o mesmo problema.
   ============================================================ */

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

Sensor *buscar_sensor(Sensor *lista, int codigo) {
    while (lista) {
        if (lista->codigo == codigo) return lista;
        lista = lista->prox;
    }
    return NULL;
}

int buscar_sensor_rec(Sensor *lista, int codigo) {
    if (!lista) return 0;
    if (lista->codigo == codigo) return 1;
    return buscar_sensor_rec(lista->prox, codigo);
}

/* ============================================================
   INSERCAO NAS LISTAS ENCADEADAS
   Percorrem a lista ate o ultimo elemento e encaixam o novo
   no final (inserir no fim = percorrer tudo ate achar o ultimo).
   ============================================================ */

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

/* Soma (ou subtrai) "valor" na quantidade de sensores de um setor */
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

/* ============================================================
   REMOCAO
   ============================================================ */

/* Remove um sensor da lista, ajustando os ponteiros dos vizinhos
   e atualizando a quantidade de sensores do setor dele */
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

/* ============================================================
   LISTAGEM / EXIBICAO NA TELA
   ============================================================ */

void listar_setores(Setor *lista) {
    if (!lista) {
        printf("Nenhum setor cadastrado.\n");
        return;
    }
    printf("\n===== Lista de Setores =====\n");
    printf("+--------+------------------------------+------------------------------------------+-----+\n");
    printf("| Codigo | Nome                         | Descricao                                | Qtd |\n");
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

/* Lista os sensores e, para cada um, mostra tambem as leituras
   registradas no setor a que ele pertence */
void listar_sensores(Sensor *lista, Setor *listaSetores) {
    if (!lista) {
        printf("Nenhum sensor cadastrado.\n");
        return;
    }
    printf("\n===== Lista de Sensores =====\n");
    printf("+--------+------------------------------+--------+\n");
    printf("| Codigo | Nome                         | Setor  |\n");
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

        if (listaSetores) {
            Setor *s = buscar_setor(listaSetores, aux->setor);
            if (s && s->leituras) {
                Leitura *lt = s->leituras;
                while (lt) {
                    if (lt->sensorCodigo == aux->codigo) {
                        printf("      -> Leitura %s: %.2f | Leitura %s: %.2f\n",
                               lt->hora1, lt->leitura1, lt->hora2, lt->leitura2);
                    }
                    lt = lt->prox;
                }
            }
        }
        aux = aux->prox;
    }
    printf("+--------+------------------------------+--------+\n");
}

/* ============================================================
   PERSISTENCIA EM ARQUIVO BINARIO (salvar / carregar)
   Cada dado eh copiado para uma "struct temporaria" so com
   os dados simples antes de gravar (fwrite) ou ler (fread).
   ============================================================ */

int salvar_setores(const char *nome, Setor *lista) {
    FILE *arq = fopen(nome, "wb");
    if (!arq) return 0;
    Setor *aux = lista;
    while (aux) {
        struct { int codigo; char nome[30]; char desc[40]; int qtd; } reg;
        reg.codigo = aux->codigo;
        strncpy(reg.nome, aux->nome, 30);
        strncpy(reg.desc, aux->desc, 40);
        reg.qtd = aux->qtd;
        fwrite(&reg, sizeof(reg), 1, arq);

        /* salvar leituras do setor */
        int countLeituras = 0;
        Leitura *lt = aux->leituras;
        while (lt) { countLeituras++; lt = lt->prox; }
        fwrite(&countLeituras, sizeof(int), 1, arq);
        lt = aux->leituras;
        while (lt) {
            struct { int sensorCodigo; int setorCodigo; float l1; float l2; char h1[6]; char h2[6]; } regL;
            regL.sensorCodigo = lt->sensorCodigo;
            regL.setorCodigo = lt->setorCodigo;
            regL.l1 = lt->leitura1;
            regL.l2 = lt->leitura2;
            strncpy(regL.h1, lt->hora1, 6);
            strncpy(regL.h2, lt->hora2, 6);
            fwrite(&regL, sizeof(regL), 1, arq);
            lt = lt->prox;
        }
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

        int countLeituras = 0;
        if (fread(&countLeituras, sizeof(int), 1, arq) != 1) countLeituras = 0;
        for (int i = 0; i < countLeituras; i++) {
            struct { int sensorCodigo; int setorCodigo; float l1; float l2; char h1[6]; char h2[6]; } regL;
            if (fread(&regL, sizeof(regL), 1, arq) != 1) break;
            Leitura *lt = malloc(sizeof(Leitura));
            if (!lt) break;
            lt->sensorCodigo = regL.sensorCodigo;
            lt->setorCodigo = regL.setorCodigo;
            lt->leitura1 = regL.l1;
            lt->leitura2 = regL.l2;
            strncpy(lt->hora1, regL.h1, 6);
            strncpy(lt->hora2, regL.h2, 6);
            lt->prox = s->leituras;
            s->leituras = lt;
        }
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
        strncpy(reg.nome, aux->nome, 30);
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

/* ============================================================
   RELATORIOS (CSV e HTML)
   modo == 1 -> escreve em arquivo | modo == 0 -> imprime na tela
   ============================================================ */

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

/* Gera um relatorio em HTML com duas tabelas: sensores e setores */
void exportar_html(Sensor *listaSensores, Setor *listaSetores, int modo, const char *nomeArquivo) {
    FILE *arq = NULL;
    if (modo == 1) {
        arq = fopen(nomeArquivo, "w");
        if (!arq) return;
    }
    if (modo == 1) {
        fprintf(arq, "<html><body><h1>Relatorio</h1>"); //abre html
        fprintf(arq, "<h2>Sensores</h2><table><tr><th>Codigo</th><th>Nome</th><th>Setor</th></tr>");
    } else {
        printf("<html><body><h1>Relatorio</h1>"); //abre html
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
    if (modo == 1) fprintf(arq, "</table></body></html>\n"); //fecha html
    else printf("</table></body></html>\n");// fecha html
    if (arq) fclose(arq);
}

/* ============================================================
   LIBERACAO DE MEMORIA
   Percorre as duas listas e da free em cada no, incluindo
   as leituras dentro de cada setor.
   ============================================================ */

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
        /* liberar leituras do setor */
        Leitura *lt = se->leituras;
        while (lt) {
            Leitura *tmp = lt;
            lt = lt->prox;
            free(tmp);
        }
        free(se);
    }
}
