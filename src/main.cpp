#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Set {
    std::string setType;
    int setNumber;
    int repsNumber;
    int weight;
};

struct Exercise {
    std::string name;
    std::vector<Set> setsVector;
};

struct Workout {
    std::vector<Exercise> exercisesVector;
    std::string date;
    std::string startTime;
    std::string location;
    int duration;
    int rating;
};

int main (int argc, char *argv[]) {

    std::ifstream file{"example.txt"};

    if (!file.is_open()) {
        std::cerr << "File could not be opened" << std::endl;
        return 1;
    }

    std::string line;

    Workout workout;
    Exercise exercise;
    Set set;
    std::string currentSetType;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        // Check for date
        else if (line.find("Date") != std::string::npos) {
            workout.date = line.substr(6);
        }
        // Check for start time
        else if (line.find("Start") != std::string::npos) {
            workout.startTime = line.substr(12);
        }
        // Check for duration
        else if (line.find("Duration") != std::string::npos) {
            workout.duration = std::stoi(line.substr(10));
        }
        // Check for location
        else if (line.find("Location") != std::string::npos) {
            workout.location = line.substr(10);
        }
        // Check for rating
        else if (line.find("Rating") != std::string::npos) {
            workout.rating = std::stoi(line.substr(8));
        }
        // Check for exercise name
        else if (line[0] == '-'){
            if (!exercise.name.empty()) {
                workout.exercisesVector.push_back(exercise);
            }
            exercise = Exercise();
            exercise.name = line.substr(2);
        }
        else if (line.find("Warm up sets") != std::string::npos) {
            currentSetType = "Warm up sets";
        } else if (line.find("Working sets") != std::string::npos) {
            currentSetType = "Working sets";
        } else if (line.find("Back off sets") != std::string::npos) {
            currentSetType = "Back off sets";
        } else if (line.find("@") != std::string::npos) {
            std::istringstream stream{line};
            char token;
            stream >> set.setNumber >> token >> set.repsNumber >> token >> set.weight;
            set.setType = currentSetType;
            exercise.setsVector.push_back(set);
        }}

    // add last exercise being parsed
    if (!exercise.name.empty()) {
        workout.exercisesVector.push_back(exercise);
    }

    file.close();

    std::cout << workout.date << std::endl;
    std::cout << workout.startTime << std::endl;
    std::cout << workout.duration << std::endl;
    std::cout << workout.location << std::endl;
    std::cout << workout.rating << std::endl;

    for (const auto& exercise : workout.exercisesVector) {
        std::cout << exercise.name << std::endl;
        std::string printedSetType = "";  // Variable to track printed set type for the current exercise
        for (const auto& set : exercise.setsVector) {
            if (set.setType != printedSetType) {
                std::cout << set.setType << std::endl;
                printedSetType = set.setType;
            }
            std::cout << set.setNumber << " " << set.repsNumber << " " << set.weight << std::endl;
        }
    }
    return 0;
}
