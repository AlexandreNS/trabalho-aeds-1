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
typedef struct FornecedorTag {
    int codigo;
    char nome[50];
    char telefone[20];
    char produtoFornecido[50];
} Fornecedor;
typedef struct FestaTag {
    int codigoFesta;
    int convidados;
    Data data;
    int diaSemana;
    int horarioInicio;
    int horarioFim;
    char tema[50];
    int codigoCliente;
} Festa;
typedef struct FuncionarioTag{
    int codigo;
    char nome[50];
    char telefone[20];
    char funcao[50];
    float salario;
    int tipo;
} Funcionario;
typedef struct ContratoTag {
    int numeroContrato;
    float valorTotal;
    float desconto;
    float valorFinal;
    int pagamento;
    int status;
    int codigoFesta;
} Contrato;

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
        case BUSCA:
            return true;
            break;
        default:
            printf("Erro >>> Esse aqui ninguem salva :D\n");
            exit(EXIT_FAILURE);
    }

    return true;
}
int getDiaSemana(Data data){ // Retorna int [0-6]
    struct tm tmData, *dataRetorno;

    tmData.tm_sec = 0;
    tmData.tm_min = 0;
    tmData.tm_hour = 12;
    tmData.tm_mday = data.dia;
    tmData.tm_mon = data.mes - 1;
    tmData.tm_year = data.ano - 1900;

    time_t tempo = mktime(&tmData);
    dataRetorno = localtime(&tempo);

    return dataRetorno->tm_wday;
}
bool selectHorario(char tema[], Data data, int *horarioInicio){
    system("clear");
    int params;
    char horario_txt[20];
    int horarios[24];
    int numDisponiveis = 0;

    for (int i = 0; i < 21; i++) {
        horarios[i] = true;
    }
    horarios[21] = false;
    horarios[22] = false;
    horarios[23] = false;

    FILE *bd;
    bd = fopen(BD_FESTA, "r");
    if(bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        Festa f;
        fread(&f, sizeof(Festa), 1, bd);
        while ( !feof(bd) ) {
            if( (data.dia == f.data.dia) && (data.mes == f.data.mes) && (data.ano == f.data.ano)){

                for (int i = (f.horarioInicio-3); i <= (f.horarioInicio+3); i++) {
                    if ((i >= 0) && (i <= 23)){
                        horarios[i] = false;
                    }
                }
                fread(&f, sizeof(Festa), 1, bd);

            }else{
                fread(&f, sizeof(Festa), 1, bd);
            }
        }
    }
    fclose(bd);

    renderizarTexto(CAPA);
    printf("\n  ### Cadastro da Festa %s ###\n", tema);

    if(getDiaSemana(data) == 6){
        if (horarios[12] == true) numDisponiveis++;
        if (horarios[18] == true) numDisponiveis++;
    }else{
        for (int i = 0; i < 24; i++) {
            if(horarios[i] == true){
                numDisponiveis++;
            }
        }

    }
    if (numDisponiveis != 0){
        if(getDiaSemana(data) == 6){
            printf("  ### Horarios disponiveis: ###\n\n");
            if (horarios[12] == true) printf("\t> 12:00\t");
            if (horarios[18] == true) printf("\t> 18:00");
            printf("\n\n\tInforme o horario de inicio: ");

            fgets(horario_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(horario_txt, "%d:00", horarioInicio);

            while ( !(
                ( (*horarioInicio == 12) && (horarios[12] == true) )
                || (*horarioInicio == 18) && (horarios[18] == true) ) || params != 1) {
                    system("clear");

                    renderizarTexto(CAPA);
                    renderizarTexto(INVALIDO);
                    printf("\n  ### Cadastro da Festa %s ###\n", tema);
                    printf("  ### Horarios disponiveis: ###\n\n");
                    if (horarios[12] == true) printf("\t> 12:00");
                    if (horarios[18] == true) printf("\t> 18:00");
                    printf("\n\n\tInforme o horario de inicio: ");

                    fgets(horario_txt, 19, stdin);
                    setbuf(stdin, NULL);
                    params = sscanf(horario_txt, "%d:00", horarioInicio);
            }

            return true;
        }else{
            printf("  ### Horarios disponiveis: ###\n");
            int layout = 1;
            for (int i = 0; i < 24; i++) {
                if (horarios[i] == true) {
                    if (layout%3 == 1) {
                        printf("\n\t> %02d:00", i);
                    }else{
                        printf("\t\t> %02d:00", i);
                    }
                    layout++;
                }
            }
            printf("\n\n\tInforme o horario de inicio: ");

            *horarioInicio = -1;
            int valido = false;

            fgets(horario_txt, 19, stdin);
            setbuf(stdin, NULL);

            params = sscanf(horario_txt, "%d:00", horarioInicio);

            for (int i = 0; i < 24; i++) {
                if (horarios[i] == true) {
                    if (*horarioInicio == i){
                        valido = true;
                    }
                }
            }

            while ((valido == false) && (params != 1)) {
                    system("clear");

                    renderizarTexto(CAPA);
                    renderizarTexto(INVALIDO);
                    printf("\n  ### Cadastro da Festa %s ###\n", tema);
                    printf("  ### Horarios disponiveis: ###\n");
                    int layout = 1;
                    for (int i = 0; i < 24; i++) {
                        if (horarios[i] == true) {
                            if (layout%3 == 1) {
                                printf("\n\t> %02d:00", i);
                            }else{
                                printf("\t\t> %02d:00", i);
                            }
                            layout++;
                        }
                    }
                    printf("\n\n\tInforme o horario de inicio: ");

                    *horarioInicio = -1;
                    valido = false;

                    fgets(horario_txt, 19, stdin);
                    setbuf(stdin, NULL);

                    params = sscanf(horario_txt, "%d:00", horarioInicio);

                    for (int i = 0; i < 24; i++) {
                        if (horarios[i] == true) {
                            if (*horarioInicio == i){
                                valido = true;
                            }
                        }
                    }
            }

            return true;
        }

    }else{
        return false;
    }

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
            goMenuPesquisas();
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
            goCadastroFesta();
            exit(EXIT_FAILURE);
            break;
        case 3:
            goCadastroFornecedor();
            exit(EXIT_FAILURE);
            break;
        case 4:
            goCadastroFuncionario();
            exit(EXIT_FAILURE);
            break;
        case 5:
            goMenuPrincipal();
            break;
    }
}
void goMenuPesquisas(){
    int params;
    int opcaoMarcada = -1;
    char opcaoMarcada_txt[20];

    renderizarTexto(CAPA);
    renderizarTexto(MENU_PESQUISAS);

    fgets(opcaoMarcada_txt, 19, stdin);
    setbuf(stdin, NULL);
    params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

    while ((opcaoMarcada < 1 || opcaoMarcada > 5) || params != 1) {
            system("clear");
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            renderizarTexto(MENU_PESQUISAS);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
    }

    system("clear");
    switch (opcaoMarcada) {
        case 1:
            goPesquisaCliente();
            break;
        case 2:
            printf("goPesquisaCliente()\n");
            break;
        case 3:
            printf("goPesquisaCliente()\n");
            break;
        case 4:
            goPesquisaFuncionario();
            break;
        case 5:
            goMenuPrincipal();
            break;
    }
}
// Views de Cadastros
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
    char salario_txt[20];
    char tipo_txt[20];
    // Durante o Cadastro
    system("clear");
    renderizarTexto(CAPA);
    printf("\n  ### Cadastro de Funcionario ###\n\n");
    printf("\tDigite o nome: ");
    fgets(f.nome, 49, stdin);
    setbuf(stdin, NULL);

    printf("\tDigite o telefone: ");
    fgets(f.telefone, 19, stdin);
    setbuf(stdin, NULL);

    printf("\tDigite o salario: ");
    fgets(salario_txt, 19, stdin);
    setbuf(stdin, NULL);
    sscanf(salario_txt, "%f", &f.salario); // PONTO IMPORTANTE HAHAHAHHAHAHAHAH

    printf("\tDigite a função exercida: ");
    fgets(f.funcao, 19, stdin);
    setbuf(stdin, NULL);

    printf("\tInforme o tipo:\n\t\t1-Temporário\n\t\t2-Fixo\n\t>>>");
    fgets(tipo_txt, 19, stdin);
    setbuf(stdin, NULL);
    sscanf(tipo_txt, "%d", &f.tipo); // PONTO IMPORTANTE HAHAHAHHAHAHAHAH

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
void goCadastroFesta(){
    Festa f;
    char busca[50];
    int codigoCliente;
    bool selectData = false;
    // Durante o Cadastro
    system("clear");

    renderizarTexto(CAPA);
    printf("\n  ### Cadastro de Festa ###\n\n");
    printf("\tProcurar um cliente: ");
    fgets(busca, 49, stdin);
    setbuf(stdin, NULL);

    codigoCliente = getClientes(busca);

    if (codigoCliente == -1){  // Caso Nao encontre nada
        int params;
        int opcaoMarcada = -1;
        char opcaoMarcada_txt[20];

        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Cadastro de Festa ###\n");
        printf("  ### Cliente não encontrado ###\n\n");
        printf("\tDeseja fazer uma nova busca ?\n\n");
        renderizarTexto(MENU_CADASTROS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

        while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            system("clear");
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Cadastro de Festa ###\n");
            printf("  ### Cliente não encontrado ###\n\n");
            printf("\tDeseja fazer uma nova busca ?\n\n");
            renderizarTexto(MENU_CADASTROS_INTERNO);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
        }

        system("clear");

        switch (opcaoMarcada) {
            case 1:
                goCadastroFesta();
                break;
            case 2:
                goMenuCadastros();
                break;
            case 3:
                goMenuPrincipal();
                break;
        }
    }else{
        f.codigoCliente = codigoCliente;

        printf("\tDigite o Tema da festa: ");
        fgets(f.tema, 49, stdin);
        setbuf(stdin, NULL);

        formataString(f.tema);

        while(selectData){ //DECIDIR a maldita DATA e o HORARIO da Tua FESTA
            char dataFesta[20];
            int params;
            int opcaoMarcada = -1;
            char opcaoMarcada_txt[20];

            printf("\tDigite a data da Festa no formato dd/mm/aaaa : ");

            fgets(dataFesta, 19, stdin);
            setbuf(stdin, NULL);

            while (validarData(&f.data, dataFesta, FESTA) == false) {
                system("clear");
                renderizarTexto(CAPA);
                printf("\n  ### Cadastro da Festa %s ###\n", f.tema);
                printf("\n  ### DATA INVALIDA ###\n\n");
                printf("\tDigite a data da Festa no formato dd/mm/aaaa : ");

                fgets(dataFesta, 19, stdin);
                setbuf(stdin, NULL);
            }

            selectData = selectHorario(f.tema, f.data, &f.horarioInicio);
        }
        f.horarioFim = f.horarioInicio+4;
        f.diaSemana = getDiaSemana(f.data);

        char convidados_txt[20];
        printf("\tQuantos convidados?: ");
        fgets(busca, 49, stdin);
        setbuf(stdin, NULL);
    }

}
void goCadastroFornecedor(){
    Fornecedor f;
    // Durante o Cadastro
    system("clear");
    renderizarTexto(CAPA);
    printf("\n  ### Cadastro de Fornecedor ###\n\n");
    printf("\tDigite o nome: ");
    fgets(f.nome, 49, stdin);
    setbuf(stdin, NULL);

    printf("\tDigite o telefone: ");
    fgets(f.telefone, 19, stdin);
    setbuf(stdin, NULL);

    printf("\tDigite o produto fornecido: ");
    fgets(f.produtoFornecido, 49, stdin);
    setbuf(stdin, NULL);

    system("clear");
    // Fazendo Cadastro com Funcao Externa

    setFornecedor(&f);

    // Depois de Efetuar o Cadastro
    renderizarTexto(CAPA);

    printf("\n  ### Fornecedor %.10s cadastrado com sucesso ####\n", f.nome);
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
            goCadastroFornecedor();
            break;
        case 2:
            goMenuCadastros();
            break;
        case 3:
            goMenuPrincipal();

    }
}
// Views de Pesquisas
void goPesquisaCliente(){
    char busca[50];
    int codigo;

    renderizarTexto(CAPA);
    printf("\n  ### Pesquisa de Clientes ###\n\n");
    printf("\tDigite um nome para a busca: ");

    fgets(busca, 49, stdin);
    setbuf(stdin, NULL);

    codigo = getClientes(busca);

    if (codigo == -1){
        int params;
        int opcaoMarcada = -1;
        char opcaoMarcada_txt[20];

        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Pesquisa de Clientes ###\n");
        printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
        renderizarTexto(MENU_PESQUISAS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

        while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Pesquisa de Clientes ###\n");
            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_PESQUISAS_INTERNO);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
        }

        system("clear");
        switch (opcaoMarcada) {
            case 1:
                goPesquisaCliente();
                break;
            case 2:
                goMenuPesquisas();
                break;
            case 3:
                goMenuPrincipal();
                break;
        }

    }else{
        Cliente resultado;
        FILE *bd;
        bd = fopen(BD_CLIENTE, "r");

        if(bd == NULL){
            printf("Erro >>> Verifique se baixou o programa corretamente\n");
            exit(EXIT_FAILURE);
        }else{
            fread(&resultado, sizeof(Cliente), 1, bd);

            while ( !feof(bd) && resultado.codigo != codigo) {
                fread(&resultado, sizeof(Cliente), 1, bd);
            }
            fclose(bd);

            renderizarTexto(CAPA);
            printf("\n  ### Dados do Cliente encontrado ###\n\n");
            printf("\t> Codigo: %d\n", resultado.codigo);
            printf("\t> Nome: %s\n", resultado.nome);
            printf("\t> Endereco: %s\n", resultado.endereco);
            printf("\t> Telefone: %s\n", resultado.telefone);
            printf("\t> Data de Nascimento: %02d/%02d/%d\n\n", resultado.aniversario.dia, resultado.aniversario.mes, resultado.aniversario.ano);

            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_PESQUISAS_INTERNO);

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
                printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
                renderizarTexto(MENU_PESQUISAS_INTERNO);

                fgets(opcaoMarcada_txt, 19, stdin);
                setbuf(stdin, NULL);
                params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
            }

            system("clear");
            switch (opcaoMarcada) {
                case 1:
                    goPesquisaCliente();
                    break;
                case 2:
                    goMenuPesquisas();
                    break;
                case 3:
                    goMenuPrincipal();
                    break;
            }
        }

    }

}
void goPesquisaFuncionario(){
    char busca[50];
    int codigo;

    renderizarTexto(CAPA);
    printf("\n  ### Pesquisa de Funcionarios ###\n\n");
    printf("\tDigite um nome para a busca: ");

    fgets(busca, 49, stdin);
    setbuf(stdin, NULL);

    codigo = getFuncionarios(busca);

    if (codigo == -1){
        int params;
        int opcaoMarcada = -1;
        char opcaoMarcada_txt[20];

        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Pesquisa de Funcionarios ###\n");
        printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
        renderizarTexto(MENU_PESQUISAS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

        while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Pesquisa de Funcionarios ###\n");
            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_PESQUISAS_INTERNO);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
        }

        system("clear");
        switch (opcaoMarcada) {
            case 1:
                goPesquisaFuncionario();
                break;
            case 2:
                goMenuPesquisas();
                break;
            case 3:
                goMenuPrincipal();
                break;
        }

    }else{
        Funcionario resultado;
        FILE *bd;
        bd = fopen(BD_FUNCIONARIO, "r");

        if(bd == NULL){
            printf("Erro >>> Verifique se baixou o programa corretamente\n");
            exit(EXIT_FAILURE);
        }else{
            fread(&resultado, sizeof(Funcionario), 1, bd);

            while ( !feof(bd) && resultado.codigo != codigo) {
                fread(&resultado, sizeof(Funcionario), 1, bd);
            }
            fclose(bd);

            renderizarTexto(CAPA);
            printf("\n  ### Dados do Funcionario encontrado ###\n\n");
            printf("\t> Codigo: %d\n", resultado.codigo);
            printf("\t> Nome: %s\n", resultado.nome);
            printf("\t> Telefone: %s\n", resultado.telefone);
            printf("\t> Função: %s\n", resultado.funcao);
            printf("\t> Salario: %.2f\n", resultado.salario);
            if ( resultado.tipo == TEMPORARIO ) printf("\t> Tipo: temporário\n");
            if ( resultado.tipo == FIXO ) printf("\t> Tipo: fixo\n");

            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_PESQUISAS_INTERNO);

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
                printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
                renderizarTexto(MENU_PESQUISAS_INTERNO);

                fgets(opcaoMarcada_txt, 19, stdin);
                setbuf(stdin, NULL);
                params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
            }

            system("clear");
            switch (opcaoMarcada) {
                case 1:
                    goPesquisaFuncionario();
                    break;
                case 2:
                    goMenuPesquisas();
                    break;
                case 3:
                    goMenuPrincipal();
                    break;
            }
        }

    }

}
void goPesquisaFornecedor(){
    char busca[50];
    int codigo;

    renderizarTexto(CAPA);
    printf("\n  ### Pesquisa de Fornecedores ###\n\n");
    printf("\tDigite um nome para a busca: ");

    fgets(busca, 49, stdin);
    setbuf(stdin, NULL);

    codigo = getFornecedores(busca);

    if (codigo == -1){
        int params;
        int opcaoMarcada = -1;
        char opcaoMarcada_txt[20];

        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Pesquisa de Fornecedores ###\n");
        printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
        renderizarTexto(MENU_PESQUISAS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

        while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Pesquisa de Fornecedores ###\n");
            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_PESQUISAS_INTERNO);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
        }

        system("clear");
        switch (opcaoMarcada) {
            case 1:
                goPesquisaFornecedor();
                break;
            case 2:
                goMenuPesquisas();
                break;
            case 3:
                goMenuPrincipal();
                break;
        }

    }else{
        Fornecedor resultado;
        FILE *bd;
        bd = fopen(BD_FORNECEDOR, "r");

        if(bd == NULL){
            printf("Erro >>> Verifique se baixou o programa corretamente\n");
            exit(EXIT_FAILURE);
        }else{
            fread(&resultado, sizeof(Funcionario), 1, bd);

            while ( !feof(bd) && resultado.codigo != codigo) {
                fread(&resultado, sizeof(Fornecedor), 1, bd);
            }
            fclose(bd);

            renderizarTexto(CAPA);
            printf("\n  ### Dados do Fornecedor encontrado ###\n\n");
            printf("\t> Codigo: %d\n", resultado.codigo);
            printf("\t> Nome: %s\n", resultado.nome);
            printf("\t> Telefone: %s\n", resultado.telefone);
            printf("\t> Produto fornecido: %s\n", resultado.produtoFornecido);

            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_PESQUISAS_INTERNO);

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
                printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
                renderizarTexto(MENU_PESQUISAS_INTERNO);

                fgets(opcaoMarcada_txt, 19, stdin);
                setbuf(stdin, NULL);
                params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
            }

            system("clear");
            switch (opcaoMarcada) {
                case 1:
                    goPesquisaFornecedor();
                    break;
                case 2:
                    goMenuPesquisas();
                    break;
                case 3:
                    goMenuPrincipal();
                    break;
            }
        }

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
void setFuncionario(Funcionario *f){
    Funcionario ultimo;
    ultimo.codigo = 0;

    formataString(f->nome);
    formataString(f->telefone);
    formataString(f->funcao);

    FILE *bd = fopen(BD_FUNCIONARIO, "a+");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        //Pega o ultimo ID Cadastrado
        fseek(bd, (-1)*sizeof(Funcionario), SEEK_END);
        fread(&ultimo, sizeof(Funcionario), 1, bd);

        // Escreve no Arquivo
        f->codigo = ultimo.codigo+1;
        fwrite(f, sizeof(Funcionario), 1, bd);

        if (ferror(bd)){
            fclose(bd);
            printf("Erro >>> Não foi possivel escrever no arquivo\n");
            exit(EXIT_FAILURE);
        }

        fclose(bd);
    }
}
void setFornecedor(Fornecedor *f){
    Fornecedor ultimo;
    ultimo.codigo = 0;

    formataString(f->nome);
    formataString(f->telefone);
    formataString(f->produtoFornecido);

    FILE *bd = fopen(BD_FORNECEDOR, "a+");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        //Pega o ultimo ID Cadastrado
        fseek(bd, (-1)*sizeof(Fornecedor), SEEK_END);
        fread(&ultimo, sizeof(Fornecedor), 1, bd);

        // Escreve no Arquivo
        f->codigo = ultimo.codigo+1;
        fwrite(f, sizeof(Fornecedor), 1, bd);


        if (ferror(bd)){
            fclose(bd);
            printf("Erro >>> Não foi possivel escrever no arquivo\n");
            exit(EXIT_FAILURE);
        }

        fclose(bd);
    }
}

int getClientes(char busca[]){ //Retorna o codigo desejado
    formataString(busca);

    Cliente c;
    int *codigos;
    int numRegistros = 0;
    codigos = (int*)malloc(1*sizeof(int));
    // Parte para selecionar
    int params;
    int opcaoMarcada = 1;
    char opcaoMarcada_txt[20];

    FILE *bd;
    bd = fopen(BD_CLIENTE, "r");

    system("clear");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        renderizarTexto(CAPA);

        fread(&c, sizeof(Cliente), 1, bd); //Primeira Leitura dos DADOS

        // Esta parte exibe a lista dos Dados Encontrados

        if( !feof(bd) ){
            printf("\n  ### Clientes encontrados com a busca - %s ###\n\n", busca);
            while ( !feof(bd) ) {
                if (strstr(c.nome, busca) != NULL) {
                    numRegistros++;

                    printf("\t%d-%s - codigo: %d\n", numRegistros, c.nome, c.codigo);

                    codigos = (int*)realloc(codigos, numRegistros*sizeof(int));
                    codigos[numRegistros-1] = c.codigo;

                    fread(&c, sizeof(Cliente), 1, bd);
                }else{
                    fread(&c, sizeof(Cliente), 1, bd);
                }
            }
            if (numRegistros == 0){
                printf("  ### Nenhum ###\n");
                printf("\t1-Voltar\n");
            }else{
                printf("\t%d-Voltar\n", numRegistros+1);
            }

            fclose(bd);

            printf("\nEscolha uma das opções acima:\n");
            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

            system("clear");
            if ( (numRegistros == 0) || (opcaoMarcada < 1 || opcaoMarcada > numRegistros) || params != 1) {
                free(codigos);
                return -1; // Realizar outra Pesquisa
            }else{
                int retorno = codigos[opcaoMarcada-1];
                free(codigos);

                return retorno;
            }
        }else{
            return -1; // Nenhum registro encontrado
        }

    }

    return -1;
}
int getFuncionarios(char busca[]){ //Retorna o codigo desejado
    formataString(busca);

    Funcionario f;
    int *codigos;
    int numRegistros = 0;
    codigos = (int*)malloc(1*sizeof(int));
    // Parte para selecionar
    int params;
    int opcaoMarcada = 1;
    char opcaoMarcada_txt[20];

    FILE *bd;
    bd = fopen(BD_FUNCIONARIO, "r");

    system("clear");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        renderizarTexto(CAPA);

        fread(&f, sizeof(Funcionario), 1, bd); //Primeira Leitura dos DADOS

        // Esta parte exibe a lista dos Dados Encontrados

        if( !feof(bd) ){
            printf("\n  ### Funcionarios encontrados com a busca - %s ###\n\n", busca);
            while ( !feof(bd) ) {
                if (strstr(f.nome, busca) != NULL) {
                    numRegistros++;

                    printf("\t%d-%s - codigo: %d\n", numRegistros, f.nome, f.codigo);

                    codigos = (int*)realloc(codigos, numRegistros*sizeof(int));
                    codigos[numRegistros-1] = f.codigo;

                    fread(&f, sizeof(Funcionario), 1, bd);
                }else{
                    fread(&f, sizeof(Funcionario), 1, bd);
                }
            }
            if (numRegistros == 0){
                printf("  ### Nenhum ###\n");
                printf("\t1-Voltar\n");
            }else{
                printf("\t%d-Voltar\n", numRegistros+1);
            }

            fclose(bd);

            printf("\nEscolha uma das opções acima:\n");
            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

            system("clear");
            if ( (numRegistros == 0) || (opcaoMarcada < 1 || opcaoMarcada > numRegistros) || params != 1) {
                free(codigos);
                return -1; // Realizar outra Pesquisa
            }else{
                int retorno = codigos[opcaoMarcada-1];
                free(codigos);

                return retorno;
            }
        }else{
            return -1; // Nenhum registro encontrado
        }

    }

    return -1;
}
int getFornecedores(char busca[]){ //Retorna o codigo desejado
    formataString(busca);

    Fornecedor f;
    int *codigos;
    int numRegistros = 0;
    codigos = (int*)malloc(1*sizeof(int));
    // Parte para selecionar
    int params;
    int opcaoMarcada = 1;
    char opcaoMarcada_txt[20];

    FILE *bd;
    bd = fopen(BD_FORNECEDOR, "r");

    system("clear");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        renderizarTexto(CAPA);

        fread(&f, sizeof(Fornecedor), 1, bd); //Primeira Leitura dos DADOS

        // Esta parte exibe a lista dos Dados Encontrados

        if( !feof(bd) ){
            printf("\n  ### Fornecedores encontrados com a busca - %s ###\n\n", busca);
            while ( !feof(bd) ) {
                if (strstr(f.nome, busca) != NULL) {
                    numRegistros++;

                    printf("\t%d-%s - codigo: %d\n", numRegistros, f.nome, f.codigo);

                    codigos = (int*)realloc(codigos, numRegistros*sizeof(int));
                    codigos[numRegistros-1] = f.codigo;

                    fread(&f, sizeof(Fornecedor), 1, bd);
                }else{
                    fread(&f, sizeof(Fornecedor), 1, bd);
                }
            }
            if (numRegistros == 0){
                printf("  ### Nenhum ###\n");
                printf("\t1-Voltar\n");
            }else{
                printf("\t%d-Voltar\n", numRegistros+1);
            }

            fclose(bd);

            printf("\nEscolha uma das opções acima:\n");
            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

            system("clear");
            if ( (numRegistros == 0) || (opcaoMarcada < 1 || opcaoMarcada > numRegistros) || params != 1) {
                free(codigos);
                return -1; // Realizar outra Pesquisa
            }else{
                int retorno = codigos[opcaoMarcada-1];
                free(codigos);

                return retorno;
            }
        }else{
            return -1; // Nenhum registro encontrado
        }

    }

    return -1;
}
