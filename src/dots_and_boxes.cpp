#include "dots_and_boxes.h"
#include "game.h"
#include "scoring_game.h"


#include <vector>
#include <ostream>
#include <string>

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

dots_and_boxes::dots_and_boxes(const std::string& game_as_string) : scoring_game()
{
    dots_and_boxes_state state = string_to_board(game_as_string);
    _horizontal = state.horizontal;
    _vertical = state.vertical;
    _boxes = state.boxes;
    _shape = state.shape;
}

