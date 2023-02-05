
#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <cstdlib>

// Simple Vector2 class
struct Vec2Di
{
    int x;
    int y;

    Vec2Di(int X, int Y)
        : x(X), y(Y)
    {}

    Vec2Di()
        : x(0), y(0)
    {}
};

// enum to represent directions
enum class DIRECTION : int
{
    NORTH = 0,
    EAST,
    SOUTH,
    WEST,
    INVALID
};

// convert direction string to enum 
DIRECTION strToDir(std::string dir)
{
    if (dir == "NORTH")
        return DIRECTION::NORTH;
    else if (dir == "SOUTH")
        return DIRECTION::SOUTH;
    else if (dir == "EAST")
        return DIRECTION::EAST;
    else if (dir == "WEST")
        return DIRECTION::WEST;
    else
        return DIRECTION::INVALID;
}

// convert direction enum to string for printing
std::string dirToStr(DIRECTION dir)
{
    switch (dir)
    {
        case DIRECTION::NORTH: return "NORTH";
        case DIRECTION::SOUTH: return "SOUTH";
        case DIRECTION::EAST: return "EAST";
        case DIRECTION::WEST: return "WEST";
        default:
        case DIRECTION::INVALID: return "INVALID";
    }
}

constexpr auto GRID_SIZE = 5;

// Robot class to manage all robot commands
class Robot
{
    DIRECTION direction = DIRECTION::INVALID;
    Vec2Di position;

public:

    // Check if have been placed on a valid position yet
    bool placed()
    {
        if (direction == DIRECTION::INVALID)
        {
            std::cout << "A valid place command must be issued!" << std::endl;
            return false;
        }

        return true;
    }

    // Move +1 space in direction facing, with clamping at borders
    bool move()
    {
        if (!placed())
            return false;

        switch (direction)
        {
            case DIRECTION::NORTH:
                position.y = std::min(position.y+1, GRID_SIZE-1);
            break;
            case DIRECTION::SOUTH:
                position.y = std::max(position.y-1, 0);
            break;
            case DIRECTION::EAST:
                position.x = std::min(position.x+1, GRID_SIZE-1);
            break;
            case DIRECTION::WEST:
                position.x = std::max(position.x-1, 0);
            default:
            case DIRECTION::INVALID:
            break;
        }

        return true;
    }

    // Rotate left by decrementing direction enum
    bool left()
    {
        if (!placed())
            return false;
        
        if (static_cast<int> (direction) - 1 == -1)
            direction = DIRECTION::WEST;
        else direction = static_cast<DIRECTION>
                (static_cast<int> (direction) - 1);
        
        return true;
    }

    // rotate right by incrementing direction enum
    bool right()
    {
        if (!placed())
            return false;

        if (static_cast<int>(direction) + 1 ==
                static_cast<int>(DIRECTION::INVALID))
            direction = DIRECTION::NORTH;
        else direction = static_cast<DIRECTION>
                (static_cast<int> (direction) + 1);
        
        return true;
    }

    // place on given position and direction if valid
    bool place(Vec2Di pos, DIRECTION dir)
    {
        if (dir == DIRECTION::INVALID)
        {
            std::cout << "Invalid Direction!" << std::endl;
            return false;
        }

        if (pos.x < 0 || pos.x >= GRID_SIZE || pos.y < 0 || pos.y >= GRID_SIZE)
        {
            std::cout << "Invalid Position!" << std::endl;
            return false;
        }

        position = pos;
        direction = dir;

        return true;
    }

    // print position and direction
    void report()
    {
        if (!placed())
            return;

        std::cout << position.x << ", " << position.y << ", " << dirToStr(direction) << std::endl;
    }

    // print a grid with an arrow to mark robot position and direction
    void print_position()
    {
        if (!placed())
            return;

        auto marker = std::string();

        switch (direction)
        {
            case DIRECTION::NORTH: marker = "↑"; break;
            case DIRECTION::SOUTH: marker = "↓"; break;
            case DIRECTION::EAST: marker = "→"; break;
            case DIRECTION::WEST: marker = "←"; break;
            default: return;
        }

        for (int y = GRID_SIZE-1; y >= 0; --y)
        {
            for (int x = 0; x < GRID_SIZE; ++x)
            {
                std::cout << "[" << ((x == position.x && y == position.y) ? marker : " ") << "]";
            }
            std::cout << std::endl;
        }
    }
};

int main()
{
    auto chief = Robot();
    auto command = std::string();

    while (command != "quit")
    {
        std::string input;

        // get a command from std input
        std::getline(std::cin, input);

        // remove white spaces
        input = std::regex_replace(input, std::regex(" "), "");

        // Any valid command will match this pattern after whitespaces are removed
        auto input_regex = std::regex(
            "((move|left|right|report|quit)(\\(\\))?)|(place\\([0-9]+\\,[0-9]+,[a-zA-Z]+\\))"
        );

        if (!std::regex_match(input, input_regex))
        {
            std::cout << "Invalid command!" << std::endl;
            continue;
        }

        // Use this pattern to tokenise command into words / numbers
        auto tokens_regex = std::regex("([a-zA-Z]+)|([0-9]+)");
        auto token_itr = std::sregex_iterator(input.begin(), input.end(), tokens_regex);

        // this will be one of move/place/etc
        command = token_itr->str();

        // to remember whether the command was accepted for output
        auto result = false;

        if (command == "place")
        {
            token_itr++;
            auto x = std::atoi(token_itr->str().c_str());
            token_itr++;
            auto y = std::atoi(token_itr->str().c_str());
            token_itr++;
            auto direction = strToDir(token_itr->str());
            result = chief.place(Vec2Di(x, y), direction);
        }

        else if (command == "move")
            result = chief.move();

        else if (command == "left")
            result = chief.left();

        else if (command == "right")
            result = chief.right();
  
        else if (command == "report")
            chief.report();

        else if (command == "quit")
            std::cout << "Bye!" << std::endl;

        if (result)
            chief.print_position();
    }

    return 0;
}
