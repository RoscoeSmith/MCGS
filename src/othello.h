#pragma once

#include "game.h"
#include "scoring_game.h"
#include "grid.h"

class othello : public scoring_game, public grid
{
public:
    othello(int n_rows, int n_cols);
    othello(const std::vector<int>& board, int_pair shape);
    othello(const std::string& game_as_string);

    void play(const move& m, bw to_play) override;
    void undo_move() override;

    int count_score(bw to_play) const override;
    bool is_terminal() const override;

    move_generator* create_move_generator(bw to_play) const override;

    void print(std::ostream& str) const override { grid::print(str); }
    game* inverse() const override { assert(false); }
    void _init_hash(local_hash& hash) const override { grid::_init_hash(hash); }
    
};
