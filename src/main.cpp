#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <sqlite3.h>

enum SetType {
    WARM_UP_SETS,
    WORKING_SETS,
    BACK_OFF_SETS
};

struct Set {
    SetType setType;
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

const std::string CONFIG_FILE_PATH = "config.txt";

std::string setTypeName(SetType setType) {
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

bool checkForDB(std::string_view dbPath){
    if (std::filesystem::exists(dbPath)){
        return true;
    }
    else {
        return false;
    }
};

int initializeDB(sqlite3 **db){
    const std::string dbPath = getDBPath(CONFIG_FILE_PATH);
    // const std::string databasePath = "/home/elpatatone/Documents/alleno-ora/database/workouts.db";
    bool db_exists = checkForDB(dbPath);
    int sqlStatus = sqlite3_open(dbPath.c_str(), db);

    if (sqlStatus != SQLITE_OK) {
        std::cerr << "Error: " <<  sqlite3_errmsg(*db) << std::endl;
        return sqlStatus;
    }

    if (!db_exists) {
        std::string createWorkoutsTableQuery = "CREATE TABLE workouts ("
                                            "id INTEGER PRIMARY KEY,"
                                            "date TEXT,"
                                            "start_time TEXT,"
                                            "duration INTEGER,"
                                            "rating INTEGER,"
                                            "location TEXT"
                                            ");";
        sqlStatus = sqlite3_exec(*db, createWorkoutsTableQuery.c_str(), NULL, 0, NULL);
        if (sqlStatus != SQLITE_OK) {
            printf("Failed to create workouts table: %s\n", sqlite3_errmsg(*db));
            sqlite3_close(*db);
            return sqlStatus;
        }

        std::string createExerciseTableQuery = "CREATE TABLE exercises ("
                                             "id INTEGER PRIMARY KEY,"
                                             "workout_id INTEGER,"
                                             "name TEXT,"
                                             "FOREIGN KEY(workout_id) REFERENCES workouts(id)"
                                             ");";
        sqlStatus = sqlite3_exec(*db, createExerciseTableQuery.c_str(), NULL, 0, NULL);
        if (sqlStatus != SQLITE_OK) {
            printf("Failed to create exercises table: %s\n", sqlite3_errmsg(*db));
            sqlite3_close(*db);
            return sqlStatus;
        }

        std::string createSetsTableQuery = "CREATE TABLE sets ("
                                        "id INTEGER PRIMARY KEY,"
                                        "exercise_id INTEGER,"
                                        "set_number INTEGER,"
                                        "reps INTEGER,"
                                        "weight INTEGER,"
                                        "set_type TEXT,"
                                        "FOREIGN KEY(exercise_id) REFERENCES exercises(id)"
                                        ");";
        sqlStatus = sqlite3_exec(*db, createSetsTableQuery.c_str(), NULL, 0, NULL);
        if (sqlStatus != SQLITE_OK) {
            printf("Failed to create sets table: %s\n", sqlite3_errmsg(*db));
            sqlite3_close(*db);
            return sqlStatus;
        }
        std::cout << "Database File has been made successfully" << std::endl;
    }
    else {
        std::cout << "Database File already exists" << std::endl;
    }

    sqlite3_close(*db);
    return SQLITE_OK;
}

Workout parseFile(std::ifstream& file){
    std::string line;

    Workout workout;
    Exercise exercise;
    Set set;

    try {
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
                try {
                    workout.duration = std::stoi(line.substr(10));
                } catch (const std::invalid_argument& e) {
                    throw std::runtime_error("[Workout File] Invalid argument during duration conversion: " + std::string(e.what()));
                } catch (const std::out_of_range& e) {
                    throw std::runtime_error("[Workout File] Out of range error during duration conversion: " + std::string(e.what()));
                }
            }
            // Check for location
            else if (line.find("Location") != std::string::npos) {
                workout.location = line.substr(10);
            }
            // Check for rating
            else if (line.find("Rating") != std::string::npos) {
                try {
                    workout.rating = std::stoi(line.substr(8));
                } catch (const std::invalid_argument& e) {
                    throw std::runtime_error("[Workout File] Invalid argument during rating conversion: " + std::string(e.what()));
                } catch (const std::out_of_range& e) {
                    throw std::runtime_error("[Workout File] Out of range error during rating conversion: " + std::string(e.what()));
                }            }
            // Check for exercise name
            else if (line[0] == '-'){
                if (!exercise.name.empty()) {
                    workout.exercisesVector.push_back(exercise);
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
            workout.exercisesVector.push_back(exercise);
        }

        std::cout << "File parsed successfully" << std::endl;
        return workout;
    } catch (const std::exception& e) {
        throw;
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

        sqlite3 *db;
        int rc = initializeDB(&db);

        Workout workout = parseFile(file);
        std::cout << workout.date << std::endl;
        std::cout << workout.startTime << std::endl;
        std::cout << workout.duration << std::endl;
        std::cout << workout.location << std::endl;
        std::cout << workout.rating << std::endl;

        for (const auto& exercise : workout.exercisesVector) {
            std::cout << exercise.name << std::endl;
            std::string printedSetType = "";  // Variable to track printed set type for the current exercise
            for (const auto& set : exercise.setsVector) {
                if (setTypeName(set.setType) != printedSetType) {
                    std::cout << setTypeName(set.setType) << std::endl;
                    printedSetType = setTypeName(set.setType);
                }
                std::cout << set.setNumber << " " << set.repsNumber << " " << set.weight << std::endl;
            }
        }

        return 0;
    } catch (const std::exception &e) {
        std::cerr << "[Error] " << e.what() << std::endl;
    }
}
