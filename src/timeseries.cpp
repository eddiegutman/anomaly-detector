#include <iostream>
#include "timeseries.h"

// returns the number of features in the timeseries
int TimeSeries::featureSize() const {
    return Data.size();
}

// return the number of lines in the timeseries
int TimeSeries::linesSize() const {
    return Data.begin()->second.size();
}

// returns the value of line in feature s
float TimeSeries::getValue(std::string s, int line) const {
    return Data.find(s)->second[line];
}

// prints the timeseriess
void TimeSeries::print() const {
    for (auto &it : Data) {
        for (auto &ip : it.second) {
            std::cout << ip << ' ';
        }
        std::cout << std::endl;
    }
}

