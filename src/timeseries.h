#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

class TimeSeries {

public:
    std::map<std::string, std::vector<float>> Data;

    TimeSeries(const char *CSVfileName) {
        bool init = true;
        std::ifstream file;
        file.open(CSVfileName);
        std::string line, cell;
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            if (init) {
                while (std::getline(ss, cell, ',')) {
                    std::vector<float> numbers;
                    Data.insert(std::pair<std::string, std::vector<float>>(cell, numbers));
                }
                init = false;
                continue;
            }
            auto it = Data.begin();
            while (std::getline(ss, cell, ',')) {
                it++->second.push_back(std::stof(cell));
            }
        }
    }

    int featureSize() const;
    int linesSize() const;
    float getValue(std::string s, int line) const;

    void print();
};


#endif /* TIMESERIES_H_ */