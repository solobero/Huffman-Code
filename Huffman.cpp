#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <bitset>
<<<<<<< HEAD

using namespace std;

struct Node
 {
=======
#include <vector>

using namespace std;

struct Node {
>>>>>>> 56b84f873b8a47271e59b95d53100a2a64ecf6bc
    char simbolo;
    int frecuencia;
    Node *izq, *der;
};

<<<<<<< HEAD
Node* getNode(char simbolo, int frecuencia, Node* izq, Node* der) 
{
    return new Node{simbolo, frecuencia, izq, der};
}

struct comp 
{
=======
Node* getNode(char simbolo, int frecuencia, Node* izq, Node* der) {
    Node* nodo = new Node();
    nodo->simbolo = simbolo;
    nodo->frecuencia = frecuencia;
    nodo->izq = izq;
    nodo->der = der;
    return nodo;
}

struct comp {
>>>>>>> 56b84f873b8a47271e59b95d53100a2a64ecf6bc
    bool operator()(Node* i, Node* d) {
        return i->frecuencia > d->frecuencia;
    }
};

<<<<<<< HEAD
void encode(Node* raiz, string str, unordered_map<char, string>& CodigoHuffman) 
{
    if (!raiz) return;
    if (!raiz->izq && !raiz->der) CodigoHuffman[raiz->simbolo] = str;
=======
void encode(Node* raiz, string str, unordered_map<char, string>& CodigoHuffman) {
    if (!raiz) return;
    if (!raiz->izq && !raiz->der) {
        CodigoHuffman[raiz->simbolo] = str;
    }
>>>>>>> 56b84f873b8a47271e59b95d53100a2a64ecf6bc
    encode(raiz->izq, str + "0", CodigoHuffman);
    encode(raiz->der, str + "1", CodigoHuffman);
}

<<<<<<< HEAD
Node* construirHuffman(const string& texto, unordered_map<char, string>& CodigoHuffman) 
{
    unordered_map<char, int> frecuencia;
    for (char c : texto) frecuencia[c]++;
    priority_queue<Node*, vector<Node*>, comp> pq;
    for (auto par : frecuencia) pq.push(getNode(par.first, par.second, nullptr, nullptr));
=======
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
>>>>>>> 56b84f873b8a47271e59b95d53100a2a64ecf6bc
    while (pq.size() > 1) {
        Node* izq = pq.top(); pq.pop();
        Node* der = pq.top(); pq.pop();
        pq.push(getNode('\0', izq->frecuencia + der->frecuencia, izq, der));
    }
    Node* raiz = pq.top();
    encode(raiz, "", CodigoHuffman);
    return raiz;
}

<<<<<<< HEAD
string leerArchivo(const string& nombre)
 {
    ifstream archivo(nombre, ios::binary);
    return {istreambuf_iterator<char>(archivo), istreambuf_iterator<char>()};
}

void escribirArchivo(const string& nombre, const string& contenido)
 {
    ofstream archivo(nombre, ios::binary);
    archivo << contenido;
}

void guardarArbol(ofstream& out, Node* raiz)
 {
    if (!raiz) {
        out << "# ";
        return;
    }
    out << raiz->simbolo << " " << raiz->frecuencia << " ";
    guardarArbol(out, raiz->izq);
    guardarArbol(out, raiz->der);
}


Node* cargarArbol(ifstream& in) 
{
    string simbolo;
    in >> simbolo;
    if (simbolo == "#") return nullptr;
    int frecuencia;
    in >> frecuencia;
    Node* nodo = getNode(simbolo[0], frecuencia, cargarArbol(in), cargarArbol(in));
    return nodo;
}

void comprimirArchivo(const string& nombre)
 {
    string texto = leerArchivo(nombre);
=======
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
>>>>>>> 56b84f873b8a47271e59b95d53100a2a64ecf6bc
    unordered_map<char, string> CodigoHuffman;
    Node* raiz = construirHuffman(texto, CodigoHuffman);
    ofstream out("comprimido.huff", ios::binary);
    guardarArbol(out, raiz);
    out << " | ";
    for (char c : texto) out << CodigoHuffman[c];
    out.close();
    cout << "Archivo comprimido guardado como 'comprimido.huff'\n";
}

<<<<<<< HEAD
void descomprimirArchivo(const string& nombreComprimido) 
{
    ifstream in(nombreComprimido, ios::binary);
    if (!in) {
        cerr << "Error al abrir el archivo comprimido." << endl;
        return;
=======
    string tablaHuffman = "";
    for (auto& par : CodigoHuffman) {
        tablaHuffman += par.first + par.second + "\n";
>>>>>>> 56b84f873b8a47271e59b95d53100a2a64ecf6bc
    }
    Node* raiz = cargarArbol(in);
    string mensajeCodificado;
    getline(in, mensajeCodificado, '|');
    getline(in, mensajeCodificado);
    in.close();
    if (!raiz) {
        cerr << "Error: Árbol de Huffman no cargado correctamente." << endl;
        return;
    }
    string textoDescomprimido;
    Node* actual = raiz;
    for (char bit : mensajeCodificado) {
        actual = (bit == '0') ? actual->izq : actual->der;
        if (!actual->izq && !actual->der) {
            textoDescomprimido += actual->simbolo;
            actual = raiz;
        }
    }
    escribirArchivo("descomprimido.txt", textoDescomprimido);
    cout << "Archivo descomprimido guardado como 'descomprimido.txt'\n";
}

<<<<<<< HEAD
int main(int argc, char* argv[]) 
{
    int main(int argc, char* argv[]) {
        if (argc < 3) {
            std::cout << "Uso: huffman [opciones] <archivo>\n"
                         "  -c, --compress    : Comprime un archivo\n"
                         "  -x, --decompress  : Descomprime un archivo\n";
            return EXIT_FAILURE;
        }
    
        std::string opcion = argv[1];
        std::string filename = argv[2];
    
        if (opcion == "-c" || opcion == "--compress") {
            std::string fileContent = readFile(filename.c_str());
    
            if (fileContent.empty()) {
                std::cerr << "Error: El archivo está vacío o no se pudo leer.\n";
                return EXIT_FAILURE;
            }
    
            std::vector<HuffmanNode*> nodeList = createNodeList(fileContent);
            sortNodeList(nodeList);
            HuffmanNode* root = buildHuffmanTree(nodeList);
            
            std::cout << "\nÁrbol de Huffman construido correctamente.\n";
            printHuffmanTable();
    
            std::string compressedContent = compressText(fileContent);
            writeCompressedFile("archivo_comprimido.txt", compressedContent);
    
            std::cout << "Compresión exitosa. Archivo guardado como: archivo_comprimido.txt\n";
    
            for (auto node : nodeList) {
                delete node;
            }
    
        } else if (opcion == "-x" || opcion == "--decompress") {
            std::string compressedContent = readFile(filename.c_str());
    
            if (compressedContent.empty()) {
                std::cerr << "Error: No se pudo leer el archivo comprimido.\n";
                return EXIT_FAILURE;
            }
    
            std::unordered_map<std::string, char> reverseHuffmanTable;
            for (const auto& pair : huffmanTable) {
                reverseHuffmanTable[pair.second] = pair.first;
            }
    
            std::string decompressedContent = decompressText(compressedContent, reverseHuffmanTable);
            writeDecompressedFile("archivo_descomprimido.txt", decompressedContent);
    
            std::cout << "Descompresión exitosa. Archivo guardado como: archivo_descomprimido.txt\n";
    
        } else {
            std::cout << "Opción no válida. Usa -h para ayuda.\n";
            return EXIT_FAILURE;
        }
    
        return EXIT_SUCCESS;
    }
}
=======
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
>>>>>>> 56b84f873b8a47271e59b95d53100a2a64ecf6bc
