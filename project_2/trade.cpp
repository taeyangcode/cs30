#include <cassert>
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

void testOne() {
    const unsigned int size = 3;
    std::vector<std::string> data;
    data[0] = "0|20";
    data[1] = "1|Tom|BUY|150000";
    data[2] = "3|25";

    const std::vector<std::string> result = findPotentialBadTraders(data);

    for (unsigned int index = 0; index < size; ++index) {
        if (index == 0) {
            assert(result[index] == "1|Tom|BUY|150000");
            continue;
        }
        assert(result[index] == "");
    }
}

void testTwo() {
    const unsigned int size = 3;
    std::vector<std::string> data;
    data[0] = "3|25";
    data[1] = "8|Kristi|SELL|60000";
    data[2] = "10|15";

    const std::vector<std::string> result = findPotentialBadTraders(data);

    for (unsigned int index = 0; index < size; ++index) {
        if (index == 0) {
            assert(result[index] == "8|Kristi|SELL|60000");
            continue;
        }
        assert(result[index] == "");
    }
}

void testThree() {
    const unsigned int size = 5;
    std::vector<std::string> data;
    data[0] = "11|5";
    data[1] = "14|Will|BUY|10000";
    data[2] = "15|Will|BUY|10000";
    data[3] = "16|Will|BUY|10000";
    data[4] = "17|25";

    const std::vector<std::string> result = findPotentialBadTraders(data);

    for (unsigned int index = 0; index < size; ++index) {
        assert(result[index] == "");
    }
}

void testFour() {
    const unsigned int size = 6;
    std::vector<std::string> data;
    data[0] = "11|25";
    data[1] = "14|Will|SELL|25000";
    data[2] = "15|Will|SELL|25000";
    data[3] = "16|Will|BUY|100000";
    data[4] = "17|Will|SELL|100000";
    data[5] = "18|5";

    const std::vector<std::string> result = findPotentialBadTraders(data);

    for (unsigned int index = 0; index < size; ++index) {
        switch (index) {
            case 0:
                assert(result[index] == "15|Will|SELL|25000");
                continue;
            case 1:
                assert(result[index] == "17|Will|SELL|100000");
                continue;
            default:
                assert(result[index] == "");
        }
    }
}

void testFive() {
    std::vector<std::string> data;
    data[0] = "1|0";
    data[1] = "2|A|BUY|100000";
    data[2] = "3|5000";
    // data[2] = "3|B|BUY|100000";

    const std::vector<std::string> result = findPotentialBadTraders(data);
    assert(result[0] == "2|A|BUY|100000");
}

void test() {
    testOne();
    testTwo();
    testThree();
    testFour();
    testFive();
}

int main() {
    test();
    return 0;
}