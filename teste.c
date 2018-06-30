#include "includes/festa.c"

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
    char *achou;
    char nome[] = "Alexandre Noronha da Silva";
    char busca[] = "alexandre";

    achou = strcasestr(nome, busca);
    if (achou == NULL) printf("Teste\n");
    printf("%d\n", *achou);
    return 0;
}
