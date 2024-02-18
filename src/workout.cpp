#include "../include/workout.hpp"
#include "../include/file.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <sstream>
#include <limits>

const std::vector<Exercise>& Workout::getExercisesVector() const {
    return exercisesVector;
}

const std::string& Workout::getDate() const {
    return date;
}

const std::string& Workout::getStartTime() const {
    return startTime;
}

const std::string& Workout::getLocation() const {
    return location;
}

const std::string& Workout::getDuration() const {
    return duration;
}

const std::string& Workout::getNotes() const {
    return notes;
}

int Workout::getWorkoutRating() const {
    return workoutRating;
}

int Workout::getPhysicalRating() const {
    return physicalRating;
}

int Workout::getMentalRating() const {
    return mentalRating;
}

void Workout::setDate(const std::string& date){
    this->date = date;
}

void Workout::setStartTime(const std::string& startTime){
    this->startTime = startTime;
}

void Workout::setLocation(const std::string& location){
    this->location = location;
}

void Workout::setDuration(const std::string& duration){
    this->duration = duration;
}

void Workout::setWorkoutRating(const int workoutRating){
    this->workoutRating = workoutRating;
}

void Workout::setPhysicalRating(const int physicalRating){
    this->physicalRating = physicalRating;
}

void Workout::setMentalRating(const int mentalRating){
    this->mentalRating = mentalRating;
}

const std::vector<Exercise>& Workout::addExercisesToVector(const Exercise& newExercise) {
    this->exercisesVector.push_back(newExercise);
    return exercisesVector;
}

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

SetType Set::convertStringToSetType(const std::string& setType) {
    if (setType == "Warm up sets") {
        return WARM_UP_SETS;
    } else if (setType == "Working sets") {
        return WORKING_SETS;
    } else if (setType == "Back off sets") {
        return BACK_OFF_SETS;
    } else {
        // Default to UNKNOWN_SETS if the string is not recognized
        std::cerr << "[Warning] Unknown set type: " << setType << "\n";
        return UNKNOWN_SETS;
    }
}

void Exercise::addSet(int setNumber, int repsNumber, int weight, const std::string& setType, bool isPR) {
    Set set;
    set.setNumber = setNumber;
    set.repsNumber = repsNumber;
    set.weight = weight;
    set.setType = set.convertStringToSetType(setType);
    set.isPR = isPR;
    setsVector.push_back(set);
}

// Parse the workout file for the data and then return the workout instance with those values.
std::optional<Workout> Workout::parseWorkoutData(std::ifstream& workoutFile){
    std::string line;
    Exercise exercise;
    Set set;

    while (getline(workoutFile, line)) {
        if (line.empty()) {
            continue;
        }
        // Check for date
        else if (line.find("Date") != std::string::npos) {
            if (!(File::isDateValid(line.substr(6)))) {
                std::cerr << "[Error] [Workout File] Workout date is in the wrong format. e.g (yyyy/mm/dd) \n";
                return std::nullopt;
            }
            date = line.substr(6);
        }
        // Check for start time
        else if (line.find("Start") != std::string::npos) {
            if (!(File::isTimeValid(line.substr(12)))) {
                std::cerr << "[Error] [Workout File] Workout start time is in the wrong format. e.g (hh:mm) \n";
                return std::nullopt;
            }
            startTime = line.substr(12);
        }
        // Check for duration
        else if (line.find("Duration") != std::string::npos) {
            if (!(File::isDurationValid(line.substr(10)))) {
                std::cerr << "[Error] [Workout File] Workout duration is in the wrong format. e.g (1h30m) \n";
                return std::nullopt;
            }
            duration = line.substr(10);
        }

        // Check for location
        else if (line.find("Location") != std::string::npos) {
            location = line.substr(10);
        }
        // Check for rating
        else if (line.find("Workout Rating") != std::string::npos) {
            size_t pos;
            workoutRating = std::stoi(line.substr(16), &pos);

            if (pos != line.substr(16).length()) {
                std::cerr << "[Error] [Workout File] Workout rating is not a valid integer\n";
                return std::nullopt;
            } else if (workoutRating < 1 || workoutRating > 5) {
                std::cerr << "[Error] Workout rating is not in the range of 1 to 5\n";
                return std::nullopt;
            }
        }

        // Check for rating
        else if (line.find("Physical Rating") != std::string::npos) {
            size_t pos;
            physicalRating = std::stoi(line.substr(17), &pos);

            if (pos != line.substr(17).length()) {
                std::cerr << "[Error] [Workout File] Physical rating is not a valid integer\n";
                return std::nullopt;
            } else if (physicalRating < 1 || physicalRating > 5) {
                std::cerr << "[Error] Physical rating is not in the range of 1 to 5\n";
                return std::nullopt;
            }
        }

        // Check for rating
        else if (line.find("Mental Rating") != std::string::npos) {
            size_t pos;
            mentalRating = std::stoi(line.substr(15), &pos);

            if (pos != line.substr(15).length()) {
                std::cerr << "[Error] [Workout File] Mental rating is not a valid integer\n";
                return std::nullopt;
            } else if (mentalRating < 1 || mentalRating > 5) {
                std::cerr << "[Error] Mental rating is not in the range of 1 to 5\n";
                return std::nullopt;
            }
        }

        // Check for notes
        else if (line.find("Notes start") != std::string::npos) {
            while (std::getline(workoutFile, line)) {
                if (line.find("Notes end") != std::string::npos) {
                    break;
                }
                notes += line + "\n";
            }
        }

        // Check for exercise name
        else if (line[0] == '-'){
            if (!exercise.name.empty()) {
                exercisesVector.push_back(exercise);
            }
            exercise = Exercise();
            exercise.name = line.substr(2);
        }
        // Check for set type.
        else if (line.find("Warm up sets") != std::string::npos) {
            set.setType = WARM_UP_SETS;
        } else if (line.find("Working sets") != std::string::npos) {
            set.setType = WORKING_SETS;
        } else if (line.find("Back off sets") != std::string::npos) {
            set.setType = BACK_OFF_SETS;
        } else if (line.find("@") != std::string::npos) {
            // Check for set information. number of sets, reps and weight done
            std::istringstream stream{line};
            char token;
            stream >> set.setNumber >> token >> set.repsNumber >> token >> set.weight;

            // Check if it is a PR. If it is found it will return true if not false.
            set.isPR = (line.find("(PR)") != std::string::npos);

            exercise.addSet(set.setNumber, set.repsNumber, set.weight, set.getSetType(), set.isPR);
        }
    }

    // add last exercise being parsed
    if (!exercise.name.empty()) {
        exercisesVector.push_back(exercise);
    }
    workoutFile.close();
    std::cout << "File parsed successfully" << std::endl;
    return *this;
}

// get user input for the header of the file, then return the workout instance with those values.
std::optional<Workout> Workout::getUserInput(){
    std::cout << "Please enter the following information:\n" << "Date: ";
    std::cin >> date;
    if (std::cin.fail() || !File::isDateValid(date)){
        std::cerr << "[Error] Workout date is in the wrong format. e.g (yyyy/mm/dd) \n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::nullopt;
    }

    std::cout << "Start time: ";
    std::cin >> startTime;
    if (std::cin.fail() || !File::isTimeValid(startTime)) {
        std::cerr << "[Error] Workout time is in the wrong format. e.g (hh:mm) \n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::nullopt;
    }

    std::cout << "Duration: ";
    std::cin >> duration;
    if (std::cin.fail() || !File::isDurationValid(duration)) {
        std::cerr << "[Error] Workout duration is in the wrong format. e.g (1h30m) \n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::nullopt;
    }

    std::cout << "Location: ";
    std::cin >> location;

    std::cout << "Workout Rating: ";
    std::cin >> workoutRating;
    if (std::cin.fail()) {
        std::cerr << "[Error] Workout rating is not a valid integer\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::nullopt;
    } else if (workoutRating < 1 || workoutRating > 5) {
            std::cerr << "[Error] Workout rating is not in the range of 1 to 5\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return std::nullopt;
        }
    return *this;
}

