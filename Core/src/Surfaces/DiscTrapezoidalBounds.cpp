// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// DiscTrapezoidalBounds.cpp, ACTS project
///////////////////////////////////////////////////////////////////

#include "ACTS/Surfaces/DiscTrapezoidalBounds.hpp"
#include <iomanip>
#include <iostream>

Acts::DiscTrapezoidalBounds::DiscTrapezoidalBounds(double minhalfx,
                                                   double maxhalfx,
                                                   double maxR,
                                                   double minR,
                                                   double avephi,
                                                   double stereo)
  : DiscBounds(DiscTrapezoidalBounds::bv_length)
{
  m_valueStore.at(DiscTrapezoidalBounds::bv_averagePhi) = avephi;
  m_valueStore.at(DiscTrapezoidalBounds::bv_stereo)     = stereo;

  m_valueStore.at(DiscTrapezoidalBounds::bv_minHalfX) = minhalfx;
  m_valueStore.at(DiscTrapezoidalBounds::bv_maxHalfX) = maxhalfx;
  if (m_valueStore.at(DiscTrapezoidalBounds::bv_minHalfX)
      > m_valueStore.at(DiscTrapezoidalBounds::bv_maxHalfX))
    std::swap(m_valueStore.at(DiscTrapezoidalBounds::bv_minHalfX),
         m_valueStore.at(DiscTrapezoidalBounds::bv_maxHalfX));

  m_valueStore.at(DiscTrapezoidalBounds::bv_rMax) = minR;
  m_valueStore.at(DiscTrapezoidalBounds::bv_rMin) = maxR;
  if (m_valueStore.at(DiscTrapezoidalBounds::bv_rMin)
      > m_valueStore.at(DiscTrapezoidalBounds::bv_rMax))
    std::swap(m_valueStore.at(DiscTrapezoidalBounds::bv_rMin),
         m_valueStore.at(DiscTrapezoidalBounds::bv_rMax));

  m_valueStore.at(DiscTrapezoidalBounds::bv_halfPhiSector)
      = asin(m_valueStore.at(DiscTrapezoidalBounds::bv_maxHalfX)
             / m_valueStore.at(DiscTrapezoidalBounds::bv_rMax));

  double hmax
      = sqrt(m_valueStore.at(DiscTrapezoidalBounds::bv_rMax)
                 * m_valueStore.at(DiscTrapezoidalBounds::bv_rMax)
             - m_valueStore.at(DiscTrapezoidalBounds::bv_maxHalfX)
                 * m_valueStore.at(DiscTrapezoidalBounds::bv_maxHalfX));

  double hmin
      = sqrt(m_valueStore.at(DiscTrapezoidalBounds::bv_rMin)
                 * m_valueStore.at(DiscTrapezoidalBounds::bv_rMin)
             - m_valueStore.at(DiscTrapezoidalBounds::bv_minHalfX)
                 * m_valueStore.at(DiscTrapezoidalBounds::bv_minHalfX));

  m_valueStore.at(DiscTrapezoidalBounds::bv_rCenter) = (hmax + hmin) / 2.;
  m_valueStore.at(DiscTrapezoidalBounds::bv_halfY)   = (hmax - hmin) / 2.;
}

Acts::DiscTrapezoidalBounds::~DiscTrapezoidalBounds()
{
}

Acts::DiscTrapezoidalBounds&
Acts::DiscTrapezoidalBounds::operator=(const DiscTrapezoidalBounds& disctrbo)
{
  if (this != &disctrbo) 
    DiscBounds::operator=(disctrbo);
  return *this;
}

double
Acts::DiscTrapezoidalBounds::minDistance(const Acts::Vector2D& pos) const
{
  const double pi2        = 2. * M_PI;
  double       alpha      = fabs(pos[Acts::eLOC_PHI]);
  if (alpha > M_PI) alpha = pi2 - alpha;

  double r = pos[Acts::eLOC_R];
  if (r == 0.)
    return m_valueStore.at(DiscTrapezoidalBounds::bv_rMin)
        * cos(m_valueStore.at(DiscTrapezoidalBounds::bv_halfPhiSector))
        / cos(alpha);

  // check if it is external in R
  double sr0 = m_valueStore.at(DiscTrapezoidalBounds::bv_rMin)
          * cos(m_valueStore.at(DiscTrapezoidalBounds::bv_halfPhiSector))
          / cos(alpha)
      - r;
  if (sr0 > 0.) return sr0;
  double sr1 = r
      - m_valueStore.at(DiscTrapezoidalBounds::bv_rMax)
          * cos(m_valueStore.at(DiscTrapezoidalBounds::bv_halfPhiSector))
          / cos(alpha);
  if (sr1 > 0.) return sr1;

  // check if it is external in phi
  if ((alpha - m_valueStore.at(DiscTrapezoidalBounds::bv_halfPhiSector)) > 0.)
    return r * fabs(sin(alpha
                        - m_valueStore.at(
                              DiscTrapezoidalBounds::bv_halfPhiSector)));

  // if here it is inside:
  // Evaluate the distance from the 4 segments
  double dist[4]
      = {sr0,
         sr1,
         -r * sin(m_valueStore.at(DiscTrapezoidalBounds::bv_halfPhiSector)
                  - alpha),
         -r * sin(m_valueStore.at(DiscTrapezoidalBounds::bv_halfPhiSector)
                  + alpha)};

  return *std::max_element(dist, dist + 4);
}

// ostream operator overload
std::ostream&
Acts::DiscTrapezoidalBounds::dump(std::ostream& sl) const
{
  sl << std::setiosflags(std::ios::fixed);
  sl << std::setprecision(7);
  sl << "Acts::DiscTrapezoidalBounds:  (innerRadius, outerRadius, hMinX, "
        "hMaxX, hlengthY, hPhiSector, averagePhi, rCenter, stereo) = ";
  sl << "(" << rMin() << ", " << rMax() << ", "
     << minHalflengthX() << ", " << maxHalflengthX() << ", "
     << halflengthY() << ", " << halfPhiSector() << ", "
     << averagePhi() << ", " << rCenter() << ", " << stereo()
     << ")";
  sl << std::setprecision(-1);
  return sl;
}
