#include <iostream>
#include "timeseries.h"

int TimeSeries::featureSize() const {
    return Data.size();
}

int TimeSeries::attributesSize() const {
    return Data.begin()->second.size();
}

int TimeSeries::linesSize() const {
    return Data.begin()->second.size();
}

float TimeSeries::getValue(std::string s, int t) const {
    return Data.find(s)->second[t];
}

std::vector<float> TimeSeries::getAttributes(std::string s) const {
    return Data.find(s)->second;
}


void TimeSeries::print() const {
    for (auto &it : Data) {
        for (auto &ip : it.second) {
            std::cout << ip << ' ';
        }
        std::cout << std::endl;
    }
}


