#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Workout {

    std::string date;
    std::string training;
    std::string start_time;
    std::string location;
    int duration;
    int rating;

};

struct ExerciseSet {
    std::string exercise;
    int sets;
    int reps;
    int weight;
    std::string unit;
};

int main (int argc, char *argv[]) {

    std::ifstream file{"example.txt"};

    if (!file) {
        std::cerr << "File could not be opened" << std::endl;
        return 1;
    }

    Workout workout;

    while (file) {
        std::string line;
        std::getline(file, line);
        std::cout << line << "\n";
    }

    return 0;
}

