// __COPYRIGHT_START__
// Package Name : efscape
// File Name : SimulatorI.cc
// Copyright (C) 2006-2014 by Jon C. Cline (clinej@alumni.stanford.edu)
// Distributed under the terms of the LGPLv3 or newer.
// __COPYRIGHT_END__
#include <efscape/server/SimulatorI.hh>

namespace efscape {

  namespace server {

    /** default constructor */
    SimulatorI::SimulatorI() :
      mCp_model(0)
    {}

    /**
     * constructor
     *
     * @param aCp_model
     */
    SimulatorI::SimulatorI(const ::efscape::ModelPrx& aCp_model) :
      mCp_model(aCp_model)
    {}

    /** destructor */
    SimulatorI::~SimulatorI() {}

    /**
     * Invokes its components initialize method.
     *
     * @param current method invocation
     * @returns status of call
     */
    bool
    SimulatorI::start(const Ice::Current& current)
    {
      if (!mCp_model)
	return false;

      return mCp_model->initialize();
    }

    /**
     * Returns time of next event.
     *
     * Equivalent to:\n
     *   - double tN_yet (Zeigler et al 1999)\n
     *   - double timeNext() (adevs 1.3.x/Simulator)\n
     *
     * @param current method invocation
     * @returns time of next event
     */
    ::Ice::Double
    SimulatorI::nextEventTime(const Ice::Current& current)
    {
      if (!mCp_model)
	return 0.0;

      return mCp_model->timeAdvance();
    }

    /**
     * Executes the next event.
     *
     * @param current method invocation
     */
    void
    SimulatorI::execNextEvent(const Ice::Current& current)
    {
      if (!mCp_model)
	return;

      mCp_model->internalTransition();
    }

    /**
     * Compute the output of the imminent models
     *
     * @param current method invocation
     */
     void
    SimulatorI::computeNextOutput(const Ice::Current& current)
    {
    }

    /**
     * Apply the specified inputs at the specified time and compute the
     * next model state.
     *
     * @param elapsedTime elapsed time
     * @param msg specified inputs
     * @param current method invocation
     */
     void
    SimulatorI::computeNextState(::Ice::Double elapsedTime,
				 const ::efscape::Message& msg,
				 const Ice::Current& current)
    {
    }
    /**
     * Returns true if the simulator should stop (e.g. model becomes passive).
     *
     * @param current method invocation
     * @returns whether simulator should stop
     */
    bool
    SimulatorI::halt(const Ice::Current& current)
    {
      return false;
    }

    /**
     * Destroys the model.
     *
     * @param current method invocation
     */
    void SimulatorI::destroy(const Ice::Current& current)
    {
      try {
	current.adapter->remove(current.id);
      } catch (const Ice::NotRegisteredException&) {
	throw Ice::ObjectNotExistException(__FILE__, __LINE__);
      }
    }

  } // namespace server

} // namespace efscape
