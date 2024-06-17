/*
    g++ -o huffman huffman.cpp 
    ./huffman input.txt  
*/

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <memory>
#include <cstdlib>
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
        wcerr << L"Não foi possível abrir o arquivo: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    while (getline(file, line)) {
        for (wchar_t ch : line) {
            frequencies[ch]++;
        }
    }

    file.close(); 

    wcout << L"Frequências dos caracteres:\n";
    for (auto& entry : frequencies) {
        wcout << entry.first << L": " << entry.second << L"\n";
    }

    return frequencies;
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

    return 0;
}
