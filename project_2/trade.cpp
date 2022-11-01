#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

class TradeData {
   private:
    unsigned int m_day;
    int m_stock;
    std::string m_data;

   public:
    TradeData() {
    }

    TradeData(const unsigned int day, const int stock, const std::string data) : m_day(day), m_stock(stock), m_data(data) {
    }

    unsigned int day() const {
        return this->m_day;
    }

    int stock() const {
        return this->m_stock;
    }

    std::string data() const {
        return this->m_data;
    }
};

std::vector<std::string> findPotentialBadTraders(std::vector<std::string> v) {
    std::unordered_map<unsigned int, TradeData> cache;
    std::map<unsigned int, std::string> m;
    int lastPrice = 0;

    for (const std::string& data : v) {
        const std::string::size_type& firstPipe = data.find('|');
        const unsigned int day = std::stoi(data.substr(0, firstPipe));

        if (data[firstPipe + 1] <= 57) {
            const int newPrice = std::stoi(data.substr(firstPipe + 1));
            for (unsigned int tradeDay = day - 3; tradeDay < day; ++tradeDay) {
                if (cache.find(tradeDay) != cache.end() && cache[tradeDay].stock() * (newPrice - lastPrice) >= 500000) {
                    m[tradeDay] = cache[tradeDay].data();
                    cache.erase(tradeDay);
                }
            }
            lastPrice = newPrice;
            continue;
        }
        const std::string::size_type& secondPipe = data.find('|', firstPipe + 1);
        const std::string::size_type& lastPipe = data.find_last_of('|');
        const int tradePrice = std::stoi(data.substr(lastPipe + 1)) * (data[secondPipe + 1] == 66 ? 1 : -1);
        cache[day] = TradeData(day, tradePrice, data.substr(0, secondPipe));
    }

    std::vector<std::string> result;
    for (auto index = m.begin(); index != m.end(); ++index) {
        result.push_back(index->second);
    }
    return result;
}

int main() {
    std::ifstream in("input.txt");
    std::string str;

    if (!in) {
        std::cerr << "No input file" << std::endl;
        exit(0);
    }

    std::vector<std::string> inputVec;
    while (getline(in, str)) {
        inputVec.push_back(str);
    }
    std::vector<std::string> resV = findPotentialBadTraders(inputVec);
    for (const std::string& r : resV) {
        std::cout << r << std::endl;
    }
    return 0;
}