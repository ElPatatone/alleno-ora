#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <sqlite3.h>

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

std::string getDBPath(std::string configPath){
    std::ifstream file{configPath};

    if (!file.is_open()){
        std::cerr << "Error: Config File could not be opened" << std::endl;
    }

    std::string line;
    std::string path;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        else {
            path = line;
        }
    }
    return path;
};

bool checkForDB(std::string_view dbPath){
    std::ofstream file{dbPath.data()};

    if (file.is_open()){
        file.close();
        std::cout << "Database File exists" << std::endl;
        return true;
    }
    return false;
};

int initializeDB(sqlite3 **db){

    const std::string dbPath = getDBPath("config.txt");
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
    }

    return SQLITE_OK;
}

int main (int argc, char *argv[]) {

    if (argc == 2) {
        std::cerr << "File: " << argv[1] << std::endl;
    }

    std::ifstream file{argv[1]};
    if (!file.is_open()){
        std::cerr << "Error: file could not be opened" << std::endl;
        return 1;
    }


    sqlite3 *db;
    int rc = initializeDB(&db);

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
        }
    }

    // add last exercise being parsed
    if (!exercise.name.empty()) {
        workout.exercisesVector.push_back(exercise);
    }

    file.close();

    // std::cout << workout.date << std::endl;
    // std::cout << workout.startTime << std::endl;
    // std::cout << workout.duration << std::endl;
    // std::cout << workout.location << std::endl;
    // std::cout << workout.rating << std::endl;
    //
    // for (const auto& exercise : workout.exercisesVector) {
    //     std::cout << exercise.name << std::endl;
    //     std::string printedSetType = "";  // Variable to track printed set type for the current exercise
    //     for (const auto& set : exercise.setsVector) {
    //         if (set.setType != printedSetType) {
    //             std::cout << set.setType << std::endl;
    //             printedSetType = set.setType;
    //         }
    //         std::cout << set.setNumber << " " << set.repsNumber << " " << set.weight << std::endl;
    //     }
    // }
    return 0;
}
