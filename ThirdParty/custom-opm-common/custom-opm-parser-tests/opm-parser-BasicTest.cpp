
#include "gtest/gtest.h"

#include "opm/parser/eclipse/Parser/Parser.hpp"
#include "opm/parser/eclipse/EclipseState/Schedule/VFPInjTable.hpp"
#include "opm/parser/eclipse/EclipseState/Schedule/VFPProdTable.hpp"


using namespace Opm;


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
TEST(OpmParserTest, ReadFromFile)
{
    ParseContext parseContext;

    {
        Parser parser;

        std::string testFile = "d:/gitroot-ceesol/ResInsight-regression-test/ModelData/norne/INCLUDE/VFP/B1BH.Ecl";

        auto deck = parser.parseFile(testFile);

        std::string myKeyword = "VFPPROD";
        auto keywordList = deck.getKeywordList(myKeyword);

        UnitSystem unitSystem;

    
        for (auto kw : keywordList)
        {
            auto name = kw->name();
    
            VFPProdTable table(*kw, unitSystem);
            std::cout << table.getDatumDepth() << std::endl;
        }
    }
    {
        Parser parser;

        std::string testFile = "d:/gitroot-ceesol/ResInsight-regression-test/ModelData/norne/INCLUDE/VFP/C1H.Ecl";

        auto deck = parser.parseFile(testFile);

        std::string myKeyword = "VFPINJ";
        auto keywordList = deck.getKeywordList(myKeyword);

        UnitSystem unitSystem;
    
        for (auto kw : keywordList)
        {
            auto name = kw->name();
    
            VFPInjTable table(*kw, unitSystem);
            std::cout << table.getDatumDepth() << std::endl;
        }
    }


}



    
