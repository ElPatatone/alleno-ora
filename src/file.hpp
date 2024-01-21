#ifndef FILE_H
#define FILE_H
#include "workout.hpp"
#include <string>
#include <fstream>

class File {
public:
    File(const std::string& fileName);
    ~File();

    void makeWorkoutFile();
    static bool isDateValid(const std::string& date);
    static bool isTimeValid(const std::string& time);
    Workout getUserInput();

private:
    std::string fileName;
    std::ifstream file;
};

#endif
