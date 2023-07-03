#include <iostream>
#include "sqlite3.h"
#include <curl\curl.h>
#include <conio.h>
#include "json.hpp"
#include <fstream>

sqlite3 *db = nullptr;
char *err = nullptr;
std::ofstream out("currencyData.txt");

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

int sqlCallback(void *data, int argc, char **argv, char **colName) {
    double sumValues = 0, median = 0, average = 0;
    std::vector<double> values;
    int n = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i] != nullptr) {
            values.push_back(std::stod(argv[i]));
            n++;
            sumValues += values[i - 1];
        }
    }
    std::sort(values.begin(), values.end());
    median += values[n / 2] + (n % 2 == 0 ? values[(n - 1) / 2] : 0);
    median /= (n % 2 == 0 ? 2 : 1);
    average += sumValues / n;
    std::string SQLCommand =
            "UPDATE currencyTable SET Average = " + std::to_string(average) + " WHERE CharCode = '" + argv[0] + "';";
    SQLCommand +=
            "UPDATE currencyTable SET Median = " + std::to_string(median) + " WHERE CharCode = '" + argv[0] + "';";
    if (sqlite3_exec(db, SQLCommand.c_str(), nullptr, nullptr, &err)) {
        fprintf(stderr, "Error: %s\n", err);
        sqlite3_free(err);
    }
    out << argv[0] << ":   " << colName[argc - 2] << " = " << average << "   " << colName[argc - 1] << " = "
        << median << '\n';
    return 0;
}

int main() {
    if (sqlite3_open("currencyDBTest.dblite", &db)) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
    std::string SQLCommand = "DROP TABLE IF EXISTS currencyTable; CREATE TABLE currencyTable(CharCode);";
    if (sqlite3_exec(db, SQLCommand.c_str(), nullptr, nullptr, &err)) {
        fprintf(stderr, "Error: %s\n", err);
        sqlite3_free(err);
    }

    int i = 0;
    std::string exitStr;
    while (!_kbhit()) {
        SQLCommand = "ALTER TABLE currencyTable ADD COLUMN Check_" + std::to_string(i + 1) + " DOUBLE;";
        if (sqlite3_exec(db, SQLCommand.c_str(), nullptr, nullptr, &err)) {
            fprintf(stderr, "Error: %s\n", err);
            sqlite3_free(err);
        }
        CURL *curl;
        CURLcode res;
        std::string readBuffer;
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "www.cbr-xml-daily.ru/daily_json.js");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed";
                return 1;
            }
            curl_easy_cleanup(curl);
        }
        nlohmann::json j = nlohmann::json::parse(readBuffer);
        nlohmann::json valute = j["Valute"];
        out << "CURRENTLY ON CYCLE #" << i + 1 << '\n';
        std::cout << "CURRENTLY ON CYCLE #" << i + 1 << '\n';
        for (auto cur : valute) {
            if (i == 0) {
                SQLCommand = "INSERT INTO currencyTable (CharCode, Check_1) VALUES(" +
                             cur["CharCode"].dump() + ", " + cur["Value"].dump() + ");";
            } else {
                SQLCommand = "UPDATE currencyTable SET Check_" + std::to_string(i + 1) + " = " + cur["Value"].dump() +
                             " WHERE CharCode = " + cur["CharCode"].dump() + ";";
            }
            if (sqlite3_exec(db, SQLCommand.c_str(), nullptr, nullptr, &err)) {
                fprintf(stderr, "Error: %s\n", err);
                sqlite3_free(err);
            }
            out << cur["CharCode"].dump() << ":   " << cur["Value"].dump() << '\n';
        }
        Sleep(1000);
        i++;
    }
    out << "---------------------------------------------------------------------------" << '\n';
    SQLCommand = "ALTER TABLE currencyTable ADD COLUMN Average DOUBLE; ALTER TABLE currencyTable ADD COLUMN Median DOUBLE;";
    if (sqlite3_exec(db, SQLCommand.c_str(), nullptr, nullptr, &err)) {
        fprintf(stderr, "Error: %s\n", err);
        sqlite3_free(err);
    }
    SQLCommand = "SELECT * FROM currencyTable;";
    if (sqlite3_exec(db, SQLCommand.c_str(), sqlCallback, nullptr, &err)) {
        fprintf(stderr, "Error: %s\n", err);
        sqlite3_free(err);
    }
    sqlite3_close(db);
    return 0;
}