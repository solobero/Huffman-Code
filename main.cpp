#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "huff.h"
#include "file_io.h"

#define BUFFER_SIZE 1024

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

        // Leer el contenido comprimido (bytes binarios)
        std::vector<char> compressedBytes;
        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;
        while ((bytesRead = read(fdHuff, buffer, BUFFER_SIZE)) > 0) {
            compressedBytes.insert(compressedBytes.end(), buffer, buffer + bytesRead);
        }

        close(fdHuff);

        // Generar la tabla de Huffman desde el árbol
        huffmanTable.clear();
        generateHuffmanCodes(root, "");

        // Descomprimir los bytes binarios
        std::string decompressedContent = decompressText(compressedBytes, root);
        writeDecompressedFile("archivo_descomprimido.txt", decompressedContent);

        delete root;

    } else {
        std::cout << "Opción no válida. Usa -h para ayuda.\n";
        return 1;
    }

    return 0;
}