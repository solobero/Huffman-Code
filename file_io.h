#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include "huff.h"

std::string readFile(const char* filename);
void writeCompressedFiles(const std::string& filename, const std::string& compressedContent, HuffmanNode* root);
void writeDecompressedFile(const std::string& filename, const std::string& decompressedContent);
void serializeHuffmanTree(HuffmanNode* root, int fd);
HuffmanNode* deserializeHuffmanTree(int fd);

#endif // FILE_IO_H