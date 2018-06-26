#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "festa.h"

typedef struct DataTag{
    int dia;
    int mes;
    int ano;
} Data;
typedef struct ClienteTag{
    int codigo;
    char nome[50];
    char endereco[200];
    char telefone[20];
    Data aniversario;

} Cliente;
typedef struct FuncionarioTag{
    int codigo;
    char nome[50];
    char telefone[20];
    char funcao[30];
    float salario;
    int tipo;
}

bool validarData(Data *data, char dataInformada[], int dataTipo){
    formataString(dataInformada);
    int numVar = sscanf(dataInformada, "%d/%d/%d", &data->dia, &data->mes, &data->ano);
    if(numVar != 3) return false;

    int diaInformado, mesInformado, anoInformado;
    diaInformado = data->dia;
    mesInformado = data->mes - 1;
    anoInformado = data->ano - 1900;

    struct tm tmData;
    tmData.tm_sec = 0;
    tmData.tm_min = 0;
    tmData.tm_hour = 12;
    tmData.tm_mday = data->dia;
    tmData.tm_mon = data->mes - 1;
    tmData.tm_year = data->ano - 1900;

    time_t agora = time(NULL);
    time_t tempo = mktime(&tmData);

    if( ( tmData.tm_mday != diaInformado )
    || ( tmData.tm_mon != mesInformado )
    || (tmData.tm_year != anoInformado ) ){
        return false;
    }

    switch (dataTipo) {
        case ANIVERSARIO:
            if(agora < tempo) return false;
            break;
        case FESTA:
            if (agora > tempo) return false;
            break;
        default:
            printf("Erro >>> Esse aqui ninguem salva :D\n");
            exit(EXIT_FAILURE);
    }

    return true;
}
void formataString(char texto[]){
    int len = strlen(texto);
    if(texto[len-1] == '\n') texto[len-1] = 0;
}
void renderizarTexto(char texto[]){
    FILE *view;
    view = fopen(texto, "r");

    if(view == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
    }else{
        char ch;
        ch = fgetc(view);
        while( !feof(view) )
        {
            printf("%c", ch);
            ch = fgetc(view);
        }
        fclose(view);
    }
}

void goMenuPrincipal(){
    int params;
    int opcaoMarcada = -1;
    char opcaoMarcada_txt[20];

    renderizarTexto(CAPA);
    renderizarTexto(MENU_PRINCIPAL);

    fgets(opcaoMarcada_txt, 19, stdin);
    setbuf(stdin, NULL);
    params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

    while ((opcaoMarcada < 1 || opcaoMarcada > 5) || params != 1) {
            system("clear");
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            renderizarTexto(MENU_PRINCIPAL);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
    }

    system("clear");
    switch (opcaoMarcada) {
        case 1:
            printf("goMenuPesquisa()\n");
            exit(EXIT_FAILURE);
            break;
        case 2:
            goMenuCadastros();
            break;
        case 3:
            printf("goMenuRelatorios()\n");
            exit(EXIT_FAILURE);
            break;
        case 4:
            printf("goStatusContrato()\n");
            exit(EXIT_FAILURE);
            break;
        case 5:
            printf("Saindo...\n");
            exit(EXIT_SUCCESS);
            break;
        case 6:
            printf("CRUD AINDA NÃO ESTÁ PRONTO\n");
            exit(EXIT_FAILURE);
    }
}
void goMenuCadastros(){
    int params;
    int opcaoMarcada = -1;
    char opcaoMarcada_txt[20];

    renderizarTexto(CAPA);
    renderizarTexto(MENU_CADASTROS);

    fgets(opcaoMarcada_txt, 19, stdin);
    setbuf(stdin, NULL);
    params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

    while ((opcaoMarcada < 1 || opcaoMarcada > 5) || params != 1) {
            system("clear");
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            renderizarTexto(MENU_CADASTROS);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
    }

    system("clear");
    switch (opcaoMarcada) {
        case 1:
            goCadastroCliente();
            break;
        case 2:
            printf("Cadastro()\n");
            exit(EXIT_FAILURE);
            break;
        case 3:
            printf("goMenuRelatorios()\n");
            exit(EXIT_FAILURE);
            break;
        case 4:
            printf("goStatusContrato()\n");
            exit(EXIT_FAILURE);
            break;
        case 5:
            goMenuPrincipal();
            break;
        case 6:
            printf("CRUD AINDA NÃO ESTÁ PRONTO\n");
            exit(EXIT_FAILURE);
    }
}
void goCadastroCliente(){
    Cliente c;
    char dataNascimento[20];
    // Durante o Cadastro
    system("clear");
    renderizarTexto(CAPA);
    printf("\n  ### Cadastro de Cliente ###\n\n");
    printf("\tDigite o nome: ");
    fgets(c.nome, 49, stdin);
    setbuf(stdin, NULL);

    printf("\tDigite o endereço: ");
    fgets(c.endereco, 199, stdin);
    setbuf(stdin, NULL);

    printf("\tDigite o telefone: ");
    fgets(c.telefone, 19, stdin);
    setbuf(stdin, NULL);

    printf("\tDigite a data de nascimento no formato dd/mm/aaaa : ");
    fgets(dataNascimento, 19, stdin);
    setbuf(stdin, NULL);

    formataString(c.nome);
    while (validarData(&c.aniversario, dataNascimento, ANIVERSARIO) == false) {
        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Processo de Cadastro do Cliente %.10s ###\n", c.nome);
        printf("  ### Data de nascimento invalida ###\n");

        printf("\tDigite novamente: ");
        fgets(dataNascimento, 19, stdin);
        setbuf(stdin, NULL);
    }
    system("clear");
    // Fazendo Cadastro com Funcao Externa

    setCliente(&c);

    // Depois de Efetuar o Cadastro
    renderizarTexto(CAPA);

    printf("\n  ### Cliente %.10s cadastrado com sucesso ####\n", "c.nome");
    printf("  ### Deseja realizar outro cadastro ? ###\n\n");

    renderizarTexto(MENU_CADASTROS_INTERNO);
    int params;
    int opcaoMarcada = -1;
    char opcaoMarcada_txt[20];

    fgets(opcaoMarcada_txt, 19, stdin);
    setbuf(stdin, NULL);
    params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

    while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
        system("clear");
        renderizarTexto(CAPA);
        renderizarTexto(INVALIDO);
        printf("  ### Deseja realizar outro cadastro ? ###\n\n");
        renderizarTexto(MENU_CADASTROS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
    }

    system("clear");
    switch (opcaoMarcada) {
        case 1:
            goCadastroCliente();
            break;
        case 2:
            goMenuCadastros();
            break;
        case 3:
            goMenuPrincipal();

    }
}
void goCadastroFuncionario(){
    Funcionario f;
    // Durante o Cadastro
    system("clear");
    renderizarTexto(CAPA);
    printf("\n  ### Cadastro de Funcionario ###\n\n");
    printf("\tDigite o nome: ");
    fgets(f.nome, 49, stdin);
    setbuf(stdin, NULL);

    printf("\tDigite o telefone: ");
    fgets(f.endereco, 199, stdin);
    setbuf(stdin, NULL);

    printf("\tDigite o salario: ");
    fgets(f.telefone, 19, stdin);
    setbuf(stdin, NULL);

    printf("\tDigite a função exercida: ");
    fgets(f.telefone, 19, stdin);
    setbuf(stdin, NULL);

    printf("\tInforme o tipo\n\t1-Temporário\n\t2-Fixo\n : ");
    fgets(dataNascimento, 19, stdin);
    setbuf(stdin, NULL);

    formataString(c.nome);
    while (validarData(&c.aniversario, dataNascimento, ANIVERSARIO) == false) {
        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Processo de Cadastro do Funcionario %.10s ###\n", f.nome);
        printf("  ### Data de nascimento invalida ###\n");

        printf("\tDigite novamente: ");
        fgets(dataNascimento, 19, stdin);
        setbuf(stdin, NULL);
    }
    system("clear");
    // Fazendo Cadastro com Funcao Externa

    setFuncionario(&f);

    // Depois de Efetuar o Cadastro
    renderizarTexto(CAPA);

    printf("\n  ### Funcionario %.10s cadastrado com sucesso ####\n", f.nome);
    printf("  ### Deseja realizar outro cadastro ? ###\n\n");

    renderizarTexto(MENU_CADASTROS_INTERNO);
    int params;
    int opcaoMarcada = -1;
    char opcaoMarcada_txt[20];

    fgets(opcaoMarcada_txt, 19, stdin);
    setbuf(stdin, NULL);
    params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

    while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
        system("clear");
        renderizarTexto(CAPA);
        renderizarTexto(INVALIDO);
        printf("  ### Deseja realizar outro cadastro ? ###\n\n");
        renderizarTexto(MENU_CADASTROS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
    }

    system("clear");
    switch (opcaoMarcada) {
        case 1:
            goCadastroFuncionario();
            break;
        case 2:
            goMenuCadastros();
            break;
        case 3:
            goMenuPrincipal();

    }
}

void setCliente(Cliente *c){
    Cliente ultimo;
    ultimo.codigo = 0;

    formataString(c->nome);
    formataString(c->endereco);
    formataString(c->telefone);

    FILE *bd = fopen(BD_CLIENTE, "a+");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        //Pega o ultimo ID Cadastrado
        fseek(bd, (-1)*sizeof(Cliente), SEEK_END);
        fread(&ultimo, sizeof(Cliente), 1, bd);

        // Escreve no Arquivo
        c->codigo = ultimo.codigo+1;
        fwrite(c, sizeof(Cliente), 1, bd);

        if (ferror(bd)){
            fclose(bd);
            printf("Erro >>> Não foi possivel escrever no arquivo\n");
            exit(EXIT_FAILURE);
        }

        fclose(bd);
    }
}
