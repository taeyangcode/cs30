#include <iostream>
#include <string>

class TradeData {
   private:
    const unsigned int m_day;
    const int m_stock;
    const std::string m_data;

   public:
    TradeData(unsigned int day, int stock, std::string data) : m_day(day), m_stock(stock), m_data(data) {
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

void handlePriceChange(TradeData** tradeHistory, const int& tradeIndex, std::string* resultArr, int resultIndex, const unsigned int day, const int priceDif) {
    int index = tradeIndex;
    do {
        const TradeData* currentTrade = tradeHistory[index];
        if (currentTrade == nullptr) {
            index = (index + 1 > 2) ? 0 : index + 1;
            continue;
        }
        if (currentTrade->day() < day - 3) {
            delete tradeHistory[index];
            tradeHistory[index] = nullptr;
        } else if (currentTrade->stock() * priceDif >= 500000) {
            resultArr[resultIndex++] = currentTrade->data();
            delete tradeHistory[index];
            tradeHistory[index] = nullptr;
        }
        index = (index + 1 > 2) ? 0 : index + 1;
    } while (index != tradeIndex);
}

std::string* handleTrades(const std::string* data, const std::size_t dataSize) {
    TradeData** tradeHistory = new TradeData*[3];
    std::string* result = new std::string[dataSize];

    int tradeIndex = 0;
    for (int dataIndex = 0, lastPrice = 0, resultIndex = 0; dataIndex < dataSize; ++dataIndex) {
        const std::string& currentData = data[dataIndex];
        const std::string::size_type& firstPipe = currentData.find('|');
        const unsigned int day = std::stoi(currentData.substr(0, firstPipe));

        if (currentData[firstPipe + 1] <= 57) {
            const int newPrice = std::stoi(currentData.substr(firstPipe + 1));
            handlePriceChange(tradeHistory, tradeIndex, result, resultIndex, day, newPrice - lastPrice);
            lastPrice = newPrice;
            continue;
        }
        const std::string::size_type& secondPipe = currentData.find('|', firstPipe + 1);
        const std::string::size_type& lastPipe = currentData.find_last_of('|');
        const int tradePrice = std::stoi(currentData.substr(lastPipe + 1)) * (currentData[secondPipe + 1] == 66 ? 1 : -1);
        if (tradeHistory[tradeIndex] != nullptr) {
            delete tradeHistory[tradeIndex];
        }
        tradeHistory[tradeIndex] = new TradeData(day, tradePrice, currentData);
        tradeIndex = (tradeIndex + 1 > 2) ? 0 : tradeIndex + 1;
    }

    for (unsigned int index = 0; index < 3; ++index) {
        if (tradeHistory[index] != nullptr) {
            delete tradeHistory[index];
        }
    }
    delete[] tradeHistory;
    return result;
}

int main() {
    const unsigned int size = 6;
    std::string* data = new std::string[size];
    data[0] = "11|25";
    data[1] = "14|Will|BUY|25000";
    data[2] = "15|Will|BUY|25000";
    data[3] = "16|Will|SELL|50000";
    data[4] = "17|Will|SELL|50000";
    data[5] = "18|5";

    const std::string* result = handleTrades(data, size);
    for (unsigned int index = 0; index < size; ++index) {
        std::cout << ((result[index] != "") ? result[index] + " " : "");
    }

    return 0;
}