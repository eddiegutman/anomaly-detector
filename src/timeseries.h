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

    /**
     * creates a new TimeSeries from a given csv file.
     *
     * @param CSVfileName given csv file.
     */

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

    /**
     * returns the number of features in the TimeSeries
     *
     * @return the number of features in the TimeSeries
     */

    int featureSize() const;

    /**
     * returns the number of attributes in the TimeSeries
     *
     * @return the number of attributes in the TimeSeries
     */

    int attributesSize() const;

    /**
     * returns the number of lines in the TimeSeries
     *
     * @return the number of lines in the TimeSeries
     */

    int linesSize() const;

    /**
     * returns the value of feature s in timestamp t
     *
     * @param s the name of the feature
     * @param t given timestamp
     * @return the value of feature s in timestamp t
     */

    float getValue(std::string s, int t) const;

    /**
     * returns the vector of feature s
     *
     * @param s the name of the feature
     * @return the vector of feature s
     */

    std::vector<float> getAttributes(std::string s) const;

    /**
     * prints the TimeSeries
     */
    void print() const;
};


#endif /* TIMESERIES_H_ */