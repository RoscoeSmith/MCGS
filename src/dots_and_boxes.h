#pragma once

#include "game.h"
#include "scoring_game.h"


#include <vector>
#include <ostream>


class dots_and_boxes : public scoring_game
{
public:
    dots_and_boxes(int n_rows, int n_cols); // for new game
    dots_and_boxes(const std::string& game_as_string); // for game as string

    void play(const Move& m, bw to_play) override;
    void undo_move() override;

    move_generator* create_move_generator(bw to_play) const override;

    void print(std::ostream& str) const override;

    game* inverse() const override;

    int count_score() const override;

protected:

    void _init_hash(local_hash& hash) const override;


private:
    int _get_total_moves(int& rows, int& cols);

    int _left_score;
    int _right_score;

    std::vector<bool> _vertical;
    std::vector<bool> _horizontal;
    std::vector<int> _boxes;

    int_pair _shape;
};

struct dots_and_boxes_state
{
    std::vector<bool> horizontal;
    std::vector<bool> vertical;
    std::vector<int> boxes;
    int_pair shape;
}
