#include <stdio.h>
#include <string.h>
#include "models.h"

int menu(void) {
    int op;
    printf("\n===== Menu =====\n");
    printf("1  - Cadastrar setor\n");
    printf("2  - Cadastrar sensor\n");
    printf("3  - Listar setores\n");
    printf("4  - Listar sensores\n");
    printf("6  - Buscar setor (recursiva)\n");
    printf("7  - Buscar sensor (recursiva)\n");
    printf("8  - Remover sensor\n");
    printf("9  - Relatorio sensores (csv)\n");
    printf("10 - Relatorio setores (csv)\n");
    printf("11 - Exportar HTML (html)\n");
    printf("11 - Cadastrar leitura (06:00 e 18:00)\n");
    printf("0  - Sair\n");
    printf("Opcao: ");
    scanf("%d", &op);
    return op;
}

int main(void) {
    Setor *listaSetores = NULL;
    Sensor *listaSensores = NULL;
    int op, codigo, setor, modo;
    char nome[30], desc[40], arquivo[40];

    carregar_setores("setores.bin", &listaSetores);
    carregar_sensores("sensores.bin", &listaSensores);

    do {
        op = menu();
        switch (op) {
            case 1:
                printf("Codigo: ");
                scanf("%d", &codigo);
                limpar_buffer();
                ler_string("Nome: ", nome, sizeof(nome));
                ler_string("Descricao: ", desc, sizeof(desc));
                adicionar_setor(&listaSetores, criar_setor(codigo, nome, desc));
                break;
            case 2:
                printf("Codigo: ");
                scanf("%d", &codigo);
                limpar_buffer();
                ler_string("Nome: ", nome, sizeof(nome));
                printf("Setor: ");
                scanf("%d", &setor);
                if (buscar_setor(listaSetores, setor)) {
                    adicionar_sensor(&listaSensores, criar_sensor(codigo, nome, setor));
                    atualizar_qtd(listaSetores, setor, 1);
                } else {
                    printf("Setor nao encontrado\n");
                }
                break;
            case 3:
                listar_setores(listaSetores);
                break;
            case 4:
                listar_sensores(listaSensores, listaSetores);
                break;
            case 11:
                {
                    int cod;
                    float l1, l2;
                    printf("Codigo do sensor: ");
                    scanf("%d", &cod);
                    Sensor *s = buscar_sensor(listaSensores, cod);
                    if (!s) {
                        printf("Sensor nao encontrado\n");
                        break;
                    }
                    printf("Leitura 06:00: ");
                    scanf("%f", &l1);
                    printf("Leitura 18:00: ");
                    scanf("%f", &l2);
                    if (adicionar_leitura(listaSetores, s->setor, s->codigo, l1, l2, "06:00", "18:00"))
                        printf("Leitura cadastrada com sucesso\n");
                    else
                        printf("Falha ao cadastrar leitura\n");
                }
                break;
            case 5:
                printf("Codigo do setor: ");
                scanf("%d", &codigo);
                {
                    Setor *s = buscar_setor_rec(listaSetores, codigo);
                    if (s) {
                        printf("Achou: %d | %s | %s | qtd %d\n", s->codigo, s->nome, s->desc, s->qtd);
                    } else {
                        printf("Nao achou\n");
                    }
                }
                break;
            case 6:
                printf("Codigo do sensor: ");
                scanf("%d", &codigo);
                printf(buscar_sensor_rec(listaSensores, codigo) ? "Achou\n" : "Nao achou\n");
                break;
            case 7:
                printf("Codigo do sensor: ");
                scanf("%d", &codigo);
                if (remover_sensor(&listaSensores, &listaSetores, codigo)) printf("Removido\n");
                else printf("Nao encontrado\n");
                break;
            case 8:
                printf("1- arquivo / 2- tela: ");
                scanf("%d", &modo);
                limpar_buffer();
                if (modo == 1) {
                    ler_string("Nome do arquivo: ", arquivo, sizeof(arquivo));
                    relatorio_sensores(listaSensores, 1, arquivo);
                } else {
                    relatorio_sensores(listaSensores, 2, "");
                }
                break;
            case 9:
                printf("1- arquivo / 2- tela: ");
                scanf("%d", &modo);
                limpar_buffer();
                if (modo == 1) {
                    ler_string("Nome do arquivo: ", arquivo, sizeof(arquivo));
                    relatorio_setores(listaSetores, 1, arquivo);
                } else {
                    relatorio_setores(listaSetores, 2, "");
                }
                break;
            case 10:
                printf("1- arquivo / 2- tela: ");
                scanf("%d", &modo);
                limpar_buffer();
                if (modo == 1) {
                    ler_string("Nome do arquivo: ", arquivo, sizeof(arquivo));
                    exportar_html(listaSensores, listaSetores, 1, arquivo);
                } else {
                    exportar_html(listaSensores, listaSetores, 2, "");
                }
                break;
        }
    } while (op != 0);

    salvar_setores("setores.bin", listaSetores);
    salvar_sensores("sensores.bin", listaSensores);
    liberar_memoria(&listaSensores, &listaSetores);
    return 0;
}
