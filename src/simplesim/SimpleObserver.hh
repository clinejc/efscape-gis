// __COPYRIGHT_START__
// __COPYRIGHT_END__
#ifndef SIMPLESIM_SIMPLEOBSERVER_HH
#define SIMPLESIM_SIMPLEOBSERVER_HH

#include <boost/serialization/version.hpp>

// parent class definitions
#include "efscape/impl/adevs_config.hh"
#include "efscape/impl/EntityI.hh"
#include "efscape/impl/InitObject.hh"

namespace simplesim {

  class SimpleState;

  /**
   * Defines a simple observer class that records output from the
   * a SimpleGenerator object.
   *
   * @author Jon C. Cline <clinej@stanfordalumni.org>
   * @version 0.08 created 11 Oct 2006, revised 05 May 2010
   *
   * ChangeLog:
   *   - 2010.05.05 fixed bug (parent class InitObject undeclared)
   *   - 2009.12.06 parent class changed from ModelI (removed) to EntityI
   *   - 2009.03.04 Extensive modifications due to API changes
   *     - changes to serialization interface
   *     - changes to implementation of adevs model hierarchy
   *   - 2008.01.04 Extensive modifications due to API changes
   *     - base class efscape::impl::ObserverBase remove (deprecated)
   *     - dataset implemented with efscape::impl::DataFrameI
   *   - 2007.10.09 changed port type as per changes to base model class
   *   - 2007.01.23 implemented changes to SimpleGenerator interface
   *   - 2007.01.12 implemented changes to ModelType_impl interface
   *   - 2007.01.06 changed ModelType to ModelType_impl<>
   *   - 2006.10.24 added private static data member and accessor methods
   *     - efscape::impl::ModelType
   *   - 2006.10.17 created class 'SimpleObserver'
   */
  class SimpleObserver : virtual public efscape::impl::EntityI,
			 public efscape::impl::InitObject,
			 public adevs::Atomic<efscape::impl::IO_Type>
  {
    friend class boost::serialization::access;

  public:

    SimpleObserver();
    ~SimpleObserver();

    void initialize()
      throw (std::logic_error);

    // Observer input port
    void input(const SimpleState* aCp_value) {}

    /// Internal transition function.
    void delta_int();

    /// External transition function.
    void delta_ext(double e, const adevs::Bag<efscape::impl::IO_Type>& xb);

    /// Confluent transition function.
    void delta_conf(const adevs::Bag<efscape::impl::IO_Type>& xb);
 
    /// Output function.  
    void output_func(adevs::Bag<efscape::impl::IO_Type>& yb);

    /// Time advance function.
    double ta();

    /// Output value garbage collection.
    void gc_output(adevs::Bag<efscape::impl::IO_Type>& g);

  private:

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & boost::serialization::make_nvp("EntityI",
					  boost::serialization::base_object<efscape::impl::EntityI >(*this) );
      ar & boost::serialization::make_nvp("Atomic",
					  boost::serialization::base_object<efscape::impl::ATOMIC >(*this) );
    }

  };				// class SimpleObserver definition

  // class SimpleObserver reflection
  struct SimpleObserver_strings {
    static const char name[];
    static const char m_input[];
  };

} // namespace simplesim

BOOST_CLASS_VERSION(simplesim::SimpleObserver, 9)

#endif	// #ifndef SIMPLESIM_SIMPLEOBSERVER_HH