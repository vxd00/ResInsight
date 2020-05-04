/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Statoil ASA
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

#include "RigFormationNames.h"

const cvf::Color3f NO_COLOR(-1.0f, -1.0f, -1.0f);


//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RigFormationNames::RigFormationNames()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RigFormationNames::~RigFormationNames()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QString RigFormationNames::formationNameFromKLayerIdx( size_t Kidx )
{
    int idx = formationIndexFromKLayerIdx( Kidx );
    if ( idx >= static_cast<int>( m_formationNames.size() ) ) return "";
    if ( idx == -1 ) return "";

    return m_formationNames[idx];
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RigFormationNames::formationColorFromKLayerIdx(size_t Kidx, cvf::Color3f* formationColor)
{
    int idx = formationIndexFromKLayerIdx(Kidx);

    if (idx == -1 || idx >= static_cast<int>(m_formationColors.size()))
    {
        return false;
    }

    if (m_formationColors[idx] == NO_COLOR) return false;

    *formationColor = m_formationColors[idx];
    return true;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RigFormationNames::appendFormationRange( const QString& name, int kStartIdx, int kEndIdx )
{
    CVF_ASSERT( kStartIdx <= kEndIdx );

    int nameIdx = static_cast<int>( m_formationNames.size() );

    m_formationNames.push_back( name );

    if ( kEndIdx >= static_cast<int>( m_nameIndexPrKLayer.size() ) )
    {
        m_nameIndexPrKLayer.resize( kEndIdx + 1, -1 );
    }

    for ( int kIdx = kStartIdx; kIdx <= kEndIdx; ++kIdx )
    {
        m_nameIndexPrKLayer[kIdx] = nameIdx;
    }

    m_formationColors.push_back(NO_COLOR);      // mark missing color
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RigFormationNames::appendFormationRange(const QString& name, cvf::Color3f color, int kStartIdx, int kEndIdx)
{
    CVF_ASSERT(kStartIdx <= kEndIdx);

    RigFormationNames::appendFormationRange(name, kStartIdx, kEndIdx);

    m_formationColors.pop_back();         // delete default color
    m_formationColors.push_back(color);   // override with missing color
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RigFormationNames::appendFormationRangeHeight( const QString& name, int kLayerCount )
{
    if ( kLayerCount < 1 ) return;

    int nameIdx = static_cast<int>( m_formationNames.size() );

    m_formationNames.push_back( name );

    int kStartIdx = static_cast<int>( m_nameIndexPrKLayer.size() );

    m_nameIndexPrKLayer.resize( kStartIdx + kLayerCount, -1 );

    for ( int kIdx = kStartIdx; kIdx < kStartIdx + kLayerCount; ++kIdx )
    {
        m_nameIndexPrKLayer[kIdx] = nameIdx;
    }

    m_formationColors.push_back(NO_COLOR);      // mark missing color
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RigFormationNames::appendFormationRangeHeight(const QString& name, cvf::Color3f color, int kLayerCount)
{
    if (kLayerCount < 1) return;

    RigFormationNames::appendFormationRangeHeight(name, kLayerCount);

    m_formationColors.pop_back();         // delete default color
    m_formationColors.push_back(color);   // override with missing color
}
