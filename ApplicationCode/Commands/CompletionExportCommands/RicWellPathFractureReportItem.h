/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018     Statoil ASA
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

#pragma once

#include "RiaEclipseUnitTools.h"

#include <QString>

//==================================================================================================
///
//==================================================================================================
class RicWellPathFractureReportItem
{
public:
    RicWellPathFractureReportItem(const QString& wellPathName, const QString& fractureName, const QString& fractureTemplateName);

    void setData(double trans, size_t connCount, double fcd, double area);
    void setWidthAndConductivity(double width, double conductivity);
    void setHeightAndHalfLength(double height, double halfLength);
    void setAreaWeightedTransmissibility(double transmissibility);

    QString wellPathName() const;

    void getNames(QString& wellPathName, QString& fractureName, QString& fractureTemplateName) const;

    void                            setUnitSystem(RiaEclipseUnitTools::UnitSystem unitSystem);
    RiaEclipseUnitTools::UnitSystem unitSystem() const;

    double transmissibility() const;
    size_t connectionCount() const;
    double fcd() const;
    double area() const;

    double kfwf() const;
    double kf() const;
    double wf() const;
    double xf() const;
    double h() const;
    double km() const;

private:
    RiaEclipseUnitTools::UnitSystem m_unitSystem;
    QString m_wellPath;
    QString m_wellPathFracture;
    QString m_wellPathFractureTemplate;

    double m_transmissibility;
    size_t m_connectionCount;
    double m_fcd;
    double m_area;

    double m_kfwf;
    double m_kf;
    double m_wf;
    double m_xf;
    double m_h;
    double m_km;
};