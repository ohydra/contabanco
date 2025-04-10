#include <iostream>
#include <windows.h>
#include <string>
#include <tchar.h>
#include <vector>
#include <cctype>

using namespace std;

struct Cliente {
    int codigo;                 // Código do cliente (ID interno, único)
    string nome;                // Nome completo
    string cartaoCidadao;       // Número do Cartão de Cidadão
    string nif;                 // NIF (único, 9 dígitos)
    double saldo;               // Saldo inicial
};

vector<Cliente> clientes;
int proximoCodigo = 1;

// Verifica se o NIF é válido (9 dígitos numéricos)
bool nifValido(const string& nif) {
    if (nif.length() != 9) return false;
    for (char c : nif) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Verifica se o NIF já existe
bool nifExiste(const string& nif) {
    for (const auto& c : clientes) {
        if (c.nif == nif) return true;
    }
    return false;
}

// Encontrar cliente pelo código (ID)
Cliente* encontrarCliente(int codigo) {
    for (auto& c : clientes) {
        if (c.codigo == codigo) return &c;
    }
    return nullptr;
}

// Adicionar cliente
void adicionarCliente() {
    Cliente c;
    c.codigo = proximoCodigo++;

    cout << "Nome completo do cliente: ";
    cin.ignore();
    getline(cin, c.nome);

    cout << "Número do Cartão de Cidadão: ";
    getline(cin, c.cartaoCidadao);

    string nif;
    do {
        cout << "NIF (9 dígitos): ";
        getline(cin, nif);
        if (!nifValido(nif)) {
            cout << "NIF inválido. Deve conter exatamente 9 dígitos.\n";
        }
        else if (nifExiste(nif)) {
            cout << "Este NIF já está registado. Introduza outro.\n";
            nif = ""; // força repetição
        }
    } while (nif.empty() || !nifValido(nif));

    c.nif = nif;

    cout << "Saldo inicial (EUR): ";
    cin >> c.saldo;
    while (c.saldo < 0) {
        cout << "Saldo não pode ser negativo. Introduza novamente: ";
        cin >> c.saldo;
    }

    clientes.push_back(c);
    cout << "\033[2J\033[1;1H";
    cout << "Cliente adicionado com sucesso. Código atribuído: " << c.codigo << "\n";
}

// Listar clientes
void listarClientes() {
    cout << "\033[2J\033[1;1H";
    cout << "\n--- Lista de Clientes ---\n";
    for (const auto& c : clientes) {
        cout << "Código: " << c.codigo << " | Nome: " << c.nome << " | CC: " << c.cartaoCidadao
            << " | NIF: " << c.nif << " | Saldo: " << c.saldo << " EUR\n";
    }
}

// Depósito
void depositar() {
    int codigo;
    double valor;
    cout << "\033[2J\033[1;1H";
    cout << "Código do cliente: ";
    cin >> codigo;
    Cliente* c = encontrarCliente(codigo);
    if (c) {
        cout << "Valor a depositar (EUR): ";
        cin >> valor;
        if (valor > 0) {
            c->saldo += valor;
            cout << "Depósito realizado. Novo saldo: " << c->saldo << " EUR\n";
        }
        else {
            cout << "Valor inválido.\n";
        }
    }
    else {
        cout << "Cliente não encontrado.\n";
    }
}

// Levantamento
void levantar() {
    int codigo;
    double valor;
    cout << "Código do cliente: ";
    cin >> codigo;
    Cliente* c = encontrarCliente(codigo);
    if (c) {
        cout << "Valor a levantar (EUR): ";
        cin >> valor;
        if (valor > 0 && valor <= c->saldo) {
            c->saldo -= valor;
            cout << "Levantamento realizado. Novo saldo: " << c->saldo << " EUR\n";
        }
        else {
            cout << "Saldo insuficiente ou valor inválido.\n";
        }
    }
    else {
        cout << "Cliente não encontrado.\n";
    }
}

// Menu principal
int main() {
    setlocale(LC_ALL, "Portuguese_Portugal.UTF-8");

    int opcao1;
    int opcao2;

    do {
        std::cout << R"(
        =======|     ____                            _  __               ______      _ __       |=======
        =======|    / __ )____ _____  _________     | |/ /___ ___  __   / ____/_  __(_) /_____  |=======
        =======|   / __  / __ `/ __ \/ ___/ __ \    |   / __ `/ / / /  / / __/ / / / / __/ __ \ |=======
        =======|  / /_/ / /_/ / / / / /__/ /_/ /   /   / /_/ / /_/ /  / /_/ / /_/ / / /_/ /_/ / |=======
        =======| /_____/\__,_/_/ /_/\___/\____/   /_/|_\__,_/\__,_/   \____/\__,_/_/\__/\____/  |=======
        =======|                                                                                |=======
        )" << "\n";
        cout << "1. Fazer depósito\n";
        cout << "2. Fazer levantamento\n";
        cout << "3. PAINEL ADMINISTRATIVO\n";
        cout << "0. Sair\n";
        cout << "\nEscolha uma opção: ";
        cin >> opcao1;
        cin.ignore();

        switch (opcao1) {
        case 1: depositar(); break;
        case 2: levantar(); break;
        case 3: {
            int admin;
            do {
                cout << "\n1. Adicionar cliente\n";
                cout << "2. Listar clientes\n";
                cout << "3. AAAAAAAAAAAAAAA\n";
                cout << "\nEscolha uma opção: ";
                cin >> admin;
                cin.ignore();

                switch (admin) {
                case 1: adicionarCliente(); break;
                case 2: listarClientes(); break;
                //case 3: levantar(); break;
                }
            } while (admin == 1 || admin == 2);
        }
        default: cout << "Opção inválida.\n";
        }





    } while (opcao1 != 0);

    return 0;
}

