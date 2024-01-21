#include "workout.hpp"
#include "file.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <sstream>
#include <limits>

Workout::Workout() {
}

Workout::~Workout() {
}

const std::vector<Exercise> Workout::getExercisesVector() const {
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

int Workout::getRating() const {
    return rating;
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

void Workout::setRating(const int rating){
    this->rating = rating;
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

std::optional<Workout> Workout::parseWorkoutFile(std::ifstream& workoutFile){
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
                std::cerr << "[Error] [Workout File] Workout duration is in the wrong format. e.g (1h 30m) \n";
                return std::nullopt;
            }
            duration = line.substr(12);
        }

        // Check for location
        else if (line.find("Location") != std::string::npos) {
            location = line.substr(10);
        }
        // Check for rating
        else if (line.find("Rating") != std::string::npos) {
            size_t pos;
            rating = std::stoi(line.substr(8),&pos);

            if (pos != line.substr(8).length()) {
                std::cerr << "[Error] [Workout File] Workout rating is not a valid integer\n";
                return std::nullopt;
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
}

std::optional<Workout> Workout::getUserInput(){
    std::cout << "Please enter the following information:\n" << "Date: ";
    std::cin >> date;
    if (std::cin.fail() || !(File::isDateValid(date))){
        std::cerr << "[Error] Workout date is in the wrong format. e.g (yyyy/mm/dd) \n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::nullopt;
    }

    std::cout << "Start time: ";
    std::cin >> startTime;
    if (std::cin.fail() || !(File::isTimeValid(startTime))) {
        std::cerr << "[Error] Workout time is in the wrong format. e.g (hh:mm) \n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::nullopt;
    }

    std::cout << "Duration: ";
    std::cin >> duration;
    if (std::cin.fail() || !(File::isDurationValid(duration))) {
        // std::cerr << "[Error] Workout duration is in the wrong format. e.g (1h 30m) \n";
        std::cout << "Input Duration: " << duration << std::endl;
        bool isValid = File::isDurationValid(duration);
        std::cout << "Is Valid: " << (isValid ? "true" : "false") << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::nullopt;
    }

    std::cout << "Location: ";
    std::cin >> location;
    
    std::cout << "Rating: ";
    std::cin >> rating;
    if (std::cin.fail()) {
        std::cerr << "[Error] Workout rating is not a valid integer\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::nullopt;
    }
    return *this;
}
