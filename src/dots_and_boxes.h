#pragma once

#include "game.h"
#include "scoring_game.h"


#include <vector>
#include <ostream>

#define MOVE_LIMIT 268435456 

// (row, col)
typedef std::pair<int, int> int_pair;

class dots_and_boxes : public scoring_game
{
public:
    dots_and_boxes(int n_rows, int n_cols); // for new game
    dots_and_boxes(const std::string& game_as_string); // for game as string

    void play(const move& m, bw to_play);
    void undo_move() override;
    
    void undo_move_single();

    std::vector<move> last_multimove() const override;

    move_generator* create_move_generator(bw to_play) const override;

    bool has_been_played(int position) const;

    void print(std::ostream& str) const override;

    game* inverse() const override;

    int count_score() const override;

    const int_pair get_shape() const;
    std::string board_as_string() const;
    std::string pretty_print() const;

    const bool left_capture(const int position) const;
    const bool right_capture(const int position) const;
    const bool up_capture(const int position) const;
    const bool down_capture(const int position) const;

protected:

    void _init_hash(local_hash& hash) const override;


private:

    const int _get_total_moves() const;

    const int _get_edge_count(const int& position) const;
    const int _get_edge_count(const int& row, const int& col) const;

    const bool _left_capture(const int position) const;
    const bool _right_capture(const int position) const;
    const bool _up_capture(const int position) const;
    const bool _down_capture(const int position) const;



    void _assert_valid_state() const;

    int _left_score;
    int _right_score;

    std::vector<bool> _vertical;
    std::vector<bool> _horizontal;
    std::vector<int> _boxes;

    int_pair _shape;
};

struct dots_and_boxes_state
{    
    std::vector<bool> vertical;
    std::vector<bool> horizontal;
    std::vector<int> boxes;
    int_pair shape;
};
