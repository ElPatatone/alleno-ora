#include "include/database.hpp"
#include "include/file.hpp"
#include "include/workout.hpp"
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
    // Command line parsing
    std::vector<std::string_view> args(argv + 1, argv + argc);

    std::string dbPath = getDBPath(CONFIG_FILE_PATH);
    Database db(dbPath);
    db.initialize();

    for (int i = 0; i < args.size(); i++) {
        if (args[i] == "-s") {
            std::ifstream workoutFile{std::string(args[i + 1])};

            if (!workoutFile.is_open()){
                std::cerr << "[Error] Could not open file " + std::string(args[i + 1]) << "\n";
                std::cerr << "[Error] Please make sure the file already exists\n";
                return 1;
            } else {
                Workout workout = {};
                if (workout.parseWorkoutData(workoutFile)) {
                    db.insertWorkout(workout);
                } else {
                    std::cerr << "[Error] Failed to parse file, please try again.\n";
                    return 1;
                }
            }
        }

        if (args[i] == "-c") {
            if (args[i + 1] == "") {
                std::cerr << "[Error] Please make sure to pass in a name for the new workout file\n";
                return 1;
            }
            else {
                std::string newWorkoutFile = std::string(args[i + 1]);
                File newFile(newWorkoutFile);
                Workout newWorkout{};
                newFile.makeWorkoutFileHeader(newWorkout);
            }
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
