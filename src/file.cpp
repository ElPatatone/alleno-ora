#include "include/file.hpp"
#include <iostream>
#include <sstream>

File::File(const std::string& fileName) : fileName(fileName), file(fileName) {}

File::~File() {}

bool File::isDateValid(const std::string& date){
    if (date.size() != 10) {
        return false;
    }

    int day, month, year;
    char slash;
    std::istringstream stream(date);
    if (stream >> year >> slash >> month >> slash >> day && stream.eof()) {
        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2000 || year > 9999){
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
    char columns;
    std::istringstream stream(time);
    if (stream >> hour >> columns >> minutes && stream.eof()) {
        if (hour < 0 || hour > 23 || minutes < 0 || minutes > 59) {
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

// DEBUG: This does not work for some reason, it should as the isTimeValid function with the same logic is working properly

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
        workoutFile << "Rating: " << workout.getRating() << '\n';
    }
    workoutFile.close();
}
