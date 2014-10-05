// __COPYRIGHT_START__
// Package Name : efscape
// File Name : AdevsModel.hh
// Copyright (C) 2006-2014 by Jon C. Cline (clinej@alumni.stanford.edu)
// Distributed under the terms of the LGPLv3 or newer.
// __COPYRIGHT_END__
#ifndef EFSCAPE_IMPL_ADEVSMODEL_HH
#define EFSCAPE_IMPL_ADEVSMODEL_HH

// boost serialization definitions
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/shared_ptr.hpp>

// parent class definition
#include <efscape/impl/EntityI.hh>
#include <efscape/impl/adevs_config.hh>
#include <efscape/impl/InitObject.hh>
#include <efscape/impl/Cloneable.hh>

// data member definitions
#include <log4cxx/logger.h>
#include <efscape/gis/LandscapeI.hpp>
#include <efscape/impl/ClockI.hh>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <typeinfo>

namespace efscape {

  namespace impl {

    // forward declaration
    class AdevsModel;

    /**
     * Implements an adevs-based ICE ::efscape::model wrapper.
     *
     * @author Jon Cline <clinej@stanfordalumni.org>
     * @version 0.0.10 created 24 Dec 2006, revised 31 May 2010
     */
    class AdevsModel : virtual public EntityI,
		       public ModelDecorator,
		       public InitObject
    {
      friend class boost::serialization::access;

    public:

      AdevsModel();
      ~AdevsModel();

      Cloneable* clone() const;

      void initialize()
	throw(std::logic_error);
      void run()
	throw(std::logic_error);

      //------------------
      // adevs model ports
      //------------------
      /** clock output port */
      ClockI* clock_out;

      // /** data output port */
      // DataFrameI* data_out;

      // /** grid coverage output port */
      // boost::any ogc_gc_out;

      // /** simple feature output port */
      // boost::any ogc_sf_out;

      //-------------------------
      // accessor/mutator methods
      //-------------------------
      //
      // clock
      //
      ClockIPtr& getClockIPtr();
      void setClockIPtr(const ClockIPtr& aCp_clock);
      const ClockI* getClock() const;

      //
      // comment
      //
      const char* comment() const;
      void comment(const char* acp_comment);

      //
      // landscape
      //
      efscape::gis::LandscapeIPtr& getLandscapeIPtr();
      void setLandscapeIPtr(const efscape::gis::LandscapeIPtr& aCp_landscape);
      efscape::gis::LandscapeI* getLandscape();
      const efscape::gis::LandscapeI* getLandscape() const;

      log4cxx::LoggerPtr getLogger() const;

      std::string getResourcePath() const;
      void setResourcePath(const char* acp_path);

      std::string getWorkDir() const;
      void setWorkDir(const char* acp_WorkDir);

      void addObserver(Observer* aCp_observer);
      void getObservers(std::set< ObserverPtr >& aCCpr_observer);

    protected:

      /** simulation clock (implementation) */
      ClockIPtr mCp_ClockI;

      /** job id */
      int mi_JobID;

    private:

      template<class Archive>
      void save(Archive & ar, const unsigned int version) const
      {
	// save parent class data
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(EntityI);
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ModelDecorator);

	// save comment
	ar  & boost::serialization::make_nvp("comment",mC_comment);

	// save clock
	ar  & boost::serialization::make_nvp("Clock",mCp_ClockI);

	// save resource path
	ar & boost::serialization::make_nvp("resource_path", mC_ResourcePath);
      }

      template<class Archive>
      void load(Archive & ar, const unsigned int version)
      {
	// read parent class data
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(EntityI);
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ModelDecorator);

	// save comment
	ar  & boost::serialization::make_nvp("comment",mC_comment);

	// load clock
	ar  & boost::serialization::make_nvp("Clock",mCp_ClockI);

	// load resource path
	ar & boost::serialization::make_nvp("resource_path", mC_ResourcePath);
      }

      BOOST_SERIALIZATION_SPLIT_MEMBER();

      /** comment */
      std::string mC_comment;

     //  /** handle to model landscape */
     // efscape::gis::LandscapeIPtr mCp_Landscape;

      /** set of observers */
      std::set<ObserverPtr> mCCp_observers;

      /** path of simulator resources (on server) */
      std::string mC_ResourcePath;

      /** path of simulator model output (on server) */
      std::string mC_WorkDir;

      /** job id generator */
      static int mSi_nextJobID;

    };				// class AdevsModel definition

    // utility method for retrieving the root AdevsModel
    const AdevsModel* getAdevsModel(adevs::Devs<IO_Type>* aCp_model);

    // class AdevsModel reflection
    struct AdevsModel_strings {
      static const char name[];
      static const char f_clock_out[];
      static const char f_data_out[];
      static const char f_ogc_gc_out[];
      static const char f_ogc_sf_out[];
    };


  } // namespace impl

} // namespace efscape

BOOST_CLASS_VERSION(efscape::impl::AdevsModel,10)

#endif	// #ifndef EFSCAPE_IMPL_ADEVSMODEL_HH