#ifndef BEEEON_MODULE_TYPE_H
#define BEEEON_MODULE_TYPE_H

#include <set>

#include "model/CustomTypeID.h"
#include "util/Enum.h"

namespace BeeeOn {

/**
 * Representation of value type that device can send. Each value
 * consists of two parts: Type and Attribute.
 *
 * The attribute is optional. The type is high level data type
 * and the attribute is additional information, for example
 * location (inner, outer).
 */
class ModuleType {
public:
	struct AttributeEnum {
		enum Raw {
			/**
			 * The sensor/actuator is located inside (a building).
			 */
			ATTR_INNER = 1,
			/**
			 * The sensor enables to set (to modify) value only from sensor.
			 * This attribute makes sense only in combination with
			 * the ATTR_CONTROLLABLE.
			 */
			ATTR_MANUAL_ONLY = 2,
			/**
			 * The sensor/actuator is located outside.
			 */
			ATTR_OUTER = 3,
			/**
			 * The sensor of this type is controllable (it is an
			 * actuator).
			 */
			ATTR_CONTROLLABLE = 4,
		};

		static EnumHelper<Raw>::ValueMap &valueMap();
	};

	struct TypeEnum {
		enum Raw {
			/**
			 * Value 0: unreachable
			 * Value 1: reachable
			 */
			TYPE_AVAILABILITY = 1,
			/**
			 * Range: 0..100
			 * Unit: %
			 */
			TYPE_BATTERY = 2,
			/**
			 * It defines specific bitmap module
			 * type which is stored in m_customID.
			 */
			TYPE_BITMAP = 3,
			/**
			 * Range: 0..100
			 * Unit: %
			 */
			TYPE_BRIGHTNESS = 4,
			/**
			 * Min: 0
			 * Unit: one part per million (ppm)
			 */
			TYPE_CO2 = 5,
			/**
			 * It defines specific enum module
			 * type which is stored in m_customID.
			 */
			TYPE_ENUM = 6,
			/**
			 * Value 0: no fire detected
			 * Value 1: fire detected
			 */
			TYPE_FIRE = 7,
			/**
			 * Range: 0..100
			 * Unit: %
			 */
			TYPE_HUMIDITY = 8,
			/**
			 * Range: 0..100000
			 * Unit: lux
			 */
			TYPE_LUMINANCE = 9,
			/**
			 * Value 0: no motion detected
			 * Value 1: motion detected
			 */
			TYPE_MOTION = 10,
			/**
			 * Unit: dB
			 */
			TYPE_NOISE = 11,
			/**
			 * Value 0: open
			 * Value 1: closed
			 */
			TYPE_OPEN_CLOSE = 12,
			/**
			 * Value 0: off
			 * Value 1: on
			 */
			TYPE_ON_OFF = 13,
			/**
			 * Range: 0..100
			 * Unit: %
			 */
			TYPE_PERFORMANCE = 14,
			/**
			 * Unit: hPa
			 */
			TYPE_PRESSURE = 15,
			/**
			 * Range: 0..100
			 * Unit: %
			 */
			TYPE_RSSI = 16,
			/**
			 * Value 0: no security alert
			 * Value 1: security alert is active
			 *
			 */
			TYPE_SECURITY_ALERT = 17,
			/**
			 * Value 0: no shake detect
			 * Value 1: shake detect
			 */
			TYPE_SHAKE = 18,
			/**
			 * Unit: °C
			 */
			TYPE_TEMPERATURE = 19,
			/**
			 * Ultraviolet light
			 * Unit: UV index
			 * Range: 0..11
			 */
			TYPE_ULTRAVIOLET = 20,
			/**
			 * Power
			 * Unit: Watt
			 */
			TYPE_POWER = 21,
			/**
			 * Voltage
			 * Unit: Volt
			 */
			TYPE_VOLTAGE = 22,
			/**
			 * Current
			 * Unit: Ampere
			 */
			TYPE_CURRENT = 23,
		};

		static EnumHelper<Raw>::ValueMap &valueMap();
	};

	struct UnitEnum {
		enum Raw {
			/**
			 * Unit for dimensionless quantities like TYPE_BITMAP,
			 * TYPE_ENUM, etc.
			 */
			NONE,
			/**
			 * Unit for dimensionless quantities with two states.
			 * Only two values possible: 0 and 1.
			 */
			BINARY,
			/**
			 * Unit 1/100 with symbol %.
			 * Range: 0..100.
			 */
			PERCENT,
			/**
			 * Unit parts-per-million with symbol ppm.
			 * Min: 0.
			 */
			PPM,
			/**
			 * Unit of luminance with symbol lux.
			 * Range: 0..100000.
			 */
			LUX,
			/**
			 * Unit of noise with symbol dB.
			 */
			DECIBEL,
			/**
			 * Unit of pressure with symbol hPa.
			 */
			HECTOPASCAL,
			/**
			 * Unit of temperature with symbol C.
			 * Min: -273.15.
			 */
			CELSIUS,
			/**
			 * Unit of ultraviolet light with no symbol.
			 * Range: 0..11.
			 */
			UVINDEX,
			/**
			 * Unit of power with symbol W.
			 */
			WATT,
			/**
			 * Unit of voltage (electric potential) with symbol V.
			 */
			VOLT,
			/**
			 * Unit of electric current with symbol A.
			 */
			AMPERE,
		};

		static EnumHelper<Raw>::ValueMap &valueMap();
	};

	typedef Enum<TypeEnum> Type;
	typedef Enum<AttributeEnum> Attribute;

	/**
	 * @brief Representation of physical units.
	 */
	class Unit : public Enum<UnitEnum> {
	public:
		Unit(const UnitEnum::Raw &raw);

		/**
		 * @returns true if the value is valid for the particular unit.
		 */
		bool isValid(double value) const;

		/**
		 * Get symbol for the given unit. If plain is false,
		 * the returned value can be a Unicode code point.
		 * When plain is true, the return value is a plain
		 * ASCII.
		 */
		std::string symbol(bool plain = false) const;
	};

	ModuleType(const Type &type);
	ModuleType(const Type &type, const std::set<Attribute> &attributes);
	ModuleType(const Type &type, const CustomTypeID &customID);
	ModuleType(const Type &type, const CustomTypeID &customID, const std::set<Attribute> &attributes);

	void setType(const Type &type);
	Type type() const;

	void setAttributes(const std::set<Attribute> &attributes);
	std::set<Attribute> attributes() const;

	bool isControllable() const;

	void setCustomTypeID(CustomTypeID id);
	CustomTypeID customTypeID() const;

	static ModuleType parse(std::string input);

protected:
	static bool hasCombination(
		const std::set<Attribute> &attributes,
		const std::set<Attribute> &check);
	static void assureValidAttributes(
		const std::set<Attribute> &attributes);

private:
	Type m_type;
	std::set<Attribute> m_attributes;
	CustomTypeID m_customID;
};

}

#endif
