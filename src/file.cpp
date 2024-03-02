#include "file.hpp"
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

File::File(const std::string& fileName) : fileName(fileName), file(fileName) {}

bool File::isDateValid(const std::string& date){
    if (date.size() != 10) {
        return false;
    }

    int day, month, year;
    char slash;
    std::istringstream stream(date);
    if (stream >> year >> slash >> month >> slash >> day && stream.eof()) {
        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2000 || year > 9999 || slash != '/'){
            return false;
        }
        return true;
    }
    return false;
}

bool File::isTimeValid(const std::string& time){
    if (time.size() != 5) {
        return false;
    }

    int hour, minutes;
    char colon;
    std::istringstream stream(time);
    if (stream >> hour >> colon >> minutes && stream.eof()) {
        if (hour < 0 || hour > 23 || colon != ':' || minutes < 0 || minutes > 59) {
            return false;
        }
        return true;
    }
    return false;
}

bool File::isDurationValid(const std::string& duration){
    if (duration.size() != 5) {
        return false;
    }

    if (duration[1] != 'h' || duration[4] != 'm') {
        return false;
    } else if (!std::isdigit(duration[0]) || !std::isdigit(duration[2]) || !std::isdigit(duration[3])) {
        return false;
    }

    return true;
}

// ISSUE: This does not work for some reason, it should as the isTimeValid function with the same logic is working properly

// bool File::isDurationValid(const std::string& duration){
//     if (duration.size() != 5) {
//         std::cerr << "[Error] Invalid duration length: " << duration << std::endl;
//         return false;
//     }
//
//     int hour, minutes;
//     char character;
//
//     std::istringstream stream(duration);
//
//     if (stream >> hour >> character >> minutes >> character) {
//         if (stream.eof()) {
//             if (hour < 0 || hour > 23 || minutes < 0 || minutes > 59) {
//                 std::cerr << "[Error] Invalid duration range: " << duration << std::endl;
//                 return false;
//             }
//             std::cerr << "Valid duration" << duration << std::endl;
//             return true;
//         }
//     }
//     std::cerr << "returns here" << duration << std::endl;
//     return false;
// }

void File::makeWorkoutFileHeader(Workout& workout){
    std::ofstream workoutFile{fileName};
    workout.getUserInput();

    if (workoutFile.is_open()){
        workoutFile << "Date: " << workout.getDate() << '\n';
        workoutFile << "Start Time: " << workout.getStartTime() << '\n';
        workoutFile << "Duration: " << workout.getDuration() << '\n';
        workoutFile << "Location: " << workout.getLocation() << '\n';
        workoutFile << "Workout Rating: " << workout.getWorkoutRating() << '\n';
        workoutFile << "Physical Rating: " << workout.getPhysicalRating() << '\n';
        workoutFile << "Mental Rating: " << workout.getMentalRating() << '\n';
    }
    workoutFile.close();
}

void File::makeFetchedWorkoutFile(const Workout& workout) {
    std::ofstream fetchedWorkoutFile{fileName};

    if (!fetchedWorkoutFile.is_open()){
        std::cerr << "[Error] File could not be opened for writing.\n";
        return;
    }
    // Write the header for the file
    fetchedWorkoutFile << "Date: " << workout.getDate() << '\n';
    fetchedWorkoutFile << "Name: " << workout.getName() << '\n';
    fetchedWorkoutFile << "Start Time: " << workout.getStartTime() << '\n';
    fetchedWorkoutFile << "Duration: " << workout.getDuration() << '\n';
    fetchedWorkoutFile << "Location: " << workout.getLocation() << '\n';
    fetchedWorkoutFile << "Workout Rating: " << workout.getWorkoutRating() << "\n";
    fetchedWorkoutFile << "Physical Rating: " << workout.getPhysicalRating() << "\n";
    fetchedWorkoutFile << "Mental Rating: " << workout.getMentalRating() << "\n\n";

    fetchedWorkoutFile << "Notes Start: \n" << workout.getNotes();
    fetchedWorkoutFile << "Notes End" << "\n\n";

    // Write the exercise names and the sets for each exercise.
    for (const auto& exercise : workout.getExercisesVector()) {
        fetchedWorkoutFile << "- " << exercise.name << '\n';
        std::string printedSetType = "";  // Variable to track printed set type for the current exercise
        for (const auto& set : exercise.setsVector) {
            if (set.getSetType() != printedSetType) {
                fetchedWorkoutFile << '\t' << set.getSetType() << '\n';
                printedSetType = set.getSetType();
            }
            if (set.isPR) {
                fetchedWorkoutFile << "\t\t" << set.setNumber << " x " << set.repsNumber << " @ " << set.weight << "kg (PR)\n";
            } else {
                fetchedWorkoutFile << "\t\t" << set.setNumber << " x " << set.repsNumber << " @ " << set.weight << "kg\n";
            }
        }
        fetchedWorkoutFile << "\n";
    }
    std::cout << "File has been made successfully\n";

    fetchedWorkoutFile.close();
}

void File::makeFetchedExerciseFile(const std::vector<Workout> workouts){
    std::ofstream fetchedExerciseFile{fileName};

    if (!fetchedExerciseFile.is_open()){
        std::cerr << "[Error] File could not be opened for writing.\n";
        return;
    }
    for (const auto& workout: workouts) {
        for (const auto& exercise : workout.getExercisesVector()) {
            fetchedExerciseFile << "Date: " << workout.getDate() << '\n';
            for (const auto& set : exercise.setsVector) {
                if (set.isPR) {
                    fetchedExerciseFile << "\t" << set.setNumber << " x " << set.repsNumber << " @ " << set.weight << "kg (PR)\n";
                } else {
                    fetchedExerciseFile << "\t" << set.setNumber << " x " << set.repsNumber << " @ " << set.weight << "kg\n";
                }
            }
            fetchedExerciseFile << "\n";
        }
    }
    std::cout << "File has been made successfully\n";
    fetchedExerciseFile.close();
}
