#include "include/database.hpp"
#include "include/file.hpp"
#include "include/workout.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sqlite3.h>
#include <unordered_set>

const std::string CONFIG_FILE_PATH = "config.txt";

std::string convertDate(const std::string& date) {
    std::string convertedDate = date;
    convertedDate[4] = '_';
    convertedDate[7] = '_';

    return convertedDate;
}

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

void helpSection(){
    std::cout << "[Usage]: Alleno [OPTIONS]\n"
              << "Options:\n"
              << "  -s, --save   Save workout information to the database.\n"
              << "  -c, --create Create a new file with a workout header after inputting the information.\n"
              << "  -g, --get    Get workout information for a given date and save it to a file.\n"
              << "  -h, --help   Show the help section.\n"
              << "Examples:\n"
              << "  ./alleno -s workout.txt\n"
              << "  ./alleno -c new_workout\n"
              << "  ./alleno -g 2024/01/15\n"
              << "  ./alleno -h\n";
}

int main (int argc, char *argv[]) {
    // Command line parsing
    std::vector<std::string_view> args(argv + 1, argv + argc);

    std::string dbPath = getDBPath(CONFIG_FILE_PATH);
    Database db(dbPath);
    db.initialize();

    std::unordered_set<std::string_view> validOptions = {"-s", "-c", "-h", "-g", "--help", "--save", "--create", "--get"};

    // Hanlding case where multiple options are passed or where no options are passed.
    int count = 0;
    for (const auto& arg : args) {
        if (validOptions.find(arg) != validOptions.end()) {
            count++;
        }
    }

    if (count > 1) {
        std::cerr << "[Error] More than one option was specified.\n";
        std::cerr << "[Error] Please only pass in one option at a time.\n";
        return 1;
    }

    if (count == 0) {
        std::cerr << "[Error] No valid option was passed in.\n";
        std::cerr << "[Error] Please make sure to pass it at least one option to use the CLI tool.\n";
        std::cerr << "[Error] Use ./alleno -h to see the list of possible commands.\n";
        return 1;
    }

    // Handling cases where 1 options is passed and their respective logics.
    for (size_t i = 0; i < args.size(); i++) {

        if (args[i] == args[i + 1]) {
            std::cout << "[Error] Cannot use the same option twice\n";
            return 1;
        }

        if (args[i] == "-h" || args[i] == "--help") {
            helpSection();
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

        // -c and --get option, get the workout data for a date given by the user. Then save the information to a file.
        if (args[i] == "-g" || args[i] == "--get") {
            if (args[i + 1] == "") {
                std::cerr << "[Error] Please make sure to pass in the date of the workout to fetch its data.\n";
                return 1;
            }
            else {
                std::string date = std::string(args[i + 1]);
                if (!File::isDateValid(date)) {
                    std::cerr << "[Error] Date value is in the wrong format. e.g (yyyy/mm/dd)\n";
                    return 1;
                } else {
                    // get the workout from the database. If the function fails to return a value the fetchedWorkoutOptional will have value of nullopt.
                    auto fetchedWorkoutOptional = db.getWorkout(date);
                    if (fetchedWorkoutOptional.has_value()) {
                        std::string fileName = convertDate(date) + ".txt";
                        File newFile(fileName);
                        Workout fetchedWorkout = fetchedWorkoutOptional.value();
                        newFile.makeFetchedWorkoutFile(fetchedWorkout);
                    } else {
                        std::cerr << "[Error] No workout found for the date: " << std::string(args[i + 1]) << "\n";
                        std::cerr << "[Error] Please make sure the workout already exists in the database.\n";
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}
