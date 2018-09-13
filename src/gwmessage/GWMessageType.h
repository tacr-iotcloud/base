#pragma once

#include "util/Enum.h"

namespace BeeeOn {

struct GWMessageTypeEnum {
	enum Raw {
		DEVICE_ACCEPT_REQUEST,
		DEVICE_LIST_REQUEST,
		DEVICE_LIST_RESPONSE,
		GATEWAY_REGISTER,
		GATEWAY_ACCEPTED,
		GENERIC_RESPONSE,
		GENERIC_ACK,
		LAST_VALUE_REQUEST,
		LAST_VALUE_RESPONSE,
		LISTEN_REQUEST,
		NEW_DEVICE_GROUP_REQUEST,
		NEW_DEVICE_REQUEST,
		PING_REQUEST,
		RESPONSE_WITH_ACK,
		SENSOR_DATA_CONFIRM,
		SENSOR_DATA_EXPORT,
		SET_VALUE_REQUEST,
		UNPAIR_REQUEST,
		UNPAIR_RESPONSE,
	};

	static EnumHelper<Raw>::ValueMap &valueMap();
};

typedef Enum<GWMessageTypeEnum> GWMessageType;

}
