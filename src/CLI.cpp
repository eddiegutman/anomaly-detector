#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    auto *info = new SharedInfo();
    menu.push_back(new UploadCsvCommand(dio, info));
    menu.push_back(new ChangeThresholdCommand(dio, info));
    menu.push_back(new AnomalyDetectionCommand(dio, info));
    menu.push_back(new DisplayResultsCommand(dio, info));
    menu.push_back(new UploadAnomalyAndAnalyzeCommand(dio, info));
    menu.push_back(new ExitCommand(dio, info));
}

CLI::~CLI()= default;

void CLI::start(){
    int input;
    do {
        dio->write("Welcome to The Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for(auto & c : menu){
            dio->write(c->description);
        }
        input = stoi(dio->read()) - 1;
        menu[input]->execute();
    } while (input != 5);
}



