#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>
#include "workout.hpp"

class Database {
public:
    Database(const std::string& dbPath);
    ~Database();

    int open();
    void close();
    bool exists();
    int initialize();
    int insertWorkout(const Workout& workout);
    int insertExercise(const Exercise& exercise, int workoutId);
    int insertSets(const Set& set, int exerciseId);

    std::optional<Workout> getWorkout(const std::string& date);
    int getExercisesForWorkout(int workoutId, Workout& workout);
    int getSetsForExercise(int exerciseId, Exercise& exercise);

private:
    sqlite3* db;
    std::string dbPath;
};

#endif
