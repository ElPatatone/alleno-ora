#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Exercise {
    std::string name;
    int set;
    int reps;
    std::string weight;
};

struct Workout {

    std::string date;
    std::vector<Exercise> exercises;
    std::string start_time;
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

    Workout currentWorkout;
    std::string line;

    while (getline(file, line)) {
        // Check for date line
        if (line.find("/") != std::string::npos) {
            currentWorkout.date = line;
        } else if (line.find("@") != std::string::npos) {
            std::istringstream stream{line};
            Exercise exercise;
            char token;
            stream >> exercise.set >> token >> exercise.reps >> token >> exercise.weight;
            currentWorkout.exercises.push_back(exercise);
        }
    }

    for (const Exercise& exercise: currentWorkout.exercises) {
        std::cout << "Set: " << exercise.set << ", Reps: " << exercise.reps << ", Weight: " << exercise.weight << std::endl;
    }

    file.close();

    return 0;
}

