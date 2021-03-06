/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020-     Equinor ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RicElasticPropertiesImportTools.h"

#include "RiaLogging.h"

#include "RimElasticProperties.h"
#include "RimFractureModel.h"

#include "RifElasticPropertiesReader.h"
#include "RifFileParseTools.h"

#include "RigElasticProperties.h"

#include <set>
#include <vector>

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicElasticPropertiesImportTools::importElasticPropertiesFromFile( const QString&    filePath,
                                                                       RimFractureModel* fractureModel )
{
    typedef std::tuple<QString, QString, QString> FaciesKey;

    // Read the facies properties from file
    std::vector<RifElasticProperties> rifElasticProperties;
    try
    {
        QStringList filePaths;
        filePaths << filePath;
        RifElasticPropertiesReader::readElasticProperties( rifElasticProperties, filePaths );
    }
    catch ( FileParseException& exception )
    {
        RiaLogging::warning( QString( "Facies properties import failed: '%1'." ).arg( exception.message ) );
        return;
    }

    // Find the unique facies keys (combination of field, formation and facies names)
    std::set<FaciesKey> faciesKeys;
    for ( RifElasticProperties item : rifElasticProperties )
    {
        FaciesKey faciesKey = std::make_tuple( item.fieldName, item.formationName, item.faciesName );
        faciesKeys.insert( faciesKey );
    }

    RimElasticProperties* rimElasticProperties = new RimElasticProperties;
    for ( FaciesKey key : faciesKeys )
    {
        std::vector<RifElasticProperties> matchingFacies;

        QString fieldName     = std::get<0>( key );
        QString formationName = std::get<1>( key );
        QString faciesName    = std::get<2>( key );

        // Group the items with a given facies key
        for ( RifElasticProperties item : rifElasticProperties )
        {
            if ( item.fieldName == fieldName && item.formationName == formationName && item.faciesName == faciesName )
            {
                matchingFacies.push_back( item );
            }
        }

        // Sort the matching items by porosity
        std::sort( matchingFacies.begin(),
                   matchingFacies.end(),
                   []( const RifElasticProperties& a, const RifElasticProperties& b ) { return a.porosity < b.porosity; } );

        // Finally add the values
        RigElasticProperties rigElasticProperties( fieldName, formationName, faciesName );
        for ( RifElasticProperties item : matchingFacies )
        {
            rigElasticProperties.appendValues( item.porosity,
                                               item.youngsModulus,
                                               item.poissonsRatio,
                                               item.K_Ic,
                                               item.proppantEmbedment,
                                               item.biotCoefficient,
                                               item.k0,
                                               item.fluidLossCoefficient,
                                               item.spurtLoss );
        }

        rimElasticProperties->setPropertiesForFacies( key, rigElasticProperties );
    }

    rimElasticProperties->setFilePath( filePath );
    fractureModel->setElasticProperties( rimElasticProperties );
    fractureModel->updateConnectedEditors();
}
