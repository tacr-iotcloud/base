#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "util/Loggable.h"
#include "loop/StoppableRunnable.h"

namespace Poco {

class Thread;

}

namespace BeeeOn {

class StoppableLoop {
public:
	typedef Poco::SharedPtr<StoppableLoop> Ptr;

	virtual ~StoppableLoop();

	/**
	 * Starts the loop to process in an internally managed thread.
	 */
	virtual void start() = 0;

	/**
	 * Stop the loop and wait until finished.
	 */
	virtual void stop() = 0;
};

class StoppableLoopAdapter :
	public Loggable,
	public StoppableLoop {
public:
	StoppableLoopAdapter(Poco::SharedPtr<StoppableRunnable> runnable);
	~StoppableLoopAdapter();

	void start() override;
	void stop() override;

	void setStopTimeout(const Poco::Timespan &timeout);

	Poco::SharedPtr<StoppableRunnable> runnable() const;

protected:
	void doStop();

private:
	Poco::Timespan m_stopTimeout;
	Poco::SharedPtr<StoppableRunnable> m_runnable;
	Poco::Thread *m_thread;
};

}
