// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// ActsExtension.h, ACTS project, DD4hepDetector plugin
///////////////////////////////////////////////////////////////////

#ifndef ACTS_DD4HEPDETECTORELEMENT_ACTSEXTENSION_H
#define ACTS_DD4HEPDETECTORELEMENT_ACTSEXTENSION_H 1

#include <ACTS/Plugins/DD4hepPlugins/IActsExtension.hpp>
#include <vector>
#include "DD4hep/Detector.h"

namespace Acts {

/// @class ActsExtension
///
/// @brief Extension of the \a %DD4hep \a DetElement needed for translation into
/// the
/// ACTS
/// tracking geometry
///
/// Implementation of the Acts::IActsExtension class, which uses the extension
/// mechanism of DD4hep for the \a %DD4hep \a DetElement.
/// This extensions are needed for the translation from the \a %DD4hep geometry
/// into
/// the tracking geometry of the ACTS package.
///
/// This extensions are necessary in order to distinguish in the translation if
/// a \a %DD4hep \a DetElement is
/// 	- the beampipe
///		- a barrel volume
///		- an endcap volume
/// 	- a layer
///
/// and to hand over needed parameters.
///
/// Every \a %DD4hep \a DetElement containing sensitive \a %DD4hep \a
/// DetElements
/// has to
/// be
/// declared as a layer. However the layer does not need to be the direct mother
/// of these sensitive \a DD4hep \a DetElements - they can also be nested in
/// other
/// \a %DD4hep \a DetElement substructures. Moreover every DD4hep \a DetElement
/// layer
/// which should carry material should also be declared as a layer and the
/// specific parameters needed for material mapping should be handed over.
/// In case the sensitive modules/components contained by a layer have a
/// different orientation in respect to the local tracking frame of ACTS, the
/// axes orientation of these modules can be set for the layer.
///
/// In \a %DD4hep cylinder and disc volumes are both described with the
/// underlying
/// \a ROOT \c TGeoConeSeg class. In ACTS one needs to distinguish these two
/// volume
/// types. Therefore volumes which are endcaps or barrels should be indicated as
/// these.
/// The ActsExtension should also be used to indicate that a \a DetElement is
/// the
/// beampipe.
///
/// In case the layers containing the sensitive modules are DD4hep::Assemblies
/// which have neither a shape nor a material/medium the two parameters
/// envelopeR and envelopeZ need to be set to a DetElement representing a layer.
/// In this case the geometrical extremities of the contained sensitive modules
/// are calculated and a tolerances (envelopeR & envelopeZ) are added to build
/// the envelope of the layer around the surfaces.
///
/// If one wants to build the ACTS Tracking Geometry with \a %DD4hep input these
/// extension should be used during the construction of the \a %DD4hep geometry
/// i.e. in the
/// \a %DD4hep detector constructors. First the ActsExtension configuration
/// object
/// should be created and then attached to the \a %DD4hep \a DetElement.
///
/// Example for a layer \a DetElement (\c layer_detElement) where also
/// parameters
/// for material mapping are handed over:
///
/// @code
/// Acts::ActsExtension::Config layConfig;
/// layConfig.isLayer               = true;
/// layConfig.axes                  = "XZy";
/// layConfig.materialBins1         = 50;
/// layConfig.materialBins2			= 100;
/// layConfig.layerMaterialPosition = Acts::LayerMaterialPos::inner
///
/// Acts::ActsExtension* layerExtension = new Acts::ActsExtension(layConfig);
/// layer_detElement.addExtension<Acts::IActsExtension>(layerExtension);
///
/// @endcode

class ActsExtension : public IActsExtension
{
public:
  /// The configuration object of an ActsExtension
  struct Config
  {
    /// Indicating that the DD4hep::DetElement is the beampipe
    bool isBeampipe;
    /// Indicating that the DD4hep::DetElement is a barrel
    bool isBarrel;
    /// Indicating that the DD4hep::DetElement is an endcap
    bool isEndcap;
    /// Indicating that the DD4hep::DetElement is a layer
    bool isLayer;
    /// This extension is needed to allow material mapping on a layer
    /// The number of bins indicate the granularity of the material map of one
    /// layer in the first direction which is phi for both, cylinder and disc
    /// layers.
    /// @note this extension should be set for a layer
    size_t materialBins1;
    /// This extension is needed to allow material mapping on a layer
    /// The number of bins indicate the granularity of the material map of one
    /// layer in the first direction which is r in case of a disc layer and z in
    /// case of a cylinder layer.
    /// @note this extension should be set for a layer
    size_t materialBins2;
    /// This extension is needed to allow material mapping
    /// States if the material should be mapped on the inner, the center or the
    /// outer surface of the layer
    /// @note this extension should be set for a layer
    LayerMaterialPos layerMaterialPosition;
    /// Orientation of the modules contained by a layer in respect to the
    /// tracking frame. A different orientation can occur because in TGeo (which
    /// is the underlying geometry model of %DD4hep) all shapes are 3D volumes
    /// also the sensitive components of a detector. In the ACTS tracking
    /// geometry these sensitive components are described as 2D surfaces, which
    /// have their local 2D coordinate system. Therefore one needs to know which
    /// coordinates should be taken as the local coordinates.
    /// A string of the three characters x, y and z (standing for the
    /// three axes) needs to be handed over. There is a distinction between
    /// capital and lower case
    /// characters :
    /// 	- capital      -> positive orientation of the axis
    ///		- lower case   -> negative oriantation of the axis
    ///
    ///
    /// Example options are:
    /// 	- "XYZ" -> identical frame definition (default value)
    /// 	- "YZX" -> node y axis is tracking x axis, etc.
    ///		- "XzY" -> negative node z axis is tracking y axis, etc.
    /// @note if the modules have a different orientation in respect to the
    /// tracking frame the axes should be set for the layer containing these
    /// modules
    std::string axes;
    /// In case the Layers of the TrackingGeometry holding the sensitive
    /// modules should be build automatically by the TrackingGeometry tools,
    /// e.g. if Layers are only helper structs holding the detector modules
    /// without any specific shape (Assemblies), or only sensitive detector
    /// modules are handed over and the user wants automatic adaption of
    /// surrounding Layers, these two tolerances (evelopeR & envelopeZ) should
    /// be set for a layer. A tolerance added to the geometrical expansion of
    /// the contained geometrical objects in r
    double envelopeR;
    /// In case the Layers of the TrackingGeometry holding the sensitive
    /// modules should be build automatically by the TrackingGeometry tools,
    /// e.g. if Layers are only helper structs holding the detector modules
    /// without any specific shape (Assemblies), or only sensitive detector
    /// modules are handed over and the user wants automatic adaption of
    /// surrounding Layers, these two tolerances (evelopeR & envelopeZ) should
    /// be set for a layer. A tolerance added to the geometrical expansion of
    /// the contained geometrical objects in z
    double envelopeZ;

    // default configuration
    Config()
      : isBeampipe(false)
      , isEndcap(false)
      , isLayer(false)
      , materialBins1(0)
      , materialBins2(0)
      , layerMaterialPosition(LayerMaterialPos::inner)
      , axes("XYZ")
      , envelopeR(0.)
      , envelopeZ(0.)
    {
    }
  };
  /// Constructor
  ActsExtension(const Config& cfg);
  /// Copy constructor
  ActsExtension(const ActsExtension&, const DD4hep::Geometry::DetElement&);
  /// Destructor
  ~ActsExtension() = default;
  /// Set configuration method
  /// @param config is the new configuration struct
  void
  setConfiguration(const Config& config);
  /// Indicates if the DD4hep::DetElement is the beampipe
  bool
  isBeampipe() const final;
  /// Indicates that the DD4hep::DetElement is a barrel
  bool
  isBarrel() const final;
  /// Indicates that the DD4hep::DetElement is an endcap
  bool
  isEndcap() const final;
  /// Indicates that the DD4hep::DetElement is a layer
  bool
  isLayer() const final;
  /// Bool returning true if the layers should carry material
  /// @note automatically set when the material bins are set
  bool
  hasSupportMaterial() const final;
  /// Access to the two bin numbers determining the granularity of the two
  /// dimensional grid on which the material of the layer should be mapped on
  /// @return std::pair with the number of bins in th first and the second
  /// direction
  std::pair<size_t, size_t>
  materialBins() const final;
  /// @return states if the material should be mapped on the inner,
  /// the center or the outer surface of the layer
  Acts::LayerMaterialPos
  layerMaterialPosition() const final;
  /// Access the orientation of the module in respect to the tracking frame
  /// @return string describing the orientation of the axes
  const std::string
  axes() const final;
  /// @return states if the geometrical boundaries of the current object should
  /// be built automatically by adding given tolerances to the expansion of the
  /// contained modules
  bool
  buildEnvelope() const final;
  /// @return the tolerance which should be added in r to the geometrical
  /// expansion of the contained surfaces (sensituive DetElements) of this
  /// DetElement to automatically create the layer envelope
  double
  envelopeR() const final;
  /// @return the tolerance which should be added in z to the geometrical
  /// expansion of the contained surfaces (sensituive DetElements) of this
  /// DetElement to automatically create the layer envelope
  double
  envelopeZ() const final;

private:
  /// The configuration object
  Config m_cfg;
};

inline bool
ActsExtension::isBeampipe() const
{
  return m_cfg.isBeampipe;
}

inline bool
ActsExtension::isBarrel() const
{
  return m_cfg.isBarrel;
}

inline bool
ActsExtension::isEndcap() const
{
  return m_cfg.isEndcap;
}

inline bool
ActsExtension::isLayer() const
{
  return m_cfg.isLayer;
}

inline bool
ActsExtension::hasSupportMaterial() const
{
  if ((m_cfg.materialBins1 > 0) || (m_cfg.materialBins2 > 0)) return true;
  return false;
}

inline std::pair<size_t, size_t>
ActsExtension::materialBins() const
{
  std::pair<size_t, size_t> materialBins(m_cfg.materialBins1,
                                         m_cfg.materialBins2);
  return (materialBins);
}

inline Acts::LayerMaterialPos
ActsExtension::layerMaterialPosition() const
{
  return m_cfg.layerMaterialPosition;
}

inline const std::string
ActsExtension::axes() const
{
  return m_cfg.axes;
}

inline bool
ActsExtension::buildEnvelope() const
{
  return ((m_cfg.envelopeR > 0.) && (m_cfg.envelopeZ > 0.));
}

inline double
ActsExtension::envelopeR() const
{
  return (m_cfg.envelopeR);
}

inline double
ActsExtension::envelopeZ() const
{
  return (m_cfg.envelopeR);
}
}

#endif  // ACTS_DD4HEPDETECTORELEMENT_ACTSEXTENSION_H