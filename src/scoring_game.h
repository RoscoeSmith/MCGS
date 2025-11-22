//---------------------------------------------------------------------------
// A class for score-counting combinatorial games
//---------------------------------------------------------------------------
#include "cgt_basics.h"
#include "game.h"

class scoring_game : public game
{
public:
    virtual int count_score() const = 0;

protected:

private:

}; // class scoring_game

