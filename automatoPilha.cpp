#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

using TransicaoValor = pair<int, string>;

struct AutomatoPilha {
    int estado_inicial;
    char simbolo_inicial_pilha;
    vector<int> estados_aceitacao;
    unordered_map<string, vector<TransicaoValor>> transicoes;

    void addTransicao(int estado, char entrada, char topo,
                      int prox, const string& empilhar) {

        string chave =
            to_string(estado) + "|" + entrada + "|" + topo;

        transicoes[chave].push_back({prox, empilhar});
    }

    vector<TransicaoValor> getTransicoes(
        int estado,
        char entrada,
        char topo) const {

        string chave =
            to_string(estado) + "|" + entrada + "|" + topo;

        auto it = transicoes.find(chave);

        return it != transicoes.end()
                   ? it->second
                   : vector<TransicaoValor>{};
    }

    bool ehAceitacao(int estado) const {
        for (int e : estados_aceitacao)
            if (e == estado)
                return true;

        return false;
    }
};

struct Configuracao {
    int estado;
    int pos;
    int passo;
    stack<char> pilha;
};

string pilhaParaString(stack<char> pilha) {

    string resultado;

    while (!pilha.empty()) {
        resultado = pilha.top() + resultado;
        pilha.pop();
    }

    return resultado;
}

bool simular(
    const AutomatoPilha& ap,
    const string& entrada,
    Configuracao cfg) {

    cout << "\n(q" << cfg.estado << ", ";

    if (cfg.pos < entrada.size())
        cout << entrada.substr(cfg.pos);
    else
        cout << "ε";

    cout << ", " << pilhaParaString(cfg.pilha) << ")";

    if (cfg.pos == (int)entrada.size()) {

        if (!cfg.pilha.empty()) {

            char topo = cfg.pilha.top();

            for (auto& [prox, empilhar]
                 : ap.getTransicoes(cfg.estado, '\0', topo)) {

                cout << "\n\nPasso " << cfg.passo;
                cout << "\nEstado: q" << cfg.estado;
                cout << "\nSímbolo lido: ε";
                cout << "\nTopo da pilha: " << topo;
                cout << "\nPilha: "
                     << pilhaParaString(cfg.pilha);

                cout << "\nTransição: δ(q"
                     << cfg.estado
                     << ", ε, "
                     << topo
                     << ") → (q"
                     << prox
                     << ", "
                     << (empilhar.empty() ? "ε" : empilhar)
                     << ")\n";

                Configuracao nova = cfg;

                nova.estado = prox;
                nova.passo++;

                nova.pilha.pop();

                for (int i = empilhar.size() - 1;
                     i >= 0;
                     i--)
                    nova.pilha.push(empilhar[i]);

                if (simular(ap, entrada, nova))
                    return true;
            }
        }

        return ap.ehAceitacao(cfg.estado);
    }

    if (cfg.pilha.empty())
        return false;

    char simbolo = entrada[cfg.pos];
    char topo = cfg.pilha.top();

    for (auto& [prox, empilhar]
         : ap.getTransicoes(cfg.estado,
                             simbolo,
                             topo)) {

        cout << "\n\nPasso " << cfg.passo;
        cout << "\nEstado: q" << cfg.estado;
        cout << "\nSímbolo lido: " << simbolo;
        cout << "\nTopo da pilha: " << topo;
        cout << "\nPilha: "
             << pilhaParaString(cfg.pilha);

        cout << "\nTransição: δ(q"
             << cfg.estado
             << ", "
             << simbolo
             << ", "
             << topo
             << ") → (q"
             << prox
             << ", "
             << (empilhar.empty() ? "ε" : empilhar)
             << ")\n";

        Configuracao nova = cfg;

        nova.estado = prox;
        nova.pos++;
        nova.passo++;

        nova.pilha.pop();

        for (int i = empilhar.size() - 1;
             i >= 0;
             i--)
            nova.pilha.push(empilhar[i]);

        if (simular(ap, entrada, nova))
            return true;
    }

    for (auto& [prox, empilhar]
         : ap.getTransicoes(cfg.estado,
                             '\0',
                             topo)) {

        cout << "\n\nPasso " << cfg.passo;
        cout << "\nEstado: q" << cfg.estado;
        cout << "\nSímbolo lido: ε";
        cout << "\nTopo da pilha: " << topo;
        cout << "\nPilha: "
             << pilhaParaString(cfg.pilha);

        cout << "\nTransição: δ(q"
             << cfg.estado
             << ", ε, "
             << topo
             << ") → (q"
             << prox
             << ", "
             << (empilhar.empty() ? "ε" : empilhar)
             << ")\n";

        Configuracao nova = cfg;

        nova.estado = prox;
        nova.passo++;

        nova.pilha.pop();

        for (int i = empilhar.size() - 1;
             i >= 0;
             i--)
            nova.pilha.push(empilhar[i]);

        if (simular(ap, entrada, nova))
            return true;
    }

    return false;
}

bool reconhece(
    const AutomatoPilha& ap,
    const string& entrada) {

    Configuracao cfg;

    cfg.estado = ap.estado_inicial;
    cfg.pos = 0;
    cfg.passo = 1;

    cfg.pilha.push(
        ap.simbolo_inicial_pilha);

    return simular(ap, entrada, cfg);
}


AutomatoPilha criar() {

    AutomatoPilha ap;

    ap.estado_inicial = 0;
    ap.simbolo_inicial_pilha = 'Z';
    ap.estados_aceitacao = {3};

    ap.addTransicao(0, 'a', 'Z', 1, "AZ");
    ap.addTransicao(1, 'a', 'A', 1, "AA");

    ap.addTransicao(1, 'b', 'A', 2, "");
    ap.addTransicao(2, 'b', 'A', 2, "");

    ap.addTransicao(2, '\0', 'Z', 3, "Z");

    return ap;
}

int main() {

    AutomatoPilha ap =
        criar();

    string entrada;

    cout << "=====================================\n";
    cout << "AUTÔMATO DE PILHA\n";
    cout << "L = { aⁿbⁿ | n >= 1 }\n";
    cout << "=====================================\n";

    while (true) {

        cout << "\nDigite um AUTÔMATO ('sair' para encerrar): ";
        cin >> entrada;

        if (entrada == "sair")
            break;

        cout << "\n============================";
        cout << "\nSIMULACAO INICIADA";
        cout << "\n============================\n";

        bool aceita =
            reconhece(ap, entrada);

        cout << "\n\n============================\n";

        if (aceita)
            cout << "RESULTADO: ACEITA\n";
        else
            cout << "RESULTADO: REJEITA\n";

        cout << "============================\n";
    }

    return 0;
}