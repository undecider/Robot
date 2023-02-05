<h1> Instructions </h1>

1. Run `make` or build robot.cpp any way you want

2. Run the program: `./robot`

3. Issue commands from the following list:
    1. place(x, y, direction)
    2. move
    3. left
    4. right
    5. report
    6. quit

<h1> Notes </h1>

Commands which don't require arguments can be specified with or without parentheses

e.g. `move()` or `move`

Place commands require parentheses as 3 parameters are required. Place direction parameters should be specified without quotations

e.g: `place(1, 2, NORTH)`

Warnings are given on invalid commands

A grid showing position and direction is given on valid commands (except report)

The robot will simply ignore attempts to push him off the edge