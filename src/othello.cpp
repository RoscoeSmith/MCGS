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

    void _next_move(bool init);

    const othello& _game;
    int _current; // current stone location to test
    bool _has_move;
    bool _north, _north_east, _east, _south_east, _south, _south_west, _west, _north_west;

};

othello_move_generator::othello_move_generator(const othello& game, bw to_play)
    : move_generator(to_play),
    _game(game),
{
    _current = -1;
    _has_move = false;

    _south = false;
    _north = false;
    _east = false;
    _west = false;
    _south_east = false;
    _south_west = false;
    _north_east = false;
    _north_west = false;

    _next_move(true);
    
}

void othello_move_generator::operator++()
{
    assert(*this);
    _next_move(false);
}

othello_move_generator::operator bool() const
{
    return _has_move;
}

::move othello_move_generator::gen_move() const
{
    assert(*this)

    ::move oth_move = 0;

    // pass move
    if(!(_south || _north || _east || _west || _south_east || _south_west || _north_east || _north_west))
        return oth_move;

    oth_move |= _current;

    oth_move |= (_south ? 1 : 0) << 29;
    oth_move |= (_north ? 1 : 0) << 28;
    oth_move |= (_east ? 1 : 0) << 27;
    oth_move |= (_west ? 1 : 0) << 26;
    oth_move |= (_south_east ? 1 : 0) << 25;
    oth_move |= (_south_west ? 1 : 0) << 24;
    oth_move |= (_north_east ? 1 : 0) << 23;
    oth_move |= (_north_west ? 1 : 0) << 22;

    return oth_move;
}

void othello_move_generator::_next_move(bool inti)
{

    assert(init || *this);

    _has_move = false;

    _south = false;
    _north = false;
    _east = false;
    _west = false;
    _south_east = false;
    _south_west = false;
    _north_east = false;
    _north_west = false;


    _current ++;

    int_pair rc;

    bool should_capture = false;
    int cap_size = 0;

    while(_current < _game.size())
    {

        if(_game.at(_current) != EMPTY)
        {
            _current ++;
            continue;
        }

        rc = point_to_coord(_current);

        // inc row
        for (int r = rc.first + 1; coord_in_bounds(int_pair(r, rc.second)); r++)
        {
            int coord = coord_to_point(int_pair(r, rc.second));
            if (at(coord) == opp)
            {
                cap_size ++;
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
        if (should_capture and cap_size >= 1)
        {
            _south = true;
        }


        // dec row
        should_capture = false;
        cap_size = 0;
        for (int r = rc.first - 1; coord_in_bounds(int_pair(r, rc.second)); r--)
        {
            int coord = coord_to_point(int_pair(r, rc.second));
            if (at(coord) == opp)
            {
                cap_size ++;
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
        if (should_capture and cap_size >= 1)
        {
            _north = true;
        }

        // inc col
        should_capture = false;
        cap_size = 0;
        for (int c = rc.second + 1; coord_in_bounds(int_pair(rc.first, c)); c++)
        {
            int coord = coord_to_point(int_pair(rc.first, c));
            if (at(coord) == opp)
            {
                cap_size ++;
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
        if (should_capture and cap_size >= 1)
        {
            _east = true;
        }

        // dec col
        should_capture = false;
        cap_size = 0;
        for (int c = rc.second - 1; coord_in_bounds(int_pair(rc.first, c)); c--)
        {
            int coord = coord_to_point(int_pair(rc.first, c));
            if (at(coord) == opp)
            {
                cap_size ++;
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
        if (should_capture and cap_size >= 1)
        {
            _west = true;
        }

        // inc row and col
        should_capture = false;
        cap_size = 0;
        for (int r = rc.first + 1, c = rc.second + 1; coord_in_bounds(int_pair(r, c)); r++, c++)
        {
            int coord = coord_to_point(int_pair(r, c));
            if (at(coord) == opp)
            {
                cap_size ++;
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
        if (should_capture and cap_size >= 1)
        {
            _south_east = true;
        }

        // inc row, dec col
        should_capture = false;
        cap_size = 0;
        for (int r = rc.first + 1, c = rc.second - 1; coord_in_bounds(int_pair(r, c)); r++, c--)
        {
            int coord = coord_to_point(int_pair(r, c));
            if (at(coord) == opp)
            {
                cap_size ++;
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
        if (should_capture and cap_size >= 1)
        {
            _south_west = true;
        }

        // dec row, inc col
        should_capture = false;
        cap_size = 0;
        for (int r = rc.first - 1, c = rc.second + 1; coord_in_bounds(int_pair(r, c)); r--, c++)
        {
            int coord = coord_to_point(int_pair(r, c));
            if (at(coord) == opp)
            {
                cap_size ++;
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
        if (should_capture and cap_size >= 1)
        {
            _north_east = true;
        }

        // dec row and col
        should_capture = false;
        cap_size = 0;
        for (int r = rc.first - 1, c = rc.second - 1; coord_in_bounds(int_pair(r, c)); r--, c--)
        {
            int coord = coord_to_point(int_pair(r, c));
            if (at(coord) == opp)
            {
                cap_size ++;
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
        if (should_capture and cap_size >= 1)
        {
            _north_west = true;
        }

        if(_south || _north || _east || _west || _south_east || _south_west || _north_east || _north_west)
        {
            _has_move = true;
            break;
        }

        _current ++;
    }

    if(init && !_has_move)
    {
        _has_move = true;
    }
    
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
