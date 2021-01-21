

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <utility>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    // you may add additional methods here
};

class StandartIO : public DefaultIO {
    string read() override {
        string input;
        cin >> input;
        return input;
    }

    void write(string text) override {
        cout << text;
    }

    void write(float f) override {
        cout << f;
    }

    void read(float *f) override {
        cin >> *f;
    }
};

class SharedInfo {
    TimeSeries *train;
    TimeSeries *test;
    HybridAnomalyDetector *detector;
    vector<AnomalyReport> *reports;

public:

    void setTrainTimeSeries(const char *file) {
        train = new TimeSeries(file);
    }

    void setTestTimeSeries(const char *file) {
        test = new TimeSeries(file);
    }

    void setDetector() {
        detector = new HybridAnomalyDetector();
        detector->learnNormal(*train);
    }

    void setReports() {
        reports = new vector<AnomalyReport>;
        for (const auto &r : detector->detect(*test))
            reports->push_back(r);
    }

    HybridAnomalyDetector* getDetector() {
        return detector;
    }

    vector<AnomalyReport>* getReports() {
        return reports;
    }

    unsigned long getTimeStamps() {
        return test->attributesSize();
    }
};


class Command {
public:
    const string description;
protected:
    DefaultIO *dio;
    SharedInfo *info;
public:
    Command(DefaultIO *dio, string desc, SharedInfo *info) : dio(dio), description(std::move(desc)), info(info) {};

    virtual void execute() = 0;

    virtual ~Command() = default;
};

class UploadCsvCommand : public Command {
public:
    explicit UploadCsvCommand(DefaultIO *dio, SharedInfo *info) : Command(dio, "1.upload a time series csv file\n",
                                                                          info) {};

    virtual void execute() {
        dio->write("Please upload your local train CSV file.\n");
        ofstream file;
        file.open("anomalyTrain.csv");
        string line;
        while ((line = dio->read()) != "done") {
            file << line << endl;
        }
        file.close();
        dio->write("Upload complete.\n");

        dio->write("Please upload your local test CSV file.\n");
        file.open("anomalyTest.csv");
        while ((line = dio->read()) != "done") {
            file << line << endl;
        }
        file.close();
        dio->write("Upload complete.\n");

        info->setTrainTimeSeries("anomalyTrain.csv");
        info->setTestTimeSeries("anomalyTest.csv");
        info->setDetector();
    }
};

class ChangeThresholdCommand : public Command {
public:
    ChangeThresholdCommand(DefaultIO *dio, SharedInfo *info) : Command(dio, "2.algorithm settings\n", info) {};

    void execute() override {
        dio->write("The current correlation threshold is ");
        dio->write(info->getDetector()->getCorrelationThreshold());
        dio->write("\n");
        dio->write("Type a new threshold\n");
        float newThreshold;
        dio->read(&newThreshold);
        while (newThreshold > 1 || newThreshold < 0) {
            dio->write("please choose a value between 0 and 1.\n");
            dio->write("Type a new threshold\n");
            dio->read(&newThreshold);
        }
        info->getDetector()->setCorrelationThreshold(newThreshold);
    }
};

class AnomalyDetectionCommand : public Command {
public:
    AnomalyDetectionCommand(DefaultIO *dio, SharedInfo *info) : Command(dio, "3.detect anomalies\n", info) {};

    void execute() override {
        info->setReports();
        dio->write("anomaly detection complete.\n");
    }
};

class DisplayResultsCommand : public Command {
public:
    DisplayResultsCommand(DefaultIO *dio, SharedInfo *info) : Command(dio, "4.display results\n", info) {};

    void execute() override {
        for (const auto &r : *info->getReports()) {
            dio->write(r.timeStep);
            dio->write("\t");
            dio->write(r.description + "\n");
        }
        dio->write("Done.\n");
    }
};

class UploadAnomalyAndAnalyzeCommand : public Command {
public:
    UploadAnomalyAndAnalyzeCommand(DefaultIO *dio, SharedInfo *info) :
    Command(dio,"5.upload anomalies and analyze results\n", info){};

    static vector<pair<long, long>> compactReports(vector<AnomalyReport> *ar) {
        vector<pair<long, long>> reports;
        auto &r = *ar->begin();
        long start = r.timeStep;
        long finish = r.timeStep;
        string desc = r.description;

        for (auto &r : *ar) {
            if (r.timeStep == ar->size()) {
                finish = r.timeStep;
                reports.emplace_back(start, finish);
            }
            if (r.description != desc) {
                reports.emplace_back(start, finish);
                start = r.timeStep;
                finish = r.timeStep;
                desc = r.description;
            } else {
                finish = r.timeStep;
            }
        }
        return reports;
    }

    static bool areIntersecting(pair<long, long> a, pair<long, long> b) {
        if ((a.first < b.first && a.second < b.first) || (b.first < a.first && b.second < a.first))
            return false;
        if (a.second == b.first || b.first == a.second)
            return false;
        return true;
    }

    void execute() override {
        string line, cell1, cell2;
        vector<pair<long, long>> userReports, compactAnomalyReports;

        dio->write("Please upload your local anomalies file.\n");
        while ((line = dio->read()) != "done") {
            stringstream ss(line);
            getline(ss, cell1, ',');
            getline(ss, cell2, ',');
            userReports.emplace_back(stoi(cell1), stoi(cell2));
        }
        dio->write("Upload complete.\n");


        compactAnomalyReports = compactReports(info->getReports());
        unsigned long positive = userReports.size();
        unsigned long negative = info->getTimeStamps() -1;

        for (auto ur : userReports)
            negative -= (ur.second - ur.first + 1);

        unsigned long falsePositive = 0, truePositive = 0;
        bool found = false;
        for (auto ar : compactAnomalyReports) {
            for (auto ur : userReports) {
                if (areIntersecting(ar, ur)) {
                    truePositive++;
                    found = true;
                }
            }

            if (!found)
                falsePositive++;
            found = false;
        }

        float tpRate, fpRate;
        tpRate = (float)truePositive / (float)positive;
        tpRate = floorf(tpRate * 1000) / 1000;
        fpRate = (float)falsePositive / (float)negative;
        fpRate = floorf(fpRate * 1000) / 1000;

        dio->write("True Positive Rate: ");
        dio->write((float)tpRate);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write((float)fpRate);
        dio->write("\n");
    }
};

class ExitCommand : public Command {
public:
    ExitCommand(DefaultIO *dio, SharedInfo *info) : Command(dio, "6.exit\n", info) {};

    void execute() override {};
};


#endif /* COMMANDS_H_ */