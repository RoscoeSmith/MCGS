#include "dots_and_boxes.h"
#include "game.h"
#include "scoring_game.h"


#include <vector>
#include <ostream>
#include <string>
#include <cassert>


/*
for dots and boxes the moves will be structurd as follows:
    bit 31 = color bit (fromt cgt_move)
    bit 30 = sign bit (fromt cgt_move)
    bit 29 = capture bit
    bits 0-28 position indicating which line we are placing
*/


//---------------------------------------------------------------------------

namespace {

dots_and_boxes_state string_to_board(const std::string& game_as_string)
{
    std::vector<bool> vertical, horizontal;
    std::vector<int> boxes;
    int n_rows = 0, n_cols = 0, dim = 0;
    for (const char& c : game_as_string)
    {
        check_is_valid_char(c);
        int color = char_to_color(c);
        if (color == LINE_SEP)
        {
            if (dim == 0)
            {
                n_rows++;
            }
            else if (dim == 1)
            {
                n_cols++;
            }
        }
        else if (color == DIM_SEP)
        {
            dim++;
        }
        else
        {
            if (dim == 0)
            {
                if (color != EMPTY)
                    horizontal.push_back(true);
                else
                    horizontal.push_back(EMPTY);
            }
            else if (dim == 1)
            {
                if (color != EMPTY)
                    vertical.push_back(true);
                else
                    vertical.push_back(EMPTY);
            }
            else if (dim == 2)
            {
                boxes.push_back(color)
            }
            counter++;
        }
    }

    int_pair shape = {n_rows, n_cols};
    return {horizontal, vertical, boxes, shape};

}

std::string board_to_string(const std::vector<bool>& horizontal,const std::vector<bool>& vertical, const std::vector<int>& boxes, const int_pair shape)
{
    std::string result;
    int n_rows = shape.first, n_cols = shape.second;

    // TODO
}

} // namespace

//---------------------------------------------------------------------------

class dots_and_boxes_move_generator : public move_generator
{
public:
    dots_and_boxes_move_generator(const dots_and_boxes& game, bw to_play);

    void operator++() override;
    operator bool() const override;
    ::move gen_move() const override;


private:

    void _next_move();
    
    bool _increment();

    const dots_and_boxes& _game;

    int _location;
    bool _has_move;

};

dots_and_boxes_move_generator::dots_and_boxes_move_generator(const dots_and_boxes& game, bw to_play)
    : move_generator(to_play),
    _game(game),
    _location(0),
    _has_move(false)
{

    if(_game.shape.first > 0 and _game.shape.second > 0)
        _next_move();
    
}

void dots_and_boxes_move_generator::operator++()
{
    assert(*this);
    _next_move();
}

dots_and_boxes_move_generator::operator bool() const
{
    return _has_move;
}

::move dots_and_boxes_move_generator::gen_move() const
{

    int n_rows = _game.shape.first, n_cols = _game.shape.second;

    assert(*this);
    assert(_location < n_rows*(n_cols + 1) + n_cols(n_rows +1));



}

void dots_and_boxes_move_generator::_next_move()
{

    assert(*this);

    int n_rows = _game.shape.first, n_cols = _game.shape.second, local_location;

    _has_move = false;

    if(++_location >= n_rows*(n_cols + 1) + n_cols(n_rows + 1))
        return;

    while(true){

        if()

        if(_location < n_rows*(n_cols + 1) + n_cols(n_rows + 1) && /* this move hasn't been played*/)
        {
            break;
        }

    }


}


dots_and_boxes::dots_and_boxes(int n_rows, int n_cols) : scoring_game()
{

    assert(n_rows*(n_cols + 1) + n_cols(n_rows + 1) < 1073741824); // otherwise the move won't fit

    _horizontal = std::vector<bool>(n_cols * (n_rows + 1), false);
    _vertical = std::vector<bool>(n_rows * (n_cols + 1), false);
    _boxes = std::vector<int>(n_rows * n_cols, 0);

    _left_score = 0;
    _right_score = 0;

    _shape.first = n_rows;
    _shape.second = n_cols;

}

dots_and_boxes::dots_and_boxes(const std::string& game_as_string) : scoring_game()
{
    dots_and_boxes_state state = string_to_board(game_as_string);

    // make sure the moves will fit into the 29 bits we use for position of the move
    int n_rows = state.shape.first, n_cols = state.shape.second;
    assert(n_rows*(n_cols + 1) + n_cols(n_rows + 1) < 536870912); // 536870912 = 2^29

    _horizontal = state.horizontal;
    _vertical = state.vertical;
    _boxes = state.boxes;
    _shape = state.shape;
}

