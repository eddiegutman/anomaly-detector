#include <iostream>
#include "timeseries.h"

void TimeSeries::print() {
    for (auto & it : Data) {
        for (auto & ip : it.second) {
            std::cout << ip << ' ';
        }
        std::cout << std::endl;
    }
}

int TimeSeries::featureSize() const{
    return Data.size();
}

int TimeSeries::linesSize() const{
    return Data.begin()->second.size();
}

float TimeSeries::getValue(std::string s, int line) const {
    return Data.find(s)->second[line];
}

