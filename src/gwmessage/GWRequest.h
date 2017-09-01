#ifndef BEEEON_GW_MESSAGE_REQUEST_H
#define BEEEON_GW_MESSAGE_REQUEST_H

#include <functional>

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWMessage.h"
#include "gwmessage/GWMessageType.h"
#include "gwmessage/GWResponse.h"
#include "model/GlobalID.h"

namespace BeeeOn {

/**
 * @brief Abstract class representing a request message. The GWRequest can not
 * be used alone, but it must be inherited by a specific request object.
 *
 * The GWRequest contains unique identifier intended to match a response
 * to a proper request. After request is sent, the sender expect a response
 * with the given id.
 *
 * Each GWRequest that would generate a specific response should override
 * the default implementation of the deriveResponse() method.
 */
class GWRequest : public GWMessage {
protected:
	GWRequest(const GWMessageType::Raw &type);
	GWRequest(const Poco::JSON::Object::Ptr object);

public:
	typedef Poco::SharedPtr<GWRequest> Ptr;

	/**
	 * @brief Set a unique identifier intended to match a proper response.
	 */
	void setID(const GlobalID &id);

	/**
	 * @brief Returns a unique identifier intended to match a proper response.
	 */
	GlobalID id() const;

	/**
	 * @brief Derive the appropriate response and apply the given function
	 * to construct it.
	 */
	template <typename T>
	Poco::SharedPtr<T> derive(
		std::function<void(Poco::SharedPtr<T>)> f = [](Poco::SharedPtr<T>){}) const
	{
		Poco::SharedPtr<T> response = deriveResponse().cast<T>();
		f(response);
		return response;
	}

	/**
	 * @brief Derive the appropriate response and apply the given function
	 * to construct it.
	 */
	GWResponse::Ptr derive(
		std::function<void(GWResponse::Ptr)> f = [](GWResponse::Ptr){}) const
	{
		GWResponse::Ptr response = deriveResponse();
		f(response);
		return response;
	}

protected:
	virtual GWResponse::Ptr deriveResponse() const;
	GWResponse::Ptr deriveGenericResponse(GWResponse::Ptr response) const;
};

}

#endif