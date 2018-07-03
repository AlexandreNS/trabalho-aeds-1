#include "includes/festa.c"
/*
int params;
int opcaoMarcada = -1;
char opcaoMarcada_txt[20];
fgets(opcaoMarcada_txt, 19, stdin);
setbuf(stdin, NULL);
params = sscanf(opcaoMarcada_txt, "%d", &opcaoMarcada);
while ((opcaoMarcada < 1 || opcaoMarcada > 5) || params != 1) {
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


*/

int main() {
    system("clear");
    Contrato c;
    FILE *bd;
    bd = fopen(BD_CONTRATO, "r");

    fread(&c, sizeof(Contrato), 1, bd);
    while (!feof(bd)) {
        printf("%f\n", c.valorFinal);
        printf("%d\n", c.codigoFesta);
        printf("%d\n", c.numeroContrato);
        printf("\n\n");
        fread(&c, sizeof(Contrato), 1, bd);
    }

    fclose(bd);

    // Festa f;
    // f.codigoFesta = getCodigoFesta();
    //
    // FILE *bd = fopen(BD_FESTA, "a+");
    // fwrite(&f, sizeof(Festa), 1, bd);
    // fclose(bd);

    // Cliente resultado = getCliente(1);
    // printf("\n  ### Dados do Cliente encontrado ###\n\n");
    // printf("\t> Codigo: %d\n", resultado.codigo);
    // printf("\t> Nome: %s\n", resultado.nome);
    // printf("\t> Endereco: %s\n", resultado.endereco);
    // printf("\t> Telefone: %s\n", resultado.telefone);
    // printf("\t> Data de Nascimento: %02d/%02d/%d\n\n", resultado.aniversario.dia, resultado.aniversario.mes, resultado.aniversario.ano);
    //
    //

    // printf("%d\n", diaSemanaContrato(0));

    // for (int i = 0; i < 7; i++) {
    //     printf("%f\n", calcularValorTotal(30, i));
    // }
    // printf("\n\n");
    // for (int i = 0; i < 7; i++) {
    //     printf("%f\n", calcularValorTotal(50, i));
    // }
    // printf("\n\n");
    // for (int i = 0; i < 7; i++) {
    //     printf("%f\n", calcularValorTotal(80, i));
    // }
    // printf("\n\n");
    // for (int i = 0; i < 7; i++) {
    //     printf("%f\n", calcularValorTotal(100, i));
    // }
    // FILE *bd;
    // Festa f;
    // bd = fopen(BD_FESTA, "a");
    // if(bd == NULL){
    //     printf("Erro >>> Verifique se baixou o programa corretamente\n");
    //     exit(EXIT_FAILURE);
    // }else{
    //     f.horarioInicio = 18;
    //     f.data.dia = 7;
    //     f.data.mes = 7;
    //     f.data.ano = 2018;
    //     fwrite(&f, sizeof(Festa), 1, bd);
    //     fclose(bd);
    // }
    // Data d;
    // d.dia = 7;
    // d.mes = 7;
    // d.ano = 2018;
    // int h;
    // selectHorario("char tema[]",d , &h);
    // printf("%d\n", h);

    // FILE *bd;
    // Funcionario f;
    // bd = fopen(BD_FESTA, "a");
    // if(bd == NULL){
    //     printf("Erro >>> Verifique se baixou o programa corretamente\n");
    //     exit(EXIT_FAILURE);
    // }else{
    //     f.horarioInicio = 18;
    //     f.data.dia = 7;
    //     f.data.mes = 7;
    //     f.data.ano = 2018;
    //     fwrite(&f, sizeof(Festa), 1, bd);
    //     fclose(bd);
    // }

    // Cliente c;
    // strcpy(c.nome, "Alexandre");
    // strcpy(c.telefone, "34343434");
    // strcpy(c.endereco, "rua teste");
    // c.aniversario.dia = 12;
    // c.aniversario.mes = 12;
    // c.aniversario.ano = 1222;
    //
    // setCliente(&c);

    // Funcionario f;
    // strcpy(f.nome, "Alexandre");
    // strcpy(f.telefone, "34343434");
    // strcpy(f.funcao, "Atendente");
    // f.salario = 3888.90;
    // f.tipo = 1;

    // setFuncionario(&f);

    // printf("%.2f\n", 1113.788);


    // formataString(c.nome);
    // while (validarData(&c.aniversario, dataNascimento, ANIVERSARIO) == false) {
    //     system("clear");
    //     renderizarTexto(CAPA);
    //     printf("\n  ### Processo de Cadastro do Funcionario %.10s ###\n", f.nome);
    //     printf("  ### Data de nascimento invalida ###\n");
    //
    //     printf("\tDigite novamente: ");
    //     fgets(dataNascimento, 19, stdin);
    //     setbuf(stdin, NULL);
    // }

    //Comparar String
    // char *achou;
    // char nome[] = "Alexandre Noronha da Silva";
    // char busca[] = "s";
    //
    // // printf("%s\n", busca);
    // achou = strstr(nome, busca);
    // if (strstr(nome, busca) == NULL) printf("Teste\n");
    // printf("%d\n", *achou);

    // char *i =


    // printf("%d\n",*i);
    return 0;
}
