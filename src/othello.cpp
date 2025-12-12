#include "othello.h"
#include "cgt_basics.h"
#include "cgt_move.h"
#include "game.h"
#include "grid.h"
#include <vector>

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

int othello::count_score(bw to_play) const
{
    int black_score = 0;
    int white_score = 0;

    for (int& v : board())
    {
        switch (v)
        {
            case BLACK:
                ++black_score;
                break;
            case WHITE:
                ++white_score;
                break;
        }
    }

    int score = black_score - white_score;
    if (to_play == WHITE)
    {
        score = -score;
    }
    return score;
}

bool othello::is_terminal() const
{
    const std::vector<::move> move_stack = grid::get_move_stack();
    if (move_stack.size() < 2)
        return false;
    if ((move_stack.at(move_stack.size() - 1) & 0x3FFFFFFF) == 0 and (move_stack.at(move_stack.size() - 2) & 0x3FFFFFFF) == 0)
        return true;
    return false;
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
            int coord = coord_to_point(int_pair(r, c));
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
            int coord = coord_to_point(int_pair(r, c));
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
            int coord = coord_to_point(int_pair(r, c));
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
            int coord = coord_to_point(int_pair(r, c));
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

    const int to = mc & MOVE_AREA;
    int dirs = mc & DIR_AREA;
    
    const bw player = cgt_move::get_color(mc);
    const bw opp = opponent(player);
    assert(at(to) == player);

    // raycast
    int_pair rc = point_to_coord(mc);
    int_pair grid_shape = shape();

    // inc row
    if (dirs & (1 << 0))
    {
        for (int r = rc.first + 2; coord_in_bounds(int_pair(r, rc.second)); r++)
        {
            if (at(coord_to_point(int_pair(r, rc.second))) != player)
            {
                break;
            }
            else
            {
                replace(coord_to_point(int_pair(r - 1, rc.second)), opp);
            }
        }
    }

    // dec row
    if (dirs & (1 << 0))
    {
        for (int r = rc.first - 2; coord_in_bounds(int_pair(r, rc.second)); r--)
        {
            if (at(coord_to_point(int_pair(r, rc.second))) != player)
            {
                break;
            }
            else
            {
                replace(coord_to_point(int_pair(r + 1, rc.second)), opp);
            }
        }
    }

    // inc col
    if (dirs & (1 << 0))
    {
        for (int c = rc.second + 2; coord_in_bounds(int_pair(rc.first, c)); c++)
        {
            if (at(coord_to_point(int_pair(rc.first, c))) != player)
            {
                break;
            }
            else
            {
                replace(coord_to_point(int_pair(rc.first, c - 1)), opp);
            }
        }
    }

    // dec col
    if (dirs & (1 << 0))
    {
        for (int c = rc.second - 2; coord_in_bounds(int_pair(rc.first, c)); c--)
        {
            if (at(coord_to_point(int_pair(rc.first, c))) != player)
            {
                break;
            }
            else
            {
                replace(coord_to_point(int_pair(rc.first, c + 1)), opp);
            }
        }
    }

    // inc row and col
    if (dirs & (1 << 0))
    {
        for (int r = rc.first + 2, c = rc.second + 2; coord_in_bounds(int_pair(r, c)); r++, c++)
        {
            if (at(coord_to_point(int_pair(r, c))) != player)
            {
                break;
            }
            else
            {
                replace(coord_to_point(int_pair(r - 1, c - 1)), opp);
            }
        }
    }

    // inc row, dec col
    if (dirs & (1 << 0))
    {
        for (int r = rc.first + 2, c = rc.second - 2; coord_in_bounds(int_pair(r, c)); r++, c--)
        {
            if (at(coord_to_point(int_pair(r, c))) != player)
            {
                break;
            }
            else
            {
                replace(coord_to_point(int_pair(r - 1, c + 1)), opp);
            }
        }
    }

    // dec row, inc col
    if (dirs & (1 << 0))
    {
        for (int r = rc.first - 2, c = rc.second + 2; coord_in_bounds(int_pair(r, c)); r--, c++)
        {
            if (at(coord_to_point(int_pair(r, c))) != player)
            {
                break;
            }
            else
            {
                replace(coord_to_point(int_pair(r + 1, c - 1)), opp);
            }
        }
    }

    // dec row and col
    if (dirs & (1 << 0))
    {
        for (int r = rc.first - 2, c = rc.second - 2; coord_in_bounds(int_pair(r, c)); r--, c--)
        {
            if (at(coord_to_point(int_pair(r, c))) != player)
            {
                break;
            }
            else
            {
                replace(coord_to_point(int_pair(r + 1, c + 1)), opp);
            }
        }
    }

    replace(to, EMPTY);
    
}

move_generator* othello::create_move_generator(bw to_play) const
{
    return new othello_move_generator(*this, to_play);
}
