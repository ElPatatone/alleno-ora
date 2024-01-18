#include "database.hpp"
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
}

int main (int argc, char *argv[]) {
    if (argc == 2) {
        std::cout << "File: " << argv[1] << std::endl;
    }

    if (argc == 1) {
        std::cerr << "[Error] No workout file was passed\n";
        return 1;
    }

    std::ifstream file{argv[1]};
    if (!file.is_open()){
        std::cerr << "[Error] Could not open file " + std::string(argv[1]) << "\n";
        return 1;
    }

    std::string dbPath = getDBPath(CONFIG_FILE_PATH);

    Database db(dbPath);
    db.initialize();

    Workout workout = {};
    workout.loadWorkoutData(file);
    db.insertWorkout(workout);
    db.getWorkout("2023/01/18");

    // std::cout << workout.getDate() << std::endl;
    // std::cout << workout.getStartTime() << std::endl;
    // std::cout << workout.getDuration() << std::endl;
    // std::cout << workout.getLocation() << std::endl;
    // std::cout << workout.getRating() << std::endl;
    //
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
