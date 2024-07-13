# alleno-ora

This CLI tool is meant to make it easier to store and access workout information.
It parses a workout file that is in a specific format and will then store that information in a sqlite
database, you can also retrieve workout information from the database by giving a specific date, this will then be saved into a file for you to view.

## Example workout file

```
Date: 2024/01/30
Start Time: 09:00
Duration: 1h30m
Location: Gym
Rating: 3

- Bench press
    Warm up sets
        1 x 10 @ 20kg
        2 x 6 @ 40kg
    Working sets
        1 x 6 @ 55kg
        1 x 4 @ 60kg (PR)
    Backoff sets
        2 x 8 @ 50kg
        1 x 6 @ 45kg

- Dumbell shoulder press
    Working sets
        3 x 10 @ 18kg
```

Above you can see the example file, the indententations do not affect the parsing but are there for better readablity. At the top there is a header consisting of 5 fields that will need to
be passed in:

- Date (**e.g yyyy/mm/dd**)
- Start Time (**in 24h format e.g hh:mm**)
- Duration (**e.g 1h35m**)
- Location
- Rating (**1 to 5**)

Each exercise has to be written in the format `- Exercise A`, the `-` is used to recognised which ones are
exercises.  
Each exercise can have multiple sets, you can specify if the set was a warm up, working or back off set.
You do not have to have all of them, if you did not do back off sets you can just omit them from the file.
Sets are in the format `set number x reps number @ weight(kg) or (lbs)` e.g `1 x 10 @50kg`  
If you wish to save the exercise without giving the set information, you can just leave that empty and only the exercise name will be saved in the database.  
You can also say if a set was a PR by adding `(PR)` at the end of the set e.g `1 x 2 @ 100kg (PR)`

## Command list

#### -s and --save

This will save a workout file into the database. Make sure it is in the specified format, the CLI tool will let you know if there are any issues.
e.g:

```shell
alleno -s workout.txt
alleno --save workout.txt
```

#### -g and --get

This will take in a date and get information for the workout with that date in the database. Make sure the date is in the specified format and that the workout is already in the database.
e.g:

```shell
alleno -g 2024/01/30
alleno --get 2024/01/30
```

#### -c and --create

This will take in user input and create a new workout file with the header filled in with the given inputs. Make sure to pass in the name for the file as well including the `.txt` extension.
Header includes: date, start time, duration, locatin, rating.
e.g:

```shell
alleno -c new_workout.txt
alleno --create new_workout.txt
```

#### -e and --exercise

This will take in user input and fetch the data for the exercise given, it will only
look for the Working sets for that exercise across all the workouts that are in the
database. It will then create a file with that data with the name of the exercise given.
e.g:

```shell
alleno -e Deadlift
alleno --exercise Deadlift
```

#### -h and --help

This will output the help section with examples for each of the commands for the CLI tool.

```shell
alleno -h
alleno --help
```

## Build

To build the CLI tool just run the `build.sh` script in the `build` directory.  
Also make sure you create a `config.txt` file in the `build` directory containing the following

```
db - /example/directory
workouts - /example/directory

```

This file tells the CLI tool where do you want the database to be created and also where do you want the workout files made by the CLI tool to be stored
