#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using int64 = std::int64_t;

std::map<int, int64> createPossiblesRolls()
{
    std::map<int, int64> result;
    for(int i = 1; i <= 3; ++i)
    {
        for(int j = 1; j <= 3; ++j)
        {
            for(int k = 1; k <= 3; ++k)
            {
                int key = i + j + k;

                if(result.find(key) == result.end())
                {
                    result.emplace(key, 1);
                }
                else
                {
                    ++result[key];
                }
            }
        }
    }
    return result;
}
const std::map<int, int64> gPossibleRolls = createPossiblesRolls();

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

    std::vector<std::pair<Player, int64>> play() const
    {
        std::vector<std::pair<Player, int64>> result;
        for(const std::pair<int, int64>& roll : gPossibleRolls)
        {
            Player p = *this;

            p.move(roll.first);
            result.emplace_back(p, roll.second);
        }
        return result;
    }

    bool wins() const
    {
        return points >= 21;
    }
    int points = 0;
};

std::ostream& operator<<(std::ostream& o, const Player& p)
{
    o << "Player " << p.id << ": " << p.points << ", " << p.pos;
    return o;
}

struct ScoringField
{
    int64 p1Wins = 0;
    int64 p2Wins = 0;

    ScoringField& operator+=(const ScoringField& other)
    {
        p1Wins += other.p1Wins;
        p2Wins += other.p2Wins;
        return *this;
    }
    ScoringField& multiply(const int64& other)
    {
        p1Wins = p1Wins * other;
        p2Wins = p2Wins * other;
        return *this;
    }
};
struct Playfield
{
    int lastPlayer = 2;

    Player p1;
    Player p2;

    std::vector<std::pair<Playfield, int64>> playPlayer1() const
    {
        std::vector<std::pair<Playfield, int64>> result;
        for(const std::pair<Player, int64>& nextPlayer : p1.play())
        {
            Playfield r = *this;

            r.p1         = nextPlayer.first;
            r.lastPlayer = 1;

            result.emplace_back(r, nextPlayer.second);
        }

        return result;
    }
    std::vector<std::pair<Playfield, int64>> playPlayer2() const
    {
        std::vector<std::pair<Playfield, int64>> result;
        for(const std::pair<Player, int64>& nextPlayer : p2.play())
        {
            Playfield r = *this;

            r.p2         = nextPlayer.first;
            r.lastPlayer = 2;

            result.emplace_back(r, nextPlayer.second);
        }

        return result;
    }
};

std::ostream& operator<<(std::ostream& o, const Playfield& pf)
{
    o << "{" << pf.p1 << "," << pf.p2 << ", last player: " << pf.lastPlayer << "}";
    return o;
}

ScoringField playTurns(const Playfield& pfield)
{
    ScoringField result;
    if(pfield.p1.wins())
    {
        ++result.p1Wins;
        return result;
    }

    if(pfield.p2.wins())
    {
        ++result.p2Wins;
        return result;
    }

    std::vector<std::pair<Playfield, int64>> nextFields;

    if(pfield.lastPlayer == 1)
    {
        nextFields = pfield.playPlayer2();
    }
    else
    {
        nextFields = pfield.playPlayer1();
    }

    for(const std::pair<Playfield, int64>& nf : nextFields)
    {
        result += playTurns(nf.first).multiply(nf.second);
    }
    return result;
}

int main(int argc, char* argv[])
{
    Playfield pf;

    Player& p1 = pf.p1;
    p1.id      = 1;

    Player& p2 = pf.p2;
    p2.id      = 2;

    p1.pos = 5;
    p2.pos = 10;

    ScoringField sf = playTurns(pf);

    std::cout << "Player 1: " << sf.p1Wins << std::endl;
    std::cout << "Player 2: " << sf.p2Wins << std::endl;

    return 0;
}
