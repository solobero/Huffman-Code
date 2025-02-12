#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <tuple>

using namespace std;

//Nodo del árbol
struct Node { 
    char simbolo;
    int frecuencia;
    Node *izq, *der;
};

// Codificación

// Función para asignar memoria dinámicamente y enlazar los hijos del nodo
Node* getNode(char simbolo, int frecuencia, Node* izq, Node* der){
    Node* nodo = new Node();

    nodo -> simbolo = simbolo;
    nodo -> frecuencia = frecuencia;
    nodo -> izq = izq;
    nodo -> der = der;

    return nodo;
}

// Comparación usada para ordenar la pila (menor frecuencia tiene mayor prioridad)
struct comp {
    bool operator()(Node* i,Node* d){
        return i->frecuencia > d-> frecuencia;
    }
};

// Recorre el árbol y asigna códigos de Huffman a los símbolos
void encode(Node *raiz, string str, unordered_map<char,string> &CodigoHuffman){
    if(raiz == nullptr)
        return;
    if(!raiz -> izq && !raiz -> der){
        CodigoHuffman[raiz->simbolo]=str;
    }
    encode(raiz -> izq,str + "0", CodigoHuffman);
    encode(raiz -> der, str + "1", CodigoHuffman);
}

// Decodificar el mensaje
string decode(Node* raiz, string codigo) {
    string resultado = "";
    Node* actual = raiz;
    for (char bit : codigo) {
        actual = (bit == '0') ? actual->izq : actual->der;
        if (!actual->izq && !actual->der) {
            resultado += actual->simbolo;
            actual = raiz;
        }
    }
    return resultado;
}

// Construcción del árbol de Huffman
tuple<Node*, unordered_map<char, string>> construirHuffman(string texto) {
    unordered_map<char, int> frecuencia;
    for (char c : texto) {
        frecuencia[c]++;
    }

    priority_queue<Node*, vector<Node*>, comp> pq;
    for (auto par : frecuencia) {
        pq.push(getNode(par.first, par.second, nullptr, nullptr));
    }

    while (pq.size() > 1) {
        Node* izq = pq.top(); pq.pop();
        Node* der = pq.top(); pq.pop();
        int suma = izq->frecuencia + der->frecuencia;
        pq.push(getNode('\0', suma, izq, der));
    }

    Node* raiz = pq.top();
    unordered_map<char, string> CodigoHuffman;
    encode(raiz, "", CodigoHuffman);
    return make_tuple(raiz, CodigoHuffman);
}

int main() {
    string texto;
    cout << "Ingrese el texto a codificar: ";
    getline(cin, texto);

    Node* raiz;
    unordered_map<char, string> CodigoHuffman;
    tie(raiz, CodigoHuffman) = construirHuffman(texto);

    cout << "\nCodigos de Huffman:\n";
    for (auto par : CodigoHuffman) {
        cout << par.first << " : " << par.second << "\n";
    }

    string mensajeCodificado = "";
    for (char c : texto) {
        mensajeCodificado += CodigoHuffman[c];
    }

    cout << "\nTexto codificado: " << mensajeCodificado << "\n";
    cout << "Texto decodificado: " << decode(raiz, mensajeCodificado) << "\n";

    return 0;
}