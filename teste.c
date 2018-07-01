#include "includes/festa.c"
// /*
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
