#include "othello.h"
#include "cgt_basics.h"
#include "cgt_move.h"
#include "game.h"
#include "grid.h"

#define MOVE_AREA 0x003FFFFF
#define DIR_AREA  0x3FC00000

using namespace std;


class othello_move_generator : public move_generator
{
public:
    othello_move_generator(const othello& game, bw to_play);

    void operator++() override;
    operator bool() const override;
    ::move gen_move() const override;

private:
    const othello& _game;
    int _current; // current stone location to test
};

othello_move_generator::othello_move_generator(const othello& game, bw to_play)
    : move_generator(to_play),
    _game(game)
{
    
}

void othello_move_generator::operator++()
{
    
}

othello_move_generator::operator bool() const
{
    return false;
}

::move othello_move_generator::gen_move() const
{
    return 0;
}


othello::othello(int n_rows, int n_cols) : grid(n_rows, n_cols)
{
}

othello::othello(const vector<int>& board, int_pair shape) : grid(board, shape)
{
}

othello::othello(const string& game_as_string) : grid(game_as_string)
{
}

void othello::play(const ::move& m, bw to_play)
{
    grid::play(m, to_play);

    const int to = m & MOVE_AREA;
    int dirs = m & DIR_AREA;
    assert(at(to) == EMPTY);

    replace(to, to_play);

    const bw opp = opponent(to_play);

    // ray cast
    int_pair rc = point_to_coord(m);
    int_pair grid_shape = shape();
    std::vector<int> to_capture;
    bool should_capture;

    // inc row
    if (dirs & (1 << 0))
    {
        should_capture = false;
        to_capture.clear();
        for (int r = rc.first + 1; coord_in_bounds(int_pair(r, rc.second)); r++)
        {
            int coord = coord_to_point(int_pair(r, rc.second));
            if (at(coord) == opp)
            {
                to_capture.push_back(coord);
                continue;
            }
            else if (at(coord) == to_play)
            {
                should_capture = true;
                break;
            }
            else if (at(coord) == EMPTY)
            {
                break;
            }
        }
        if (should_capture and (not to_capture.empty()))
        {
            for (int& i : to_capture)
            {
                replace(i, to_play);
            }
        }
    }

    // dec row
    if (dirs & (1 << 1))
    {
        should_capture = false;
        to_capture.clear();
        for (int r = rc.first - 1; coord_in_bounds(int_pair(r, rc.second)); r--)
        {
            int coord = coord_to_point(int_pair(r, rc.second));
            if (at(coord) == opp)
            {
                to_capture.push_back(coord);
                continue;
            }
            else if (at(coord) == to_play)
            {
                should_capture = true;
                break;
            }
            else if (at(coord) == EMPTY)
            {
                break;
            }
        }
        if (should_capture and (not to_capture.empty()))
        {
            for (int& i : to_capture)
            {
                replace(i, to_play);
            }
        }
    }

    // inc col
    if (dirs & (1 << 2))
    {
        should_capture = false;
        to_capture.clear();
        for (int c = rc.second + 1; coord_in_bounds(int_pair(rc.first, c)); c++)
        {
            int coord = coord_to_point(int_pair(rc.first, c));
            if (at(coord) == opp)
            {
                to_capture.push_back(coord);
                continue;
            }
            else if (at(coord) == to_play)
            {
                should_capture = true;
                break;
            }
            else if (at(coord) == EMPTY)
            {
                break;
            }
        }
        if (should_capture and (not to_capture.empty()))
        {
            for (int& i : to_capture)
            {
                replace(i, to_play);
            }
        }
    }

    // dec col
    if (dirs & (1 << 3))
    {
        should_capture = false;
        to_capture.clear();
        for (int c = rc.second - 1; coord_in_bounds(int_pair(rc.first, c)); c--)
        {
            int coord = coord_to_point(int_pair(rc.first, c));
            if (at(coord) == opp)
            {
                to_capture.push_back(coord);
                continue;
            }
            else if (at(coord) == to_play)
            {
                should_capture = true;
                break;
            }
            else if (at(coord) == EMPTY)
            {
                break;
            }
        }
        if (should_capture and (not to_capture.empty()))
        {
            for (int& i : to_capture)
            {
                replace(i, to_play);
            }
        }
    }

    // inc row and col
    if (dirs & (1 << 4))
    {
        should_capture = false;
        to_capture.clear();
        for (int r = rc.first + 1, c = rc.second + 1; coord_in_bounds(int_pair(r, c)); r++, c++)
        {
            int coord = coord_to_point(int_pair(r, rc.second));
            if (at(coord) == opp)
            {
                to_capture.push_back(coord);
                continue;
            }
            else if (at(coord) == to_play)
            {
                should_capture = true;
                break;
            }
            else if (at(coord) == EMPTY)
            {
                break;
            }
        }
        if (should_capture and (not to_capture.empty()))
        {
            for (int& i : to_capture)
            {
                replace(i, to_play);
            }
        }
    }

    // inc row, dec col
    if (dirs & (1 << 5))
    {
        should_capture = false;
        to_capture.clear();
        for (int r = rc.first + 1, c = rc.second - 1; coord_in_bounds(int_pair(r, c)); r++, c--)
        {
            int coord = coord_to_point(int_pair(r, rc.second));
            if (at(coord) == opp)
            {
                to_capture.push_back(coord);
                continue;
            }
            else if (at(coord) == to_play)
            {
                should_capture = true;
                break;
            }
            else if (at(coord) == EMPTY)
            {
                break;
            }
        }
        if (should_capture and (not to_capture.empty()))
        {
            for (int& i : to_capture)
            {
                replace(i, to_play);
            }
        }
    }

    // dec row, inc col
    if (dirs & (1 << 6))
    {
        should_capture = false;
        to_capture.clear();
        for (int r = rc.first - 1, c = rc.second + 1; coord_in_bounds(int_pair(r, c)); r--, c++)
        {
            int coord = coord_to_point(int_pair(r, rc.second));
            if (at(coord) == opp)
            {
                to_capture.push_back(coord);
                continue;
            }
            else if (at(coord) == to_play)
            {
                should_capture = true;
                break;
            }
            else if (at(coord) == EMPTY)
            {
                break;
            }
        }
        if (should_capture and (not to_capture.empty()))
        {
            for (int& i : to_capture)
            {
                replace(i, to_play);
            }
        }
    }

    // dec row and col
    if (dirs & (1 << 7))
    {
        should_capture = false;
        to_capture.clear();
        for (int r = rc.first - 1, c = rc.second - 1; coord_in_bounds(int_pair(r, c)); r--, c--)
        {
            int coord = coord_to_point(int_pair(r, rc.second));
            if (at(coord) == opp)
            {
                to_capture.push_back(coord);
                continue;
            }
            else if (at(coord) == to_play)
            {
                should_capture = true;
                break;
            }
            else if (at(coord) == EMPTY)
            {
                break;
            }
        }
        if (should_capture and (not to_capture.empty()))
        {
            for (int& i : to_capture)
            {
                replace(i, to_play);
            }
        }
    }
}

void othello::undo_move()
{
    const ::move mc = grid::last_move();
    grid::undo_move();

    const int to = cgt_move::decode(mc);
    const bw player = cgt_move::get_color(mc);
    assert(at(to) == player);
    const int N = size();

    replace(to, EMPTY);
    
}

move_generator* othello::create_move_generator(bw to_play) const
{
    return new othello_move_generator(*this, to_play);
}
