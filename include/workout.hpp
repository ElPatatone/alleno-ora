#ifndef WORKOUT_H
#define WORKOUT_H

#include <optional>
#include <vector>
#include <string>

enum SetType {
    WARM_UP_SETS,
    WORKING_SETS,
    BACK_OFF_SETS,
    UNKNOWN_SETS
};

struct Set {
    SetType setType;
    int setNumber;
    int repsNumber;
    int weight;
    bool isPR;

    std::string getSetType() const;
    SetType convertStringToSetType(const std::string& setType);

    // initialising default values as this was causing memory leaks shown on valgrind. 
    // issue was: Conditional jump or move depends on uninitialised value(s)
    Set() :  setType(UNKNOWN_SETS), setNumber(0), repsNumber(0), weight(0), isPR(false) {}
};

struct Exercise {
    std::string name;
    std::vector<Set> setsVector;

    void addSet(int setNumber, int repsNumber, int weight, const std::string& setType, bool isPR);
};

class Workout {
public:
    Workout();
    ~Workout();

    std::optional<Workout> parseWorkoutData(std::ifstream& workoutFile);
    std::optional<Workout> getUserInput();

    const std::string& getDate() const;
    const std::string& getStartTime() const;
    const std::string& getLocation() const;
    const std::string& getDuration() const;
    int getRating() const;

    const std::vector<Exercise>& getExercisesVector() const;
    const std::vector<Exercise>& addExercisesToVector(const Exercise& newExercise);

    void setDate(const std::string& date);
    void setStartTime(const std::string& startTime);
    void setLocation(const std::string& location);
    void setDuration(const std::string& duration);
    void setRating(const int rating);

private:
    std::vector<Exercise> exercisesVector;
    std::string date;
    std::string startTime;
    std::string location;
    std::string duration;
    int rating;
};

#endif
