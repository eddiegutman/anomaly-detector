#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <fstream>
#include <sstream>

class TimeSeries {

public:
    std::vector<std::vector<std::string>> Lines;

    TimeSeries(const char *CSVfileName) {
        std::ifstream file;
        file.open(CSVfileName);
        std::string line, cell;
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            while (std::getline(ss, cell, ',')){
                row.push_back(cell);
            }
            Lines.push_back(row);
        }
    }

    void print();
};


#endif /* TIMESERIES_H_ */