#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits> // Para numeric_limits

using namespace std;

class Conta {
public:
    int numeroConta;
    string nomeTitular;
    double saldo;
    vector<string> extrato;

    Conta(int numero, string nome, double saldoInicial = 0.0) {
        numeroConta = numero;
        nomeTitular = nome;
        saldo = saldoInicial;
        extrato.push_back("Conta criada com saldo inicial: " + to_string(saldoInicial));
    }

    void depositar(double valor) {
        saldo += valor;
        extrato.push_back("Depósito: +" + to_string(valor));
    }

    bool sacar(double valor) {
        if (saldo >= valor) {
            saldo -= valor;
            extrato.push_back("Saque: -" + to_string(valor));
            return true;
        } else {
            cout << "Saldo insuficiente." << endl;
            return false;
        }
    }

    void exibirExtrato() {
        cout << "\nExtrato da conta " << numeroConta << " (" << nomeTitular << "):" << endl;
        for (const string& transacao : extrato) {
            cout << transacao << endl;
        }
        cout << "Saldo atual: " << fixed << setprecision(2) << saldo << endl;
    }
};

vector<Conta> contas;
string nomeArquivo = "contas.txt";

// Funções para carregar e salvar contas (igual ao exemplo anterior)
vector<Conta> carregarContas(const string& nomeArquivo);
void salvarContas(const vector<Conta>& contas, const string& nomeArquivo);


// Função para encontrar uma conta pelo número
Conta* encontrarConta(int numero) {
    for (Conta& conta : contas) {
        if (conta.numeroConta == numero) {
            return &conta;
        }
    }
    return nullptr;
}

// Função para limpar o buffer do cin
void limparBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


int main() {
    contas = carregarContas(nomeArquivo);

    int opcao;
    do {
        cout << "\nSimulador de Sistema Bancário\n";
        cout << "1. Criar conta\n";
        cout << "2. Depositar\n";
        cout << "3. Sacar\n";
        cout << "4. Transferir\n";
        cout << "5. Consultar saldo\n";
        cout << "6. Exibir extrato\n";
        cout << "7. Listar todas as contas \n"; // Nova opção
        cout << "0. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;


        switch (opcao) {
            case 1: {
                int numero;
                string nome;
                double saldoInicial;
                cout << "Número da conta: ";
                cin >> numero;
                cout << "Nome do titular: ";
                cin >> nome;
                cout << "Saldo inicial: ";
                cin >> saldoInicial;
                contas.push_back(Conta(numero, nome, saldoInicial));
               
                break;
            }
            case 2: {
                 int numero;
                double valor;
                cout << "Número da conta: ";
                cin >> numero;
                Conta* conta = encontrarConta(numero);
                if (conta) {
                    cout << "Valor do depósito: ";
                    cin >> valor;
                    conta->depositar(valor);
                } else {
                    cout << "Conta não encontrada." << endl;
                }
                break;
            }
            case 3: {
                int numero;
                double valor;
                cout << "Número da conta: ";
                cin >> numero;
                Conta* conta = encontrarConta(numero);
                if (conta) {
                    cout << "Valor do saque: ";
                    cin >> valor;
                    conta->sacar(valor);
                } else {
                    cout << "Conta não encontrada." << endl;
                }
                break;
            }
            case 4: {
                int numeroOrigem, numeroDestino;
                double valor;
                cout << "Número da conta de origem: ";
                cin >> numeroOrigem;
                cout << "Número da conta de destino: ";
                cin >> numeroDestino;

                Conta* contaOrigem = encontrarConta(numeroOrigem);
                Conta* contaDestino = encontrarConta(numeroDestino);

                if (contaOrigem && contaDestino) {
                    cout << "Valor da transferência: ";
                    cin >> valor;
                   if (contaOrigem->sacar(valor)) {
                        contaDestino->depositar(valor);
                        contaDestino->extrato.push_back("Transferência recebida da conta " + to_string(numeroOrigem) + ": +" + to_string(valor));
                        contaOrigem->extrato.push_back("Transferência enviada para a conta " + to_string(numeroDestino) + ": -" + to_string(valor));
                   }
                } else {
                    cout << "Uma das contas não foi encontrada." << endl;
                }
                break;
            }
            case 5: {
                int numero;
                cout << "Número da conta: ";
                cin >> numero;
                Conta* conta = encontrarConta(numero);
                if (conta) {
                    cout << "Saldo: " << fixed << setprecision(2) << conta->saldo << endl;
                } else {
                   cout << "Conta não encontrada." << endl;
                }
                break;
            }
            case 6: {
                int numero;
                cout << "Número da conta: ";
                cin >> numero;
                Conta* conta = encontrarConta(numero);
                if (conta) {
                    conta->exibirExtrato();
                } else {
                    cout << "Conta não encontrada." << endl;
                }
                break;
            }

            case 7: { // Nova opção para listar contas
                if (contas.empty()) {
                    cout << "Nenhuma conta cadastrada." << endl;
                } else {
                    cout << "\nLista de Contas:\n";
                    for (const auto& conta : contas) {
                        cout << "Número: " << conta.numeroConta << ", Titular: " << conta.nomeTitular << ", Saldo: " << conta.saldo << endl;
                    }
                }
                break;
            }

            case 0:
                cout << "Saindo...\n";
                break;
            default:
                cout << "Opção inválida.\n";
        }
        limparBuffer(); // Limpa o buffer após cada operação
    } while (opcao != 0);

    salvarContas(contas, nomeArquivo);

    return 0;
}




vector<Conta> carregarContas(const string& nomeArquivo) {
    vector<Conta> contas;
    ifstream arquivo(nomeArquivo);

    if (arquivo.is_open()) {
        int numero;
        string nome;
        double saldo;
        while (arquivo >> numero >> nome >> saldo) {
            contas.push_back(Conta(numero, nome, saldo));
        }
        arquivo.close();
    }
    return contas;
}

void salvarContas(const vector<Conta>& contas, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);

    if (arquivo.is_open()) {
        for (const Conta& conta : contas) {
             arquivo << conta.numeroConta << " " << conta.nomeTitular << " " << conta.saldo << endl;
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir o arquivo para salvar." << endl;
    }
}