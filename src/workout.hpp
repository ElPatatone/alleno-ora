#ifndef WORKOUT_H
#define WORKOUT_H

#include <vector>
#include <string>
enum SetType {
    WARM_UP_SETS,
    WORKING_SETS,
    BACK_OFF_SETS
};

struct Set {
    SetType setType;
    int setNumber;
    int repsNumber;
    int weight;

    std::string getSetType() const;
};

struct Exercise {
    std::string name;
    std::vector<Set> setsVector;
};

class Workout {
public:
    Workout();
    ~Workout();
    // std::string getSetType(SetType setType);
    Workout loadWorkoutData(std::ifstream& workoutFile);

    const std::string& getDate() const;
    const std::string& getStartTime() const;
    const std::string& getLocation() const;
    int getDuration() const;
    int getRating() const;
    const std::vector<Exercise> getExercisesVector() const;

    void setDate(const std::string& date);
    void setStartTime(const std::string& startTime);
    void setLocation(const std::string& location);
    void setDuration(const int duration);
    void setRating(const int rating);

private:
    std::vector<Exercise> exercisesVector;
    std::string date;
    std::string startTime;
    std::string location;
    int duration;
    int rating;
};

#endif
