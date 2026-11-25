/*===========================================================================
    Centro Universitario de Vicosa - UNIVICOSA
    Disciplina: INF101 - Programacao de Computadores I
    Trabalho - Sistema de Registro e Gestao de Contas Bancarias (ETAPA 3)
    Aluno: Vinicius dos Passos Ribeiro - 28770

    ETAPA 3: persistencia dos dados em arquivos.
    ===========================================================================*/
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream> // para usar arquivos
#include <sstream> // stringstream
#include <limits>  // numeric_limits para limpar o buffer do cin

using namespace std;

// variaveis globais (as funcoes de arquivo usam elas)
const int MAX_CONTAS = 100;
int numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS];
string cpf[MAX_CONTAS];
int tipoConta[MAX_CONTAS];
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];
int quantidadeContas = 0;

const int MAX_MOVIMENTACOES = 500;
int contaMovimentacao[MAX_MOVIMENTACOES];
char tipoMovimentacao[MAX_MOVIMENTACOES];
double valorMovimentacao[MAX_MOVIMENTACOES];
int quantidadeMovimentacoes = 0;

const string ARQUIVO_CONTAS = "contas.txt";
const string ARQUIVO_MOVIMENTACOES = "movimentacoes.txt";

// funcao auxiliar para ler inteiro com seguranca
bool lerInteiro(int &valor) {
    cin >> valor;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// funcao auxiliar para ler double com seguranca
bool lerDouble(double &valor) {
    cin >> valor;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// le o contas.txt e joga nos vetores
// se nao tiver o arquivo comeca sem nenhuma conta
void carregarContas() {
    ifstream arquivo(ARQUIVO_CONTAS);
    quantidadeContas = 0;

    if (!arquivo.is_open()) {
        cout << "Arquivo " << ARQUIVO_CONTAS
             << " nao encontrado. Iniciando sem contas cadastradas." << endl;
        return;
    }

    string linha;
    while (getline(arquivo, linha) && quantidadeContas < MAX_CONTAS) {
        if (linha.empty()) {
            continue; // pula linha vazia
        }

        // separa cada campo pelo ;
        stringstream ss(linha);
        string campoNumero, campoNome, campoCpf, campoTipo, campoSaldo, campoAtiva;

        getline(ss, campoNumero, ';');
        getline(ss, campoNome, ';');
        getline(ss, campoCpf, ';');
        getline(ss, campoTipo, ';');
        getline(ss, campoSaldo, ';');
        getline(ss, campoAtiva, ';');

        if (campoNumero.empty() || campoSaldo.empty() || campoAtiva.empty()) {
            continue; // linha errada, pula
        }

        numeroConta[quantidadeContas] = stoi(campoNumero);
        nomeCliente[quantidadeContas] = campoNome;
        cpf[quantidadeContas] = campoCpf;
        tipoConta[quantidadeContas] = stoi(campoTipo);
        saldo[quantidadeContas] = stod(campoSaldo);
        contaAtiva[quantidadeContas] = (stoi(campoAtiva) == 1);
        quantidadeContas++;
    }

    arquivo.close();
    cout << quantidadeContas << " conta(s) carregada(s) de " << ARQUIVO_CONTAS << "." << endl;
}

// grava todas as contas no contas.txt
// sem o ios::app ele apaga o que tinha e escreve de novo
void salvarContas() {
    ofstream arquivo(ARQUIVO_CONTAS);

    if (!arquivo.is_open()) {
        cout << "ERRO: nao foi possivel gravar " << ARQUIVO_CONTAS << "!" << endl;
        return;
    }

    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeContas; i++) {
        arquivo << numeroConta[i] << ";"
                << nomeCliente[i] << ";"
                << cpf[i] << ";"
                << tipoConta[i] << ";"
                << saldo[i] << ";"
                << (contaAtiva[i] ? 1 : 0) << endl;
    }

    arquivo.close();
}

// le o movimentacoes.txt
void carregarMovimentacoes() {
    ifstream arquivo(ARQUIVO_MOVIMENTACOES);
    quantidadeMovimentacoes = 0;

    if (!arquivo.is_open()) {
        cout << "Arquivo " << ARQUIVO_MOVIMENTACOES
             << " nao encontrado. Iniciando sem movimentacoes." << endl;
        return;
    }

    string linha;
    while (getline(arquivo, linha) && quantidadeMovimentacoes < MAX_MOVIMENTACOES) {
        if (linha.empty()) {
            continue;
        }

        stringstream ss(linha);
        string campoConta, campoTipo, campoValor;

        getline(ss, campoConta, ';');
        getline(ss, campoTipo, ';');
        getline(ss, campoValor, ';');

        if (campoConta.empty() || campoTipo.empty() || campoValor.empty()) {
            continue;
        }

        contaMovimentacao[quantidadeMovimentacoes] = stoi(campoConta);
        tipoMovimentacao[quantidadeMovimentacoes] = campoTipo[0]; // D ou S
        valorMovimentacao[quantidadeMovimentacoes] = stod(campoValor);
        quantidadeMovimentacoes++;
    }

    arquivo.close();
    cout << quantidadeMovimentacoes << " movimentacao(oes) carregada(s) de "
         << ARQUIVO_MOVIMENTACOES << "." << endl;
}

// grava todas as movimentacoes no arquivo
// como as antigas ja foram carregadas no inicio, elas nao se perdem
void salvarMovimentacoes() {
    ofstream arquivo(ARQUIVO_MOVIMENTACOES);

    if (!arquivo.is_open()) {
        cout << "ERRO: nao foi possivel gravar " << ARQUIVO_MOVIMENTACOES << "!" << endl;
        return;
    }

    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeMovimentacoes; i++) {
        arquivo << contaMovimentacao[i] << ";"
                << tipoMovimentacao[i] << ";"
                << valorMovimentacao[i] << endl;
    }

    arquivo.close();
}

int main() {
    cout << fixed << setprecision(2);

    // carrega o que ja estava salvo
    carregarContas();
    carregarMovimentacoes();

    int opcao;

    do {
        cout << "\n--- MENU ---" << endl;
        cout << "1 - Cadastrar contas" << endl;
        cout << "2 - Listar contas cadastradas" << endl;
        cout << "3 - Consultar conta pelo numero" << endl;
        cout << "4 - Realizar deposito" << endl;
        cout << "5 - Realizar saque" << endl;
        cout << "6 - Realizar transferencia" << endl;
        cout << "7 - Exibir extrato" << endl;
        cout << "0 - Sair" << endl;
        cout << "Escolha uma opcao: ";

        if (!lerInteiro(opcao)) {
            cout << "Entrada invalida! Digite apenas numeros." << endl;
            opcao = -1; // garante que nao saia do loop
            continue;
        }

        switch (opcao) {

            // 1 - cadastrar conta
            case 1: {
                if (quantidadeContas < MAX_CONTAS) {
                    cout << "\n--- Cadastro de Conta ---" << endl;

                    int novoNumero;
                    cout << "Numero da conta: ";
                    if (!lerInteiro(novoNumero)) {
                        cout << "Numero de conta invalido! Cadastro cancelado." << endl;
                        break;
                    }

                    // verifica se o numero ja existe
                    bool jaExiste = false;
                    for (int i = 0; i < quantidadeContas; i++) {
                        if (numeroConta[i] == novoNumero) {
                            jaExiste = true;
                        }
                    }

                    if (jaExiste) {
                        cout << "Ja existe uma conta com esse numero!" << endl;
                        break;
                    }

                    // a conta nova vai na posicao quantidadeContas
                    numeroConta[quantidadeContas] = novoNumero;

                    cout << "Nome do cliente: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpa resto da linha anterior
                    getline(cin, nomeCliente[quantidadeContas]);
                    if (nomeCliente[quantidadeContas].find(';') != string::npos) {
                        // o ; e o separador do arquivo
                        cout << "O nome nao pode conter ';'! Cadastro cancelado." << endl;
                        break;
                    }

                    cout << "CPF: ";
                    cin >> cpf[quantidadeContas];

                    cout << "Tipo da conta (1-Corrente, 2-Poupanca): ";
                    if (!lerInteiro(tipoConta[quantidadeContas])) {
                        cout << "Tipo de conta invalido! Cadastro cancelado." << endl;
                        break;
                    }
                    if (tipoConta[quantidadeContas] != 1 && tipoConta[quantidadeContas] != 2) {
                        cout << "Tipo de conta invalido! Cadastro cancelado." << endl;
                        break;
                    }

                    cout << "Saldo inicial: ";
                    if (!lerDouble(saldo[quantidadeContas])) {
                        cout << "Saldo inicial invalido! Cadastro cancelado." << endl;
                        break;
                    }
                    if (saldo[quantidadeContas] < 0) {
                        cout << "Saldo inicial nao pode ser negativo! Cadastro cancelado." << endl;
                        break;
                    }

                    contaAtiva[quantidadeContas] = true;
                    quantidadeContas++; // so soma depois de validar tudo

                    salvarContas(); // salva no arquivo

                    cout << "Conta cadastrada com sucesso!" << endl;
                } else {
                    cout << "Limite maximo de contas atingido!" << endl;
                }
                break;
            }

            // 2 - listar contas
            case 2: {
                cout << "\n--- Lista de Contas ---" << endl;
                if (quantidadeContas == 0) {
                    cout << "Nenhuma conta cadastrada ainda." << endl;
                } else {
                    // mostra todas as contas
                    for (int i = 0; i < quantidadeContas; i++) {
                        cout << "Conta: " << numeroConta[i] << " | ";
                        cout << "Nome: " << nomeCliente[i] << " | ";
                        cout << "CPF: " << cpf[i] << " | ";
                        cout << "Tipo: " << (tipoConta[i] == 1 ? "Corrente" : "Poupanca") << " | ";
                        cout << "Saldo: R$ " << saldo[i] << " | ";
                        cout << "Situacao: " << (contaAtiva[i] ? "Ativa" : "Inativa") << endl;
                    }
                }
                break;
            }

            // 3 - consultar conta pelo numero
            case 3: {
                cout << "\n--- Consulta de Conta ---" << endl;
                int numeroBusca;
                cout << "Numero da conta: ";
                if (!lerInteiro(numeroBusca)) {
                    cout << "Numero de conta invalido!" << endl;
                    break;
                }

                // procura a conta no vetor
                // se continuar -1 e porque nao achou
                int indice = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numeroBusca) {
                        indice = i;
                    }
                }

                if (indice == -1) {
                    cout << "Conta nao encontrada!" << endl;
                } else {
                    cout << "Conta: " << numeroConta[indice] << endl;
                    cout << "Nome: " << nomeCliente[indice] << endl;
                    cout << "CPF: " << cpf[indice] << endl;
                    cout << "Tipo: " << (tipoConta[indice] == 1 ? "Corrente" : "Poupanca") << endl;
                    cout << "Saldo: R$ " << saldo[indice] << endl;
                    cout << "Situacao: " << (contaAtiva[indice] ? "Ativa" : "Inativa") << endl;
                }
                break;
            }

            // 4 - deposito
            case 4: {
                cout << "\n--- Deposito ---" << endl;
                int numeroBusca;
                double valor;
                cout << "Numero da conta: ";
                if (!lerInteiro(numeroBusca)) {
                    cout << "Numero de conta invalido!" << endl;
                    break;
                }

                // procura a conta
                int indice = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numeroBusca) {
                        indice = i;
                    }
                }

                if (indice == -1) {
                    cout << "Conta nao encontrada!" << endl;
                } else if (!contaAtiva[indice]) {
                    cout << "Conta inativa!" << endl;
                } else if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
                    // ve se ainda cabe no historico
                    cout << "Historico de movimentacoes cheio!" << endl;
                } else {
                    cout << "Valor do deposito: ";
                    if (!lerDouble(valor)) {
                        cout << "Valor invalido!" << endl;
                        break;
                    }

                    if (valor <= 0) {
                        cout << "O valor deve ser positivo!" << endl;
                    } else {
                        // soma no saldo
                        saldo[indice] = saldo[indice] + valor;

                        // guarda no historico
                        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indice];
                        tipoMovimentacao[quantidadeMovimentacoes] = 'D';
                        valorMovimentacao[quantidadeMovimentacoes] = valor;
                        quantidadeMovimentacoes++;

                        // salva nos arquivos
                        salvarContas();
                        salvarMovimentacoes();

                        cout << "Deposito realizado! Novo saldo: R$ " << saldo[indice] << endl;
                    }
                }
                break;
            }

            // 5 - saque (verifica se ha saldo suficiente)
            case 5: {
                cout << "\n--- Saque ---" << endl;
                int numeroBusca;
                double valor;
                cout << "Numero da conta: ";
                if (!lerInteiro(numeroBusca)) {
                    cout << "Numero de conta invalido!" << endl;
                    break;
                }

                int indice = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numeroBusca) {
                        indice = i;
                    }
                }

                if (indice == -1) {
                    cout << "Conta nao encontrada!" << endl;
                } else if (!contaAtiva[indice]) {
                    cout << "Conta inativa!" << endl;
                } else if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
                    cout << "Historico de movimentacoes cheio!" << endl;
                } else {
                    cout << "Valor do saque: ";
                    if (!lerDouble(valor)) {
                        cout << "Valor invalido!" << endl;
                        break;
                    }

                    if (valor <= 0) {
                        cout << "O valor deve ser positivo!" << endl;
                    } else if (valor > saldo[indice]) {
                        // nao deixa sacar mais do que tem
                        cout << "Saldo insuficiente! Saldo atual: R$ " << saldo[indice] << endl;
                    } else {
                        saldo[indice] = saldo[indice] - valor;

                        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indice];
                        tipoMovimentacao[quantidadeMovimentacoes] = 'S';
                        valorMovimentacao[quantidadeMovimentacoes] = valor;
                        quantidadeMovimentacoes++;

                        // salva nos arquivos
                        salvarContas();
                        salvarMovimentacoes();

                        cout << "Saque realizado! Novo saldo: R$ " << saldo[indice] << endl;
                    }
                }
                break;
            }

            // 6 - transferencia entre duas contas
            case 6: {
                cout << "\n--- Transferencia ---" << endl;
                int numeroOrigem, numeroDestino;
                double valor;

                cout << "Numero da conta de origem: ";
                if (!lerInteiro(numeroOrigem)) {
                    cout << "Numero de conta de origem invalido!" << endl;
                    break;
                }

                cout << "Numero da conta de destino: ";
                if (!lerInteiro(numeroDestino)) {
                    cout << "Numero de conta de destino invalido!" << endl;
                    break;
                }

                // procura as duas contas
                int indiceOrigem = -1;
                int indiceDestino = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numeroOrigem) {
                        indiceOrigem = i;
                    }
                    if (numeroConta[i] == numeroDestino) {
                        indiceDestino = i;
                    }
                }

                if (indiceOrigem == -1 || indiceDestino == -1) {
                    cout << "Conta de origem ou destino nao encontrada!" << endl;
                } else if (indiceOrigem == indiceDestino) {
                    cout << "Origem e destino devem ser contas diferentes!" << endl;
                } else if (!contaAtiva[indiceOrigem] || !contaAtiva[indiceDestino]) {
                    cout << "As duas contas precisam estar ativas!" << endl;
                } else if (quantidadeMovimentacoes + 2 > MAX_MOVIMENTACOES) {
                    // precisa de 2 espacos (saida e entrada)
                    cout << "Historico sem espaco para a transferencia!" << endl;
                } else {
                    cout << "Valor da transferencia: ";
                    if (!lerDouble(valor)) {
                        cout << "Valor invalido!" << endl;
                        break;
                    }

                    if (valor <= 0) {
                        cout << "O valor deve ser positivo!" << endl;
                    } else if (valor > saldo[indiceOrigem]) {
                        cout << "Saldo insuficiente na conta de origem! Saldo: R$ "
                             << saldo[indiceOrigem] << endl;
                    } else {
                        // tira de uma e coloca na outra
                        saldo[indiceOrigem] = saldo[indiceOrigem] - valor;
                        saldo[indiceDestino] = saldo[indiceDestino] + valor;

                        // saida na origem
                        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indiceOrigem];
                        tipoMovimentacao[quantidadeMovimentacoes] = 'S';
                        valorMovimentacao[quantidadeMovimentacoes] = valor;
                        quantidadeMovimentacoes++;

                        // entrada no destino
                        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indiceDestino];
                        tipoMovimentacao[quantidadeMovimentacoes] = 'D';
                        valorMovimentacao[quantidadeMovimentacoes] = valor;
                        quantidadeMovimentacoes++;

                        // salva nos arquivos
                        salvarContas();
                        salvarMovimentacoes();

                        cout << "Transferencia realizada com sucesso!" << endl;
                        cout << "Saldo conta " << numeroConta[indiceOrigem] << ": R$ "
                             << saldo[indiceOrigem] << endl;
                        cout << "Saldo conta " << numeroConta[indiceDestino] << ": R$ "
                             << saldo[indiceDestino] << endl;
                    }
                }
                break;
            }

            // 7 - extrato
            case 7: {
                cout << "\n--- Extrato ---" << endl;
                int numeroBusca;
                cout << "Numero da conta: ";
                if (!lerInteiro(numeroBusca)) {
                    cout << "Numero de conta invalido!" << endl;
                    break;
                }

                int indice = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numeroBusca) {
                        indice = i;
                    }
                }

                if (indice == -1) {
                    cout << "Conta nao encontrada!" << endl;
                } else {
                    cout << "Extrato da conta " << numeroConta[indice]
                         << " - " << nomeCliente[indice] << endl;

                    bool temMovimentacao = false; // pra saber se tem alguma movimentacao

                    for (int i = 0; i < quantidadeMovimentacoes; i++) {
                        if (contaMovimentacao[i] == numeroConta[indice]) {
                            temMovimentacao = true;

                            if (tipoMovimentacao[i] == 'D') {
                                cout << "Deposito: R$ " << valorMovimentacao[i] << endl;
                            } else if (tipoMovimentacao[i] == 'S') {
                                cout << "Saque: R$ " << valorMovimentacao[i] << endl;
                            }
                        }
                    }

                    if (!temMovimentacao) {
                        cout << "Nenhuma movimentacao registrada." << endl;
                    }
                    cout << "Saldo atual: R$ " << saldo[indice] << endl;
                }
                break;
            }

            // 0 - sair
            case 0:
                // salva de novo antes de sair
                salvarContas();
                salvarMovimentacoes();
                cout << "Dados salvos. Saindo..." << endl;
                break;

            default:
                cout << "Opcao invalida, digite novamente." << endl;
                break;
        }

    } while (opcao != 0); // repete ate digitar 0

    return 0;
}