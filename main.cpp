#include <iostream>
#include <fcntl.h>    // Para open, O_RDONLY, etc.
#include <unistd.h>   // Para read, close, etc.
#include <sys/stat.h> // Para mode_t (opcional, dependiendo de tu sistema)
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "huff.h"
#include "file_io.h"
#define BUFFER_SIZE 1024 // Definir BUFFER_SIZE si no está definido


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