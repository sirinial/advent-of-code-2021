#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct Dice
{
    int value = 0;

    int roll()
    {
        ++value;
        return value;
    }
};

struct Player
{
    int id;

    int pos;

    void move(int p)
    {
        pos = (pos + p) % 10;

        if(pos == 0)
        {
            pos = 10;
        }
        points += pos;
    }

    void play(Dice& dice)
    {
        int value = dice.roll() + dice.roll() + dice.roll();
        move(value);
    }

    bool wins() const
    {
        return points >= 1000;
    }
    int points = 0;
};

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Missing argument!" << std::endl;
        return 1;
    }
    const std::string fileName = argv[1];

    std::ifstream infile(fileName);
    std::cout << "Reading " << fileName << std::endl;

    std::string line;
    while(getline(infile, line))
    {
        std::cout << line << std::endl;
    }
    Dice   dice;
    Player p1;
    p1.id = 1;
    Player p2;
    p2.id = 2;

    p1.pos = 5;
    p2.pos = 10;

    while(p1.wins() == false && p2.wins() == false)
    {
        p1.play(dice);
        if(p1.wins())
        {
            std::cout << "Player 1 wins:" << std::endl;
            std::cout << "   Player 1:" << p1.points << std::endl;
            std::cout << "   Player 2:" << p2.points << std::endl;

            std::cout << (p2.points * dice.value) << std::endl;
            return 0;
        }

        p2.play(dice);

        if(p2.wins())
        {
            std::cout << "Player 2 wins:" << std::endl;
            std::cout << "   Player 1:" << p1.points << std::endl;
            std::cout << "   Player 2:" << p2.points << std::endl;
            std::cout << (p1.points * dice.value) << std::endl;

            return 0;
        }
    }

    return 0;
}
