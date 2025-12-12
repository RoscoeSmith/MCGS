#include "othello.h"
#include "cgt_basics.h"
#include "cgt_move.h"
#include "game.h"
#include "grid.h"

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
};

othello_move_generator::othello_move_generator(const othello& game, bw to_play)
    : move_generator(to_play),
    _game(game),
{
    _current = -1;
    _hash_move = false;
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
    return _current;
}

void othello_move_generator::_next_move(bool inti)
{

    assert(init || *this);

    _has_move = false;

    _current ++;

    int_pair cord;

    while(_current < _game.size())
    {

        cord = point_to_coord(_current);

        for(int i = cord.first + 1; coord_in_bounds({cord.first, i}), i++)
        {

        }

        _current ++;
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

    const int to = m;
    assert(at(to) == EMPTY);

    replace(to, to_play);

    const bw opp = opponent(to_play);
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
