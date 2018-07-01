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
            printf("goCadastroFesta();\n");
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
        case 6:
            printf("CRUD AINDA NÃO ESTÁ PRONTO\n");
            exit(EXIT_FAILURE);
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
            exit(EXIT_FAILURE);
            break;
        case 2:
            printf("goPesquisaCliente()\n");
            break;
        case 3:
            printf("goPesquisaCliente()\n");
            exit(EXIT_FAILURE);
            break;
        case 4:
            printf("goPesquisaCliente()\n");
            exit(EXIT_FAILURE);
            break;
        case 5:
            goMenuPrincipal();
            exit(EXIT_SUCCESS);
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
    sscanf(salario_txt, "%.2f", &f.salario); // PONTO IMPORTANTE HAHAHAHHAHAHAHAH

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
    int *codigos;

    renderizarTexto(CAPA);
    printf("\n  ### Pesquisa de Clientes ###\n\n");
    printf("\tDigite um nome para a busca: ");

    fgets(busca, 49, stdin);
    setbuf(stdin, NULL);

    if (getClientes(busca) == -1){
        int params;
        int opcaoMarcada = -1;
        char opcaoMarcada_txt[20];

        renderizarTexto(CAPA);
        printf("\n  ### Pesquisa de Clientes ###\n");
        printf("  ### O \"%s\" não foi encontrado ou a OPCAO foi INVALIDA ###\n\n", busca);
        printf("  ### Deseja realizar outra Pesquisa ? ###\n");
        renderizarTexto(MENU_PESQUISAS_INTERNO);

        fgets(opcaoMarcada_txt, 19, stdin);
        setbuf(stdin, NULL);
        params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

        while ((opcaoMarcada < 1 || opcaoMarcada > 3) || params != 1) {
            renderizarTexto(CAPA);
            renderizarTexto(INVALIDO);
            printf("\n  ### Pesquisa de Clientes ###\n");
            printf("  ### O \"%s\" não foi encontrado ###\n\n", busca);
            printf("  ### Deseja realizar outra Pesquisa ? ###\n");
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
        //Informações do Cliente vao ser carregadas
        
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

        if( (strstr(c.nome, busca) != NULL) && !feof(bd) ){
            printf("\n  ### Clientes encontrados com a busca - %s ###\n\n", busca);
            numRegistros++;

            printf("\t%d-%s - codigo: %d\n", numRegistros, c.nome, c.codigo);

            codigos = (int*)malloc(1*sizeof(int));
            codigos[numRegistros-1] = c.codigo;

            fread(&c, sizeof(Cliente), 1, bd);

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
            printf("\t%d-Realizar outra busca\n", numRegistros+1);

            fclose(bd);

            printf("\nEscolha uma das opções acima:\n");
            fgets(opcaoMarcada_txt, 19, stdin);
            setbuf(stdin, NULL);
            params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);

            system("clear");
            if ((opcaoMarcada < 1 || opcaoMarcada > numRegistros) || params != 1) {
                free(codigos);
                return -1; // Realizar outra Pesquisa
            }else{
                return codigos[opcaoMarcada-1];
            }
        }else{
            return -1; // Nenhum registro encontrado
        }

    }

    return -1;
}
