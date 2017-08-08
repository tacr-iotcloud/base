#pragma once

#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>

namespace BeeeOn {

class FailDetector {
public:
	const static int TRESHOLD_UNLIMITED = 0;

	/**
	 * The failsTreshold is number of fails needed to failure detection.
	 * Treshold = 0 means unlimited treshold.
	 */
	FailDetector(const unsigned int failsTreshold = TRESHOLD_UNLIMITED);

	~FailDetector();

	/**
	 * Increments count of fails and updates timeOfLastFail.
	 */
	void fail();

	/**
	 * Resets the fails counter.
	 */
	void success();

	/**
	 * Returns true if counted of fails greater than or equal to fails treshold.
	 */
	bool isFailed() const;

	/**
	 * Returns Timestamp containing the time of last fail.
	 */
	Poco::Timestamp timeOfLastFail() const;

	/**
	 * Returns true if the timeout has elapsed since last fail.
	 *
	 * Warning: If fail() was not called yet, returns ture if timeout is
	 * elapsed since creating FailDetector.
	 */
	bool lastFailBefore(const Poco::Timespan &timeout) const;

private:
	unsigned int m_fails;
	unsigned int m_failsTreshold;
	Poco::Timestamp m_timeOfLastFail;
};

}
