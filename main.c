#include <stdio.h>
#include <string.h>
#include "models.h"

int menu(void) {
    int op;
    printf("\n1 - Cadastrar setor\n");
    printf("2 - Cadastrar sensor\n");
    printf("3 - Listar setores\n");
    printf("4 - Listar sensores\n");
    printf("5 - Buscar setor\n");
    printf("6 - Buscar sensor\n");
    printf("7 - Remover sensor\n");
    printf("8 - Relatorio sensores\n");
    printf("9 - Relatorio setores\n");
    printf("10 - Exportar HTML\n");
    printf("0 - Sair\n");
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
                printf("Nome: ");
                scanf("%29s", nome);
                printf("Descricao: ");
                scanf("%39s", desc);
                adicionar_setor(&listaSetores, criar_setor(codigo, nome, desc));
                break;
            case 2:
                printf("Codigo: ");
                scanf("%d", &codigo);
                printf("Nome: ");
                scanf("%29s", nome);
                printf("Setor: ");
                scanf("%d", &setor);
                adicionar_sensor(&listaSensores, criar_sensor(codigo, nome, setor));
                atualizar_qtd(listaSetores, setor, 1);
                break;
            case 3:
                listar_setores(listaSetores);
                break;
            case 4:
                listar_sensores(listaSensores);
                break;
            case 5:
                printf("Codigo do setor: ");
                scanf("%d", &codigo);
                printf(buscar_setor_rec(listaSetores, codigo) ? "Achou\n" : "Nao achou\n");
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
                if (modo == 1) {
                    printf("Nome do arquivo: ");
                    scanf("%39s", arquivo);
                    relatorio_sensores(listaSensores, 1, arquivo);
                } else {
                    relatorio_sensores(listaSensores, 2, "");
                }
                break;
            case 9:
                printf("1- arquivo / 2- tela: ");
                scanf("%d", &modo);
                if (modo == 1) {
                    printf("Nome do arquivo: ");
                    scanf("%39s", arquivo);
                    relatorio_setores(listaSetores, 1, arquivo);
                } else {
                    relatorio_setores(listaSetores, 2, "");
                }
                break;
            case 10:
                printf("1- arquivo / 2- tela: ");
                scanf("%d", &modo);
                if (modo == 1) {
                    printf("Nome do arquivo: ");
                    scanf("%39s", arquivo);
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
