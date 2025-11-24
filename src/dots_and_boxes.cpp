#include "dots_and_boxes.h"
#include "game.h"
#include "scoring_game.h"


#include <vector>
#include <ostream>
#include <string>
#include <iostream>
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
const int LINE_SEP = 3;
const int DIM_SEP = 4;

const int get_total_moves(const int& rows, const int& cols)
{
    return rows * (cols + 1) + cols * (rows + 1);
}

void check_is_valid_char(char c)
{
    THROW_ASSERT(c == 'X' || c == 'O' || c == '.' || c == '$' || c == '|');
}

int char_to_color(char c)
{
    if (c == 'X')
        return BLACK;
    else if (c == 'O')
        return WHITE;
    else if (c == '.')
        return EMPTY;
    else if (c == '|')
        return LINE_SEP;
    else if (c == '$')
        return DIM_SEP;
    else
        assert(false);

    exit(-1);
    return -1;
}

int color_to_char(int color)
{
    static char db_char[] = {'X', 'O', '.', '|', '$'};

    assert_range(color, BLACK, DIM_SEP + 1);
    return db_char[color];
}

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
                boxes.push_back(color);
            }
        }
    }

    int_pair shape = {n_rows, n_cols};
    return {horizontal, vertical, boxes, shape};

}

std::string board_to_string(const std::vector<bool>& horizontal,const std::vector<bool>& vertical, const std::vector<int>& boxes, const int_pair shape)
{
    std::string result;
    int n_rows = shape.first, n_cols = shape.second;
    // add horizontal lines
    for (int h = 0; h < n_rows + 1; h++)
    {
        for (int c = 0; c < n_cols; c++)
        {
        if (horizontal[h * n_cols + c])
            result += color_to_char(BLACK);
        }
        if (h != n_rows)
            result += color_to_char(LINE_SEP);
    }
    result += '$';
    // add vertical lines
    for (int v = 0; v < n_cols + 1; v++)
    {
        for (int r = 0; r < n_rows; r++)
        {
        if (vertical[r * n_rows + r])
            result += color_to_char(BLACK);
        }
        if (v != n_cols)
            result += color_to_char(LINE_SEP);
    }
    result += '$';
    // add box colorings
    for (int b = 0; b < n_rows * n_cols; b++)
    {
        result += color_to_char(boxes[b]);
    }
    return result;

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

    if(_game.get_shape().first > 0 and _game.get_shape().second > 0)
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

    int n_rows = _game.get_shape().first, n_cols = _game.get_shape().second;

    assert(*this);
    assert(_location < get_total_moves(n_rows, n_cols));

    // first check if the move is a capture move




}

void dots_and_boxes_move_generator::_next_move()
{

    assert(*this);

    int n_rows = _game.get_shape().first, n_cols = _game.get_shape().second;

    _has_move = false;

    if(++_location >= get_total_moves(n_rows, n_cols))
        return;

    while(true){

        if(_location < get_total_moves(n_rows, n_cols) || !has_been_played(_location))
        {
            break;
        }

        _location ++;

    }


}


dots_and_boxes::dots_and_boxes(int n_rows, int n_cols) : scoring_game()
{

    assert(get_total_moves(n_rows, n_cols) < 1073741824); // otherwise the move won't fit

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
    std::cout << "in d&b string constructor" << std::endl;
    dots_and_boxes_state state = string_to_board(game_as_string);

    // make sure the moves will fit into the 29 bits we use for position of the move
    int n_rows = state.shape.first, n_cols = state.shape.second;
    assert(get_total_moves(n_rows, n_cols) < 536870912); // 536870912 = 2^29

    _horizontal = state.horizontal;
    _vertical = state.vertical;
    _boxes = state.boxes;
    _shape = state.shape;

    std::cout << "got state\nhorizontal array:" << std::endl;
    for (auto x : _horizontal)
    {
        std::cout << "  " << int(x);
    }
    std::cout << std::endl << "vertical array:" << std::endl;
    for (auto x : _vertical)
    {
        std::cout << "  " << int(x);
    }
    std::cout << std::endl << "boxes array:" << std::endl;
    for (auto x : _boxes)
    {
        std::cout << "  " << int(x);
    }
    std::cout << std::endl;
}

move_generator* dots_and_boxes::create_move_generator(bw to_play) const
{
    return new dots_and_boxes_move_generator(*this, to_play);
}

bool dots_and_boxes::has_been_played(int position) const{

    int n_rows = _game.shape.first, n_cols = _game.shape.second;

    assert(position < n_rows*(n_cols + 1) + n_cols(n_rows +1));

    if(position < n_rows*(n_cols + 1))
    {
        return _vertical.at(position);
    }
    else
    {
        return _horizontal.at(position - n_rows*(n_cols + 1));
    }

}

game* dots_and_boxes::inverse() const
{
    assert(false);
}

int dots_and_boxes::count_score() const
{
    return _left_score - _right_score;
}

const int dots_and_boxes::_get_total_moves() const
{
    return get_total_moves(_shape.first, _shape.second);
}

const int_pair dots_and_boxes::get_shape() const
{
    return _shape;
}

void dots_and_boxes::_init_hash(local_hash& hash) const
{
    
}

void dots_and_boxes::play(const move& m, bw to_play)
{
    
}

void dots_and_boxes::undo_move()
{
    
}

void dots_and_boxes::print(std::ostream& str) const
{
    
}

