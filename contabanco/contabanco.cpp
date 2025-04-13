#define EURO char(128)
#include <iostream>
#include <windows.h>
#include <string>
#include <tchar.h>
#include <vector>
#include <cctype>
#include <limits>

using namespace std;





struct Cliente {
    int codigo;                 // Código do cliente (ID interno, único)
    string nome;                // Nome completo
    int cartaoCidadao;       // Número do Cartão de Cidadão
    int nif;                 // NIF (único, 9 dígitos)
    double saldo;               // Saldo inicial
};

vector<Cliente> clientes;
int proximoCodigo = 1;

// Valida se o NIF tem 9 dígitos
bool nifValido(int nif) {
    return (nif >= 100000000 && nif <= 999999999);
}

// Verifica se o Cartao de Cidadao é válido (8 dígitos numéricos)
bool ccValido(int cc) {
    return (cc >= 10000000 && cc <= 99999999);
}

// Verifica se o NIF já existe
bool nifExiste(int nif) {
    for (const auto& c : clientes) {
        if (c.nif == nif) return true;
    }
    return false;
}

// Encontrar cliente pelo NIF
Cliente* encontrarCliente(int codigo) {
    for (auto& c : clientes) {
        if (c.codigo == codigo) return &c;
    }
    return nullptr;
}
















//1. Registar cliente
void adicionarCliente() {
    system("cls"); //Limpa o ecra
    Cliente c;
    c.codigo = proximoCodigo++;

    cout << "Nome completo do cliente: ";
    cin.ignore();
    getline(cin, c.nome);


    //validacao do cartao de cidadao
    int cc = 0;
    string ccStr;
    bool ccValido = false;

    do {
        cout << "Número do Cartão de Cidadão (8 números): ";
        cin >> ccStr;

        // Verifica se todos os caracteres são dígitos
        bool soNumeros = true;
        for (char c : ccStr) {
            if (!isdigit(c)) {
                soNumeros = false;
                break;
            }
        }

        if (!soNumeros || ccStr.length() != 8) {
            cout << "Cartão de Cidadão inválido. Deve conter exatamente 8 numéros.\n";
            continue;
        }

        cc = stoi(ccStr);
        ccValido = true;

    } while (!ccValido);

    c.cartaoCidadao = cc;
    



    //validacao do nif
    int nif = 0;
    string nifStr;
    bool valido = false;

    do {
        cout << "Introduza o NIF (9 números): ";
        cin >> nifStr;

        // Verifica se todos os caracteres são dígitos
        bool soNumeros = true;
        for (char c : nifStr) {
            if (!isdigit(c)) {
                soNumeros = false;
                break;
            }
        }

        if (!soNumeros || nifStr.length() != 9) {
            cout << "NIF inválido. Deve conter exatamente 9 numéros.\n";
            continue;
        }

        nif = stoi(nifStr); // Converter para int

        if (nifExiste(nif)) {
            cout << "Este NIF já está registado. Introduza outro!\n";
        }
        else {
            valido = true;
        }

    } while (!valido);

    c.nif = nif;




    cout << "Saldo inicial (EUR): ";
    cin >> c.saldo;
    while (c.saldo < 0) {
        cout << "Saldo não pode ser negativo oh burro! Introduz novamente: ";
        cin >> c.saldo;
    }

    clientes.push_back(c);
    system("cls"); //Limpa o ecra
    cout << "Cliente adicionado com sucesso. Código (ID) atribuído: " << c.codigo << "\n";
}















//2. Depósito
void depositar() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "Não existem clientes registados.\n";
        return;
    }

    int codigo;
    double valor;
    cout << "Código do cliente (ID): ";
    cin >> codigo;
    Cliente* c = encontrarCliente(codigo);
    if (c) {
        cout << "Valor a depositar (EUR): ";
        cin >> valor;
        if (valor > 0) {
            c->saldo += valor;
            cout << "Depósito realizado com sucesso. Novo saldo: " << c->saldo << " €\n";
        }
        else {
            cout << "Valor inválido.\n";
        }
    }
    else {
        cout << "Cliente não encontrado.\n";
    }
}















//3. Levantamento
void levantar() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "Não existem clientes registados.\n";
        return;
    }

    int codigo;
    double valor;
    cout << "Código do cliente (ID): ";
    cin >> codigo;
    Cliente* c = encontrarCliente(codigo);
    if (c) {
        cout << "Valor a levantar (EUR): ";
        cin >> valor;
        if (valor > 0 && valor <= c->saldo) {
            c->saldo -= valor;
            cout << "Levantamento com sucesso. Novo saldo: " << c->saldo << " €, póbre que fode!\n";
        }
        else {
            cout << "Saldo insuficiente ou valor inválido.\n";
        }
    }
    else {
        cout << "Cliente não encontrado.\n";
    }
}















//4. Transferência
void transferir() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "Não existem clientes registados.\n";
        return;
    }

    int codigoOrigem, codigoDestino;
    double valor;

    cout << "Código (ID) do cliente de origem: ";
    cin >> codigoOrigem;
    Cliente* origem = encontrarCliente(codigoOrigem);

    cout << "Código (ID) do cliente de destino: ";
    cin >> codigoDestino;
    Cliente* destino = encontrarCliente(codigoDestino);

    if (!origem || !destino) {
        cout << "Um ou ambos os clientes não foram encontrados.\n";
        return;
    }

    if (codigoOrigem == codigoDestino) {
        cout << "Não é permitido transferir para o próprio cliente oh burro, aqui não há multiplicação de guito!\n";
        return;
    }

    cout << "Valor a transferir (EUR): ";
    cin >> valor;

    if (valor <= 0) {
        cout << "Valor inválido.\n";
        return;
    }

    if (origem->saldo < valor) {
        cout << "Saldo insuficiente para a transferência.\n";
        return;
    }

    origem->saldo -= valor;
    destino->saldo += valor;
    cout << "Transferência realizada com sucesso.\n";
}















//5. Listar clientes
void listarClientes() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "Não existem clientes registados.\n";
        return;
    }

    cout << "\n--- Lista de Clientes ---\n";
    for (const auto& c : clientes) {
        cout << "Código (ID): " << c.codigo << " | Nome: " << c.nome << " | CC: " << c.cartaoCidadao
            << " | NIF: " << c.nif << " | Saldo: " << c.saldo << " €\n";
    }
    cout << "\n\n\n";
}















//6. Pesquisa de cliente por NIF
void pesquisarPorNIF() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "Não existem clientes registados.\n";
        return;
    }

    int nif;
    cout << "Introduza o NIF para pesquisar: ";
    cin >> nif;

    bool encontrado = false;
    for (const auto& c : clientes) {
        if (c.nif == nif) {
            cout << "Cliente encontrado:\n";
            cout << "Código (ID): " << c.codigo << " | Nome: " << c.nome
                << " | CC: " << c.cartaoCidadao << " | NIF: " << c.nif
                
                << " | Saldo: " << c.saldo << " \xE2\x82\xAC\n";
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Não foi encontrado nenhum cliente com esse NIF.\n";
    }
}















//7. Estatísticas
void mostrarEstatisticas() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "Não existem clientes registados.\n";
        return;
    }

    double totalSaldo = 0;
    double maiorSaldo = clientes[0].saldo;
    double menorSaldo = clientes[0].saldo;
    Cliente* clienteMaior = &clientes[0];
    Cliente* clienteMenor = &clientes[0];
    int acimaDeValor = 0;
    double valorLimite;

    for (const auto& c : clientes) {
        totalSaldo += c.saldo;
        if (c.saldo > maiorSaldo) {
            maiorSaldo = c.saldo;
            clienteMaior = const_cast<Cliente*>(&c);
        }
        if (c.saldo < menorSaldo) {
            menorSaldo = c.saldo;
            clienteMenor = const_cast<Cliente*>(&c);
        }
    }

    double media = totalSaldo / clientes.size();

    do {
        cout << "Introduza um determinado valor em € (EUR) para listar a quantidade de clientes com o saldo igual ou superior: ";
        cin >> valorLimite;

        if (cin.fail() || valorLimite < 0) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Valor inválido. Tente novamente.\n";
            valorLimite = -1;
        }
    } while (valorLimite < 0);



    for (const auto& c : clientes) {
        if (c.saldo > valorLimite) acimaDeValor++;
    }

    cout << "\n--- Estatísticas ---\n";
    cout << "Total de dinheiro no banco: " << totalSaldo << " €\n";
    cout << "Média de saldo: " << media << " €\n";
    cout << "Cliente com maior saldo: " << clienteMaior->nome << " (" << clienteMaior->saldo << " €)\n";
    cout << "Cliente com menor saldo: " << clienteMenor->nome << " (" << clienteMenor->saldo << " €)\n";
    cout << "Quantidade de clientes com saldo superior a " << valorLimite << "€: " << acimaDeValor << "\n";
}















// Menu principal
int main() {
    system("chcp 65001 > nul"); //Força a consola para UTF-8 sem mostrar a mensagem

	bool cls = false; //variavel para verificar se o ecra foi limpo inicialmente
    int opcao1;
    int opcao2;


    do {
		if (cls==false) {
			system("cls"); //Limpa o ecra
			cls = true;
		}
        std::cout << R"(
    _     ____                         __  __              ____       _ _            _  
   | |   | __ )  __ _ _ __   ___ ___   \ \/ /__ _ _   _   / ___|_   _(_) |_ ___     | | 
  / __)  |  _ \ / _` | '_ \ / __/ _ \   \  // _` | | | | | |  _| | | | | __/ _ \   / __)
  \__ \  | |_) | (_| | | | | (_| (_) |  /  \ (_| | |_| | | |_| | |_| | | || (_) |  \__ \
  (   /  |____/ \__,_|_| |_|\___\___/  /_/\_\__,_|\__,_|  \____|\__,_|_|\__\___/   (   /
   |_|                                                                              |_| 
        )" << "\n";
        cout << "  1. Fazer depósito\n";
        cout << "  2. Fazer levantamento\n";
        cout << "  3. Fazer uma transferência\n";
        cout << "  4. Pesquisar por cliente\n";
        cout << "  9. PAINEL ADMINISTRATIVO\n";
        cout << "  0. Sair\n";
        cout << "\nEscolha uma opção: ";
        cin >> opcao1;
        cin.ignore();

        switch (opcao1) {
        case 1: depositar(); break;
        case 2: levantar(); break;
        case 3: transferir(); break;
        case 4: pesquisarPorNIF(); break;
        case 9: 
            system("cls"); //Limpa o ecra
            do {
                std::cout << R"(
    _     ____                         __  __              ____       _ _            _  
   | |   | __ )  __ _ _ __   ___ ___   \ \/ /__ _ _   _   / ___|_   _(_) |_ ___     | | 
  / __)  |  _ \ / _` | '_ \ / __/ _ \   \  // _` | | | | | |  _| | | | | __/ _ \   / __)
  \__ \  | |_) | (_| | | | | (_| (_) |  /  \ (_| | |_| | | |_| | |_| | | || (_) |  \__ \
  (   /  |____/ \__,_|_| |_|\___\___/  /_/\_\__,_|\__,_|  \____|\__,_|_|\__\___/   (   /
   |_|                             PAINEL ADMINISTRATIVO                            |_| 
                )" << "\n";
                cout << "  1. Adicionar cliente\n";
                cout << "  2. Listar clientes\n";
                cout << "  3. Estatísticas\n";
                cout << "< 0. Voltar ao menu anterior\n";
                cout << "\nEscolha uma opção: ";
                cin >> opcao2;
                cin.ignore();

                switch (opcao2) {
                case 1: adicionarCliente(); break;
				case 2: listarClientes(); break;
                case 3: mostrarEstatisticas(); break;
                case 0: cls = false; break;
                default: cout << "\033[2J\033[1;1HOpção inválida.\n";
				
                }
            } while (opcao2 != 0);
        default: cout << "\033[2J\033[1;1HOpção inválida.\n";
            
        }
    } while (opcao1 != 0);

    return 0;
}

