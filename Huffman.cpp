#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <bitset>
#include <vector>

using namespace std;

struct Node {
    char simbolo;
    int frecuencia;
    Node *izq, *der;
};

Node* getNode(char simbolo, int frecuencia, Node* izq, Node* der) {
    Node* nodo = new Node();
    nodo->simbolo = simbolo;
    nodo->frecuencia = frecuencia;
    nodo->izq = izq;
    nodo->der = der;
    return nodo;
}

struct comp {
    bool operator()(Node* i, Node* d) {
        return i->frecuencia > d->frecuencia;
    }
};

void encode(Node* raiz, string str, unordered_map<char, string>& CodigoHuffman) {
    if (!raiz) return;
    if (!raiz->izq && !raiz->der) {
        CodigoHuffman[raiz->simbolo] = str;
    }
    encode(raiz->izq, str + "0", CodigoHuffman);
    encode(raiz->der, str + "1", CodigoHuffman);
}

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

void escribirArchivo(const char* nombre, const string& contenido) {
    ofstream archivo(nombre, ios::binary);
    if (!archivo) {
        cerr << "Error al escribir el archivo\n";
        exit(EXIT_FAILURE);
    }
    archivo.write(contenido.c_str(), contenido.size());
    archivo.close();
}

string leerArchivo(const char* nombre) {
    ifstream archivo(nombre, ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo\n";
        exit(EXIT_FAILURE);
    }
    string contenido((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
    archivo.close();
    return contenido;
}

void comprimirArchivo(const char* nombre) {
    string texto = leerArchivo(nombre);
    Node* raiz;
    unordered_map<char, string> CodigoHuffman;
    tie(raiz, CodigoHuffman) = construirHuffman(texto);

    string tablaHuffman = "";
    for (auto& par : CodigoHuffman) {
        tablaHuffman += par.first + par.second + "\n";
    }

    string mensajeCodificado = "";
    for (char c : texto) {
        mensajeCodificado += CodigoHuffman[c];
    }

    string binario = "";
    for (size_t i = 0; i < mensajeCodificado.size(); i += 8) {
        string byte = mensajeCodificado.substr(i, 8);
        while (byte.length() < 8) byte += "0";
        char caracter = static_cast<char>(bitset<8>(byte).to_ulong());
        binario += caracter;
    }

    escribirArchivo("comprimido.huff", tablaHuffman + "##\n" + binario);
    cout << "Archivo comprimido guardado como 'comprimido.huff'\n";
}

void descomprimirArchivo(const char* nombre) {
    string contenido = leerArchivo(nombre);
    size_t separador = contenido.find("##\n");
    if (separador == string::npos) {
        cerr << "Error: Formato de archivo comprimido incorrecto.\n";
        return;
    }

    string tablaHuffman = contenido.substr(0, separador);
    string contenidoComprimido = contenido.substr(separador + 3);

    unordered_map<string, char> mapaInverso;
    size_t pos = 0;
    while (pos < tablaHuffman.size()) {
        char simbolo = tablaHuffman[pos];
        size_t fin = tablaHuffman.find("\n", pos + 1);
        if (fin == string::npos) break;
        string codigo = tablaHuffman.substr(pos + 1, fin - pos - 1);
        mapaInverso[codigo] = simbolo;
        pos = fin + 1;
    }

    string bits = "";
    for (unsigned char c : contenidoComprimido) {
        bits += bitset<8>(c).to_string();
    }

    string textoOriginal = "";
    string buffer = "";
    for (char bit : bits) {
        buffer += bit;
        if (mapaInverso.count(buffer)) {
            textoOriginal += mapaInverso[buffer];
            buffer = "";
        }
    }

    escribirArchivo("descomprimido.txt", textoOriginal);
    cout << "Archivo descomprimido guardado como 'descomprimido.txt'\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: huffman -c <archivo> para comprimir o -x <archivo> para descomprimir\n";
        return EXIT_FAILURE;
    }

    string opcion = argv[1];
    if (opcion == "-c" && argc == 3) {
        comprimirArchivo(argv[2]);
    } else if (opcion == "-x" && argc == 3) {
        descomprimirArchivo(argv[2]);
    } else {
        cerr << "Opción no válida. Usa -c para comprimir o -x para descomprimir\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}