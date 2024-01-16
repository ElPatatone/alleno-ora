#include "database.hpp"
#include "workout.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sqlite3.h>

const std::string CONFIG_FILE_PATH = "config.txt";

std::string getDBPath(std::string configPath) {
    try {
        std::ifstream configFile{configPath};

        if (!configFile.is_open()) {
            throw std::runtime_error("[Config File] Config File could not be opened");
        }

        std::string line;
        std::string path;

        while (getline(configFile, line)) {
            if (!line.empty()) {
                path = line;
            }
        }

        configFile.close(); // Close the file after reading
        return path;
    } catch (const std::exception &e) {
        std::cerr << "[Error] " << e.what() << std::endl;
        return "";
    }
}

int main (int argc, char *argv[]) {
    if (argc == 2) {
        std::cout << "File: " << argv[1] << std::endl;
    }

    try {
        if (argc == 1) {
            throw std::runtime_error("no workout file was selected");
            return 1;
        }

        std::ifstream file{argv[1]};
        if (!file.is_open()){
            throw std::runtime_error("could not open file " + std::string(argv[1]));
            return 1;
        }

        std::string dbPath = getDBPath(CONFIG_FILE_PATH);
        Database db(dbPath);
        int rc = db.initialize();
        Workout workout = {};
        workout.loadWorkoutData(file);
        std::cout << workout.getDate() << std::endl;
        std::cout << workout.getStartTime() << std::endl;
        std::cout << workout.getDuration() << std::endl;
        std::cout << workout.getLocation() << std::endl;
        std::cout << workout.getRating() << std::endl;

        for (const auto& exercise : workout.getExercisesVector()) {
            std::cout << exercise.name << std::endl;
            std::string printedSetType = "";  // Variable to track printed set type for the current exercise
            for (const auto& set : exercise.setsVector) {
                if (set.getSetType() != printedSetType) {
                    std::cout << set.getSetType() << std::endl;
                    printedSetType = set.getSetType();
                }
                std::cout << set.setNumber << " " << set.repsNumber << " " << set.weight << std::endl;
            }
        }

        return 0;
    } catch (const std::exception &e) {
        std::cerr << "[Error] " << e.what() << std::endl;
    }
}
