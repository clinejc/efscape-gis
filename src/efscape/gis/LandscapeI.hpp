// __COPYRIGHT_START__
// Package Name : efscape
// File Name : LandscapeI.hpp
// Copyright (C) 2006-2014 by Jon C. Cline (clinej@alumni.stanford.edu)
// Distributed under the terms of the LGPLv3 or newer.
// __COPYRIGHT_END__
#ifndef EFSCAPE_GIS_LANDSCAPEI_HPP
#define EFSCAPE_GIS_LANDSCAPEI_HPP

#include <efscape/gis/Geogrid_impl.hh>
#include <gdal_priv.h>		// Geospatial Data Abstraction Library
#include <ogrsf_frmts.h>	// Adds support for GDAL/OGR simple features
#include <string>
#include <vector>
#include <set>
#include <map>

namespace efscape {

  namespace gis {

    // forward declarations
    class LandscapeI;

    // typedefs
    typedef std::shared_ptr<LandscapeI> LandscapeIPtr;
    typedef std::shared_ptr<OGRLayer> OGRLayerPtr;
    typedef std::shared_ptr<GDALDataset> GDALDatasetPtr;
    typedef std::set<GeogridPtr> GeogridSet;
    typedef std::map<std::string, GeogridPtr> GeogridMap;
    typedef std::set<OGRLayerPtr> OGRLayerSet;
    typedef std::map<std::string, OGRLayerPtr> OGRLayerMap;
    typedef std::map<std::string, GDALDatasetPtr> GDALDatasetMap;

    /**
     * The LandscapeI class provides a single unified interface for accessing
     * geospatial data. It includes covenience functions for accessing
     * individual raster grids by name.<br>
     * Major revision due to a major revision of GDAL/OGR (version 2.x).
     *
     * @author Jon Cline <clinej@stanfordalumni.org>
     * @version 1.0 created 13 Sep 2006, revised 05 Oct 2014
     *
     * ChangeLog:
     *  - 2014-10-05 major revision corresponding to GDAL/OGR updates.
     *  - 2010-11-26 revised
     *  - 2006-09-06 Created class LandscapeI
     */
    class LandscapeI {

    public:

      LandscapeI();
      ~LandscapeI();

      //
      // methods for accessing geogrids
      //
      Geogrid* addGeogrid(Geogrid* aCp_grid);

      Geogrid* getGeogrid(const char* acp_name);
      ByteGeogrid* getByteGeogrid(const char* acp_name);
      ShortGeogrid* getShortGeogrid(const char* acp_name);
      IntGeogrid* getIntGeogrid(const char* acp_name);
      LongGeogrid* getLongGeogrid(const char* acp_name);
      FloatGeogrid* getFloatGeogrid(const char* acp_name);
      DoubleGeogrid* getDoubleGeogrid(const char* acp_name);

      const Geogrid* getGeogrid(const char* acp_name) const;
      const ByteGeogrid* getByteGeogrid(const char* acp_name) const;
      const ShortGeogrid* getShortGeogrid(const char* acp_name) const;
      const IntGeogrid* getIntGeogrid(const char* acp_name) const;
      const LongGeogrid* getLongGeogrid(const char* acp_name) const;
      const FloatGeogrid* getFloatGeogrid(const char* acp_name) const;
      const DoubleGeogrid* getDoubleGeogrid(const char* acp_name) const;

      void getGeogrids(GeogridSet& aCCpr_geogrids);

      //
      // methods for accessing feature layers
      //
      void addDataset( std::string aC_name,
		       const GDALDatasetPtr& aCp_dataset );

      GDALDatasetPtr addDataset( std::string aC_DriverName,
				 std::string aC_FileName,
				 std::string aC_name );
      
      GDALDatasetPtr getDataset(std::string aC_name);

      void addLayer( std::string aC_name,
		     const OGRLayerPtr& aCp_layer );

      OGRLayerPtr getLayer(std::string aC_name);
      
    protected:

      /** map of landscape geogrid layers */
      std::unique_ptr<GeogridMap> mCCCp_Geogrids;

      /** map of landscape feature layers */
      std::unique_ptr<OGRLayerMap> mCpCp_layers;

      /** map of landscape feature datasets */
      std::unique_ptr<GDALDatasetMap> mCpCp_datasets;

    };				// class LandscapeI definition

  } // namespace gis

} // namespace efscape

#endif	// #ifndef EFSCAPE_GIS_LANDSCAPEI_HPP
