#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#define BUFFER_SIZE 1024

// Definición del nodo de Huffman
struct HuffmanNode {
    char value;
    int repetitions;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char val, int freq) {
        value = val;
        repetitions = freq;
        left = nullptr;
        right = nullptr;
    }
};

// Función para crear la lista de nodos
std::vector<HuffmanNode*> createNodeList(const std::string& fileContent) {
    std::unordered_map<char, int> frequencyMap;

    for (char ch : fileContent) {
        frequencyMap[ch]++;
    }

    std::vector<HuffmanNode*> nodeList;
    for (const auto& pair : frequencyMap) {
        nodeList.push_back(new HuffmanNode(pair.first, pair.second));
    }

    return nodeList;
}

// Función para ordenar la lista de nodos por frecuencia
void sortNodeList(std::vector<HuffmanNode*>& nodeList) {
    std::sort(nodeList.begin(), nodeList.end(), [](HuffmanNode* a, HuffmanNode* b) {
        return a->repetitions < b->repetitions;
    });
}

// Tabla de códigos Huffman
std::unordered_map<char, std::string> huffmanTable;

// Función para generar los códigos Huffman
void generateHuffmanCodes(HuffmanNode* root, const std::string& code) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanTable[root->value] = code;
    }
    generateHuffmanCodes(root->left, code + "0");
    generateHuffmanCodes(root->right, code + "1");
}

// Función para construir el Árbol de Huffman
HuffmanNode* buildHuffmanTree(std::vector<HuffmanNode*>& nodeList) {
    while (nodeList.size() > 1) {
        sortNodeList(nodeList);

        HuffmanNode* leftNode = nodeList[0];
        HuffmanNode* rightNode = nodeList[1];

        HuffmanNode* parentNode = new HuffmanNode('\0', leftNode->repetitions + rightNode->repetitions);
        parentNode->left = leftNode;
        parentNode->right = rightNode;

        nodeList.erase(nodeList.begin());
        nodeList.erase(nodeList.begin());

        nodeList.push_back(parentNode);
    }

    HuffmanNode* root = nodeList.front();
    generateHuffmanCodes(root, "");
    return root;
}

// Función para comprimir texto usando Huffman
std::string compressText(const std::string& fileContent) {
    std::string compressedText;
    for (char ch : fileContent) {
        compressedText += huffmanTable[ch];
    }
    return compressedText;
}

// Función para serializar el árbol de Huffman
void serializeHuffmanTree(HuffmanNode* root, int fd) {
    if (!root) {
        return;
    }

    if (!root->left && !root->right) {
        write(fd, "1", 1); // Marcador para nodo hoja
        write(fd, &root->value, sizeof(char));
        write(fd, &root->repetitions, sizeof(int));
    } else {
        write(fd, "0", 1); // Marcador para nodo interno
        serializeHuffmanTree(root->left, fd);
        serializeHuffmanTree(root->right, fd);
    }
}

// Función para deserializar el árbol de Huffman
HuffmanNode* deserializeHuffmanTree(int fd) {
    char marker;
    read(fd, &marker, 1);

    if (marker == '1') {
        char value;
        int repetitions;
        read(fd, &value, sizeof(char));
        read(fd, &repetitions, sizeof(int));
        return new HuffmanNode(value, repetitions);
    } else {
        HuffmanNode* left = deserializeHuffmanTree(fd);
        HuffmanNode* right = deserializeHuffmanTree(fd);
        HuffmanNode* node = new HuffmanNode('\0', left->repetitions + right->repetitions);
        node->left = left;
        node->right = right;
        return node;
    }
}

// Función para leer un archivo
std::string readFile(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return "";
    }

    char buffer[BUFFER_SIZE];
    std::string content;
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {
        content.append(buffer, bytesRead);
    }

    if (bytesRead == -1) {
        perror("Error reading file");
    }

    close(fd);
    return content;
}

// Función para escribir archivos comprimidos
void writeCompressedFiles(const std::string& filename, const std::string& compressedContent, HuffmanNode* root) {
    std::string huffFilename = filename + ".huff";
    std::string txtFilename = filename + "_compressed.txt"; // Archivo para ver el binario en texto

    int fdHuff = open(huffFilename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    int fdTxt = open(txtFilename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fdHuff == -1 || fdTxt == -1) {
        perror("Error al abrir los archivos para escritura");
        return;
    }

    // Serializar el árbol de Huffman en el archivo .huff
    serializeHuffmanTree(root, fdHuff);

    // Escribir el contenido comprimido en el archivo .huff
    write(fdHuff, compressedContent.c_str(), compressedContent.size());

    // Escribir el contenido comprimido en formato de texto en el archivo .txt
    write(fdTxt, compressedContent.c_str(), compressedContent.size());

    std::cout << "Archivo comprimido guardado como: " << huffFilename << "\n";
    std::cout << "Archivo de texto con el binario comprimido guardado como: " << txtFilename << "\n";

    close(fdHuff);
    close(fdTxt);
}

// Función para descomprimir el texto
std::string decompressText(const std::string& compressedContent, HuffmanNode* root) {
    std::unordered_map<std::string, char> reverseHuffmanTable;
    for (const auto& pair : huffmanTable) {
        reverseHuffmanTable[pair.second] = pair.first;
    }

    std::string tempCode, decodedText;
    for (char bit : compressedContent) {
        tempCode += bit;

        if (reverseHuffmanTable.count(tempCode)) {
            decodedText += reverseHuffmanTable[tempCode];
            tempCode.clear();
        }
    }

    return decodedText;
}

// Función para escribir el archivo descomprimido
void writeDecompressedFile(const std::string& filename, const std::string& decompressedContent) {
    int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error al abrir el archivo descomprimido para escritura");
        return;
    }

    write(fd, decompressedContent.c_str(), decompressedContent.size());
    std::cout << "Archivo descomprimido guardado como: " << filename << "\n";

    close(fd);
}

// Función principal
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " [opciones] <archivo>\n"
                  << "  -c, --compress    : Comprime un archivo\n"
                  << "  -x, --decompress  : Descomprime un archivo\n";
        return 1;
    }

    std::string opcion = argv[1];
    std::string filename = argv[2];

    if (opcion == "-c" || opcion == "--compress") {
        std::string fileContent = readFile(filename.c_str());

        if (fileContent.empty()) {
            std::cerr << "Error: El archivo está vacío o no se pudo leer.\n";
            return 1;
        }

        std::vector<HuffmanNode*> nodeList = createNodeList(fileContent);
        HuffmanNode* root = buildHuffmanTree(nodeList);

        std::string compressedContent = compressText(fileContent);
        writeCompressedFiles(filename, compressedContent, root);

        for (auto node : nodeList) {
            delete node;
        }

    } else if (opcion == "-x" || opcion == "--decompress") {
        std::string huffFilename = filename + ".huff";
        int fdHuff = open(huffFilename.c_str(), O_RDONLY);
        if (fdHuff == -1) {
            perror("Error al abrir el archivo .huff para lectura");
            return 1;
        }

        // Deserializar el árbol de Huffman
        HuffmanNode* root = deserializeHuffmanTree(fdHuff);

        // Leer el contenido comprimido
        std::string compressedContent;
        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;
        while ((bytesRead = read(fdHuff, buffer, BUFFER_SIZE)) > 0) {
            compressedContent.append(buffer, bytesRead);
        }

        close(fdHuff);

        // Generar la tabla de Huffman desde el árbol
        huffmanTable.clear();
        generateHuffmanCodes(root, "");

        std::string decompressedContent = decompressText(compressedContent, root);
        writeDecompressedFile("archivo_descomprimido.txt", decompressedContent);

        delete root;

    } else {
        std::cout << "Opción no válida. Usa -h para ayuda.\n";
        return 1;
    }

    return 0;
}