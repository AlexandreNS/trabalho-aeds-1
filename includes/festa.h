//VARIAVEIS DE CONTROLE
#define FESTA 0
#define ANIVERSARIO 1
#define BUSCA 2

#define TEMPORARIO 0
#define FIXO 1

#define CANCELADO -1
#define A_PAGAR 0
#define PAGO 1
//TEXTO PARA IMPRIMIR NA TELA
#define INVALIDO "assets/invalido.txt"
#define CAPA "assets/artes/headerPrincipal.txt"
#define MENU_PRINCIPAL "assets/menus/menuPrincipal.txt"
#define MENU_CADASTROS "assets/menus/menuCadastros.txt"
#define MENU_PESQUISAS "assets/menus/menuPesquisas.txt"
#define MENU_CADASTROS_INTERNO "assets/menus/menuCadastrosInterno.txt"
#define MENU_PESQUISAS_INTERNO "assets/menus/menuPesquisasInterno.txt"
//DEFINES DOS BDS
#define BD_CLIENTE "db/cliente.dat"
#define BD_FUNCIONARIO "db/funcionario.dat"
#define BD_FORNECEDOR "db/fornecedor.dat"
#define BD_FESTA "db/festa.dat"
#define BD_CONTRATO "db/contrato.dat"

#ifndef _FESTA_H
#define _FESTA_H

enum boolean {true = 1, false = 0};
typedef enum boolean bool;

enum Semana {Segunda, Terca, Quarta, Quinta, Sexta, Sabado, Domingo};

// Variaveis de Armazenamento

typedef struct DataTag Data;
typedef struct ClienteTag Cliente;
typedef struct FornecedorTag Fornecedor;
typedef struct FestaTag Festa;
typedef struct FuncionarioTag Funcionario;
typedef struct ContratoTag Contrato;

void formataString(char texto[]);                   // Tira possiveis quebras de linha na String
void renderizarTexto(char texto[]);                 //carrega TXT's
bool selectHorario(char tema[], Data data, int *horarioInicio);
bool validarData(Data *data, char dataInformada[], int dataTipo);  // Verifica se a data existe
int getDiaSemana(Data data);
int diaSemanaContrato(int dia);
int getDesconto(int pagamento);
float calcularValorTotal(int convidados, int dia);
// Renders
void goMenuPrincipal();
void goMenuCadastros();
void goMenuPesquisas();
// Cadastros
void goCadastroCliente();
void goCadastroFuncionario();
void goCadastroFesta();
void goCadastroFornecedor();
// Pesquisas
void goPesquisaCliente();
void goPesquisaFuncionario();
// Funcoes BDS
void setCliente(Cliente *c);
void setFuncionario(Funcionario *f);
void setFornecedor(Fornecedor *f);
void setFesta(Festa *f);
void setContrato(Contrato *c);

Cliente getCliente(int codigo);
int getClientes(char busca[]);
int getFuncionarios(char busca[]);
int getFornecedores(char busca[]);
int getCodigoFesta();
#endif
