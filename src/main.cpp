#include "include/database.hpp"
#include "include/file.hpp"
#include "include/workout.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sqlite3.h>
#include <unordered_set>

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

    std::unordered_set<std::string_view> validOptions = {"-s", "-c", "-h", "--help", "--save", "--create"};

    // Hanlding case where multiple options are passed.
    int count = 0;
    for (const auto& arg : args) {
        if (validOptions.find(arg) != validOptions.end()) {
            count++;
        }
    }

    if (count > 1) {
        std::cerr << "[Error] More than one option specified.\n";
        return 1;
    }

    // Handling cases where 1 options is passed and their respective logics.
    for (int i = 0; i < args.size() - 1; i++) {
        if (args[i] == args[i + 1]) {
            std::cout << "[Error] Cannot use the same option twice\n";
            return 1;
        }

        // -s and --save option, save the workout information in the database.
        if (args[i] == "-s" || args[i] == "--save") {
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

        // -c and --create option, create a new workout file by prompting the user for header information for the workout.
        if (args[i] == "-c" || args[i] == "--create") {
            if (args[i + 1] == "") {
                std::cerr << "[Error] Please make sure to pass in a name for the new workout file\n";
                return 1;
            }
            else {
                std::string fileName = std::string(args[i + 1]);
                File newFile(fileName);
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
