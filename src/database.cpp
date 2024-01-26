#include "include/database.hpp"
#include <iostream>
#include <filesystem>
#include <sqlite3.h>

Database::Database(const std::string& dbPath) : db(nullptr), dbPath(dbPath) {
}

Database::~Database() {
    close();
}

int Database::open() {
    // the sqlite3_open() function is causing memory leaks.
    // I am not sure what is going on as the error is not descriptive.
    // This could be an issue with the actual sqlite3 lib and not my code (it is most likely me...).
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

        // workouts table
        std::string createWorkoutsTableQuery = "CREATE TABLE workouts ("
                                            "id INTEGER PRIMARY KEY,"
                                            "date TEXT,"
                                            "start_time TEXT,"
                                            "duration TEXT,"
                                            "rating INTEGER,"
                                            "location TEXT"
                                            ");";
        SQLStatus = sqlite3_exec(db, createWorkoutsTableQuery.c_str(), NULL, 0, NULL);
        if (SQLStatus != SQLITE_OK) {
            std::cerr << "[Error] Failed to create workouts table: " <<  sqlite3_errmsg(db) << "\n";
            close();
            return SQLStatus;
        }

        // exercise table
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

        // set table
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
    sqlite3_bind_text(stmt, 3, workout.getDuration().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, workout.getRating());
    sqlite3_bind_text(stmt, 5, workout.getLocation().c_str(), -1, SQLITE_STATIC);

    SQLStatus = sqlite3_step(stmt);

    if (SQLStatus != SQLITE_DONE) {
        std::cerr << "[Error] Failed to insert workout: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return SQLStatus;
    }

    // Retrieve the last inserted workout ID
    int workoutId = sqlite3_last_insert_rowid(db);

    // main loop to make sure the individual exercises and their sets are all linked with the right workout_id
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
    return exerciseId;
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

std::optional<Workout> Database::getWorkout(const std::string& date) {
    int openStatus = open();
    if (openStatus != SQLITE_OK) {
        return std::nullopt;
    }

    std::string getWorkoutQuery = "SELECT * FROM workouts WHERE date = ?";
    sqlite3_stmt* stmt = nullptr;
    int SQLStatus = sqlite3_prepare_v2(db, getWorkoutQuery.c_str(), -1, &stmt, nullptr);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to prepare statement for selecting workout: " << sqlite3_errmsg(db) << "\n";
        close();
        return std::nullopt;
    }

    SQLStatus = sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to bind parameters to query: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        close();
        return std::nullopt;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        Workout workout;

        // Populate workout details
        workout.setDate(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        workout.setStartTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        workout.setDuration(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        workout.setRating(sqlite3_column_int(stmt, 4));
        workout.setLocation(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));

        // Fetch exercises for the workout
        getExercisesForWorkout(sqlite3_column_int(stmt, 0), workout);

        sqlite3_finalize(stmt);
        close();
        return std::make_optional(workout);
    } else {
        sqlite3_finalize(stmt);
        close();
        return std::nullopt;
    }
}

void Database::getExercisesForWorkout(int workoutId, Workout& workout) {
    std::string getExercisesQuery = "SELECT id, name FROM exercises WHERE workout_id = ?";
    sqlite3_stmt* stmt = nullptr;
    int SQLStatus = sqlite3_prepare_v2(db, getExercisesQuery.c_str(), -1, &stmt, nullptr);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to prepare statement for selecting exercises: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    SQLStatus = sqlite3_bind_int(stmt, 1, workoutId);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to bind parameters to query: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int exerciseId = sqlite3_column_int(stmt, 0);
        std::string exerciseName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        Exercise exercise;
        exercise.name = exerciseName;

        // Fetch sets for the exercise
        getSetsForExercise(exerciseId, exercise);

        workout.addExercisesToVector(exercise);
    }

    sqlite3_finalize(stmt);
}

void Database::getSetsForExercise(int exerciseId, Exercise& exercise) {
    int openStatus = open();
    if (openStatus != SQLITE_OK) {
        return;
    }

    std::string getSetsQuery = "SELECT set_number, reps, weight, set_type FROM sets WHERE exercise_id = ?";

    sqlite3_stmt* stmt = nullptr;
    int SQLStatus = sqlite3_prepare_v2(db, getSetsQuery.c_str(), -1, &stmt, nullptr);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to prepare statement for selecting sets: " << sqlite3_errmsg(db) << "\n";
        close();
        return;
    }

    SQLStatus = sqlite3_bind_int(stmt, 1, exerciseId);

    if (SQLStatus != SQLITE_OK) {
        std::cerr << "[Error] Failed to bind parameters to query: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        close();
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int setNumber = sqlite3_column_int(stmt, 0);
        int reps = sqlite3_column_int(stmt, 1);
        int weight = sqlite3_column_int(stmt, 2);
        std::string setType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        // Create a Set object, initialize with the fetched values and add it to the exercise
        exercise.addSet(setNumber, reps, weight, setType);
    }

    sqlite3_finalize(stmt);
    close();
}
