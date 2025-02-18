#include "huff.h"
#include <algorithm>
#include <bitset> // Incluir la biblioteca bitset

std::unordered_map<char, std::string> huffmanTable;

HuffmanNode::HuffmanNode(char val, int freq) {
    value = val;
    repetitions = freq;
    left = nullptr;
    right = nullptr;
}

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

void sortNodeList(std::vector<HuffmanNode*>& nodeList) {
    std::sort(nodeList.begin(), nodeList.end(), [](HuffmanNode* a, HuffmanNode* b) {
        return a->repetitions < b->repetitions;
    });
}

void generateHuffmanCodes(HuffmanNode* root, const std::string& code) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanTable[root->value] = code;
    }
    generateHuffmanCodes(root->left, code + "0");
    generateHuffmanCodes(root->right, code + "1");
}

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

std::string compressText(const std::string& fileContent) {
    std::string compressedText;
    for (char ch : fileContent) {
        compressedText += huffmanTable[ch];
    }
    return compressedText;
}

std::string decompressText(const std::vector<char>& compressedBytes, HuffmanNode* root) {
    // Convertir los bytes binarios en una cadena de bits
    std::string compressedBits;
    for (char byte : compressedBytes) {
        compressedBits += std::bitset<8>(static_cast<unsigned char>(byte)).to_string();
    }

    // Crear la tabla inversa de Huffman
    std::unordered_map<std::string, char> reverseHuffmanTable;
    for (const auto& pair : huffmanTable) {
        reverseHuffmanTable[pair.second] = pair.first;
    }

    // Descomprimir la cadena de bits
    std::string tempCode, decodedText;
    for (char bit : compressedBits) {
        tempCode += bit;

        if (reverseHuffmanTable.count(tempCode)) {
            decodedText += reverseHuffmanTable[tempCode];
            tempCode.clear();
        }
    }

    return decodedText;
}