#ifndef FILE_H
#define FILE_H
#include "workout.hpp"
#include <string>
#include <fstream>
#include <optional>

class File {
public:
    File(const std::string& fileName);
    ~File();

    void makeWorkoutFileHeader(Workout& workout);
    static bool isDateValid(const std::string& date);
    static bool isTimeValid(const std::string& time);
    static bool isDurationValid(const std::string& duration);
    std::optional<Workout> getUserInput();

private:
    std::string fileName;
    std::ifstream file;
};

#endif
