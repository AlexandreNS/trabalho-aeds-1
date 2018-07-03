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
    int desconto;
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
int diaSemanaContrato(int dia){
    int semana[7] = {6, 0, 1, 2, 3, 4, 5};
    for(int iTime = 0; iTime < 7; iTime++){
        if(iTime == dia){
            return semana[iTime];
        }
    }
    return -1;
} // Retorna para ser usado no Contrato 0 1 2 3  4 5 6
int getDesconto(int pagamento){
    if ( pagamento == 1) return 10;
    if ( pagamento == 2) return 5;
    if ( pagamento == 3) return 2;
    if ( pagamento == 4) return 0;

    return -1;
}
float calcularValorTotal(int convidados, int dia){
    float valoresTotais[8] =  {1899.00, 2099.00, 2199.00, 2299.00, 3199.00, 3499.00, 3799.00, 3999.00};
    int tabelaValorTotal[8][3] = {  30, 0, 3,
                                    30, 4, 6,
                                    50, 0, 3,
                                    50, 4, 6,
                                    80, 0, 3,
                                    80, 4, 6,
                                    100, 0, 3,
                                    100, 4, 6
    };
    for (int i = 0; i < 8; i++) {
        if ( convidados == tabelaValorTotal[i][0]
        &&  (dia >= tabelaValorTotal[i][1]  &&  dia <= tabelaValorTotal[i][2]) ) {
            return valoresTotais[i];
        }
    }

    return -1;
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
void printDiaSemana(int diaSemana){
    char dias[7][10] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
    for (int i = 0; i < 7; i++) {
        if (diaSemana == i) {
            printf("%s\n", dias[i]);
            break;
        }
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
            break;
        case 2:
            goMenuCadastros();
            break;
        case 3:
            goMenuRelatorios();
            break;
        case 4:
            goMenuStatus();
            break;
        case 5:
            renderizarTexto(ARTE_SAIR);
            exit(EXIT_SUCCESS);
            break;
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
            goPesquisaFesta();
            break;
        case 3:
            goPesquisaFornecedor();
            break;
        case 4:
            goPesquisaFuncionario();
            break;
        case 5:
            goMenuPrincipal();
            break;
    }
}
void goMenuStatus(){
    int params;
    int opcaoMarcada = -1;
    char opcaoMarcada_txt[20];

    renderizarTexto(CAPA);
    renderizarTexto(MENU_STATUS);

    fgets(opcaoMarcada_txt, 19, stdin);
    setbuf(stdin, NULL);
    params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

    while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            system("clear");
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            renderizarTexto(MENU_STATUS);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
    }

    system("clear");
    switch (opcaoMarcada) {
        case 1:
            goStatusCliente();
            break;
        case 2:
            goStatusData();
            break;
        case 3:
            goMenuPrincipal();
            break;
    }
}
void goMenuRelatorios(){
    int params;
    int opcaoMarcada = -1;
    char opcaoMarcada_txt[20];

    renderizarTexto(CAPA);
    renderizarTexto(MENU_RELATORIOS);

    fgets(opcaoMarcada_txt, 19, stdin);
    setbuf(stdin, NULL);
    params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

    while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            system("clear");
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            renderizarTexto(MENU_RELATORIOS);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
    }

    system("clear");
    switch (opcaoMarcada) {
        case 1:
            goRelatoriosCliente();
            break;
        case 2:
            goRelatoriosFesta();
            break;
        case 3:
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
        printf("\n  ### Processo de Cadastro do Cliente %s ###\n", c.nome);
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

    printf("\n  ### Cliente %s cadastrado com sucesso ####\n", "c.nome");
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

    printf("\n  ### Funcionario %s cadastrado com sucesso ####\n", f.nome);
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

        renderizarTexto(CAPA);
        printf("\n  ### Cadastro de Festa ###\n\n");
        printf("\tDigite o Tema da festa: ");
        fgets(f.tema, 49, stdin);
        setbuf(stdin, NULL);

        formataString(f.tema);

        while(!selectData){ //DECIDIR a maldita DATA e o HORARIO da Tua FESTA
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
        int params;
        char convidados_txt[20];

        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Cadastro da Festa %s ###\n\n", f.tema);

        printf("\tQuantos convidados ?\n\n");
        printf("\t> 30\n");
        printf("\t> 50\n");
        printf("\t> 80\n");
        printf("\t> 100\n\n");
        printf("\tDigite a quantidade: ");

        fgets(convidados_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(convidados_txt, "%d", &f.convidados);

        while ( ( (f.convidados != 30) && (f.convidados != 50)
        && (f.convidados != 80) && (f.convidados != 100) ) || (params != 1)){
            system("clear");
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Cadastro da Festa %s ###\n\n", f.tema);

            printf("\tQuantos convidados ?\n\n");
            printf("\t> 30\n");
            printf("\t> 50\n");
            printf("\t> 80\n");
            printf("\t> 100\n\n");
            printf("\tDigite a quantidade: ");

            fgets(convidados_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(convidados_txt, "%d", &f.convidados);
        }
        // Dados do Contrato
        Cliente cliente = getCliente(f.codigoCliente);
        Contrato c;
        c.codigoFesta = getCodigoFesta();
        c.status = A_PAGAR;

        char pagamento_txt[20];
        params = -1;

        system("clear");
        renderizarTexto(CAPA);
        c.valorTotal = calcularValorTotal(f.convidados, diaSemanaContrato(f.diaSemana));

        printf("\n  ### Cadastro da Festa %s ###\n\n", f.tema);
        printf("  ### Valor total (sem desconto): R$ %.2f ###\n", c.valorTotal);
        printf("  ### %s como deseja realizar o pagamento ? ###\n\n", cliente.nome);
        printf("\t1-A vista - 10%% de desconto\n");
        printf("\t2-Duas vezes - 5%% de desconto\n");
        printf("\t3-Três vezes - 2%% de desconto\n");
        printf("\t4-Quatro ou mais vezes - sem desconto\n\n");
        printf("\tEscolha uma opção: ");

        fgets(pagamento_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(pagamento_txt, "%d", &c.pagamento);

        while ((c.pagamento < 1 || c.pagamento > 5) || params != 1) {
            system("clear");
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Cadastro da Festa %s ###\n\n", f.tema);
            printf("  ### Valor total (sem desconto): R$ %.2f\n", c.valorTotal);
            printf("  ### %s como deseja realizar o pagamento ? ###\n\n", cliente.nome);
            printf("\t1-A vista - 10%% de desconto\n");
            printf("\t2-Duas vezes - 5%% de desconto\n");
            printf("\t3-Três vezes - 2%% de desconto\n");
            printf("\t4-Quatro ou mais vezes - sem desconto\n\n");
            printf("\tEscolha uma opção: ");

            fgets(pagamento_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(pagamento_txt, "%d", &c.pagamento);
        }
        c.desconto = getDesconto(c.pagamento);
        c.valorFinal = c.valorTotal - c.valorTotal*((float)c.desconto/100);

        system("clear");

        setContrato(&c);
        setFesta(&f);

        renderizarTexto(CAPA);
        printf("\n  ### Não Esqueça de realizar o Pagamento :D ####\n", f.tema);
        printf("\n  ### Festa %s cadastrada com sucesso ####\n", f.tema);
        printf("  ### Deseja realizar outro cadastro ? ###\n\n");

        renderizarTexto(MENU_CADASTROS_INTERNO);
        params = -1;
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
                goCadastroFesta();
                break;
            case 2:
                goMenuCadastros();
                break;
            case 3:
                goMenuPrincipal();

        }
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

    printf("\n  ### Fornecedor %s cadastrado com sucesso ####\n", f.nome);
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
            printf("  Pressione <enter> ");

            fgetc(stdin);
            setbuf(stdin, NULL);
            system("clear");

            renderizarTexto(CAPA);
            printf("\n  ### Deseja realizar outra Pesquisa ? ###\n\n");
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
            if ( resultado.tipo == TEMPORARIO ) printf("\t> Tipo: temporário\n\n");
            if ( resultado.tipo == FIXO ) printf("\t> Tipo: fixo\n\n");
            printf("  Pressione <enter> ");

            fgetc(stdin);
            setbuf(stdin, NULL);
            system("clear");

            renderizarTexto(CAPA);
            printf("\n  ### Deseja realizar outra Pesquisa ? ###\n\n");
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
            printf("\t> Produto fornecido: %s\n\n", resultado.produtoFornecido);
            printf("  Pressione <enter> ");

            fgetc(stdin);
            setbuf(stdin, NULL);
            system("clear");

            renderizarTexto(CAPA);
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
void goPesquisaFesta(){
    char busca[50];
    int codigo;

    renderizarTexto(CAPA);
    printf("\n  ### Pesquisa de Festas: ###\n\n");
    printf("\tDigite o Tema da festa: ");

    fgets(busca, 49, stdin);
    setbuf(stdin, NULL);

    codigo = getFestas(busca);

    if (codigo == -1){
        int params;
        int opcaoMarcada = -1;
        char opcaoMarcada_txt[20];

        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Pesquisa de Festas ###\n");
        printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
        renderizarTexto(MENU_PESQUISAS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

        while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Pesquisa de Festas ###\n");
            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_PESQUISAS_INTERNO);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
        }

        system("clear");
        switch (opcaoMarcada) {
            case 1:
                goPesquisaFesta();
                break;
            case 2:
                goMenuPesquisas();
                break;
            case 3:
                goMenuPrincipal();
                break;
        }

    }else{
        Festa resultado;
        FILE *bd;
        bd = fopen(BD_FESTA, "r");

        if(bd == NULL){
            printf("Erro >>> Verifique se baixou o programa corretamente\n");
            exit(EXIT_FAILURE);
        }else{
            fread(&resultado, sizeof(Festa), 1, bd);

            while ( !feof(bd) && resultado.codigoFesta != codigo) {
                fread(&resultado, sizeof(Festa), 1, bd);
            }
            fclose(bd);

            Cliente cliente = getCliente(resultado.codigoCliente);

            renderizarTexto(CAPA);
            printf("\n  ### Dados da Festa encontrado ###\n\n");
            printf("\t> Codigo: %d\n", resultado.codigoFesta);
            printf("\t> Tema: %s\n", resultado.tema);
            printf("\t> Data: %02d/%02d/%d\n\n", resultado.data.dia, resultado.data.mes, resultado.data.ano);
            printf("\t> Inicio: %02d:00\n", resultado.horarioInicio);
            printf("\t> Fim: %02d:00\n\n", resultado.horarioFim);
            printf("  ### Realizador ###\n\n");
            printf("\t> Codigo: %d\n", cliente.codigo);
            printf("\t> Nome: %s\n", cliente.nome);
            printf("\t> Telefone: %s\n\n", cliente.telefone);
            printf("  Pressione <enter> ");

            fgetc(stdin);
            setbuf(stdin, NULL);
            system("clear");

            renderizarTexto(CAPA);
            printf("\n  ### Deseja realizar outra Pesquisa ? ###\n\n");
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
                    goPesquisaFesta();
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
// Views de Status
void goStatusCliente(){
    char busca[50];
    int codigoCliente;

    renderizarTexto(CAPA);
    printf("\n  ### Editar Status Contrato ###\n");
    printf("\n  ### Pesquisa por Cliente ###\n\n");
    printf("\tDigite um nome para a busca: ");

    fgets(busca, 49, stdin);
    setbuf(stdin, NULL);

    codigoCliente = getClientes(busca);
    // Verifica se a busca deu certo
    if (codigoCliente == -1){
        int params;
        int opcaoMarcada = -1;
        char opcaoMarcada_txt[20];

        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Editar Status Contrato ###\n");
        printf("\n  ### Pesquisa por Cliente ###\n\n");
        printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
        renderizarTexto(MENU_STATUS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

        while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Editar Status Contrato ###\n");
            printf("\n  ### Pesquisa por Cliente ###\n\n");
            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_STATUS_INTERNO);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
        }

        system("clear");
        switch (opcaoMarcada) {
            case 1:
                goStatusCliente();
                break;
            case 2:
                goMenuStatus();
                break;
            case 3:
                goMenuPrincipal();
                break;
        }

    }else{
        //Procura se Cliente tem uma Festa
        Cliente cliente = getCliente(codigoCliente);
        int codigoFesta = getFestasCodigoCliente(codigoCliente, cliente.nome);

        if (codigoFesta == -1){
            int params;
            int opcaoMarcada = -1;
            char opcaoMarcada_txt[20];

            system("clear");
            renderizarTexto(CAPA);
            printf("\n  ### Editar Status Contrato ###\n");
            printf("\n  ### Cliente não possui Festa Associada ###\n\n");
            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_STATUS_INTERNO);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

            while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
                renderizarTexto(CAPA);
                renderizarTexto(INVALIDO);
                printf("\n  ### Editar Status Contrato ###\n");
                printf("\n  ### Cliente não possui Festa Associada ###\n\n");
                printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
                renderizarTexto(MENU_STATUS_INTERNO);

                fgets(opcaoMarcada_txt, 19, stdin);
                setbuf(stdin, NULL);
                params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
            }

            system("clear");
            switch (opcaoMarcada) {
                case 1:
                    goStatusCliente();
                    break;
                case 2:
                    goMenuStatus();
                    break;
                case 3:
                    goMenuPrincipal();
                    break;
            }

        }else{
            // Carrega dados da Festa e Cliente
            Festa resultado;
            FILE *bd;
            bd = fopen(BD_FESTA, "r");

            if(bd == NULL){
                printf("Erro >>> Verifique se baixou o programa corretamente\n");
                exit(EXIT_FAILURE);
            }else{
                fread(&resultado, sizeof(Festa), 1, bd);

                while ( !feof(bd) && resultado.codigoFesta != codigoFesta) {
                    fread(&resultado, sizeof(Festa), 1, bd);
                }
                fclose(bd);

                renderizarTexto(CAPA);
                printf("\n  ### Dados da Festa encontrado ###\n\n");
                printf("\t> Codigo: %d\n", resultado.codigoFesta);
                printf("\t> Tema: %s\n", resultado.tema);
                printf("\t> Data: %02d/%02d/%d\n\n", resultado.data.dia, resultado.data.mes, resultado.data.ano);
                printf("\t> Inicio: %02d:00\n", resultado.horarioInicio);
                printf("\t> Fim: %02d:00\n\n", resultado.horarioFim);
                printf("  ### Realizador ###\n\n");
                printf("\t> Codigo: %d\n", cliente.codigo);
                printf("\t> Nome: %s\n", cliente.nome);
                printf("\t> Telefone: %s\n\n", cliente.telefone);
                printf("  Pressione <enter> ");

                fgetc(stdin);
                setbuf(stdin, NULL);
                system("clear");

                // Pergunta se Deseja alterar o Status

                renderizarTexto(CAPA);
                printf("\n  ### Editar Status Contrato ###\n");
                printf("\n  ### Alterar Status da Festa para: ###\n\n");
                renderizarTexto(MENU_STATUS_ALTERAR);

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
                    printf("\n  ### Editar Status Contrato ###\n");
                    printf("\n  ### Alterar Status da Festa para: ###\n\n");
                    renderizarTexto(MENU_STATUS_ALTERAR);

                    fgets(opcaoMarcada_txt, 19, stdin);
                    setbuf(stdin, NULL);
                    params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
                }

                system("clear");
                switch (opcaoMarcada) {
                    case 1:
                        setContratoStatus(PAGO, resultado.codigoFesta);
                        break;
                    case 2:
                        setContratoStatus(CANCELADO, resultado.codigoFesta);
                        break;
                    case 3:
                        goMenuStatus();
                        break;
                    case 4:
                        goMenuPrincipal();
                        break;
                }
            }
        }
    }
}
void goStatusData(){
    Data dataFesta;
    char busca[20];
    int codigoFesta;

    renderizarTexto(CAPA);
    printf("\n  ### Editar Status Contrato ###\n");
    printf("\n  ### Pesquisa por Data da Festa ###\n\n");
    printf("\tDigite no formato dd/mm/aaaa: ");

    fgets(busca, 19, stdin);
    setbuf(stdin, NULL);

    while (validarData(&dataFesta, busca, BUSCA) == false) {
        system("clear");
        renderizarTexto(CAPA);
        renderizarTexto(INVALIDO);
        printf("\n  ### Editar Status Contrato ###\n");
        printf("\n  ### Pesquisa por Data da Festa ###\n\n");
        printf("\tDigite no formato dd/mm/aaaa: ");

        fgets(busca, 19, stdin);
        setbuf(stdin, NULL);
    }

    codigoFesta = getFestasData(dataFesta);
    // Verifica se a busca deu certo
    if (codigoFesta == -1){
        int params;
        int opcaoMarcada = -1;
        char opcaoMarcada_txt[20];

        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Editar Status Contrato ###");
        printf("\n  ### Pesquisa por Data da Festa ###\n");
        printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
        renderizarTexto(MENU_STATUS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

        while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Editar Status Contrato ###");
            printf("\n  ### Pesquisa por Data da Festa ###\n");
            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_STATUS_INTERNO);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
        }

        system("clear");
        switch (opcaoMarcada) {
            case 1:
                goStatusData();
                break;
            case 2:
                goMenuStatus();
                break;
            case 3:
                goMenuPrincipal();
                break;
        }

    }else{
        // Carrega dados da Festa e Cliente
        Festa resultado;
        FILE *bd;
        bd = fopen(BD_FESTA, "r");

        if(bd == NULL){
            printf("Erro >>> Verifique se baixou o programa corretamente\n");
            exit(EXIT_FAILURE);
        }else{
            fread(&resultado, sizeof(Festa), 1, bd);

            while ( !feof(bd) && resultado.codigoFesta != codigoFesta) {
                fread(&resultado, sizeof(Festa), 1, bd);
            }
            fclose(bd);

            Cliente cliente = getCliente(resultado.codigoCliente);

            renderizarTexto(CAPA);
            printf("\n  ### Dados da Festa encontrado ###\n\n");
            printf("\t> Codigo: %d\n", resultado.codigoFesta);
            printf("\t> Tema: %s\n", resultado.tema);
            printf("\t> Data: %02d/%02d/%d\n\n", resultado.data.dia, resultado.data.mes, resultado.data.ano);
            printf("\t> Inicio: %02d:00\n", resultado.horarioInicio);
            printf("\t> Fim: %02d:00\n\n", resultado.horarioFim);
            printf("  ### Realizador ###\n\n");
            printf("\t> Codigo: %d\n", cliente.codigo);
            printf("\t> Nome: %s\n", cliente.nome);
            printf("\t> Telefone: %s\n\n", cliente.telefone);
            printf("  Pressione <enter> ");

            fgetc(stdin);
            setbuf(stdin, NULL);
            system("clear");

            // Pergunta se Deseja alterar o Status

            renderizarTexto(CAPA);
            printf("\n  ### Editar Status Contrato ###\n");
            printf("\n  ### Alterar Status da Festa para: ###\n\n");
            renderizarTexto(MENU_STATUS_ALTERAR);

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
                printf("\n  ### Editar Status Contrato ###\n");
                printf("\n  ### Alterar Status da Festa para: ###\n\n");
                renderizarTexto(MENU_STATUS_ALTERAR);

                fgets(opcaoMarcada_txt, 19, stdin);
                setbuf(stdin, NULL);
                params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
            }

            system("clear");
            switch (opcaoMarcada) {
                case 1:
                    setContratoStatus(PAGO, resultado.codigoFesta);
                    break;
                case 2:
                    setContratoStatus(CANCELADO, resultado.codigoFesta);
                    break;
                case 3:
                    goMenuStatus();
                    break;
                case 4:
                    goMenuPrincipal();
                    break;
            }
        }
    }

}
// Views de Relatorios
void goRelatoriosCliente(){
    char busca[50];
    int codigoCliente;

    renderizarTexto(CAPA);
    printf("\n  ### Relatorios ###\n");
    printf("  ### Pesquisa por Cliente ###\n\n");
    printf("\tDigite um nome para a busca: ");

    fgets(busca, 49, stdin);
    setbuf(stdin, NULL);

    codigoCliente = getClientes(busca);
    // Verifica se a busca deu certo
    if (codigoCliente == -1){
        int params;
        int opcaoMarcada = -1;
        char opcaoMarcada_txt[20];

        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Relatorios ###\n");
        printf("  ### Pesquisa por Cliente ###\n\n");
        printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
        renderizarTexto(MENU_RELATORIOS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

        while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Relatorios ###\n");
            printf("  ### Pesquisa por Cliente ###\n\n");
            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_RELATORIOS_INTERNO);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
        }

        system("clear");
        switch (opcaoMarcada) {
            case 1:
                goRelatoriosCliente();
                break;
            case 2:
                goMenuRelatorios();
                break;
            case 3:
                goMenuPrincipal();
                break;
        }

    }else{
        //Procura se Cliente tem uma Festa
        Cliente cliente = getCliente(codigoCliente);

        Festa f;
        int numRegistros = 0;

        FILE *bd;
        bd = fopen(BD_FESTA, "r");

        system("clear");
        if (bd == NULL){
            printf("Erro >>> Verifique se baixou o programa corretamente\n");
            exit(EXIT_FAILURE);
        }else{
            renderizarTexto(CAPA);

            fread(&f, sizeof(Festa), 1, bd); //Primeira Leitura dos DADOS

            // Esta parte exibe a lista dos Dados Encontrados

            if( !feof(bd) ){
                printf("\n  ### Festas do Cliente - %s ###\n\n", cliente.nome);
                while ( !feof(bd) ) {
                    if (codigoCliente == f.codigoCliente) {
                        numRegistros++;

                        printf("\t%d-%s - %02d/%02d/%d - codigo: %d\n", numRegistros, f.tema,
                        f.data.dia, f.data.mes, f.data.ano, f.codigoFesta);
                        fread(&f, sizeof(Festa), 1, bd);
                    }else{
                        fread(&f, sizeof(Festa), 1, bd);
                    }
                }
                if (numRegistros == 0){
                    printf("  ### Nenhuma ###\n");
                    printf("Pressione <enter>\n");
                }else{
                    printf("\tPressione <enter>\n");
                }

                fclose(bd);
                fgetc(stdin);
                setbuf(stdin, NULL);

                system("clear");
                goMenuPrincipal();
            }
        }
    }
}
void goRelatoriosFesta(){
    Data dataFesta;
    char busca[50];
    int codigoFesta;

    renderizarTexto(CAPA);
    printf("\n  ### Relatorios ###\n");
    printf("  ### Pesquisa por Data da Festa ###\n\n");
    printf("\tDigite no formato dd/mm/aaaa: ");

    fgets(busca, 19, stdin);
    setbuf(stdin, NULL);

    while (validarData(&dataFesta, busca, BUSCA) == false) {
        system("clear");
        renderizarTexto(CAPA);
        renderizarTexto(INVALIDO);
        printf("\n  ### Relatorios ###\n");
        printf("  ### Pesquisa por Data da Festa ###\n\n");
        printf("\tDigite no formato dd/mm/aaaa: ");

        fgets(busca, 19, stdin);
        setbuf(stdin, NULL);
    }

    codigoFesta = getFestasData(dataFesta);
    // Verifica se a busca deu certo
    if (codigoFesta == -1){
        int params;
        int opcaoMarcada = -1;
        char opcaoMarcada_txt[20];

        system("clear");
        renderizarTexto(CAPA);
        printf("\n  ### Relatorios ###\n");
        printf("  ### Pesquisa por Data da Festa ###\n");
        printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
        renderizarTexto(MENU_RELATORIOS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

        while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Relatorios ###\n");
            printf("  ### Pesquisa por Data da Festa ###\n");
            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_RELATORIOS_INTERNO);

            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
        }

        system("clear");
        switch (opcaoMarcada) {
            case 1:
                goRelatoriosFesta();
                break;
            case 2:
                goMenuRelatorios();
                break;
            case 3:
                goMenuPrincipal();
                break;
        }

    }else{
        Festa resultado;
        FILE *bd;
        bd = fopen(BD_FESTA, "r");

        if(bd == NULL){
            printf("Erro >>> Verifique se baixou o programa corretamente\n");
            exit(EXIT_FAILURE);
        }else{
            fread(&resultado, sizeof(Festa), 1, bd);

            while ( !feof(bd) && resultado.codigoFesta != codigoFesta) {
                fread(&resultado, sizeof(Festa), 1, bd);
            }
            fclose(bd);

            Cliente cliente = getCliente(resultado.codigoCliente);

            Contrato contrato = getContrato(resultado.codigoFesta);

            renderizarTexto(CAPA);
            printf("\n  ### Dados da Festa encontrado ###\n\n");
            printf("\t> Codigo: %d\n", resultado.codigoFesta);
            printf("\t> Tema: %s\n", resultado.tema);
            printf("\t> Data: %02d/%02d/%d\n\n", resultado.data.dia, resultado.data.mes, resultado.data.ano);
            printf("\t> Inicio: %02d:00\n", resultado.horarioInicio);
            printf("\t> Fim: %02d:00\n\n", resultado.horarioFim);
            printf("  ### Contrato ###\n\n");
            printf("\t> Valor Total: R$ %.2f\n", contrato.valorTotal);
            printf("\t> Desconto: %d\n", contrato.desconto);
            printf("\t> Valor Final: R$ %.2f\n", contrato.valorFinal);
            if (contrato.status == 1) printf("\t> Status: Foi Pago :D\n", contrato.status);
            if (contrato.status == 0) printf("\t> Status: precisa pagar :D\n", contrato.status);
            printf("  ### Realizador ###\n\n");
            printf("\t> Codigo: %d\n", cliente.codigo);
            printf("\t> Nome: %s\n", cliente.nome);
            printf("\t> Telefone: %s\n\n", cliente.telefone);
            printf("  Pressione <enter> ");

            fgetc(stdin);
            setbuf(stdin, NULL);
            system("clear");

            renderizarTexto(CAPA);
            printf("  ### Deseja realizar outra Pesquisa ? ###\n\n");
            renderizarTexto(MENU_RELATORIOS_INTERNO);

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
                renderizarTexto(MENU_RELATORIOS_INTERNO);

                fgets(opcaoMarcada_txt, 19, stdin);
                setbuf(stdin, NULL);
                params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
            }

            system("clear");
            switch (opcaoMarcada) {
                case 1:
                    goRelatoriosFesta();
                    break;
                case 2:
                    goMenuRelatorios();
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
void setFesta(Festa *f){
    Festa ultimo;
    ultimo.codigoFesta = 0;

    FILE *bd = fopen(BD_FESTA, "a+");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        //Pega o ultimo ID Cadastrado
        fseek(bd, (-1)*sizeof(Festa), SEEK_END);
        fread(&ultimo, sizeof(Festa), 1, bd);

        // Escreve no Arquivo
        f->codigoFesta = ultimo.codigoFesta+1;
        fwrite(f, sizeof(Festa), 1, bd);

        if (ferror(bd)){
            fclose(bd);
            printf("Erro >>> Não foi possivel escrever no arquivo\n");
            exit(EXIT_FAILURE);
        }

        fclose(bd);
    }
}
void setContrato(Contrato *c){
    Contrato ultimo;
    ultimo.numeroContrato = 0;

    FILE *bd = fopen(BD_CONTRATO, "a+");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        //Pega o ultimo ID Cadastrado
        fseek(bd, (-1)*sizeof(Contrato), SEEK_END);
        fread(&ultimo, sizeof(Contrato), 1, bd);

        // Escreve no Arquivo
        c->numeroContrato = ultimo.numeroContrato+1;
        fwrite(c, sizeof(Contrato), 1, bd);

        if (ferror(bd)){
            fclose(bd);
            printf("Erro >>> Não foi possivel escrever no arquivo\n");
            exit(EXIT_FAILURE);
        }

        fclose(bd);
    }
}
void setContratoStatus(int pagamento, int codigoFesta){
    int numRegistros = 0;
    Contrato c;
    Contrato *todos;

    FILE *bd = fopen(BD_CONTRATO, "r");

    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        fread(&c, sizeof(Contrato), 1, bd); //Primeira Leitura dos DADOS

        while ( !feof(bd) ) {
            numRegistros++;
            fread(&c, sizeof(Contrato), 1, bd);
        }

        if (numRegistros == 0){
            fclose(bd);
            printf("Erro >>> Arquivo apagado inesperadamente\n");
            exit(EXIT_FAILURE);
        }else{
            todos = (Contrato*)malloc(numRegistros*sizeof(Contrato));
            rewind(bd);
            // printf("teste\n");
            fread(todos, sizeof(Contrato), numRegistros, bd);

            fclose(bd);

            for (int i = 0; i < numRegistros; i++) {
                if (todos[i].codigoFesta == codigoFesta) {
                    todos[i].status = pagamento;
                    if (pagamento == CANCELADO) {
                        removeFesta(codigoFesta);
                    }
                    break;
                }
            }
            bd = fopen(BD_CONTRATO, "w");
            fwrite(todos, sizeof(Contrato), numRegistros, bd);
            fclose(bd);
        }
    }
    goMenuPrincipal();
}

Contrato getContrato(int codigoFesta){
    Contrato resultado;
    FILE *bd;
    bd = fopen(BD_CONTRATO, "r");

    if(bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        fread(&resultado, sizeof(Contrato), 1, bd);

        while ( !feof(bd) && resultado.codigoFesta != codigoFesta) {
            fread(&resultado, sizeof(Contrato), 1, bd);
        }
        fclose(bd);
    }
    return resultado;
}
Cliente getCliente(int codigo){
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
    }
    return resultado;
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
int getFestas(char busca[]){
    formataString(busca);

    Festa f;
    int *codigos;
    int numRegistros = 0;
    codigos = (int*)malloc(1*sizeof(int));
    // Parte para selecionar
    int params;
    int opcaoMarcada = 1;
    char opcaoMarcada_txt[20];

    FILE *bd;
    bd = fopen(BD_FESTA, "r");

    system("clear");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        renderizarTexto(CAPA);

        fread(&f, sizeof(Festa), 1, bd); //Primeira Leitura dos DADOS

        // Esta parte exibe a lista dos Dados Encontrados

        if( !feof(bd) ){
            printf("\n  ### Festas encontrados com a busca - %s ###\n\n", busca);
            while ( !feof(bd) ) {
                if (strstr(f.tema, busca) != NULL) {
                    numRegistros++;

                    printf("\t%d-%s - codigo: %d\n", numRegistros, f.tema, f.codigoFesta);

                    codigos = (int*)realloc(codigos, numRegistros*sizeof(int));
                    codigos[numRegistros-1] = f.codigoFesta;

                    fread(&f, sizeof(Festa), 1, bd);
                }else{
                    fread(&f, sizeof(Festa), 1, bd);
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
int getFestasData(Data data){
    Festa f;
    int *codigos;
    int numRegistros = 0;
    codigos = (int*)malloc(1*sizeof(int));
    // Parte para selecionar
    int params;
    int opcaoMarcada = 1;
    char opcaoMarcada_txt[20];

    FILE *bd;
    bd = fopen(BD_FESTA, "r");

    system("clear");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        renderizarTexto(CAPA);

        fread(&f, sizeof(Festa), 1, bd); //Primeira Leitura dos DADOS

        // Esta parte exibe a lista dos Dados Encontrados

        if( !feof(bd) ){
            printf("\n  ### Festas com a Data - %02d/%02d/%d ###\n\n", data.dia, data.mes, data.ano);
            while ( !feof(bd) ) {
                if ((data.dia == f.data.dia) && (data.mes == f.data.mes) && (data.ano == f.data.ano)) {
                    numRegistros++;

                    printf("\t%d-%s - codigo: %d\n", numRegistros, f.tema, f.codigoFesta);

                    codigos = (int*)realloc(codigos, numRegistros*sizeof(int));
                    codigos[numRegistros-1] = f.codigoFesta;

                    fread(&f, sizeof(Festa), 1, bd);
                }else{
                    fread(&f, sizeof(Festa), 1, bd);
                }
            }
            if (numRegistros == 0){
                printf("  ### Nenhuma ###\n");
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
int getFestasCodigoCliente(int codigoCliente, char nomeCliente[]){
    Festa f;
    int *codigos;
    int numRegistros = 0;
    codigos = (int*)malloc(1*sizeof(int));
    // Parte para selecionar
    int params;
    int opcaoMarcada = 1;
    char opcaoMarcada_txt[20];

    FILE *bd;
    bd = fopen(BD_FESTA, "r");

    system("clear");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        renderizarTexto(CAPA);

        fread(&f, sizeof(Festa), 1, bd); //Primeira Leitura dos DADOS

        // Esta parte exibe a lista dos Dados Encontrados

        if( !feof(bd) ){
            printf("\n  ### Festas do Cliente - %s ###\n\n", nomeCliente);
            while ( !feof(bd) ) {
                if (codigoCliente == f.codigoCliente) {
                    numRegistros++;

                    printf("\t%d-%s - codigo: %d\n", numRegistros, f.tema, f.codigoFesta);

                    codigos = (int*)realloc(codigos, numRegistros*sizeof(int));
                    codigos[numRegistros-1] = f.codigoFesta;

                    fread(&f, sizeof(Festa), 1, bd);
                }else{
                    fread(&f, sizeof(Festa), 1, bd);
                }
            }
            if (numRegistros == 0){
                printf("  ### Nenhuma ###\n");
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
int getCodigoFesta(){
    Festa ultimo;
    int retorno = -1;
    ultimo.codigoFesta = 0;

    FILE *bd = fopen(BD_FESTA, "a+");
    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        //Pega o ultimo ID Cadastrado
        fseek(bd, (-1)*sizeof(Festa), SEEK_END);
        fread(&ultimo, sizeof(Festa), 1, bd);

        // Escreve no Arquivo
        retorno = ultimo.codigoFesta+1;

        if (ferror(bd)){
            fclose(bd);
            printf("Erro >>> Não foi possivel escrever no arquivo\n");
            exit(EXIT_FAILURE);
        }

        fclose(bd);
    }
    return retorno;
}

void removeFesta(int codigoFesta){
    int numRegistros = 0;
    Festa *todos;
    Festa f;

    todos = (Festa*)malloc(1*sizeof(Festa));

    FILE *bd = fopen(BD_FESTA, "r");

    if (bd == NULL){
        printf("Erro >>> Verifique se baixou o programa corretamente\n");
        exit(EXIT_FAILURE);
    }else{
        fread(&f, sizeof(Festa), 1, bd); //Primeira Leitura dos DADOS
        while ( !feof(bd) ) {
            if(f.codigoFesta != codigoFesta){
                numRegistros++;
                todos = (Festa*)realloc(todos, numRegistros*sizeof(Festa));

                todos[numRegistros-1] = f;
            }
            fread(&f, sizeof(Festa), 1, bd);
        }
        fclose(bd);

        bd = fopen(BD_FESTA, "w");
        fwrite(todos, sizeof(Festa), numRegistros, bd);
        fclose(bd);
    }
}
