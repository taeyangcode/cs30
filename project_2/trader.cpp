#include <string>

class TradeData {
   private:
    unsigned int m_day;
    int m_stock;
    std::string m_data;

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

std::string* handleTrades(std::string* data, std::size_t dataSize) {
    TradeData* tradeHistory[dataSize];
    std::string* result = new std::string[dataSize];

    int tradeIndex = 0;
    for (int dataIndex = 0, lastPrice = 0, resultIndex = 0; dataIndex < dataSize; ++dataIndex) {
        const std::string& currentData = data[dataIndex];
        const std::string::size_type& firstPipe = currentData.find('|');

        const bool isPriceUpdate = currentData[firstPipe + 1] <= 57;
        const unsigned int day = std::stoi(currentData.substr(0, firstPipe));

        if (isPriceUpdate) {
            const int newPrice = std::stoi(currentData.substr(firstPipe + 1));
            for (int recentTrade = (tradeIndex - 3 <= 0) ? 0 : tradeIndex - 3; recentTrade < tradeIndex; ++recentTrade) {
                const TradeData* currentTrade = tradeHistory[recentTrade];
                if (currentTrade != nullptr && currentTrade->day() >= day - 3 && currentTrade->stock() * (newPrice - lastPrice) >= 500000) {
                    result[resultIndex++] = currentTrade->data();
                    delete tradeHistory[recentTrade];
                    tradeHistory[recentTrade] = nullptr;
                }
            }
            lastPrice = newPrice;
            continue;
        }

        const std::string::size_type& secondPipe = currentData.find('|', firstPipe + 1);
        const std::string::size_type& lastPipe = currentData.find_last_of('|');
        const int tradePrice = std::stoi(currentData.substr(lastPipe + 1)) * (currentData[secondPipe + 1] == 66 ? 1 : -1);
        tradeHistory[tradeIndex++] = new TradeData(day, tradePrice, currentData);
    }

    for (unsigned int index = 0; index < tradeIndex; ++index) {
        if (tradeHistory[index] != nullptr) {
            delete tradeHistory[index];
        }
    }
    return result;
}

int main() {
    return 0;
}