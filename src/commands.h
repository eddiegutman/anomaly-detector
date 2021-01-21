

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

// you may add here helper classes
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
};

// you may edit this class
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

// implement here your command classes



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

    void execute() override {

    }
};

class ExitCommand : public Command {
public:
    ExitCommand(DefaultIO *dio, SharedInfo *info) : Command(dio, "6.exit\n", info) {};

    void execute() override {};
};


#endif /* COMMANDS_H_ */