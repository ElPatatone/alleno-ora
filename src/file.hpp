#ifndef FILE_H
#define FILE_H
#include "workout.hpp"
#include <string>

class File {
public:
    File(const std::string& fileName);
    ~File();

    void workoutFileMaker();
    bool isValidDate(const std::string& date);
    Workout getUserInput();

private:
    std::string fileName;
};

#endif
