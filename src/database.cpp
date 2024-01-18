#include "database.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sqlite3.h>

Database::Database(const std::string& dbPath) {
    this->dbPath = dbPath;
}

Database::~Database() {
    close();
}

int Database::open() {
    int SQLStatus = sqlite3_open(dbPath.c_str(), &db);
    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to open database: " <<  sqlite3_errmsg(db) << "\n";
        close();
        return SQLStatus; }
    return SQLStatus;
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
        int SQLStatus = open();

        if (SQLStatus != SQLITE_OK) {
            std::cerr << "[Error] " << sqlite3_errmsg(db) << "\n";
            close();
            return SQLStatus;
        }

        std::string createWorkoutsTableQuery = "CREATE TABLE workouts ("
                                            "id INTEGER PRIMARY KEY,"
                                            "date TEXT,"
                                            "start_time TEXT,"
                                            "duration INTEGER,"
                                            "rating INTEGER,"
                                            "location TEXT"
                                            ");";
        SQLStatus = sqlite3_exec(db, createWorkoutsTableQuery.c_str(), NULL, 0, NULL);
        if (SQLStatus != SQLITE_OK) {
            std::cerr << "[Error] Failed to create workouts table: " <<  sqlite3_errmsg(db) << "\n";
            close();
            return SQLStatus;
        }

        std::string createExerciseTableQuery = "CREATE TABLE exercises ("
                                             "id INTEGER PRIMARY KEY,"
                                             "workout_id INTEGER,"
                                             "name TEXT,"
                                             "FOREIGN KEY(workout_id) REFERENCES workouts(id)"
                                             ");";
        SQLStatus = sqlite3_exec(db, createExerciseTableQuery.c_str(), NULL, 0, NULL);
        if (SQLStatus != SQLITE_OK) {
            std::cerr << "[Error] Failed to create exercises table: " <<  sqlite3_errmsg(db) << "\n";
            close();
            return SQLStatus;
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
        SQLStatus = sqlite3_exec(db, createSetsTableQuery.c_str(), NULL, 0, NULL);
        if (SQLStatus != SQLITE_OK) {
            std::cerr << "[Error] Failed to create sets table: " <<  sqlite3_errmsg(db) << "\n";
            close();
            return SQLStatus;
        }
        close();
        std::cerr << "Database File has been made successfully\n";
    } else {
        std::cerr << "Database File already exists\n";
    }

    return SQLITE_OK;
}

int Database::insertWorkout(const Workout& workout) {
    int openStatus = open();
    if (openStatus != SQLITE_OK) {
        return openStatus;
    }

    std::string insertWorkoutQuery = "INSERT INTO workouts (date, start_time, duration, rating, location) VALUES (?, ?, ?, ?, ?)";

    sqlite3_stmt* stmt = nullptr;
    int SQLStatus = sqlite3_prepare_v2(db, insertWorkoutQuery.c_str(), -1, &stmt, nullptr);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to prepare statement for inserting workout: " << sqlite3_errmsg(db) << "\n";
        return SQLStatus;
    }

    sqlite3_bind_text(stmt, 1, workout.getDate().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, workout.getStartTime().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, workout.getDuration());
    sqlite3_bind_text(stmt, 4, workout.getLocation().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, workout.getRating());

    SQLStatus = sqlite3_step(stmt);

    if (SQLStatus != SQLITE_DONE) {
        std::cerr << "[Error] Failed to insert workout: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return SQLStatus;
    }

    // Retrieve the last inserted workout ID
    int workoutId = sqlite3_last_insert_rowid(db);

    for (const auto& exercise : workout.getExercisesVector()) {
        int exerciseId = insertExercise(exercise, workoutId);
        for (const auto& set: exercise.setsVector) {
            insertSets(set, exerciseId);
        }
    }

    sqlite3_finalize(stmt);
    return SQLStatus;
}

int Database::insertExercise(const Exercise& exercise, int workoutId) {
    int openStatus = open();
    if (openStatus != SQLITE_OK) {
        return openStatus;
    }

    std::string insertExercisesQuery = "INSERT INTO exercises (workout_id, name) VALUES (?, ?)";

    sqlite3_stmt* stmt = nullptr;
    int SQLStatus = sqlite3_prepare_v2(db, insertExercisesQuery.c_str(), -1, &stmt, nullptr);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to prepare statement for inserting exercises: " << sqlite3_errmsg(db) << "\n";
        return SQLStatus;
    }

    sqlite3_bind_int(stmt, 1, workoutId);
    sqlite3_bind_text(stmt, 2, exercise.name.c_str(), -1, SQLITE_STATIC);

    SQLStatus = sqlite3_step(stmt);

    if (SQLStatus != SQLITE_DONE) {
        std::cerr << "[Error] Failed to insert exercise: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return SQLStatus;
    }

    // Retrieve the last inserted exercise ID
    int exerciseId = sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt);
    return exerciseId;  // Return the exercise ID
}

int Database::insertSets(const Set& set, int exerciseId) {
    int openStatus = open();
    if (openStatus != SQLITE_OK) {
        return openStatus;
    }

    std::string insertSetsQuery = "INSERT INTO sets (exercise_id, set_number, reps, weight, set_type) VALUES (?, ?, ?, ?, ?)";

    sqlite3_stmt* stmt = nullptr;
    int SQLStatus = sqlite3_prepare_v2(db, insertSetsQuery.c_str(), -1, &stmt, nullptr);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to prepare statement for inserting sets: " << sqlite3_errmsg(db) << "\n";
        return SQLStatus;
    }

    sqlite3_bind_int(stmt, 1, exerciseId);
    sqlite3_bind_int(stmt, 2, set.setNumber);
    sqlite3_bind_int(stmt, 3, set.repsNumber);
    sqlite3_bind_int(stmt, 4, set.weight);
    sqlite3_bind_text(stmt, 5, set.getSetType().c_str(), -1, SQLITE_STATIC);

    SQLStatus = sqlite3_step(stmt);

    if (SQLStatus != SQLITE_DONE) {
        std::cerr << "[Error] Failed to insert set: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return SQLStatus;
    }

    sqlite3_finalize(stmt);
    return SQLStatus;
}

int Database::getWorkout(const std::string& date){
    int openStatus = open();
    if (openStatus != SQLITE_OK) {
        return openStatus;
    }

    std::string getWorkoutQuery = "SELECT * FROM workouts WHERE date = ?";

    sqlite3_stmt* stmt = nullptr;
    int SQLStatus = sqlite3_prepare_v2(db, getWorkoutQuery.c_str(), -1, &stmt, nullptr);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to prepare statement for selecting workout: " << sqlite3_errmsg(db) << "\n";
        return SQLStatus;
    }

    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to bind parameters to query: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return SQLStatus;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* date = sqlite3_column_text(stmt, 1);
        std::cout << date << "\n";
    }

    sqlite3_finalize(stmt);
    close();

    return SQLStatus;
}
