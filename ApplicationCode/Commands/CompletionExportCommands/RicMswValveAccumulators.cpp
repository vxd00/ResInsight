/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018 Equinor ASA
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
#include "RicMswValveAccumulators.h"

#include "RiaStatisticsTools.h"

#include "RicMswCompletions.h"

#include "RimPerforationInterval.h"
#include "RimWellPathValve.h"

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RicMswICDAccumulator::RicMswICDAccumulator( RiaEclipseUnitTools::UnitSystem unitSystem )
    : RicMswValveAccumulator( unitSystem )
    , m_areaSum( 0.0 )
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RicMswICDAccumulator::accumulateValveParameters( const RimWellPathValve* wellPathValve,
                                                      size_t                  subValve,
                                                      double                  contributionFraction,
                                                      double                  totalValveLengthOpenForFlow )
{
    CVF_ASSERT( wellPathValve );
    if ( wellPathValve->componentType() == RiaDefines::WellPathComponentType::ICV ||
         wellPathValve->componentType() == RiaDefines::WellPathComponentType::ICD )
    {
        double lengthFraction = 1.0;

        if ( wellPathValve->componentType() == RiaDefines::WellPathComponentType::ICD )
        {
            std::pair<double, double> valveSegment       = wellPathValve->valveSegments()[subValve];
            double                    valveSegmentLength = std::fabs( valveSegment.second - valveSegment.first );
            if ( totalValveLengthOpenForFlow > 1.0e-8 )
            {
                lengthFraction = valveSegmentLength / totalValveLengthOpenForFlow;
            }
        }

        double combinedFraction = contributionFraction * lengthFraction;

        double icdOrificeRadius = wellPathValve->orificeDiameter( m_unitSystem ) / 2;
        double icdArea          = icdOrificeRadius * icdOrificeRadius * cvf::PI_D;

        m_areaSum += icdArea * combinedFraction;
        m_coefficientCalculator.addValueAndWeight( wellPathValve->flowCoefficient(), icdArea * combinedFraction );
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicMswICDAccumulator::applyToSuperValve( std::shared_ptr<RicMswValve> valve )
{
    std::shared_ptr<RicMswWsegValve> icd = std::dynamic_pointer_cast<RicMswWsegValve>( valve );
    CVF_ASSERT( icd );
    icd->setArea( m_areaSum );
    if ( m_coefficientCalculator.validAggregatedWeight() )
    {
        icd->setFlowCoefficient( m_coefficientCalculator.weightedMean() );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RicMswAICDAccumulator::RicMswAICDAccumulator( RiaEclipseUnitTools::UnitSystem unitSystem )
    : RicMswValveAccumulator( unitSystem )
    , m_valid( false )
    , m_deviceOpen( false )
    , m_accumulatedLength( 0.0 )
    , m_accumulatedFlowScalingFactorDivisor( 0.0 )
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RicMswAICDAccumulator::accumulateValveParameters( const RimWellPathValve* wellPathValve,
                                                       size_t                  subValve,
                                                       double                  contributionFraction,
                                                       double                  totalValveLengthOpenForFlow )
{
    CVF_ASSERT( wellPathValve );
    if ( wellPathValve->componentType() == RiaDefines::WellPathComponentType::AICD )
    {
        const RimWellPathAicdParameters* params = wellPathValve->aicdParameters();
        if ( params->isValid() )
        {
            m_valid      = true;
            m_deviceOpen = m_deviceOpen || params->isOpen();
            if ( params->isOpen() )
            {
                std::pair<double, double> valveSegment       = wellPathValve->valveSegments()[subValve];
                double                    valveSegmentLength = std::fabs( valveSegment.second - valveSegment.first );
                double                    lengthFraction     = 1.0;
                if ( totalValveLengthOpenForFlow > 1.0e-8 )
                {
                    lengthFraction = valveSegmentLength / totalValveLengthOpenForFlow;
                }

                double combinedFraction = contributionFraction * lengthFraction;

                std::array<double, AICD_NUM_PARAMS> values = params->doubleValues();
                for ( size_t i = 0; i < (size_t)AICD_NUM_PARAMS; ++i )
                {
                    if ( RiaStatisticsTools::isValidNumber( values[i] ) )
                    {
                        m_meanCalculators[i].addValueAndWeight( values[i], combinedFraction );
                    }
                }

                // https://github.com/OPM/ResInsight/issues/6126
                //
                // flowScalingFactor =  1 / (length_fraction * N_AICDs)
                // where:
                // length_fraction = length_COMPSEGS / Sum_length_COMPSEGS_for_valve
                // N_AICDs = number of AICDs in perforation interval

                double divisor = wellPathValve->valveLocations().size() * combinedFraction;

                m_accumulatedFlowScalingFactorDivisor += divisor;

                m_accumulatedLength += combinedFraction;
            }
        }
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicMswAICDAccumulator::applyToSuperValve( std::shared_ptr<RicMswValve> valve )
{
    std::shared_ptr<RicMswPerforationAICD> aicd = std::dynamic_pointer_cast<RicMswPerforationAICD>( valve );

    if ( aicd )
    {
        std::array<double, AICD_NUM_PARAMS> values;

        for ( size_t i = 0; i < (size_t)AICD_NUM_PARAMS; ++i )
        {
            if ( m_meanCalculators[i].validAggregatedWeight() )
            {
                values[i] = m_meanCalculators[i].weightedMean();
            }
            else
            {
                values[i] = std::numeric_limits<double>::infinity();
            }
        }
        aicd->setIsValid( m_valid );
        aicd->setIsOpen( m_deviceOpen );
        aicd->setLength( m_accumulatedLength );

        // See https://github.com/OPM/ResInsight/issues/6126
        double flowScalingFactor = 0.0;
        if ( m_accumulatedFlowScalingFactorDivisor > 1.0e-8 )
        {
            flowScalingFactor = 1.0 / m_accumulatedFlowScalingFactorDivisor;
        }

        aicd->setflowScalingFactor( flowScalingFactor );

        aicd->values() = values;
    }
}
