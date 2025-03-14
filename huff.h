#ifndef HUFF_H
#define HUFF_H

#include <string>
#include <vector>
#include <unordered_map>

struct HuffmanNode {
    char value;
    int repetitions;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char val, int freq);
};

extern std::unordered_map<char, std::string> huffmanTable;

std::vector<HuffmanNode*> createNodeList(const std::string& fileContent);
void sortNodeList(std::vector<HuffmanNode*>& nodeList);
void generateHuffmanCodes(HuffmanNode* root, const std::string& code);
HuffmanNode* buildHuffmanTree(std::vector<HuffmanNode*>& nodeList);
std::string compressText(const std::string& fileContent);
std::string decompressText(const std::vector<char>& compressedBytes, HuffmanNode* root); // Cambiado a std::vector<char>

#endif // HUFF_H