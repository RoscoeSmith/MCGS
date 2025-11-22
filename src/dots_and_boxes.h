#pragma once

#include "game.h"
#include "scoring_game.h"


#include <vector>
#include <ostream>


enum class box_value : unsigned char {
    EMPTY = 0,
    LEFT = 2,
    RIGHT = 3
};

class dots_and_boxes : public scoring_game
{
public:
    dots_and_boxes(int n_rows, int n_cols);

    void play(const Move& m, bw to_play) override;
    void undo_move() override;

    move_generator* create_move_generator(bw to_play) const override;

    void print(std::ostream& str) const override;

    game* inverse() const override;

    int count_score() const override;

protected:

    void _init_hash(local_hash& hash) const override;


private:

    int _left_score;
    int _right_score;

    std::vector<bool> _vertical;
    std::vector<bool> _horizontal;
    std::vector<box_value> _boxes;

}
