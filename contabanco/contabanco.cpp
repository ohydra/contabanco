// TRABALHO DE PROGRAMACAO REALIZADO POR:
//  - Miguel Andrade
//  - António Dias
//  - Mickael Ferreira

#include <iostream>     // Biblioteca para operações de entrada e saída (cin, cout, etc.)
#include <string>       // Biblioteca que permite trabalhar com objetos da classe string
#include <tchar.h>      // Biblioteca para suportar caracteres Unicode
#include <vector>       // Biblioteca da STL que permite usar o container vector
#include <cctype>       // Biblioteca com funções para manipulação de caracteres (ex: isdigit, toupper, etc.)
#include <ctime>        // Biblioteca para manipular tempo e datas (ex: time, localtime, etc.)
#include <limits>       // Biblioteca que define propriedades dos tipos primitivos (ex: limites máximos e mínimos)
#include <thread>       // Permite criar e manipular threads (execução paralela)
#include <chrono>       // Fornece funcionalidades relacionadas com tempo (ex: medir tempo de execução, delays, etc.)
#include <conio.h>      // Biblioteca específica do Windows usada para ler caracteres do teclado sem precisar carregar no Enter (ex: _getch())

using namespace std;

// Estrutura para guardar os dados de cada cliente
struct Cliente {
    int codigo;             // Código do cliente (ID interno, único)
    string nome;            // Nome completo
    int cartaoCidadao;      // Número do Cartão de Cidadão
    int nif;                // NIF (único, 9 dígitos)
    double saldo;           // Saldo
    int pin;		        // PIN da conta
};
vector<Cliente> clientes;


// estrutura para guardar os movimentos de todos os clientes
struct Movimento {
    string tipo;     // "Depósito", "Levantamento", "Transferência Enviada", "Transferência Recebida"
    double valor;
    string detalhes; // Info extra usado nas transferencias (função 4.) (destinatário/origem)
    string dataHora; // data e hora da operação
};
vector<vector<Movimento>> historicoMovimentos; // Um vetor de movimentos por cliente (indexado por código - 1)


int proximoCodigo = 1; // Código do próximo cliente a ser adicionado (começa em 1)




// Valida se o NIF tem 9 dígitos
bool nifValido(int nif) {
    return (nif >= 100000000 && nif <= 999999999);
}

// Verifica se o Cartao de Cidadao é válido (8 dígitos numéricos)
bool ccValido(int cc) {
    return (cc >= 10000000 && cc <= 99999999);
}

// Valida se o codigo do cliente tem 4 dígitos
bool pinValido(int pin) {
    return (pin >= 1000 && pin <= 9999);
}

// Verifica se o NIF já existe
bool nifExiste(int nif) {
    for (const auto& c : clientes) {
        if (c.nif == nif) return true;
    }
    return false;
}

// Encontrar cliente pelo codigo e valida com o pin (usado na função 2., 3., 4.)
Cliente* encontrarCliente(int codigo, int pin) {
    for (auto& c : clientes) {
        if (c.codigo == codigo && c.pin == pin) return &c;
    }
    return nullptr;
}

// Encontrar cliente de destino (usado na função 4.)
Cliente* encontrarDestino(int codigo) {
    for (auto& c : clientes) {
        if (c.codigo == codigo) return &c;
    }
    return nullptr;
}

// Mostra a data/hora atual formatada como string
string DataHoraAtual() {
    time_t agora = time(0);
    tm tempoLocal;
    localtime_s(&tempoLocal, &agora);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &tempoLocal);
    return string(buffer);
}

// Lê o código oculto (PIN) e retorna como string
string lerCodigoOculto() {
    string input;
    char ch;
    while ((ch = _getch()) != '\r') { // Enter
        if (isdigit(ch)) {
            input += ch;
            cout << '*';
        }
        else if (ch == '\b' && !input.empty()) { // Backspace
            input.pop_back();
            cout << "\b \b";
        }
    }
    cout << endl;
    return input;
}













//1. Registar cliente
void adicionarCliente() {
    system("cls"); //Limpa o ecra
    Cliente c;
    c.codigo = proximoCodigo++;

	//Nome do cliente
    string nome;
    bool nomeValido = false;

	//Validacao do nome, não pode ter números ou caracteres especiais e deve contter no mínimo 10 caracteres
    do {
        cout << "Nome completo do cliente: ";
        getline(cin >> ws, nome);

        bool apenasLetrasEspacos = true;
        for (char ch : nome) {
            if (!isalpha(ch) && !isspace(ch)) {
                apenasLetrasEspacos = false;
                break;
            }
        }

        if (!apenasLetrasEspacos || nome.length() < 10) {
            cout << "\033[31mNome inválido. Deve conter apenas letras e ter no mínimo 10 caracteres.\033[0m\n";
            continue;
        }

        nomeValido = true;

    } while (!nomeValido);

    c.nome = nome;

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
            cout << "\033[31mNúmero do Cartão de Cidadão inválido. Deve conter exatamente 8 números.\033[0m\n";
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
            cout << "\033[31mNIF inválido. Deve conter exatamente 9 números.\033[0m\n";
            continue;
        }

        nif = stoi(nifStr); // Converter para int

        if (nifExiste(nif)) {
            cout << "\033[31mEste NIF já está registado.Tente novamente!\033[0m\n";
        }
        else {
            valido = true;
        }

    } while (!valido);

    c.nif = nif;



	//validacao do registo do PIN
    int pin = 0;
    string pinStr;
    bool pinValido = false;

    do {
        cout << "Introduza um PIN (entre 4 a 6 números): ";
        cin >> pinStr;

        // Verifica se todos os caracteres são dígitos
        bool soNumeros = true;
        for (char c : pinStr) {
            if (!isdigit(c)) {
                soNumeros = false;
                break;
            }
        }

        if (!soNumeros || pinStr.length() < 4 || pinStr.length() > 6) {
            cout << "\033[31mPIN inválido. Deve conter entre 4 a 6 números.\033[0m\n";
            continue;
        }

        pin = stoi(pinStr);
        pinValido = true;

    } while (!pinValido);

    c.pin = pin;



    cout << "Saldo de abertura (EUR): ";
    cin >> c.saldo;
    while (c.saldo < 0) {
        cout << "\033[31mO saldo não pode ser negativo! Tente novamente: \033[0m\n";
        cin >> c.saldo;
    }

    clientes.push_back(c);
    cout << "\n\033[32mCliente registado com sucesso. Código (ID) atribuído: " << c.codigo << "\033[0m\n";
    historicoMovimentos.push_back(vector<Movimento>()); // Cria histórico vazio para novo cliente
    std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
    system("cls"); //Limpa o ecra
}















//2. Depósito
void depositar() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "\033[31mNão existem clientes registados.\033[0m\n";
        return;
    }

    int codigo, pin;
    double valor;
    cout << "Introduza o seu código (ID) de cliente : ";
    cin >> codigo;
    cout << "Introduza o seu PIN: ";
    cin >> pin;
    Cliente* c = encontrarCliente(codigo, pin);

    if (c) {
        cout << "Valor a depositar (EUR): ";
        cin >> valor;
        if (valor > 0) {
            c->saldo += valor;
			historicoMovimentos[c->codigo - 1].push_back({ "Depósito", valor, "", DataHoraAtual() }); // Adiciona movimento de Depósito ao histórico e o valor
            cout << "Depósito realizado com sucesso. Novo saldo: " << c->saldo << " €\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
            system("cls"); // Limpa o ecra
        }
        else {
            cout << "\033[31mValor inválido.\033[0m\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
            system("cls"); // Limpa o ecra
        }
    }
    else {
        cout << "\033[31mCliente não encontrado ou PIN errado.\033[0m\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
        system("cls"); // Limpa o ecra
    }
}















//3. Levantamento
void levantar() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "\033[31mNão existem clientes registados.\033[0m\n";
        return;
    }

    int codigo, pin;
    double valor;
    cout << "Introduza o seu código (ID) do cliente : ";
    cin >> codigo;
    cout << "Introduza o seu PIN: ";
    cin >> pin;
    Cliente* c = encontrarCliente(codigo,pin);
    if (c) {
        cout << "Valor a levantar (EUR): ";
        cin >> valor;
        if (valor > 0 && valor <= c->saldo) {
            c->saldo -= valor;
			historicoMovimentos[c->codigo - 1].push_back({ "Levantamento", valor, "", DataHoraAtual() }); // Adiciona movimento de Levantamento ao histórico e o valor
            cout << "Levantamento com sucesso. Novo saldo: " << c->saldo << " €\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
            system("cls"); // Limpa o ecra
        }
        else {
            cout << "\033[31mSaldo insuficiente ou valor inválido.\033[0m\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
            system("cls"); // Limpa o ecra
        }
    }
    else {
        cout << "\033[31mCliente não encontrado ou PIN errado.\033[0m\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
        system("cls"); // Limpa o ecra
    }
}















//4. Transferência
void transferir() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "\033[31mNão existem clientes registados.\033[0m\n";
        return;
    }

    int codigoOrigem, codigoDestino, pin;
    double valor;

    cout << "Introduza o seu código (ID) de cliente: ";
    cin >> codigoOrigem;
    cout << "Introduza o seu PIN: ";
    cin >> pin;
    Cliente* origem = encontrarCliente(codigoOrigem,pin);

    if (origem) {
        cout << "Introduza o código (ID) do cliente de destino: ";
        cin >> codigoDestino;
        Cliente* destino = encontrarDestino(codigoDestino);

        if (!destino) {
            cout << "\033[31mCliente de destino não encontrado.\033[0m\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
            system("cls"); // Limpa o ecra
            return;
        }

        if (codigoOrigem == codigoDestino) {
            cout << "Não é permitido transferir para o próprio cliente oh burro, aqui não há multiplicação de guito!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
            system("cls"); // Limpa o ecra
            return;
        }

        cout << "Valor a transferir (EUR): ";
        cin >> valor;

        if (valor <= 0) {
            cout << "\033[31mValor inválido.\033[0m\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
            system("cls"); // Limpa o ecra
            return;
        }

        if (origem->saldo < valor) {
            cout << "\033[31mSaldo insuficiente para a transferência.\033[0m\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
            system("cls"); // Limpa o ecra
            return;
        }

        origem->saldo -= valor;
        destino->saldo += valor;
		historicoMovimentos[origem->codigo - 1].push_back({ "Transferência Enviada", valor, "Para: " + destino->nome, DataHoraAtual() }); // Adiciona movimento de Transferência Enviada ao histórico, o valor, e o destino
		historicoMovimentos[destino->codigo - 1].push_back({ "Transferência Recebida", valor, "De: " + origem->nome, DataHoraAtual() }); // Adiciona movimento de Transferência Recebida ao histórico e o valor, e a origem
        cout << "\033[32mTransferência realizada com sucesso.\033[0m\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
        system("cls"); // Limpa o ecra

    }
    else {
        cout << "\033[31mCliente não encontrado ou PIN errado.\033[0m\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Para o programa 3 segundos
        system("cls"); // Limpa o ecra
    }
}















//5. Listar todos os clientes
void listarClientes() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "\033[31mNão existem clientes registados.\033[0m\n";
        return;
    }

    cout << "\n--- Lista de Clientes ---\n";
    for (const auto& c : clientes) {
        cout << "Código (ID): " << c.codigo << " | Nome: " << c.nome << " | CC: " << c.cartaoCidadao
            << " | NIF: " << c.nif << " | Saldo: " << c.saldo << "€ | PIN: " << c.pin << "\n";
    }
    cout << "\n\n\n";
	system("pause"); // Pausa para o utilizador ver a lista
    system("cls"); //Limpa o ecra
}















//6. Pesquisa de cliente por NIF
void pesquisarPorNIF() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "\033[31mNão existem clientes registados.\033[0m\n";
        return;
    }

    int nif;
    cout << "Introduza o NIF para pesquisar: ";
    cin >> nif;

    bool encontrado = false;
    for (const auto& c : clientes) {
        if (c.nif == nif) {
            cout << "\nCliente encontrado:\n";
            cout << "Código (ID): " << c.codigo << " | Nome: " << c.nome
                << " | CC: " << c.cartaoCidadao << " | NIF: " << c.nif
                << " | Saldo: " << c.saldo << "€"
                << " | PIN: " << c.pin << "\n\n";
            encontrado = true;
            break;
        }

    }


    if (!encontrado) {
        cout << "\033[31mNão foi encontrado nenhum cliente com esse NIF.\033[0m\n";
    }
    system("pause"); // Pausa para o utilizador ver a lista
    system("cls"); //Limpa o ecra
}















//7. Estatísticas
void mostrarEstatisticas() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "\033[31mNão existem clientes registados.\033[0m\n";
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
            cout << "\033[31mValor inválido. Tente novamente.\033[0m\n";
            valorLimite = -1;
        }
    } while (valorLimite < 0);

   

    for (const auto& c : clientes) {
        if (c.saldo > valorLimite) acimaDeValor++;
    }

    system("cls"); //Limpa o ecra
    cout << "\n--- Estatísticas ---\n";
    cout << "Total de dinheiro no banco: " << totalSaldo << "€\n";
    cout << "Média de saldo: " << media << "€\n";
    cout << "Cliente com maior saldo: " << clienteMaior->nome << " (" << clienteMaior->saldo << "€)\n";
    cout << "Cliente com menor saldo: " << clienteMenor->nome << " (" << clienteMenor->saldo << "€)\n";
    cout << "Quantidade de clientes com saldo superior a " << valorLimite << "€: " << acimaDeValor << "\n\n";
    system("pause"); // Pausa para o utilizador ver a lista
    system("cls"); //Limpa o ecra
}














//8. Mostrar movimentos de um cliente
void mostrarMovimentos() {
    system("cls"); //Limpa o ecra
    if (clientes.empty()) {
        cout << "\033[31mNão existem clientes registados.\033[0m\n";
        return;
    }

    int codigo, pin;
    cout << "Introduza o seu código (ID) do cliente : ";
    cin >> codigo;
    cout << "Introduza o seu PIN: ";
    cin >> pin;

    Cliente* c = encontrarCliente(codigo, pin);
    if (!c) {
        cout << "\033[31mCódigo de cliente ou PIN incorreto. Tente novamente.\033[0m\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        system("cls");
        return;
    }

    cout << "\n--- Últimos movimentos de " << c->nome << " ---\n";

    const auto& movimentos = historicoMovimentos[codigo - 1];
    int total = movimentos.size();

    if (total == 0) {
        cout << "\033[31mSem movimentos registados.\033[0m\n";
        cout << "\n\n[ Saldo atual: " << c->saldo << "€ ]\n";
    }

    int inicio = max(0, total - 20);
    for (int i = total - 1; i >= inicio; --i) {
        cout << "[" << movimentos[i].dataHora << "] ";
        cout << movimentos[i].tipo << " de " << movimentos[i].valor << " €";
        if (!movimentos[i].detalhes.empty()) {
            cout << " (" << movimentos[i].detalhes << ")";
        }
		cout << "\n\n[ Saldo atual: " << c->saldo << "€ ]";
        cout << endl;
    }
    cout << "\n";
    system("pause"); // Pausa para o utilizador ver a lista
    system("cls"); //Limpa o ecra
    return;
}















// Menu principal
int main() {
    system("chcp 65001 > nul"); // Força UTF-8 no terminal

    bool volta = false;
    int opcao1, opcao2;

    do {
        if (!volta) {
            system("cls");
            volta = true;
        }

        std::cout << R"(
    _     ____                         __  __              ____       _ _            _  
   | |   | __ )  __ _ _ __   ___ ___   \ \/ /__ _ _   _   / ___|_   _(_) |_ ___     | | 
  / __)  |  _ \ / _` | '_ \ / __/ _ \   \  // _` | | | | | |  _| | | | | __/ _ \   / __)
  \__ \  | |_) | (_| | | | | (_| (_) |  /  \ (_| | |_| | | |_| | |_| | | || (_) |  \__ \
  (   /  |____/ \__,_|_| |_|\___\___/  /_/\_\__,_|\__,_|  \____|\__,_|_|\__\___/   (   /
   |_|                                                                              |_| 
        )" << "\n";

        cout << "  1. Verificar movimentos\n";
        cout << "  2. Fazer depósito\n";
        cout << "  3. Fazer levantamento\n";
        cout << "  4. Fazer uma transferência\n";
        cout << "  9. ADMINISTRAÇÃO\n";
        cout << "  0. Sair\n";
        cout << "\nEscolha uma opção: ";
        cin >> opcao1;
        cin.ignore();

        switch (opcao1) {
        case 1: mostrarMovimentos(); break;
        case 2: depositar(); break;
        case 3: levantar(); break;
        case 4: transferir(); break;

        case 9: {
            string codStr;
            bool codigoValido = false;

            do {
                cout << "Introduza o código administrativo: ";
                codStr = lerCodigoOculto();

                if (codStr == "1111") {
                    cout << "\n\033[32mPainel desbloqueado, bem-vindo!\033[0m";
                    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                    system("cls");
                    codigoValido = true;

                    // Menu administrativo
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
                        cout << "  4. Pesquisar por cliente\n";
                        cout << "< 0. Voltar ao menu anterior\n";
                        cout << "\nEscolha uma opção: ";
                        cin >> opcao2;
                        cin.ignore();

                        switch (opcao2) {
                        case 1: adicionarCliente(); break;
                        case 2: listarClientes(); break;
                        case 3: mostrarEstatisticas(); break;
                        case 4: pesquisarPorNIF(); break;
                        case 0: volta = false; break;
                        cout << "\033[2J\033[1;1H\033[31mOpção inválida. Tente novamente.\033[0m\n";
                        }

                    } while (opcao2 != 0);
                }
                else if (codStr == "0") {
                    volta = false;
                    break;
                }
                else {
                    cout << "\033[31mCódigo errado! Tente novamente ou introduza 0 para voltar.\033[0m\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                }

            } while (!codigoValido);
            break;
        }

        case 0:
            cout << "A sair do programa...\n";
            break;

        default:
            cout << "\033[2J\033[1;1H\033[31mOpção inválida. Tente novamente.\033[0m\n";
        }

    } while (opcao1 != 0);

    return 0;
}
