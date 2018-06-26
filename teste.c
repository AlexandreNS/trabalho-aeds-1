#include "includes/festa.c"

int main() {
    system("clear");

    Cliente c;
    strcpy(c.nome, "Alexandre");
    strcpy(c.telefone, "34343434");
    strcpy(c.endereco, "rua teste");
    c.aniversario.dia = 12;
    c.aniversario.mes = 12;
    c.aniversario.ano = 1222;

    setCliente(&c);

    return 0;
}
