// This file is part of the Acts project.
//
// Copyright (C) 2016-2018 Acts project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// ExtrapolationEngine.cpp, Acts project
///////////////////////////////////////////////////////////////////

#include "Acts/Extrapolation/ExtrapolationEngine.hpp"

// constructor
Acts::ExtrapolationEngine::ExtrapolationEngine(
    const Acts::ExtrapolationEngine::Config& eeConfig,
    std::unique_ptr<const Logger>            logger)
  : m_cfg(), m_logger(std::move(logger))
{
  setConfiguration(eeConfig);
}

// destructor
Acts::ExtrapolationEngine::~ExtrapolationEngine()
{
}

// configuration
void
Acts::ExtrapolationEngine::setConfiguration(
    const Acts::ExtrapolationEngine::Config& eeConfig)
{
  // steering of the screen outoput (SOP)
  IExtrapolationEngine::m_sopPrefix  = eeConfig.prefix;
  IExtrapolationEngine::m_sopPostfix = eeConfig.postfix;
  // copy the configuration
  m_cfg = eeConfig;
}

void
Acts::ExtrapolationEngine::setLogger(std::unique_ptr<const Logger> newLogger)
{
  m_logger = std::move(newLogger);
}

Acts::ExtrapolationCode
Acts::ExtrapolationEngine::extrapolate(ExCellCharged&       ecCharged,
                                       const Surface*       sf,
                                       const BoundaryCheck& bcheck) const
{
  return extrapolateT<TrackParameters>(
      ecCharged, sf, ecCharged.navigationDirection, bcheck);
}

/** neutral extrapolation */
Acts::ExtrapolationCode
Acts::ExtrapolationEngine::extrapolate(ExCellNeutral&       ecNeutral,
                                       const Surface*       sf,
                                       const BoundaryCheck& bcheck) const
{
  return extrapolateT<NeutralParameters>(
      ecNeutral, sf, ecNeutral.navigationDirection, bcheck);
}