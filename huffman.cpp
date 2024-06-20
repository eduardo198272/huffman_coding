/*
    g++ -o huffman huffman.cpp 
    ./huffman input.txt  

    Eduardo Steffens Hoppen - 198272
    Leonardo Cologneze Dias - 178873
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cstring> 

using namespace std;

struct HuffmanNode {
    wchar_t symbol;
    int frequency;
    shared_ptr<HuffmanNode> left;
    shared_ptr<HuffmanNode> right;

    HuffmanNode(wchar_t s, int f) : symbol(s), frequency(f), left(nullptr), right(nullptr) {}
};

struct CompareNode {
    bool operator()(shared_ptr<HuffmanNode> const& n1, shared_ptr<HuffmanNode> const& n2) {
        return n1->frequency > n2->frequency;
    }
};

unordered_map<wchar_t, int> analyzeText(const wstring& filename) {
    unordered_map<wchar_t, int> frequencies;
    wifstream file(filename.c_str()); 
    wstring line;

    if (!file.is_open()) {
        wcerr << L"Nao foi possivel abrir o arquivo: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    while (getline(file, line)) {
        for (wchar_t ch : line) {
            frequencies[ch]++;
        }
    }

    return frequencies;
}

shared_ptr<HuffmanNode> buildHuffmanTree(const unordered_map<wchar_t, int>& frequencies) {
    priority_queue<shared_ptr<HuffmanNode>, vector<shared_ptr<HuffmanNode>>, CompareNode> minHeap;

    for (auto pair : frequencies) {
        minHeap.push(make_shared<HuffmanNode>(pair.first, pair.second));
    }

    while (minHeap.size() != 1) {
        auto left = minHeap.top(); minHeap.pop();
        auto right = minHeap.top(); minHeap.pop();

        auto sum = make_shared<HuffmanNode>(L'\0', left->frequency + right->frequency);
        sum->left = left;
        sum->right = right;

        minHeap.push(sum);
    }

    return minHeap.top();
}

void generateCodes(shared_ptr<HuffmanNode> root, wstring str, unordered_map<wchar_t, wstring>& huffmanCode) {
    if (root == nullptr)
        return;

    if (!root->left && !root->right) {
        huffmanCode[root->symbol] = str;
    }

    generateCodes(root->left, str + L"0", huffmanCode);
    generateCodes(root->right, str + L"1", huffmanCode);
}

void printHuffmanTree(shared_ptr<HuffmanNode> root, wstring indent = L"", bool last = true) {
    if (root != nullptr) {
        wcout << indent;
        if (last) {
            wcout << L"\\-";
            indent += L"  ";
        } else {
            wcout << L"+-";
            indent += L"| ";
        }

        wcout << root->symbol << L" (" << root->frequency << L")" << endl;

        printHuffmanTree(root->left, indent, false);
        printHuffmanTree(root->right, indent, true);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        wcerr << L"Uso: " << argv[0] << L" <nome_do_arquivo>\n";
        return 1;
    }

    size_t size = strlen(argv[1]) + 1;
    wchar_t* wfilename = new wchar_t[size];
    mbstowcs(wfilename, argv[1], size);
    wstring filename = wfilename;
    delete[] wfilename;

    auto frequencies = analyzeText(filename);

    vector<pair<wchar_t, int>> sortedChars(frequencies.begin(), frequencies.end());
    sort(sortedChars.begin(), sortedChars.end(), [](const pair<wchar_t, int>& a, const pair<wchar_t, int>& b) {
        return a.second > b.second;
    });

    wcout << endl << "|------------------------------------------------------------------|" << endl;
    wcout << L"\nCaracteres ordenados por frequencia:\n";
    wcout << endl << "|------------------------------------------------------------------|" << endl << endl;

    for (const auto& pair : sortedChars) {
        wcout << pair.first << L" (" << pair.second << L")\n";
    }

    auto root = buildHuffmanTree(frequencies);

    unordered_map<wchar_t, wstring> huffmanCode;
    generateCodes(root, L"", huffmanCode);

    wcout << endl << "|------------------------------------------------------------------|" << endl;
    wcout << L"\nCodigos Huffman:\n" << endl;
    wcout << "|------------------------------------------------------------------|" << endl << endl;
    for (auto pair : huffmanCode) {
        wcout << pair.first << L" " << pair.second << L"\n";
    }

    wcout << endl << "|------------------------------------------------------------------|" << endl;
    wcout << L"\nArvore de Huffman:\n" << endl;
    wcout << "|------------------------------------------------------------------|" << endl << endl;    
    printHuffmanTree(root);

    int originalSize = 0, compressedSize = 0;
    for (auto pair : frequencies) {
        originalSize += pair.second * sizeof(wchar_t);
        compressedSize += pair.second * huffmanCode[pair.first].length();
    }

    wcout << "|------------------------------------------------------------------|" << endl;
    wcout << L"\nTamanho do arquivo original: " << originalSize << L" bytes\n" << endl;
    wcout << L"Tamanho do arquivo compactado: " << compressedSize / 8 << L" bytes\n" << endl;
    wcout << L"Reducao: " << ((originalSize - (compressedSize / 8.0)) / originalSize) * 100 << L"%\n" << endl;
    wcout << "|------------------------------------------------------------------|" << endl << endl;

    return 0;
}

