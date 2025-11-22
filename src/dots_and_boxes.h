#pragma once

#include "game.h"
#include "scoring_game.h"


#include <vector>
#include <ostream>




class dots_and_boxes : public scoring_game
{
public:
    dots_and_boxes(int n_rows, int n_cols);


    void play(const Move& m, bw to_play) override;
    void undo_move() override;

    move_generator* create_move_generator(bw to_play) const override;

    void print(std::ostream& str) const override;

    game* inverse() override;

    int count_score();

protected:

    void _init_hash(local_hash& hash) const override;


private:

    int _left_score;
    int _right_score;

    std::vector<bool> _vertical;
    std::vector<bool> _horizontal;

}