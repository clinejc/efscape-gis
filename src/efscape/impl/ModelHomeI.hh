// __COPYRIGHT_START__
// Package Name : efscape
// File Name : ModelHomeI.hh
// Copyright (C) 2006-2009 by Jon C. Cline
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Bugs, comments, and questions can be sent to clinej@stanfordalumni.org
// __COPYRIGHT_END__
#ifndef EFSCAPE_IMPL_MODELHOME_I_HH
#define EFSCAPE_IMPL_MODELHOME_I_HH

#include <efscape/impl/adevs_config.hh>
#include <efscape/utils/CommandOpt.hh>
#include <efscape/impl/ModelFactory.hpp>
#include <loki/Factory.h>
#include <boost/scoped_ptr.hpp>
#include <log4cxx/logger.h>

// boost serialization definitions
#include <boost/serialization/version.hpp>
#include <boost/serialization/type_info_implementation.hpp>

#include <stdexcept>
#include <efscape/utils/type.hpp>

namespace efscape {

  namespace impl {

    // typedefs
    typedef ::Loki::Factory<efscape::utils::CommandOpt,
			    std::string> CommandFactory;

    /**
     * Implements the local server-side ModelHome interface, a factory for\
     * simulation models.
     *
     * @author Jon Cline <clinej@stanfordalumni.org>
     * @version 0.0.10 created 24 Dec 2006, revised 13 Sep 2014
     *
     * ChangeLog:
     *   - 2014-09-13 Add methods for loading models with config files 
     *   - 2014-08-12 Added CommandFactory (moved from efscape::utils)
     *   - 2014-07-16 Merged all singletons into ModelHome*
     *   - 2009-09-11 Added class accessor method for a adevs cell factory.
     *   - 2009-03-11 revised
     *   - 2009-02-28 replaced ModelI* return with adevs
     *   - 2006-12-24 created Class
     */
    class ModelHomeI
    {
    public:

      ModelHomeI();
      virtual ~ModelHomeI();

      // methods for creating models
      adevs::Devs<IO_Type>* CreateModel(const char* acp_classname);
      adevs::Devs<IO_Type>* CreateModelFromFile(const char* acp_filename)
	throw(std::logic_error);
      adevs::Devs<IO_Type>* CreateModelFromStr(const std::wstring& aCr_buffer)
	throw(std::logic_error);
      adevs::Devs<IO_Type>*
      CreateModelWithConfig(const char* acp_classname,
			    const char* acp_propsfile)
	throw(std::logic_error);
      adevs::Devs<IO_Type>*
      CreateModelWithConfig(const char* acp_classname,
			    const std::wstring& aCr_propsfile)
	throw(std::logic_error);

      ModelFactory& GetModelFactory();
      CommandFactory& TheCommandFactory();

      /**
       * Returns the output path on the server.
       *
       * @returns output path
       */
      static std::string getHomeDir() { return mSC_HomeDir; }

      /**
       * Sets the path of resources on the server.
       *
       * @param acp_path resource path
       */
      void setHomeDir(const char* acp_path) { mSC_HomeDir = acp_path;}

      static void LoadLibrary(const char* acp_libname)
	throw(std::logic_error);

      static void LoadLibraries()
	throw(std::logic_error);

      static log4cxx::LoggerPtr& getLogger();

    private:

      /** path of resources (on server) */
      static std::string mSC_HomeDir;

      /** smart handle to program log */
      static log4cxx::LoggerPtr mSCp_logger;

      boost::scoped_ptr<ModelFactory> mCp_ModelFactory;
      boost::scoped_ptr<CommandFactory> mCp_CommandFactory;

    };				// class ModelHomeI definition

  } // namespace impl

} // namespace efscape

#endif	// #ifndef EFSCAPE_IMPL_MODELHOME_I_HH
