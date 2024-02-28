#pragma once
#include "workout.hpp"
#include <string>
#include <fstream>
#include <optional>
#include <vector>

class File {
public:
    File(const std::string& fileName);
    ~File() = default;

    void makeWorkoutFileHeader(Workout& workout);
    void makeFetchedWorkoutFile(const Workout& workout);
    void makeFetchedExerciseFile(const std::vector<Workout> workouts);

    // made this static to allow usage without needing an instance of the File class. Should maybe move this.
    static bool isDateValid(const std::string& date);
    static bool isTimeValid(const std::string& time);
    static bool isDurationValid(const std::string& duration);

    std::optional<Workout> getUserInput();

private:
    std::string fileName;
    std::ifstream file;
};
