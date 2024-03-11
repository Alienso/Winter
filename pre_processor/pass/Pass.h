//
// Created by Alienson on 18.2.2024..
//

#ifndef WINTER_PASS_H
#define WINTER_PASS_H

#include <iostream>

//TODO move passes to source folder since they are not part of the library

class Pass {
public:
    virtual void begin(std::string& fileName) = 0;
    virtual void process(std::ifstream &inputFile, std::ofstream &outputFile, std::string& line, std::string& previousLine) = 0;
    virtual void end(std::ifstream &inputFile, std::ofstream &outputFile, std::string& fileName) = 0;
    virtual void processingFinished() = 0;

    [[nodiscard]] virtual bool shouldProcess(std::string &fileName) const = 0 ;
};


#endif //WINTER_PASS_H
