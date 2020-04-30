#include "gtest/gtest.h"

#include "RifColorLegendData.h"
#include "RigFormationNames.h"

#include "QDir"
#include "RiaTestDataDirectory.h"
#include <QString>
#include <QStringList>


/*
TEST( RifColorLegendData, ReadLYRFileWithoutColor )
{
    QDir baseFolder( TEST_DATA_DIR );

    const QString filename( "RifColorLegendData/Norne_ATW2013.lyr" );
    const QString filePath = baseFolder.absoluteFilePath( filename );
    EXPECT_TRUE( QFile::exists( filePath ) );

    QString errormessage;

    cvf::ref<RigFormationNames> fm = RifColorLegendData::readFormationNamesFile(filePath, &errormessage);
    EXPECT_TRUE( errormessage.isEmpty() );

    const std::vector<QString>& formationNames = fm->formationNames();

    for (int i = 0; i < formationNames.size(); i++)
    {
        std::cout << formationNames[i].toStdString() << '\n';
        std::cout << fm->formationIndexFromKLayerIdx(i) << '\n';
    }


    QString formationName_K1 = fm->formationNameFromKLayerIdx(0);
    int     formationIndex   = fm->formationIndexFromKLayerIdx(1);

    std::cout << formationName_K1.toStdString() << " " << formationIndex << '\n';
    
    EXPECT_TRUE( formationName_K1 == "Garn 3" );
    EXPECT_EQ  ( 1, formationIndex );
}


TEST(RifColorLegendData, ReadLYRFileWithColorName)
{
    QDir baseFolder(TEST_DATA_DIR);

    const QString filename("RifColorLegendData/Norne_ATW2013ColorName.lyr");
    const QString filePath = baseFolder.absoluteFilePath(filename);
    EXPECT_TRUE(QFile::exists(filePath));

    QString errormessage;

    cvf::ref<RigFormationNames> fm = RifColorLegendData::readFormationNamesFile(filePath, &errormessage);
    EXPECT_TRUE(errormessage.isEmpty());

    QString formationName_K1 = fm->formationNameFromKLayerIdx(0);
    int     formationIndex = fm->formationIndexFromKLayerIdx(1);

    std::cout << formationName_K1.toStdString() << " " << formationIndex << '\n';

    EXPECT_TRUE(formationName_K1 == "Garn 3");
    EXPECT_EQ(1, formationIndex);
}
*/

TEST(RifColorLegendData, ReadLYRFileWithColorHTML)
{
    QDir baseFolder(TEST_DATA_DIR);

    const QString filename("RifColorLegendData/Norne_ATW2013ColorHTML.lyr");
    const QString filePath = baseFolder.absoluteFilePath(filename);
    EXPECT_TRUE(QFile::exists(filePath));

    QString errormessage;

    cvf::ref<RigFormationNames> fm = RifColorLegendData::readFormationNamesFile(filePath, &errormessage);
    EXPECT_TRUE(errormessage.isEmpty());

    QString formationName_K1 = fm->formationNameFromKLayerIdx(0);
    int     formationIndex = fm->formationIndexFromKLayerIdx(1);

    std::cout << formationName_K1.toStdString() << " " << formationIndex << '\n';

    EXPECT_TRUE(formationName_K1 == "Garn 3");
    EXPECT_EQ(1, formationIndex);
}


