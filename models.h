#ifndef MODELS_H
#define MODELS_H

typedef struct Sensor {
    int codigo;
    char nome[30];
    int setor;
    struct Sensor *prox;
} Sensor;

typedef struct Setor {
    int codigo;
    char nome[30];
    char desc[40];
    int qtd;
    struct Setor *prox;
} Setor;

Sensor *criar_sensor(int codigo, const char *nome, int setor);
Setor *criar_setor(int codigo, const char *nome, const char *desc);

void adicionar_setor(Setor **lista, Setor *novo);
void adicionar_sensor(Sensor **lista, Sensor *novo);
void listar_setores(Setor *lista);
void listar_sensores(Sensor *lista);

int buscar_setor_rec(Setor *lista, int codigo);
int buscar_sensor_rec(Sensor *lista, int codigo);

void atualizar_qtd(Setor *lista, int codigo, int valor);
int remover_sensor(Sensor **listaSensores, Setor **listaSetores, int codigo);

int salvar_setores(const char *nome, Setor *lista);
int carregar_setores(const char *nome, Setor **lista);
int salvar_sensores(const char *nome, Sensor *lista);
int carregar_sensores(const char *nome, Sensor **lista);

void relatorio_sensores(Sensor *lista, int modo, const char *nomeArquivo);
void relatorio_setores(Setor *lista, int modo, const char *nomeArquivo);
void exportar_html(Sensor *listaSensores, Setor *listaSetores, int modo, const char *nomeArquivo);

void liberar_memoria(Sensor **listaSensores, Setor **listaSetores);

#endif
