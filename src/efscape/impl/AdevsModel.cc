// __COPYRIGHT_START__
// Package Name : efscape
// File Name : AdevsModel.cc
// Copyright (C) 2006-2014 by Jon C. Cline (clinej@alumni.stanford.edu)
// Distributed under the terms of the LGPLv3 or newer.
// __COPYRIGHT_END__
#include <efscape/impl/AdevsModel.hh>

// definitions for accessing the model factory
#include <efscape/impl/ModelHomeI.hh>

#include <efscape/utils/type.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <sstream>

using namespace boost::posix_time;
using namespace boost::filesystem;

namespace efscape {

  namespace impl {

    // set the class name and port names
    const char AdevsModel_strings::name[] = "efscape::impl::AdevsModel";
    const char AdevsModel_strings::f_clock_out[] = "clock_out";
    const char AdevsModel_strings::f_data_out[] = "data_out";
    const char AdevsModel_strings::f_ogc_gc_out[] = "ogc_gc_out";
    const char AdevsModel_strings::f_ogc_sf_out[] = "ogc_sf_out";

    // instantiate class data members
    int AdevsModel::mSi_nextJobID = 0;

    /** constructor */
    AdevsModel::AdevsModel() :
      EntityI(),
      mC_ResourcePath("."),
      mC_WorkDir("."),
      mi_JobID(0)
    {
      name("efscape");	// root model name (name of main library)

      // mCp_Landscape.reset(new efscape::gis::LandscapeI);

      mi_JobID = ++mSi_nextJobID;

    } // AdevsModel::AdevsModel()

    /** destructor */
    AdevsModel::~AdevsModel()
    {
      LOG4CXX_DEBUG(getLogger(),
		    "Deleting root model<" << name() << ">...");

      DEVS* lCp_WrappedModel = getWrappedModel();
      if (lCp_WrappedModel != NULL)
	delete lCp_WrappedModel;
    }

    /**
     * Returns a copy of this object.
     *
     * @returns handle to new object copy
     */
    Cloneable* AdevsModel::clone() const {

      // std::string lC_error_msg = "AdevsModel::clone()";
      // std::string lC_class_name = efscape::utils::type<AdevsModel*>(*this);

      // // create clone
      // DEVS* lCp_model = dynamic_cast<DEVS*>(this->EntityI::clone());
      // if (lCp_model == NULL)
      // 	return 0;

      // AdevsModel* lCp_clone;
      // if ( (lCp_clone = dynamic_cast<AdevsModel*>(lCp_model)) == 0) {
      // 	delete lCp_clone;
      // 	lC_error_msg += "\n\tUnable to cast model class <"
      // 	  + lC_class_name + "> as a AdevsModel model";
      // 	throw std::logic_error( lC_error_msg );
      // }

      // // clone wrapped model
      // const DEVS* lCp_WrappedModel = getWrappedModel();
      // DEVS* lCp_WrappedModelClone;
      // if (lCp_WrappedModel) {
      // 	if ( ( lCp_WrappedModelClone = cloneModel(lCp_WrappedModel) ) ) {
      // 	  lCp_clone->setWrappedModel(lCp_WrappedModelClone);
      // 	}
      // 	else {
      // 	  delete lCp_clone;

      // 	  lC_error_msg += "\n\tUnable to clone wrapped model for model class <"
      // 	    + lC_class_name + ">";
      // 	  throw std::logic_error( lC_error_msg );
      // 	}
      // }	// if (lCp_WrappedModel)

      // // copy comment
      // lCp_clone->comment(comment());

      // // clone clock
      // lCp_clone->mCp_ClockI = mCp_ClockI;

      // // copy resource path
      // lCp_clone->setResourcePath(getResourcePath().c_str());

      // return dynamic_cast<Cloneable*>(lCp_model);

      return NULL;

    } // AdevsModel::clone() const

    /**
     * Initializes the model. The primary model is paired with a simulator.
     * Derived classes are responsible for generating the model hierarchy (pre-
     * initialization) and supplying additional listeners (post-initialization).
     *
     * @throws std::logic_error
     */
    void AdevsModel::initialize()
      throw(std::logic_error)
    {
      std::string lC_message = "AdevsModel::initialize(): ";

      LOG4CXX_DEBUG(getLogger(),
		    lC_message << "initializing root model..."); 

      // verify that the wrapped model has been set properly
      adevs::Devs<IO_Type>* lCp_model = getWrappedModel();
      if (lCp_model == 0) {
	  lC_message += "main model does not exist";
	  throw std::logic_error(lC_message);
      }

      // create output directory in $EFSCAPE_HOME/jobs/scenario-id
      std::ostringstream lC_OstringJobID;
      lC_OstringJobID << this->name() << "-"
		      << std::setw(4) << std::setfill('0') << mi_JobID;
      std::string lC_ModelWorkDir =
	ModelHomeI::getHomeDir() + "/jobs/"
	+ lC_OstringJobID.str();

      path p = complete(path(lC_ModelWorkDir.c_str()));
      if ( !exists(p) ) {
	try {
	  create_directory(p);
	}
	catch(std::exception& e) {
	  lC_message += e.what();
	  throw std::logic_error(lC_message);
	}
      }
      this->setWorkDir(lC_ModelWorkDir.c_str());

      if (!initializeModel(lCp_model)) {
	lC_message += "unable to initialize wrapped model";
	throw (std::logic_error(lC_message));
      }

    } // AdevsModel::initialize()

    /**
     * Runs the model simulation locally.
     *
     * @throws std::logic_error
     */
    void AdevsModel::run()
      throw(std::logic_error)
    {
      if (getWrappedModel() == 0) {
	std::string lC_error_msg =
	  "AdevsModel::run(): wrapped model is missing\n";
	throw std::logic_error(lC_error_msg);
      }

      boost::scoped_ptr< adevs::Simulator<IO_Type> > lCp_simulator;
      lCp_simulator.reset( new adevs::Simulator<IO_Type>(this) );
      
      // simulate model until infinity (DBL_MAX)
      while ( (mCp_ClockI->time() = lCp_simulator->nextEventTime()) < DBL_MAX) {
	lCp_simulator->execNextEvent();
      }

    } // AdevsModel::run(...)

    //-------------------------
    // accessor/mutator methods
    //-------------------------

    /** @returns handle to clock */
    ClockIPtr& AdevsModel::getClockIPtr() { return mCp_ClockI; }

    /**
     * Sets the handle to a clock.
     *
     * @param aCp_clock handle to clock
     */
    void AdevsModel::setClockIPtr(const ClockIPtr& aCp_clock) {
      mCp_ClockI = aCp_clock;
    }

    /** @returns handle to clock */
    const ClockI* AdevsModel::getClock() const { return mCp_ClockI.get(); }

    /** @returns comment */
    const char* AdevsModel::comment() const { return mC_comment.c_str(); }

    /**
     * Sets comment.
     *
     * @param acp_comment comment
     */
    void AdevsModel::comment(const char* acp_comment) { mC_comment = acp_comment; }

    // /** @returns smart handle to landscape */
    // efscape::gis::LandscapeIPtr& AdevsModel::getLandscapeIPtr() { return mCp_Landscape; }

    // /**
    //  * Sets the handle to the landscape.
    //  *
    //  * @param aCp_landscape handle to landscape
    //  */
    // void AdevsModel::setLandscapeIPtr(const efscape::gis::LandscapeIPtr& aCp_landscape) {
    //   mCp_Landscape = aCp_landscape;
    // }

    // /** @returns handle to model landscape */
    // efscape::gis::LandscapeI* AdevsModel::getLandscape() { return mCp_Landscape.get(); }

    // /** @returns handle to model landscape */
    // const efscape::gis::LandscapeI* AdevsModel::getLandscape() const { return mCp_Landscape.get(); }

    /**
     * Adds an observer.
     *
     * @param aCp_observer handle to observer
     */
    void AdevsModel::addObserver(Observer* aCp_observer)
    {
      ObserverPtr lCp_observer( aCp_observer ); // wrap with smart pointer
      mCCp_observers.insert( lCp_observer );
    }

    /**
     * Returns a reference to the set of observers.
     *
     * @param aCCpr_observers reference to the set of observers
     */
    void AdevsModel::getObservers(std::set<ObserverPtr>& aCCpr_observers)
    {
      aCCpr_observers = mCCp_observers;
    }

    /**
     * Returns smart handle to logger
     *
     * @returns smart handle to logger
     */
    log4cxx::LoggerPtr AdevsModel::getLogger() const
    {
      std::ostringstream lC_OstringJobID;
      lC_OstringJobID << this->name() << "-"
		      << std::setw(4) << std::setfill('0') << mi_JobID;

      std::string lC_LoggerName =
	ModelHomeI::getLogger()->getName() + "." + lC_OstringJobID.str();

      return log4cxx::Logger::getLogger(lC_LoggerName.c_str());
    }

    /**
     * Returns the resource path on the server.
     *
     * @returns resource path
     */
    std::string AdevsModel::getResourcePath() const { return mC_ResourcePath; }

    /**
     * Sets the path of resources on the server.
     *
     * @param acp_path resource path
     */
    void AdevsModel::setResourcePath(const char* acp_path) { mC_ResourcePath = acp_path;}

    /**
     * Returns the model output path on the server.
     *
     * @returns model output path
     */
    std::string AdevsModel::getWorkDir() const { return mC_WorkDir; }

    /**
     * Sets the model output path on the server.
     *
     * @param acp_WorkDir new output path
     */
    void AdevsModel::setWorkDir(const char* acp_WorkDir) { mC_WorkDir = acp_WorkDir; }

    // class AdevsModel implementation

    /**
     * Utility function that returns the root AdevsModel of the adevs hierarchy
     * containing this model.
     *
     * @param aCp_model
     * @returns handle to root model
     */
    const AdevsModel* getAdevsModel(adevs::Devs<IO_Type>* aCp_model) {
      return dynamic_cast<const AdevsModel*>(getRootModel(aCp_model));
    }

  } // namespace impl

} // namespace efscape