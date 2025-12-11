#include "dots_and_boxes_test.h"
#include "cgt_move.h"
#include "dots_and_boxes.h"

#include "test/test_utilities.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

using std::cout, std::endl, std::vector, std::string, std::tuple, std::get;


namespace
{

void test_outcomes()
{
    vector<tuple<string, bool, bool>> boards = { // results are assumed to be for the first player and draws are losses
        {"@1,1", false, false},
        {"@1,2", false, false},
        {"@2,1", false, false},
        {"@2,2", true, true},
        {"@3,2", false, false},
        {"@2,3", false, false},
        {"@3,3", true, true},
    };

    for(const auto& t : boards)
    {

        dots_and_boxes d(get<0>(t));

        assert_solve(d, BLACK, get<1>(t));
        assert_solve(d, WHITE, get<2>(t));

    }

}





} // namespace


void dots_and_boxes_test_all()
{

    test_outcomes();


}
