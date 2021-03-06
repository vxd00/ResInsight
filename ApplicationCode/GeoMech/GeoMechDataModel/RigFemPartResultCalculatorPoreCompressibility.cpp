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

#include "RigFemPartResultCalculatorPoreCompressibility.h"

#include "RiaEclipseUnitTools.h"
#include "RiaLogging.h"

#include "RigFemPart.h"
#include "RigFemPartCollection.h"
#include "RigFemPartResultsCollection.h"
#include "RigFemResultAddress.h"
#include "RigFemScalarResultFrames.h"

#include "cafProgressInfo.h"

#include <QString>

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RigFemPartResultCalculatorPoreCompressibility::RigFemPartResultCalculatorPoreCompressibility( RigFemPartResultsCollection& collection )
    : RigFemPartResultCalculator( collection )
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RigFemPartResultCalculatorPoreCompressibility::~RigFemPartResultCalculatorPoreCompressibility()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RigFemPartResultCalculatorPoreCompressibility::isMatching( const RigFemResultAddress& resVarAddr ) const
{
    return ( resVarAddr.fieldName == "COMPRESSIBILITY" &&
             ( resVarAddr.componentName == "PORE" || resVarAddr.componentName == "VERTICAL" ||
               resVarAddr.componentName == "VERTICAL-RATIO" ) );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RigFemScalarResultFrames*
    RigFemPartResultCalculatorPoreCompressibility::calculate( int partIndex, const RigFemResultAddress& resVarAddr )
{
    caf::ProgressInfo frameCountProgress( m_resultCollection->frameCount() * 6, "" );
    frameCountProgress.setProgressDescription( "Calculating Pore Compressibility" );

    frameCountProgress.setNextProgressIncrement( m_resultCollection->frameCount() );
    RigFemScalarResultFrames* srcPORDataFrames =
        m_resultCollection->findOrLoadScalarResult( partIndex, RigFemResultAddress( RIG_NODAL, "POR-Bar", "" ) );
    frameCountProgress.incrementProgress();

    // Volumetric Strain
    frameCountProgress.setNextProgressIncrement( m_resultCollection->frameCount() );
    RigFemScalarResultFrames* srcEVDataFrames =
        m_resultCollection->findOrLoadScalarResult( partIndex, RigFemResultAddress( resVarAddr.resultPosType, "NE", "EV" ) );

    frameCountProgress.incrementProgress();

    // Vertical Strain
    frameCountProgress.setNextProgressIncrement( m_resultCollection->frameCount() );
    RigFemScalarResultFrames* verticalStrainDataFrames =
        m_resultCollection->findOrLoadScalarResult( partIndex,
                                                    RigFemResultAddress( resVarAddr.resultPosType, "NE", "E33" ) );

    frameCountProgress.incrementProgress();

    // Biot porelastic coeffisient (alpha)
    frameCountProgress.setNextProgressIncrement( m_resultCollection->frameCount() );
    RigFemScalarResultFrames* biotCoefficient = nullptr;
    if ( !m_resultCollection->biotResultAddress().isEmpty() )
    {
        biotCoefficient =
            m_resultCollection
                ->findOrLoadScalarResult( partIndex,
                                          RigFemResultAddress( RIG_ELEMENT,
                                                               m_resultCollection->biotResultAddress().toStdString(),
                                                               "" ) );
    }
    frameCountProgress.incrementProgress();

    frameCountProgress.setNextProgressIncrement( m_resultCollection->frameCount() );
    RigFemScalarResultFrames* youngsModuliFrames =
        m_resultCollection->findOrLoadScalarResult( partIndex, RigFemResultAddress( RIG_ELEMENT, "MODULUS", "" ) );
    if ( youngsModuliFrames->frameData( 0 ).empty() )
    {
        RiaLogging::error( "Missing Youngs Moduli element data (MODULUS)." );
        return nullptr;
    }

    RigFemScalarResultFrames* poissonRatioFrames =
        m_resultCollection->findOrLoadScalarResult( partIndex, RigFemResultAddress( RIG_ELEMENT, "RATIO", "" ) );
    if ( poissonRatioFrames->frameData( 0 ).empty() )
    {
        RiaLogging::error( "Missing Poisson Ratio element data (RATIO)." );
        return nullptr;
    }

    RigFemScalarResultFrames* voidRatioFrames =
        m_resultCollection->findOrLoadScalarResult( partIndex,
                                                    RigFemResultAddress( resVarAddr.resultPosType, "VOIDR", "" ) );

    RigFemScalarResultFrames* poreCompressibilityFrames =
        m_resultCollection->createScalarResult( partIndex,
                                                RigFemResultAddress( resVarAddr.resultPosType, resVarAddr.fieldName, "PORE" ) );

    RigFemScalarResultFrames* verticalCompressibilityFrames =
        m_resultCollection->createScalarResult( partIndex,
                                                RigFemResultAddress( resVarAddr.resultPosType,
                                                                     resVarAddr.fieldName,
                                                                     "VERTICAL" ) );
    RigFemScalarResultFrames* verticalCompressibilityRatioFrames =
        m_resultCollection->createScalarResult( partIndex,
                                                RigFemResultAddress( resVarAddr.resultPosType,
                                                                     resVarAddr.fieldName,
                                                                     "VERTICAL-RATIO" ) );
    frameCountProgress.incrementProgress();

    const RigFemPart* femPart = m_resultCollection->parts()->part( partIndex );
    float             inf     = std::numeric_limits<float>::infinity();

    frameCountProgress.setNextProgressIncrement( 1u );

    int referenceFrameIdx = m_resultCollection->referenceTimeStep();

    int frameCount = srcEVDataFrames->frameCount();
    for ( int fIdx = 0; fIdx < frameCount; ++fIdx )
    {
        const std::vector<float>& evData                      = srcEVDataFrames->frameData( fIdx );
        const std::vector<float>& referenceEvData             = srcEVDataFrames->frameData( referenceFrameIdx );
        const std::vector<float>& verticalStrainData          = verticalStrainDataFrames->frameData( fIdx );
        const std::vector<float>& referenceVerticalStrainData = verticalStrainDataFrames->frameData( referenceFrameIdx );
        const std::vector<float>& youngsModuliData            = youngsModuliFrames->frameData( fIdx );
        const std::vector<float>& poissonRatioData            = poissonRatioFrames->frameData( fIdx );
        const std::vector<float>& voidRatioData               = voidRatioFrames->frameData( 0 );
        const std::vector<float>& referencePorFrameData       = srcPORDataFrames->frameData( referenceFrameIdx );
        const std::vector<float>& porFrameData                = srcPORDataFrames->frameData( fIdx );

        std::vector<float>& poreCompressibilityFrameData          = poreCompressibilityFrames->frameData( fIdx );
        std::vector<float>& verticalCompressibilityFrameData      = verticalCompressibilityFrames->frameData( fIdx );
        std::vector<float>& verticalCompressibilityRatioFrameData = verticalCompressibilityRatioFrames->frameData( fIdx );

        size_t valCount = evData.size();
        poreCompressibilityFrameData.resize( valCount );
        verticalCompressibilityFrameData.resize( valCount );
        verticalCompressibilityRatioFrameData.resize( valCount );

        int elementCount = femPart->elementCount();

        std::vector<float> biotData;
        if ( biotCoefficient )
        {
            biotData = biotCoefficient->frameData( fIdx );
            if ( !m_resultCollection->isValidBiotData( biotData, elementCount ) )
            {
                m_resultCollection->deleteResult( resVarAddr );
                return nullptr;
            }
        }

#pragma omp parallel for
        for ( int elmIdx = 0; elmIdx < elementCount; ++elmIdx )
        {
            RigElementType elmType = femPart->elementType( elmIdx );

            int elmNodeCount = RigFemTypes::elementNodeCount( femPart->elementType( elmIdx ) );

            if ( elmType == HEX8P )
            {
                for ( int elmNodIdx = 0; elmNodIdx < elmNodeCount; ++elmNodIdx )
                {
                    size_t elmNodResIdx = femPart->elementNodeResultIdx( elmIdx, elmNodIdx );
                    if ( elmNodResIdx < evData.size() )
                    {
                        if ( fIdx == referenceFrameIdx )
                        {
                            // The time step and the reference time step are the same: results undefined
                            poreCompressibilityFrameData[elmNodResIdx]          = inf;
                            verticalCompressibilityFrameData[elmNodResIdx]      = inf;
                            verticalCompressibilityRatioFrameData[elmNodResIdx] = inf;
                        }
                        else
                        {
                            // Use biot coefficient for all timesteps
                            double biotCoefficient = 1.0;
                            if ( biotData.empty() )
                            {
                                biotCoefficient = m_resultCollection->biotFixedFactor();
                            }
                            else
                            {
                                // Use coefficient from element property table
                                biotCoefficient = biotData[elmIdx];
                            }

                            int nodeIdx = femPart->nodeIdxFromElementNodeResultIdx( elmNodResIdx );

                            // Calculate bulk modulus for solids (grains).
                            // Incoming unit for Young's Modulus is GPa: convert to Pa.
                            double poissonRatio = poissonRatioData[elmIdx];
                            double youngsModuli = RiaEclipseUnitTools::gigaPascalToPascal( youngsModuliData[elmIdx] );
                            double bulkModulusFrame = youngsModuli / ( 3.0 * ( 1.0 - 2.0 * poissonRatio ) );
                            double bulkModulus      = bulkModulusFrame / ( 1.0 - biotCoefficient );

                            // Calculate initial porosity (always from geostatic timestep)
                            double voidr    = voidRatioData[elmNodResIdx];
                            double porosity = voidr / ( 1.0 + voidr );

                            // Calculate difference in pore pressure between reference state and this state,
                            // and convert unit from Bar to Pascal.
                            double referencePorePressure = referencePorFrameData[nodeIdx];
                            double framePorePressure     = porFrameData[nodeIdx];
                            double deltaPorePressure =
                                RiaEclipseUnitTools::barToPascal( framePorePressure - referencePorePressure );

                            // Calculate pore compressibility
                            double poreCompressibility = inf;
                            if ( deltaPorePressure != 0.0 && porosity != 0.0 )
                            {
                                double deltaEv      = evData[elmNodResIdx] - referenceEvData[elmNodResIdx];
                                poreCompressibility = -( biotCoefficient * deltaEv ) / ( deltaPorePressure * porosity );
                                // Guard against divide by zero: second term can be ignored when bulk modulus is zero,
                                // which can happens when biot coefficient is 1.0
                                if ( biotCoefficient != 1.0 && porosity != 1.0 )
                                {
                                    poreCompressibility += ( 1.0 / bulkModulus ) * ( biotCoefficient / porosity - 1.0 );
                                }
                            }
                            // Convert from 1/Pa to 1/GPa
                            poreCompressibilityFrameData[elmNodResIdx] = poreCompressibility * 1.0e9;

                            double verticalCompressibility      = inf;
                            double verticalCompressibilityRatio = inf;

                            if ( biotCoefficient != 0.0 && deltaPorePressure != 0.0 )
                            {
                                double deltaStrain = verticalStrainData[elmNodResIdx] -
                                                     referenceVerticalStrainData[elmNodResIdx];

                                // Calculate vertical compressibility (unit: 1/Pa)
                                verticalCompressibility = -deltaStrain / ( biotCoefficient * deltaPorePressure );

                                // Calculate vertical compressibility ratio
                                verticalCompressibilityRatio =
                                    ( verticalCompressibility * youngsModuli * ( 1.0 - poissonRatio ) ) /
                                    ( ( 1.0 + poissonRatio ) * ( 1.0 - 2.0 * poissonRatio ) );
                            }

                            // Convert from 1/Pa to 1/GPa
                            verticalCompressibilityFrameData[elmNodResIdx]      = verticalCompressibility * 1.0e9;
                            verticalCompressibilityRatioFrameData[elmNodResIdx] = verticalCompressibilityRatio;
                        }
                    }
                }
            }
            else
            {
                for ( int elmNodIdx = 0; elmNodIdx < elmNodeCount; ++elmNodIdx )
                {
                    size_t elmNodResIdx = femPart->elementNodeResultIdx( elmIdx, elmNodIdx );
                    if ( elmNodResIdx < poreCompressibilityFrameData.size() )
                    {
                        poreCompressibilityFrameData[elmNodResIdx] = inf;
                    }
                }
            }
        }

        frameCountProgress.incrementProgress();
    }

    RigFemScalarResultFrames* requestedResultFrames = m_resultCollection->findOrLoadScalarResult( partIndex, resVarAddr );
    return requestedResultFrames;
}
