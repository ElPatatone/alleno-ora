#ifndef FILE_H
#define FILE_H
#include "workout.hpp"
#include <string>

class File {
public:
    File(const std::string& fileName);
    ~File();

    void makeWorkoutFile();
    bool isDateValid(const std::string& date);
    Workout getUserInput();

private:
    std::string fileName;
};

#endif
