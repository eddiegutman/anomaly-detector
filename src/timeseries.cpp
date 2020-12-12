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

