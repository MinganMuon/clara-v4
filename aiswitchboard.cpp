// aiswitchboard.cpp

#include "aiswitchboard.h"

namespace AISwitchboard {

const std::vector<std::string> getListOfAllAIs()
{
    std::vector<std::string> list;
    for (std::string s : SimpleAIs::simpleAIsNameList)
    {
        list.push_back(s);
    }
    return list;
}

const std::vector<std::array<std::string,2>> getListOfAllAIsByCategory()
{
    std::vector<std::array<std::string,2>> list;
    for (std::string s : SimpleAIs::simpleAIsNameList)
    {
        list.push_back(std::array<std::string,2>{"simple ais", s});
    }
    return list;
}

Checkers::Move getMoveFromAI(const std::string selectedAI, const Checkers::GameState &thegamestate, const std::vector<std::string> aiparameters)
{
    std::vector<std::array<std::string,2>> list = getListOfAllAIsByCategory();
    auto listiter = std::find_if(list.begin(), list.end(), [selectedAI](std::array<std::string,2> aiarray){ return selectedAI == aiarray[1]; });

    if (!(listiter == list.end()))
    {
        if ((*listiter)[0] == "simple ais")
        {
            auto simpleaiiter = std::find(SimpleAIs::simpleAIsNameList.begin(), SimpleAIs::simpleAIsNameList.end(), (*listiter)[1]);
            if (simpleaiiter != SimpleAIs::simpleAIsNameList.end())
            {
                return (*SimpleAIs::simpleAIsFuncList[simpleaiiter - SimpleAIs::simpleAIsNameList.begin()])(thegamestate);
            }
        }
    }

    // we shouldn't have reached this point...but if we have, return an invalid move
    return Checkers::Move(Checkers::Piece(Checkers::TileType::Man,Checkers::PlayerColor::White,Checkers::BoardPos(-1,-1)),
                            Checkers::Piece(Checkers::TileType::Man,Checkers::PlayerColor::White,Checkers::BoardPos(-2,-2)),
                            std::vector<Checkers::Piece>());
}

}
