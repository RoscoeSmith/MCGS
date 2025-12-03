#include "dots_and_boxes.h"
#include "game.h"
#include "scoring_game.h"
#include "cgt_move.h"
#include <unistd.h>


#include <vector>
#include <ostream>
#include <string>
#include <iostream>
#include <cassert>
#include <cctype>
#include <algorithm>


/*
for dots and boxes the moves will be structurd as follows:
    bit 31 = color bit (fromt cgt_move)
    bit 30 = sign bit (fromt cgt_move)
    bits 28-29 = capture bits (0, 1, or 2 boxes captured)
    bits 0-27 position indicating which line we are placing
*/

//---------------------------------------------------------------------------

namespace {
const int LINE_SEP = 3;
const int DIM_SEP = 4;

// gets the no. of possible edge move for a given board shape
const int get_total_moves(const int& rows, const int& cols)
{
    return rows * (cols + 1) + cols * (rows + 1);
}

void check_is_valid_char(char c)
{
    THROW_ASSERT(c == 'X' || c == 'O' || c == '.' || c == '#' || c == '|');
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
    else if (c == '#')
        return DIM_SEP;
    else
        assert(false);

    exit(-1);
    return -1;
}

int color_to_char(int color)
{
    static char db_char[] = {'X', 'O', '.', '|', '#'};

    assert_range(color, BLACK, DIM_SEP + 1);
    return db_char[color];
}

dots_and_boxes_state string_to_board(const std::string& game_as_string)
{
    std::vector<bool> vertical, horizontal;
    std::vector<int> boxes;
    unsigned int n_rows = 0, n_cols = 0, dim = 0;
    unsigned int n_cols_c = 0;

    if (game_as_string.at(0) == '@')
    {
        // row,col mode
        std::string number;
        for (const char& c : game_as_string.substr(1))
        {
            if (c == ',')
            {
                n_rows = stoi(number);
                number = "";
            } else
            {
                assert(isdigit(c));
                number += c;
            }
        }
        n_cols = stoi(number);
        for (unsigned int i = 0; i < n_rows * (n_cols + 1); i++)
        {
            vertical.push_back(false);
        }
        for (unsigned int i = 0; i < n_cols * (n_rows + 1); i++)
        {
            horizontal.push_back(false);
        }
        for (unsigned int i = 0; i < n_cols * n_rows; i++)
        {
            boxes.push_back(EMPTY);
        }
        
    } else {
        // string parse mode
        for (const char& c : game_as_string)
        {
            check_is_valid_char(c);
            int color = char_to_color(c);
            if (color == LINE_SEP)
            {
                if (dim == 0)  // vertical
                {
                    n_rows++;
                }
                else if (dim == 1)  // horizontal
                {
                    if (n_cols == 0)
                        n_cols = n_cols_c;
                    else
                        assert(n_cols == n_cols_c);
                    n_cols_c = 0;
                }
            }
            else if (color == DIM_SEP)
            {
                dim++;
            }
            else
            {
                if (dim == 0)  // vertical
                {
                    vertical.push_back(color != EMPTY);
                }
                else if (dim == 1)  // horizontal
                {
                    horizontal.push_back(color != EMPTY);
                    n_cols_c++;
                }
                else if (dim == 2)
                {
                    boxes.push_back(color); 
                }
            }
        }
        n_rows++;
    }

    // post-construction error checking
    assert(horizontal.size() == n_cols * (n_rows + 1));
    assert(vertical.size() == n_rows * (n_cols + 1));
    assert(boxes.size() == n_rows * n_cols);

    int_pair shape = {n_rows, n_cols};
    return {vertical, horizontal, boxes, shape};
}

std::string board_to_string(const std::vector<bool>& horizontal,const std::vector<bool>& vertical, const std::vector<int>& boxes, const int_pair& shape)
{
    std::string result;
    int n_rows = shape.first, n_cols = shape.second;
    // add horizontal lines
    for (int h = 0; h < n_cols * (n_rows + 1); h++)
    {
        if (h != 0 and h % n_cols == 0)
        {
            result += color_to_char(LINE_SEP);
        }
        result += color_to_char(horizontal[h] ? BLACK : EMPTY);
    }
    result += color_to_char(DIM_SEP);
    // add vertical lines
    for (int v = 0; v < n_rows * (n_cols + 1); v++)
    {
        if (v != 0 and v % n_rows == 0)
        {
            result += color_to_char(LINE_SEP);
        }
        result += color_to_char(vertical[v] ? BLACK : EMPTY);
    }
    result += color_to_char(DIM_SEP);
    // add box colorings
    for (int b = 0; b < n_rows * n_cols; b++)
    {
        result += color_to_char(boxes[b]);
    }
    return result;
}

} // namespace

//---------------------------------------------------------------------------

class dots_and_boxes_move_generator : public multimove_generator
{
public:
    dots_and_boxes_move_generator(const dots_and_boxes& game, bw to_play);

    void operator++() override;
    operator bool() const override;
    std::vector<::move> gen_multimove() const override;


private:

    void _next_move(bool init);
    
    const dots_and_boxes& _game;

    int _location;
    bool _has_move;

};

dots_and_boxes_move_generator::dots_and_boxes_move_generator(const dots_and_boxes& game, bw to_play)
    : multimove_generator(to_play),
    _game(game),
    _location(0),
    _has_move(false)
{

    if(_game.get_shape().first > 0 and _game.get_shape().second > 0)
        _next_move(true);
    
}

void dots_and_boxes_move_generator::operator++()
{
    assert(*this);
    _next_move(false);
}

dots_and_boxes_move_generator::operator bool() const
{
    return _has_move;
}

std::vector<::move> dots_and_boxes_move_generator::gen_multimove() const
{
    std::vector<::move> multimove;

    int n_rows = _game.get_shape().first, n_cols = _game.get_shape().second, num_captures = 0;

    assert(*this);
    assert(_location < get_total_moves(n_rows, n_cols));

    // first check if the move is a capture move
    if(_location < n_rows*(n_cols + 1)) // vertical move
    {
        if(_location % (n_cols + 1) > 0 && _game.left_capture(_location)) // not on the left side of the board, can check box to the left
            num_captures ++;
        
        if(_location % (n_cols + 1) < n_cols && _game.right_capture(_location)) // not on the right side of the board, can check the box to the right
            num_captures ++;
    }
    else // horizontal move, no more checks necessary since assert passed
    {
        if((_location - n_rows*(n_cols + 1)) >= n_cols && _game.up_capture(_location)) // not on the top side of the board, can check the box above
            num_captures++;
        
        if((_location - n_rows*(n_cols + 1)) < n_rows*n_cols && _game.down_capture(_location)) // not on the bottom of the board, can check box below
            num_captures ++;
    }

    multimove.push_back((num_captures << 28) | _location);

    return multimove;

} // TODO: rewrite to generate multimoves

void dots_and_boxes_move_generator::_next_move(bool init)
{

    assert(init || *this);

    std::cout << "in _next_move start " << _location << " " << _has_move << "\n" << _game.pretty_print() << std::endl;

    int n_rows = _game.get_shape().first, n_cols = _game.get_shape().second;

    _has_move = false;

    if(!init && ++_location > get_total_moves(n_rows, n_cols))
        return;


    while(_location < get_total_moves(n_rows, n_cols))
    {

        if(!_game.has_been_played(_location))
        {
            _has_move = true;
            break;
        }

        _location ++;

    }

    std::cout << "in _next_move end " << _location << " " << _has_move << "\n\n" << std::endl;


}


dots_and_boxes::dots_and_boxes(int n_rows, int n_cols) : scoring_game()
{

    assert(get_total_moves(n_rows, n_cols) < MOVE_LIMIT);
    
    _vertical = std::vector<bool>(n_rows * (n_cols + 1), false);
    _horizontal = std::vector<bool>(n_cols * (n_rows + 1), false);
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
    assert(get_total_moves(n_rows, n_cols) < MOVE_LIMIT);
    
    _vertical = state.vertical;
    _horizontal = state.horizontal;
    _boxes = state.boxes;
    _shape = state.shape;

    _left_score = 0;
    _right_score = 0;

    for(int& i : _boxes)
        if(i == BLACK)
        {
            _left_score ++;
        }
        else if(i == WHITE)
        {
            _right_score ++;
        }

    _assert_valid_state();

    std::cout << "pretty-printing...\n" << pretty_print() << std::endl;

    std::cout << "shape: " << _shape.first << ", " << _shape.second << std::endl;
}

move_generator* dots_and_boxes::create_move_generator(bw to_play) const
{
    return new dots_and_boxes_move_generator(*this, to_play);
}

bool dots_and_boxes::has_been_played(int position) const{

    int n_rows = _shape.first, n_cols = _shape.second;

    assert(position < get_total_moves(n_rows, n_cols));

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

    hash.toggle_value(0, _shape.first);
    hash.toggle_value(1, _shape.second);

    int num_hashes = 2;

    for(bool a : _vertical)
    {
        if(a)
            hash.toggle_value(num_hashes, static_cast<int>(a));
        
        num_hashes ++;
    }

    for(bool a : _horizontal)
    {
        if(a)
            hash.toggle_value(num_hashes, static_cast<int>(a));

        num_hashes ++;
    }

    for(int a : _boxes)
    {
        if(a != EMPTY)
            hash.toggle_value(num_hashes, a);

        num_hashes ++;
    }
    
}

void dots_and_boxes::play(const move& m, bw to_play)
{

    std::cout << "in play call" << std::endl;
    game::play(m, to_play);

    unsigned int pos = m & (MOVE_LIMIT - 1);
    int num_captures = (m >> 28) & 3;


    std::cout << "in play start " << pos << " " << num_captures << " " << _left_score << " " << _right_score << "\n" << this->pretty_print() << "\n" << std::endl;


    if(pos < _vertical.size())
    {
        assert(!_vertical.at(pos));
        _vertical.at(pos) = true;

        if(num_captures != 0)
        {
            if(pos % (_shape.second + 1) > 0 && _left_capture(pos)) // not on the left side of the board, can check box to the left
                _boxes.at(pos - pos/(_shape.second + 1) - 1) = to_play;

            if(pos % (_shape.second + 1) < static_cast<unsigned int>(_shape.second) && _right_capture(pos)) // not on the right side of the board, can check the box to the right
                _boxes.at(pos - pos/(_shape.second + 1)) = to_play;
        }
    }
    else
    {
        assert(!_horizontal.at(pos - _vertical.size()));
        _horizontal.at(pos - _vertical.size()) = true;

        if(num_captures != 0)
        {
            if((pos - _vertical.size()) >= static_cast<unsigned int>(_shape.second) && _up_capture(pos)) // not on the top side of the board, can check the box above
                _boxes.at(pos - _vertical.size() - _shape.second) = to_play;

            if((pos - _vertical.size()) < static_cast<unsigned int>(_shape.first*_shape.second) && _down_capture(pos)) // not on the bottom of the board, can check box below
                _boxes.at(pos - _vertical.size()) = to_play;
        }
    }

    if(to_play == BLACK)
    {        
        _left_score += num_captures;
    }
    else
    {        
        _right_score += num_captures;
    }

    std::cout << "in play end " << pos << " " << num_captures << " " << _left_score << " " << _right_score << "\n" << this->pretty_print() << "\n\n" << std::endl;

    std::cout << get_local_hash() << std::endl;

}

void dots_and_boxes::undo_move()
{
    // TODO: rewrite to undo multimoves properly

    std::vector<::move> mv = last_multimove();
    for (std::size_t i = 0; i < mv.size(); i++)
    {
        // ::move m = last_move();
        ::move m = mv.at(i);

        game::undo_move();

        bw to_play = cgt_move::get_color(m);

        unsigned int pos = m & (MOVE_LIMIT - 1);
        int num_captures = (m >> 28) & 3;

        std::cout << "in undo start " << pos << " " << num_captures << " " << _left_score << " " << _right_score << "\n" << this->pretty_print() << "\n" << std::endl;


        if(pos < _vertical.size())
        {
            assert(_vertical.at(pos));
            _vertical.at(pos) = false;

            if(num_captures != 0)
            {
                if(pos % (_shape.second + 1) > 0) // not on the left side of the board
                    _boxes.at(pos - pos/(_shape.second + 1) - 1) = EMPTY;

                if(pos % (_shape.second + 1) < static_cast<unsigned int>(_shape.second)) // not on the right side of the board
                    _boxes.at(pos - pos/(_shape.second + 1)) = EMPTY;
            }

        }
        else
        {
            assert(_horizontal.at(pos - _vertical.size()));
            _horizontal.at(pos - _vertical.size()) = false;


            if(num_captures != 0)
            {
                if((pos - _vertical.size()) >= static_cast<unsigned int>(_shape.second)) // not on the top side of the board
                    _boxes.at(pos - _vertical.size() - _shape.second) = EMPTY;

                if((pos - _vertical.size()) < static_cast<unsigned int>(_shape.first*_shape.second)) // not on the bottom of the board, can check box below
                    _boxes.at(pos - _vertical.size()) = EMPTY;
            }

        }

        if(to_play == BLACK)
        {
            _left_score -= num_captures;
        }
        else
        {
            _right_score -= num_captures;
        }
        
        std::cout << "in undo end " << pos << " " << num_captures << " " << _left_score << " " << _right_score << "\n" << this->pretty_print() << "\n\n" << std::endl;
    }
}

std::string dots_and_boxes::board_as_string() const
{
    return board_to_string(_horizontal, _vertical, _boxes, _shape);
}

void dots_and_boxes::print(std::ostream& str) const
{
    str << "dots_and_boxes:" << board_as_string();
}

std::string dots_and_boxes::pretty_print() const
{
    std::string result;
    for (int r = 0; r < _shape.first; r++)
    {
        // add horizontal lines
        for (int c = 0; c < _shape.second; c++)
        {
            result += '*';
            if (_horizontal[r * _shape.second + c])
                result += "---";
            else
                result += "   ";
        }
        result += "*\n";

        // add vertical lines
        for (int c = 0; c < _shape.second; c++)
        {
            if (_vertical[r * (_shape.second + 1) + c])
                result += '|';
            else
                result += ' ';
            // add box color
            switch (_boxes[r * _shape.second + c])
            {
                case BLACK:
                    result += " X ";
                    break;
                case WHITE:
                    result += " O ";
                    break;
                case EMPTY:
                    result += "   ";
                    break;
            }
        }
        if (_vertical[r * (_shape.second + 1) + _shape.second])
            result += "|\n";
        else
            result += " \n";
    }
    for (int c = 0; c < _shape.second; c++)
    {
        result += '*';
        if (_horizontal[_shape.first * _shape.second + c])
            result += "---";
        else
            result += "   ";
    }
    result += '*';

    return result;
}

const int dots_and_boxes::_get_edge_count(const int& position) const
{
    return static_cast<int>(_horizontal.at(position)) +
           static_cast<int>(_horizontal.at(position + _shape.second)) +
           static_cast<int>(_vertical.at(position - position/_shape.second)) +
           static_cast<int>(_vertical.at(position - position/_shape.second + 1));
}

const int dots_and_boxes::_get_edge_count(const int& row, const int& col) const
{
    return _get_edge_count(row*_shape.second + col);
}

const bool dots_and_boxes::_left_capture(const int position) const
{
    int n_rows = _shape.first, n_cols = _shape.second;

    assert(position < n_rows * (n_cols + 1));

    return has_been_played(position - 1) && 
           has_been_played(position + n_rows*(n_cols + 1) - (position/(n_cols + 1) + 1)) && 
           has_been_played(position + n_rows*(n_cols + 1) - (position/(n_cols + 1) + 1) + n_cols);
}

const bool dots_and_boxes::_right_capture(const int position) const
{
    int n_rows = _shape.first, n_cols = _shape.second;

    assert(position < n_rows * (n_cols + 1));

    return has_been_played(position + 1) && 
           has_been_played(position + n_rows*(n_cols + 1) - (position/(n_cols + 1) + 1) + 1) && 
           has_been_played(position + n_rows*(n_cols + 1) - (position/(n_cols + 1) + 1) + (n_cols + 1));
}

const bool dots_and_boxes::_up_capture(const int position) const
{
    int n_rows = _shape.first, n_cols = _shape.second;
    int horizontal_location = position - n_rows*(n_cols + 1);

    assert(position >= n_rows * (n_cols + 1));

    return has_been_played(position - n_cols) && 
           has_been_played(horizontal_location - (n_cols - horizontal_location/n_cols + 1)) && 
           has_been_played(horizontal_location - (n_cols - horizontal_location/n_cols + 1) + 1);
}

const bool dots_and_boxes::_down_capture(const int position) const
{
    int n_rows = _shape.first, n_cols = _shape.second;
    int horizontal_location = position - n_rows*(n_cols + 1);

    assert(position >= n_rows * (n_cols + 1));

    return has_been_played(position + n_cols) && 
           has_been_played(horizontal_location + (horizontal_location/n_cols)) && 
           has_been_played(horizontal_location + (horizontal_location/n_cols) + 1);
}

const bool dots_and_boxes::left_capture(const int position) const
{
    return _left_capture(position);
}

const bool dots_and_boxes::right_capture(const int position) const
{
    return _right_capture(position);
}

const bool dots_and_boxes::up_capture(const int position) const
{
    return _up_capture(position);
}

const bool dots_and_boxes::down_capture(const int position) const
{
    return _down_capture(position);
}

void dots_and_boxes::_assert_valid_state() const {
    for (int r = 0; r < _shape.first; r++)
    {
        for (int c = 0; c < _shape.second; c++)
        {
            if (_boxes.at(r * _shape.second + c) != EMPTY)
                assert(_get_edge_count(r, c) == 4);
            else
                assert(_get_edge_count(r, c) < 4);
        }
    }
}

std::vector<::move> dots_and_boxes::last_multimove() const {
    std::vector<::move> moves;
    const std::vector<::move> move_stack = get_move_stack();
    bool add_edge = static_cast<unsigned int>(move_stack.back() & (3 << 28)) >> 28 == 0;
    int player = EMPTY;
    for (auto riter = move_stack.rbegin(); riter != move_stack.rend(); ++riter)
    {
        if (static_cast<unsigned int>(*riter & (3 << 28)) >> 28 == 0)
        {
            if (add_edge)
            {
                moves.push_back((*riter));
                add_edge = false;

                if (player == EMPTY)
                {
                    player = ((*riter) >> 31) & 1 ? WHITE : BLACK;
                }
                else
                {
                    int move_player = ((*riter) >> 31) & 1 ? WHITE : BLACK;
                    // assert(move_player == player);
                    if (move_player != player)
                    {
                        std::cout << "!! swapped player, quitting" << std::endl;
                        break;
                    }
                }
                std::cout << "!! pushed edge move to multimove" << std::endl;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (player == EMPTY)
            {
                player = ((*riter) >> 31) & 1 ? WHITE : BLACK;
            }
            else
            {
                int move_player = ((*riter) >> 31) & 1 ? WHITE : BLACK;
                // assert(move_player == player);
                if (move_player != player)
                {
                    continue;
                }
            }
            std::cout << "!! pushed capture move to multimove" << std::endl;
            moves.push_back((*riter));
        }
    }
    std::reverse(moves.begin(), moves.end());
    return moves;
}
