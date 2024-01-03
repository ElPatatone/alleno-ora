#include <iostream>
#include <string>
#include <vector>

struct Workout {

    std::string date;
    std::string training;
    std::string start_time;
    std::string location;
    int duration;
    int rating;

};

int main (int argc, char *argv[]) {

    std::cout << "argc: " << argc << std::endl;

    for (int i = 0; i < argc; i++) {
        std::cout << "argv: " << argv[i] << std::endl;
    }

    std::vector<Workout> workouts;
    Workout firstWorkout;
    Workout secondWorkout;

    workouts.push_back(firstWorkout);
    workouts.push_back(secondWorkout);

    workouts[0].date = "2024";
    workouts[1].date = "2023";

    std::cout << "date of the first workout: " << workouts[0].date << std::endl;
    std::cout << "date of the second workout: " << workouts[1].date << std::endl;

    workouts[0].rating = 1;
    std::cout << "rating of the first workout: " << workouts[0].rating << std::endl;
    return 0;
}

