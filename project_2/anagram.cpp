#include <algorithm>
#include <cassert>
#include <csignal>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

const int MAXRESULTS = 20;    // Max matches that can be found
const int MAXDICTWORDS = 10;  // Max words that can be read in

int loadDictionary(std::istream& dictfile, std::vector<std::string>& dict) {
    int wordsLoaded = 0;
    std::string currentWord;
    for (dictfile >> currentWord; !dictfile.eof() && wordsLoaded < MAXDICTWORDS; ++wordsLoaded, dictfile >> currentWord) {
        dict.push_back(currentWord);
    }
    return wordsLoaded;
}

void findPermutations(std::unordered_set<std::string>& permutations, const std::string& current, unsigned int index, unsigned int size) {
    if (index + 1 >= size) {
        permutations.insert(current);
        return;
    }
    for (unsigned int i = index; i < size; ++i) {
        std::string temp = current;
        temp[index] = current[i];
        temp[i] = current[index];
        findPermutations(permutations, temp, index + 1, size);
    }
}

int permute(std::string word, std::vector<std::string>& dict, std::vector<std::string>& results) {
    int matches = 0;
    std::unordered_set<std::string> permutations;
    findPermutations(permutations, word, 0, word.size());

    for (const std::string& word : dict) {
        if (permutations.find(word) != permutations.end()) {
            results[matches++] = word;
            if (matches == MAXRESULTS) {
                return matches;
            }
        }
    }
    return matches;
}

void display(std::vector<std::string>& results) {
    for (auto resultIndex = results.rbegin(); resultIndex != results.rend() && *resultIndex != ""; ++resultIndex) {
        std::cout << "Matching word " << *resultIndex << "\n";
    }
}

void testtwo(int n) {
    vector<string> dictionary;
    vector<string> results(MAXRESULTS);

    switch (n) {
        default: {
            cout << "Bad argument" << endl;
        } break;
        case 1: {
            istringstream
                iss("dog\ncat\nrat\neel\ntar\nart\nlee\nact\ngod\n");
            int numResults = loadDictionary(iss, dictionary);
            sort(dictionary.begin(), dictionary.end());
            assert(numResults == 9 && dictionary[0] == "act" &&
                   dictionary[1] == "art" && dictionary[numResults - 1] == "tar");
        } break;
        case 2: {
            // File is empty, Checks that file is empty and loadDictionary returns 0.
            istringstream iss("");
            int numResults = loadDictionary(iss, dictionary);
            assert(numResults == 0);
        } break;
        case 3: {
            // Input file is larger than the dictionary size
            istringstream
                iss("dog\ncat\nrat\neel\ntar\nart\nlee\nact\ngod\ntoo\nmany\nwords\n");
            int numResults = loadDictionary(iss, dictionary);
            sort(dictionary.begin(), dictionary.end());
            assert(numResults == 10 && dictionary[MAXDICTWORDS -
                                                  1] == "too");
        } break;
        case 4: {
            // If a certain word with repeated letter is shown in results more than once - still accept.
            vector<string> dictionary{"one", "oone", "ne",
                                      "e", "too"};
            int numResults = permute("oto", dictionary,
                                     results);
            assert(numResults == 1 && results[0] == "too");
        } break;
        case 5: {
            // No fraction of a permutation is being searched in dictionary
            vector<string> dictionary{"one", "oone", "ne",
                                      "e", "neoo", "oneo"};
            int numResults = permute("neo", dictionary,
                                     results);
            assert(numResults == 1 && results[0] == "one");
        } break;
        case 6: {
            // Checking that no error occurs if more than MAXRESULTS are found.
            vector<string> dictionary{"true", "treu", "teru",
                                      "teur", "ture", "tuer", "rtue", "rteu", "retu", "reut", "ruet",
                                      "rute", "utre", "uter", "uetr", "uert", "urte", "uret", "etru",
                                      "etur", "ertu", "erut", "eurt", "eutr"};
            // All 24 permutations
            int numResults = permute("true", dictionary,
                                     results);
            assert(numResults == MAXRESULTS);
        } break;
        case 7: {
            // Checking one word was found, no duplicates.
            vector<string> dictionary{"ppp"};
            int numResults = permute("ppp", dictionary,
                                     results);
            assert(numResults == 1 && results[0] == "ppp");
        } break;
        case 8: {
            streambuf* oldCoutStreamBuf = cout.rdbuf();
            ostringstream strCout;
            cout.rdbuf(strCout.rdbuf());
            vector<string> results{"cat", "act"};
            display(results);
            cout.rdbuf(oldCoutStreamBuf);
            string temp = strCout.str();
            bool match1 = temp.find("act") != string::npos;
            bool match2 = temp.find("cat") != string::npos;
            assert(match1 && match2);
        } break;
        case 9: {
            istringstream iss("tier\nrite\nbate\ntire\nttir");
            int numWords = loadDictionary(iss, dictionary);
            sort(dictionary.begin(), dictionary.end());
            assert(numWords == 5 && dictionary[0] == "bate");
            int numResults = permute("tier", dictionary,
                                     results);
            assert(numResults == 3 && (results[2] == "tire" ||
                                       results[2] == "tier" || results[2] == "rite"));
        } break;
        case 10: {
            vector<string> example{"kool", "moe", "dee"};
            int numResults = permute("look", example, results);
            assert(numResults == 1 && results[0] == "kool");
        } break;
    }
}

int main() {
    // for (int i = 1; i <= 10; ++i)
    testtwo(3);
    cout << "Passed";
}