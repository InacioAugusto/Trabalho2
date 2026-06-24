#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//Programa 1

#define T_STR        100
#define T_SENSORES   50
#define T_SETORES    30
#define T_MAX_SPS    3 //SENSORES POR SETOR
#define FIM          0

typedef char string[T_STR];

typedef struct sensor {
    int id;
    string tipo;
    string unidade;
    float leitura_min;
    float leitura_max;

} sensor_t;

typedef struct setor {
    int id;
    string descricao;
    int qtd_sps;    //SENSORES POR SETOR
    float hora_leitura; //Rever
    int id_sensores[T_MAX_SPS];
} setor_t;

typedef struct leitura {
    int id_sensor;
    int id_setor;

    float leitura1;
    float leitura2;

    char hora1[10];
    char hora2[10];
} leitura_t;

// Variáveis globais
sensor_t sensores[T_SENSORES];
setor_t setores[T_SETORES];
leitura_t leituras[100];

int qtd_sensores = 0;
int qtd_setores = 0;
int qtd_leituras = 0;

// Protótipo das funções auxiliares
int menu_principal(void);
int menu_sensores(void);
int menu_setores(void);
int menu_consultas(void);
int menu_relatorios(void);
sensor_t novo_sensor(void);
setor_t novo_setor(void);
void mostrar_dados_setor(setor_t setor);
void mostrar_dados_sensor(sensor_t sensor);
void listar_setores(void);
void listar_sensores(void);
void alocar_sensor(int indice_setor);
void pesquisar_setor_descricao(void); //funcao de consulta apenas
void pesquisar_sensor_tipo(void); //Funcao consulta apenas
int buscar_indice_setor_por_descricao(void); //uso em alocar
int sensor_disponivel_alocacao(int id_sensor);

void retirar_enter(string str);
void formatar_maiuscula(string str);
void mostrar_msg(char* msg);
void limpar_tela(void);
void logo(void);

void cadastrar_leitura(void);
void listar_leituras(void);
void relatorio_sensor_tipo(void);
void relatorio_leituras_setor(void);
void relatorio_variacao_sensor(void);
void relatorio_media_sensor(void);

int main()
{
    int opcao, opc_1, opc_2, opc_3, opc_4, retorno;

    do {
        limpar_tela();
        logo();
        opcao = menu_principal();

        switch (opcao) {
            case 1:
                do {
                    limpar_tela();
                    opc_1 = menu_sensores();

                    switch (opc_1) {
                        case 1:
                            sensores[qtd_sensores] = novo_sensor(); // cadastro sensor
                            qtd_sensores++;
                            break;

                        case 2:
                            listar_sensores(); // lista sensor
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;

                        case 3:
                            cadastrar_leitura();
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;

                        case 4:
                            listar_leituras();
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;
                    }
                } while (opc_1 != FIM);
                break;

            case 2:
                do {
                    limpar_tela();
                    opc_2 = menu_setores();

                    switch (opc_2) {
                        case 1:
                            setores[qtd_setores] = novo_setor(); // cadastro setor
                            qtd_setores++;
                            break;

                        case 2:
                            listar_setores(); //lista setor
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;

                        case 3:
                            if (qtd_setores == 0) {
                                printf("\nNenhum setor cadastrado!\n");
                                mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                                break;
                            }

                            printf("Em qual setor voce gostaria de alocar?\n");
                            int indice = buscar_indice_setor_por_descricao();
                            alocar_sensor(indice);
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;
                    }
                } while (opc_2 != FIM);

                break;

            case 3:
                do {
                    limpar_tela();
                    opc_3 = menu_consultas();

                    switch (opc_3) {
                        case 1:
                            pesquisar_sensor_tipo();
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;

                        case 2:
                            pesquisar_setor_descricao();
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;
                    }
                } while (opc_3 != FIM);
                break;

            case 4:
                do {
                    limpar_tela();
                    opc_4 = menu_relatorios();

                    switch (opc_4) {
                        case 1:
                            listar_sensores();
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;

                        case 2:
                            listar_setores();
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;

                        case 3:
                            relatorio_sensor_tipo();
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;

                        case 4:
                            relatorio_leituras_setor();
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;

                        case 5:
                            relatorio_variacao_sensor();
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;

                        case 6:
                            relatorio_media_sensor();
                            mostrar_msg("PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL");
                            break;
                    }
                } while (opc_4 != FIM);
                break;
        }

    } while (opcao != FIM);

    return 0;
}

// Implementação das funções auxiliares

int menu_principal(void)
{
    int opcao;

    printf("------------------------------\n");
    printf("| INACIO & OTAVIO AUTOMACOES S/A |\n");
    printf("------------------------------\n");
    printf("1.Menu Sensores\n");
    printf("2.Menu Setores\n");
    printf("3.Menu Consultas\n");
    printf("4.Menu Relatorios\n");
    printf("0.Sair do programa\n");
    printf("------------------------------\n");
    printf("::: ");
    scanf("%i", &opcao);
    getchar();

    return opcao;
}

int menu_sensores(void)
{
    int opcao;

    printf("----------------------------\n");
    printf("|        MENU SENSORES      |\n");
    printf("----------------------------\n");
    printf("1.Cadastrar Sensor\n");
    printf("2.Listar Sensores cadastrados\n");
    printf("3.Cadastrar leituras\n");
    printf("4.Listar leituras\n");
    printf("0.Retornar ao menu principal\n");
    printf("----------------------------\n");
    printf("::: ");
    scanf("%i", &opcao);
    getchar();

    return opcao;
}

int menu_setores(void)
{
    int opcao;

    printf("----------------------------\n");
    printf("|        MENU SETORES      |\n");
    printf("----------------------------\n");
    printf("1.Cadastrar Setor\n");
    printf("2.Listar Setores cadastrados\n");
    printf("3.Alocar Sensores em Setor\n");
    printf("0.Retornar ao menu principal\n");
    printf("----------------------------\n");
    printf("::: ");
    scanf("%i", &opcao);
    getchar();

    return opcao;
}

int menu_consultas(void)
{
    int opcao;

    printf("----------------------------\n");
    printf("|       MENU CONSULTAS     |\n");
    printf("----------------------------\n");
    printf("1.Consultar Sensor por Tipo\n");
    printf("2.Consultar Setor por Descrição\n");
    printf("0.Retornar ao menu principal\n");
    printf("----------------------------\n");
    printf("::: ");
    scanf("%i", &opcao);
    getchar();

    return opcao;
}

int menu_relatorios(void)
{
    int opcao;

    printf("----------------------------\n");
    printf("|      MENU RELATORIOS     |\n");
    printf("----------------------------\n");
    printf("1.Relatorio de sensores\n");
    printf("2.Relatorio de setores\n");
    printf("3.Relatorio de sensores por tipo\n");
    printf("4.Relatorio de leituras por setor\n");
    printf("5.Relatorio de variacao por sensor\n");
    printf("6.Relatorio de media por sensor\n");
    printf("0.Retornar ao menu principal\n");
    printf("----------------------------\n");
    printf("::: ");
    scanf("%i", &opcao);
    getchar();

    return opcao;
}
      
sensor_t novo_sensor(void)
{
    sensor_t sensor;

    limpar_tela();
    printf("\033[34mCadastro de Sensores\033[0m\n");

    sensor.id = qtd_sensores + 1;
    printf("Id.....................: %i\n", sensor.id);

    printf("Sensor de.........: ");
    fgets(sensor.tipo, T_STR, stdin);
    retirar_enter(sensor.tipo);
    formatar_maiuscula(sensor.tipo);

    printf("Unidade de medida.......: ");
    fgets(sensor.unidade, T_STR, stdin);
    retirar_enter(sensor.unidade);
    formatar_maiuscula(sensor.unidade);

    printf("Leitura Minima.......: ");
    scanf("%f", &sensor.leitura_min);

    printf("Leitura Maxima.......: ");
    scanf("%f", &sensor.leitura_max);
    getchar();

    return sensor;
}

setor_t novo_setor(void)
{
    setor_t setor;

    limpar_tela();
    printf("\033[34mCadastro de Setores\033[0m\n");

    setor.id = qtd_setores + 101;
    printf("Id...................: %i\n", setor.id);

    printf("Descricao.........: ");
    fgets(setor.descricao, T_STR, stdin);
    retirar_enter(setor.descricao);
    formatar_maiuscula(setor.descricao);

    setor.qtd_sps = 0;

    for (int i = 0; i < T_MAX_SPS; i++) {
        setor.id_sensores[i] = 0;
    }

    return setor;
}

void mostrar_dados_sensor(sensor_t sensor)
{
    printf("Id...............: %i\n", sensor.id);
    printf("Tipo.............: %s\n", sensor.tipo);
    printf("Faixa: %.2f a %.2f (%s)\n\n",
           sensor.leitura_min,
           sensor.leitura_max,
           sensor.unidade);
}

void mostrar_dados_setor(setor_t setor)
{
    printf("Id.................: %i\n", setor.id);
    printf("Descricao..........: %s\n", setor.descricao);
    printf("Sensores instalados no setor:\n");
    printf("Quantidade de Sensores alocados: %i\n", setor.qtd_sps);

    for (int i = 0; i < T_MAX_SPS; i++) {
        if (setor.id_sensores[i] != 0)
            printf("Id Sensores Alocados: %i\n", setor.id_sensores[i]);
    }

    printf("\n");
}

void listar_sensores(void)
{
    limpar_tela();
    printf("\033[34mRelatório de Sensores\033[0m\n");

    for (int i = 0; i < qtd_sensores; i++) {
        mostrar_dados_sensor(sensores[i]);
    }
}

void listar_setores(void)
{
    limpar_tela();
    printf("\033[34mRelatório de Setores\033[0m\n");

    for (int i = 0; i < qtd_setores; i++) {
        mostrar_dados_setor(setores[i]);
    }
}

void alocar_sensor(int indice_setor)
{
    int id;
    int disponivel;

    if (indice_setor == -1) {
        printf("\nSetor nao encontrado!\n");
        return;
    }

    if (setores[indice_setor].qtd_sps >= T_MAX_SPS) {
        printf("\nSetor cheio!\n");
        return;
    }

    if (qtd_sensores == 0) {
        printf("\nNenhum sensor cadastrado!\n");
        return;
    }

    listar_sensores();
    printf("\nQual Sensor sera alocado?\n");

    printf("\nDigite o ID do sensor: ");
    scanf("%d", &id);
    getchar();

    disponivel = sensor_disponivel_alocacao(id);

    if (!disponivel) {
        printf("\nSensor inexistente ou ja alocado!\n");
        return;
    }

    for (int i = 0; i < T_MAX_SPS; i++) {
        if (setores[indice_setor].id_sensores[i] == 0) {
            setores[indice_setor].id_sensores[i] = id;
            setores[indice_setor].qtd_sps++;

            printf("\nSensor alocado com sucesso!\n");
            return;
        }
    }
}

void retirar_enter(string str)
{
    str[strlen(str) - 1] = '\0';
}

void formatar_maiuscula(string str)
{
    for (int i = 0; i < strlen(str); i++) {
        str[i] = toupper(str[i]);
    }
}

void limpar_tela(void)
{
    system("clear");
    //system("cls");
}

void mostrar_msg(char* msg)
{
    printf("\n\033[34m%s\033[0m\n", msg);
    getchar();
}

void pesquisar_setor_descricao(void)
{
    string busca;

    limpar_tela();

    printf("Digite a descricao do setor: ");
    fgets(busca, T_STR, stdin);

    retirar_enter(busca);
    formatar_maiuscula(busca);

    for (int i = 0; i < qtd_setores; i++) {
        if (!strcmp(setores[i].descricao, busca)) {
            mostrar_dados_setor(setores[i]);
            return;
        }
    }

    printf("\nSetor nao encontrado.\n");
}

void pesquisar_sensor_tipo(void)
{
    string busca;
    int encontrou = 0;

    limpar_tela();

    printf("Digite o tipo do sensor: ");
    fgets(busca, T_STR, stdin);

    retirar_enter(busca);
    formatar_maiuscula(busca);

    for (int i = 0; i < qtd_sensores; i++) {
        if (!strcmp(sensores[i].tipo, busca)) {
            mostrar_dados_sensor(sensores[i]);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("\nNenhum sensor encontrado.\n");
    }
}

int buscar_indice_setor_por_descricao(void) //alocar
{
    string busca;

    printf("Digite a descricao do setor: ");
    fgets(busca, T_STR, stdin);

    retirar_enter(busca);
    formatar_maiuscula(busca);

    for (int i = 0; i < qtd_setores; i++) {
        if (strcmp(setores[i].descricao, busca) == 0) {
            return i;
        }
    }

    return -1;
}

int sensor_disponivel_alocacao(int id_sensor) // alocar
{
    int existe = 0;

    // 1 verificacao - sensor existe
    for (int i = 0; i < qtd_sensores; i++) {
        if (sensores[i].id == id_sensor) {
            existe = 1;
            break;
        }
    }

    if (!existe)
        return 0;

    // 2 verificacao - sensor alocado
    for (int j = 0; j < qtd_setores; j++) {
        for (int k = 0; k < T_MAX_SPS; k++) {
            if (setores[j].id_sensores[k] == id_sensor)
                return 0;
        }
    }

    return 1;
}
          
void logo(void)
{
    printf("@@@@          @@@@    @@@@@@@@@@@@@@@@@@      ..@@@@@@@@@@@@@@        @@@@@@@@@@@@\n");
    printf("@@@@          @@@@    @@@@@@@@@@@@@@@@@@    @@@@@@@@@@@@@@@@@@    @@@@@@@@@@@@@@@@\n");
    printf("@@@@          @@@@    @@@@MMMMMMMMMMMMMM    @@@@@@MMMMMMMMMMmm  mm@@@@@@MMMMMMMMMM\n");
    printf("@@@@          @@@@    @@@@..              ::@@@@                @@@@mm\n");
    printf("@@@@          @@@@    @@@@..              ..@@@@                @@@@\n");
    printf("@@@@          @@@@    @@@@@@@@@@@@@@        @@@@@@@@@@@@@@      @@@@\n");
    printf("@@@@          @@@@    @@@@@@@@@@@@@@          @@@@@@@@@@@@@@    @@@@\n");
    printf("@@@@          @@@@    @@@@..                            @@@@--  @@@@\n");
    printf("@@@@        @@@@@@    @@@@..                            @@@@@@  @@@@\n");
    printf("@@@@@@mm  @@@@@@      @@@@..                          ..@@@@..  @@@@@@mm\n");
    printf("  @@@@@@@@@@@@@@      @@@@..              ++@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@\n");
    printf("    ++@@@@@@          @@@@..              ++@@@@@@@@@@@@@@          ++@@@@@@@@@@@@\n");
    printf("---------------------------------------------------------------");
}

void cadastrar_leitura(void)
{
    leitura_t leitura;
    int id;

    printf("ID do sensor: ");
    scanf("%d", &leitura.id_sensor);

    printf("ID do setor: ");
    scanf("%d", &leitura.id_setor);

    id = leitura.id_sensor - 1;

    if (id < 0 || id >= qtd_sensores)
    {
        printf("\nSensor invalido!\n");
        getchar();
        return;
    }

    leitura.leitura1 = sensores[id].leitura_min +
                       rand() % ((int)(sensores[id].leitura_max - sensores[id].leitura_min + 1));

    leitura.leitura2 = sensores[id].leitura_min +
                       rand() % ((int)(sensores[id].leitura_max - sensores[id].leitura_min + 1));

    strcpy(leitura.hora1, "08:00");
    strcpy(leitura.hora2, "18:00");

    printf("\nPrimeira leitura: %.2f\n", leitura.leitura1);
    printf("Segunda leitura: %.2f\n", leitura.leitura2);

    leituras[qtd_leituras] = leitura;
    qtd_leituras++;

    getchar();
}

void listar_leituras(void)
{
    for (int i = 0; i < qtd_leituras; i++)
    {
        printf("\n-----------------------------\n");
        printf("\nSensor: %d", leituras[i].id_sensor);
        printf("\nSetor: %d", leituras[i].id_setor);

        printf("\nLeitura 1: %.2f (%s)",
               leituras[i].leitura1,
               leituras[i].hora1);

        printf("\nLeitura 2: %.2f (%s)\n",
               leituras[i].leitura2,
               leituras[i].hora2);
    }
}

void relatorio_sensor_tipo(void)
{
    string busca;
    int encontrou = 0;

    printf("Digite o tipo do sensor: ");
    fgets(busca, T_STR, stdin);

    retirar_enter(busca);
    formatar_maiuscula(busca);

    printf("\n---- RELATORIO POR TIPO -----\n\n");

    for (int i = 0; i < qtd_sensores; i++)
    {
        if (strcmp(sensores[i].tipo, busca) == 0)
        {
            mostrar_dados_sensor(sensores[i]);
            encontrou = 1;
        }
    }

    if (!encontrou)
    {
        printf("Nenhum sensor encontrado para este tipo.\n");
    }
}

void relatorio_leituras_setor(void)
{
    int id_setor;
    int encontrou = 0;

    printf("Digite o ID do setor: ");
    scanf("%d", &id_setor);
    getchar();

    printf("\n===== LEITURAS DO SETOR %d =====\n", id_setor);

    for (int i = 0; i < qtd_leituras; i++)
    {
        if (leituras[i].id_setor == id_setor)
        {
            printf("\nSensor: %d", leituras[i].id_sensor);

            printf("\nLeitura 1: %.2f (%s)",
                   leituras[i].leitura1,
                   leituras[i].hora1);

            printf("\nLeitura 2: %.2f (%s)",
                   leituras[i].leitura2,
                   leituras[i].hora2);

            printf("\n---------------------\n");

            encontrou = 1;
        }
    }

    if (!encontrou)
    {
        printf("\nNenhuma leitura encontrada.\n");
    }
}

void relatorio_variacao_sensor(void)
{
    int id_sensor;
    int encontrou = 0;
    float variacao;

    printf("Digite o ID do sensor: ");
    scanf("%d", &id_sensor);
    getchar();

    printf("\n---- VARIACAO DO SENSOR %d ----\n", id_sensor);

    for (int i = 0; i < qtd_leituras; i++)
    {
        if (leituras[i].id_sensor == id_sensor)
        {
            variacao = leituras[i].leitura2 - leituras[i].leitura1;

            printf("\nLeitura 1: %.2f",
                   leituras[i].leitura1);

            printf("\nLeitura 2: %.2f",
                   leituras[i].leitura2);

            printf("\nVariacao: %.2f\n",
                   variacao);

            encontrou = 1;
        }
    }

    if (!encontrou)
    {
        printf("\nNenhuma leitura encontrada.\n");
    }
}

void relatorio_media_sensor(void)
{
    int id_sensor;
    int encontrou = 0;

    float soma = 0;
    int quantidade = 0;

    printf("Digite o ID do sensor: ");
    scanf("%d", &id_sensor);
    getchar();

    for (int i = 0; i < qtd_leituras; i++)
    {
        if (leituras[i].id_sensor == id_sensor)
        {
            soma += leituras[i].leitura1;
            soma += leituras[i].leitura2;

            quantidade += 2;

            encontrou = 1;
        }
    }

    if (encontrou)
    {
        printf("\n===== MEDIA DO SENSOR %d =====\n", id_sensor);
        printf("Media: %.2f\n", soma / quantidade);
    }
    else
    {
        printf("\nNenhuma leitura encontrada.\n");
    }
}