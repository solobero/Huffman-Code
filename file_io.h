#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include "huff.h"

std::string readFile(const char* filename);
void writeCompressedFiles(const std::string& filename, const std::string& compressedContent, HuffmanNode* root);
void writeDecompressedFile(const std::string& filename, const std::string& decompressedContent);
void serializeHuffmanTree(HuffmanNode* root, int fd);
HuffmanNode* deserializeHuffmanTree(int fd);
std::vector<char> bitsToBytes(const std::string& bits);
std::string bytesToBits(const std::vector<char>& bytes);

#endif // FILE_IO_H