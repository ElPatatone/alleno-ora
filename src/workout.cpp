#include "workout.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

Workout::Workout() {
}

Workout::~Workout() {
}

const std::vector<Exercise> Workout::getExercisesVector() const {
    return exercisesVector;
};

const std::string& Workout::getDate() const {
    return date;
};

const std::string& Workout::getStartTime() const {
    return startTime;
};

const std::string& Workout::getLocation() const {
    return location;
};

int Workout::getDuration() const {
    return duration;
};

int Workout::getRating() const {
    return rating;
};

std::string Set::getSetType() const {
    switch (setType) {
        case WARM_UP_SETS:
            return "Warm up sets";
        case WORKING_SETS:
            return "Working sets";
        case BACK_OFF_SETS:
            return "Back off sets";
        default:
            return "Unknown";
    }
}
Workout Workout::loadWorkoutData(std::ifstream& workoutFile){
    std::string line;
    Exercise exercise;
    Set set;

    try {
        while (getline(workoutFile, line)) {
            if (line.empty()) {
                continue;
            }
            // Check for date
            else if (line.find("Date") != std::string::npos) {
                date = line.substr(6);
            }
            // Check for start time
            else if (line.find("Start") != std::string::npos) {
                startTime = line.substr(12);
            }
            // Check for duration
            else if (line.find("Duration") != std::string::npos) {
                try {
                    duration = std::stoi(line.substr(10));
                } catch (const std::invalid_argument& e) {
                    throw std::runtime_error("[Workout File] Invalid argument during duration conversion: " + std::string(e.what()));
                } catch (const std::out_of_range& e) {
                    throw std::runtime_error("[Workout File] Out of range error during duration conversion: " + std::string(e.what()));
                }
            }
            // Check for location
            else if (line.find("Location") != std::string::npos) {
                location = line.substr(10);
            }
            // Check for rating
            else if (line.find("Rating") != std::string::npos) {
                try {
                    rating = std::stoi(line.substr(8));
                } catch (const std::invalid_argument& e) {
                    throw std::runtime_error("[Workout File] Invalid argument during rating conversion: " + std::string(e.what()));
                } catch (const std::out_of_range& e) {
                    throw std::runtime_error("[Workout File] Out of range error during rating conversion: " + std::string(e.what()));
                }            }
            // Check for exercise name
            else if (line[0] == '-'){
                if (!exercise.name.empty()) {
                    exercisesVector.push_back(exercise);
                }
                exercise = Exercise();
                exercise.name = line.substr(2);
            }
            else if (line.find("Warm up sets") != std::string::npos) {
                set.setType = WARM_UP_SETS;
            } else if (line.find("Working sets") != std::string::npos) {
                set.setType = WORKING_SETS;
            } else if (line.find("Back off sets") != std::string::npos) {
                set.setType = BACK_OFF_SETS;
            } else if (line.find("@") != std::string::npos) {
                std::istringstream stream{line};
                char token;
                stream >> set.setNumber >> token >> set.repsNumber >> token >> set.weight;
                exercise.setsVector.push_back(set);
            }
        }

        // add last exercise being parsed
        if (!exercise.name.empty()) {
            exercisesVector.push_back(exercise);
        }
        std::cout << "File parsed successfully" << std::endl;
        return *this;
    } catch (const std::exception& e) {
        throw;
    }
}
