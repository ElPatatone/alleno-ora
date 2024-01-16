#include "database.hpp"
#include <iostream>
#include <filesystem>
#include <sqlite3.h>

Database::Database(const std::string& dbPath) {
}

Database::~Database() {
    close();
}

int Database::open() {
    return sqlite3_open(dbPath.c_str(), &db);
}

void Database::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::exists() {
    return std::filesystem::exists(dbPath);
}

int Database::initialize() {
    if (!exists()) {
        int sqlStatus = open();

        if (sqlStatus != SQLITE_OK) {
            std::cerr << "Error: " << sqlite3_errmsg(db) << std::endl;
            close();
            return sqlStatus;
        }

        std::string createWorkoutsTableQuery = "CREATE TABLE workouts ("
                                            "id INTEGER PRIMARY KEY,"
                                            "date TEXT,"
                                            "start_time TEXT,"
                                            "duration INTEGER,"
                                            "rating INTEGER,"
                                            "location TEXT"
                                            ");";
        sqlStatus = sqlite3_exec(db, createWorkoutsTableQuery.c_str(), NULL, 0, NULL);
        if (sqlStatus != SQLITE_OK) {
            std::cout << "Failed to create workouts table: " <<  sqlite3_errmsg(db) << std::endl;
            close();
            return sqlStatus;
        }

        std::string createExerciseTableQuery = "CREATE TABLE exercises ("
                                             "id INTEGER PRIMARY KEY,"
                                             "workout_id INTEGER,"
                                             "name TEXT,"
                                             "FOREIGN KEY(workout_id) REFERENCES workouts(id)"
                                             ");";
        sqlStatus = sqlite3_exec(db, createExerciseTableQuery.c_str(), NULL, 0, NULL);
        if (sqlStatus != SQLITE_OK) {
            std::cout << "Failed to create exercises table: " <<  sqlite3_errmsg(db) << std::endl;
            close();
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
        sqlStatus = sqlite3_exec(db, createSetsTableQuery.c_str(), NULL, 0, NULL);
        if (sqlStatus != SQLITE_OK) {
            std::cout << "Failed to create sets table: " <<  sqlite3_errmsg(db) << std::endl;
            close();
            return sqlStatus;
        }
        close();
        std::cout << "Database File has been made successfully" << std::endl;
    } else {
        std::cout << "Database File already exists" << std::endl;
    }

    return SQLITE_OK;
}
