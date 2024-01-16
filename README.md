# alleno-ora

Alleno-ora means train now in Italian.
This CLI tool is meant to make it easier to store and access workout information.
It parses a `workout.txt` file and will then store that information in a sqlite
database, you can then access the workout information for any given date whenever you want.

## Example workout.txt file

Below you can see the example file. At the top there are 5 fields that will need to
be passed in:

- Date
- Start Time (in 24h format)
- Duration (in minutes)
- Location
- Rating (1 to 5)

```
Date: 2023/12/25
Start time: 11:00
Duration: 50
Location: Gym
Rating: 4

- Exercise A
    Warm up sets
        3 x 10 @ 20kg
    Working sets
        3 x 10 @ 60kg
        3 x 10 @ 60kg
        3 x 10 @ 60kg
    Back off sets
        3 x 10 @ 60kg
        3 x 10 @ 60kg
- Exercise B
    Working sets
        3 x 10 @ 60kg
        3 x 10 @ 60kg
        3 x 10 @ 60kg
```
