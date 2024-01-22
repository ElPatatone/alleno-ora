#include "database.hpp"
#include "file.hpp"
#include "workout.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sqlite3.h>

const std::string CONFIG_FILE_PATH = "config.txt";

std::string getDBPath(std::string_view configPath) {
    std::ifstream configFile{configPath.data()};

    if (!configFile.is_open()) {
        std::cerr << "[Config File] Config File could not be opened\n";
        return "";
    }

    std::string line;
    std::string path;

    while (getline(configFile, line)) {
        if (!line.empty()) {
            path = line;
        }
    }

    configFile.close();
    return path;
}

int main (int argc, char *argv[]) {
    if (argc == 3) {
        std::cout << "File: " << argv[2] << std::endl;
    }

    std::string dbPath = getDBPath(CONFIG_FILE_PATH);
    Database db(dbPath);
    db.initialize();

    if (argc >= 2) {
        std::string flag = argv[1];
        if (flag == "-s") {
            std::ifstream file{argv[2]};

            if (!file.is_open()){
                std::cerr << "[Error] Could not open file " + std::string(argv[2]) << "\n";
                return 1;
            } else {
                Workout workout = {};
                if (workout.parseWorkoutFile(file)) {
                    db.insertWorkout(workout);
                } else {
                    std::cerr << "[Error] Failed to parse file, please try again.\n";
                }
            }
        }

        if (flag == "-c") {
            File newFile("test1.txt");
            Workout testWorkout{};
            newFile.makeWorkoutFile(testWorkout);
        }
    }


    // for (const auto& exercise : workout.getExercisesVector()) {
    //     std::cout << exercise.name << std::endl;
    //     std::string printedSetType = "";  // Variable to track printed set type for the current exercise
    //     for (const auto& set : exercise.setsVector) {
    //         if (set.getSetType() != printedSetType) {
    //             std::cout << set.getSetType() << std::endl;
    //             printedSetType = set.getSetType();
    //         }
    //         std::cout << set.setNumber << " " << set.repsNumber << " " << set.weight << std::endl;
    //     }
    // }

    return 0;
}
