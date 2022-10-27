#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <vector>

const int MAXRESULTS = 20;       // Max matches that can be found
const int MAXDICTWORDS = 30000;  // Max words that can be read in

int loadDictionary(std::istream& dictfile, std::vector<std::string>& dict) {
    int wordsLoaded = 0;
    std::string currentWord;

    for (; !dictfile.eof(); ++wordsLoaded) {
        dictfile >> currentWord;
        dict.push_back(currentWord);
    }

    return wordsLoaded;
}

void findPermutations(std::vector<std::string>& permutations, std::string current, unsigned int index, unsigned int size) {
    if (index + 1 >= size) {
        for (std::string s : permutations) {
            if (current == s) {
                return;
            }
        }
        permutations.push_back(current);
        return;
    }
    for (int i = index; i < size; ++i) {
        std::string currentCopy = current;
        currentCopy[index] = current[i];
        currentCopy[i] = current[index];
        findPermutations(permutations, currentCopy, index + 1, size);
    }
}

int permute(std::string word, std::vector<std::string>& dict, std::vector<std::string>& results) {
    int matches = 0;
    std::vector<std::string> permutations;
    findPermutations(permutations, word, 0, word.size());
    for (std::string permutation : permutations) {
        for (auto dictIndex = dict.begin(); dictIndex != dict.end(); ++dictIndex) {
            if (permutation == *dictIndex) {
                results.push_back(permutation);
                ++matches;
                break;
            }
        }
    }
    return matches;
}

void display(std::vector<std::string>& results) {
    for (std::string s : results) {
        if (s != "") {
            std::cout << "Matching word " << s << "\n";
        }
    }
}

int main() {
    std::vector<std::string> results(MAXRESULTS);
    std::vector<std::string> dict(MAXDICTWORDS);
    std::fstream dictfile;  // file containing the list of words
    int nwords;             // number of words read from dictionary
    std::string word;

    dictfile.open("words.txt");
    if (!dictfile) {
        std::cout << "File not found!" << std::endl;
        return (1);
    }

    nwords = loadDictionary(dictfile, dict);

    dictfile.close();

    std::cout << "Please enter a string for an anagram: ";
    std::cin >> word;

    int numMatches = permute(word, dict, results);
    if (!numMatches)
        std::cout << "No matches found" << std::endl;
    else
        display(results);

    return 0;
}
