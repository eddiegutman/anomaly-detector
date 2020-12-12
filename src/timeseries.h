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

        // opening the csv file.
        std::ifstream file;
        file.open(CSVfileName);
        std::string line, cell;

        // reading each line from the file into line.
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);

            // if its the first line, its the features names.
            if (init) {

                // split the line by ',' into 'cells'.
                while (std::getline(ss, cell, ',')) {

                    // create a vector for each feature and insert it into the map.
                    std::vector<float> numbers;
                    Data.insert(std::pair<std::string, std::vector<float>>(cell, numbers));
                }
                init = false;
                continue;
            }

            // if its float values.
            auto it = Data.begin();

            // split the line by ',' into 'cells'.
            while (std::getline(ss, cell, ',')) {

                // insert the float value into the correct vector.
                it++->second.push_back(std::stof(cell));
            }
        }
    }

    // returns the number of features in the timeseries
    int featureSize() const;

    // return the number of lines in the timeseries
    int linesSize() const;

    // returns the value of line in feature s
    float getValue(std::string s, int line) const;

    // prints the timeseries
    void print() const;
};


#endif /* TIMESERIES_H_ */