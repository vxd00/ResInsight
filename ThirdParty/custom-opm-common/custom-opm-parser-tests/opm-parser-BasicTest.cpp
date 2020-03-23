
#include "gtest/gtest.h"

#include "opm/parser/eclipse/Parser/Parser.hpp"


using namespace Opm;


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
TEST(OpmParserTest, ReadFromFile)
{
    ParseContext parseContext;
    Parser parser;

    //std::string testFile = "d:/gitroot-ceesol/ResInsight-regression-test/ModelData/norne/NORNE_ATW2013.DATA";
    std::string testFile = "d:/gitroot-ceesol/ResInsight-regression-test/ModelData/norne/INCLUDE/VFP/B1BH.Ecl";

    auto deck = parser.parseFile(testFile);

    std::string myKeyword = "VFPPROD";
    auto keywordList = deck.getKeywordList(myKeyword);

    for (auto kw : keywordList)
    {
        auto name = kw->name();
    }

}



    
