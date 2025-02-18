#include "file_io.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <bitset>
#include <vector>

#define BUFFER_SIZE 1024

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

void writeCompressedFiles(const std::string& filename, const std::string& compressedContent, HuffmanNode* root) {
    std::string huffFilename = filename + ".huff";

    int fdHuff = open(huffFilename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fdHuff == -1) {
        perror("Error al abrir el archivo .huff para escritura");
        return;
    }

    // Serializar el árbol de Huffman
    serializeHuffmanTree(root, fdHuff);

    // Convertir la cadena de bits en bytes binarios
    std::vector<char> bytes = bitsToBytes(compressedContent);

    // Escribir los bytes binarios en el archivo .huff
    write(fdHuff, bytes.data(), bytes.size());

    std::cout << "Archivo comprimido guardado como: " << huffFilename << "\n";

    close(fdHuff);
}

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

void serializeHuffmanTree(HuffmanNode* root, int fd) {
    if (!root) {
        return;
    }

    if (!root->left && !root->right) {
        write(fd, "1", 1);
        write(fd, &root->value, sizeof(char));
        write(fd, &root->repetitions, sizeof(int));
    } else {
        write(fd, "0", 1);
        serializeHuffmanTree(root->left, fd);
        serializeHuffmanTree(root->right, fd);
    }
}

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

std::vector<char> bitsToBytes(const std::string& bits) {
    std::vector<char> bytes;
    for (size_t i = 0; i < bits.size(); i += 8) {
        std::string byteStr = bits.substr(i, 8); // Toma 8 bits
        if (byteStr.size() < 8) {
            byteStr.append(8 - byteStr.size(), '0'); // Rellena con ceros si es necesario
        }
        char byte = static_cast<char>(std::bitset<8>(byteStr).to_ulong());
        bytes.push_back(byte);
    }
    return bytes;
}

std::string bytesToBits(const std::vector<char>& bytes) {
    std::string bits;
    for (char byte : bytes) {
        bits += std::bitset<8>(byte).to_string();
    }
    return bits;
}