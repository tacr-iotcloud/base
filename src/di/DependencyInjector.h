#ifndef BEEEON_DEPENDENCY_INJECTOR_H
#define BEEEON_DEPENDENCY_INJECTOR_H

#include <map>
#include <Poco/Exception.h>
#include <Poco/AutoPtr.h>
#include <Poco/SharedPtr.h>
#include <Poco/Logger.h>
#include <Poco/Util/AbstractConfiguration.h>
#include "di/DIWrapper.h"
#include "util/Loggable.h"

namespace BeeeOn {

class InstanceInfo;

/**
 * DependencyInjector automates creation of application top-level
 * instances and glues them together. It is inspired by the Java
 * framework https://projects.spring.io/spring-framework/ but it
 * is highly simplified for C++ and for the current purposes.
 *
 * The instances are configured in a configuration file represented
 * by the Poco::AbstractConfiguration. The XMLConfiguration is
 * assumed. DependencyInjector handles tags:
 *
 * * instance: defines a new instance (singleton)
 *   + attribute name   - name of the instance
 *   + attribute class  - class name with namespace
 *   + attribute init   - lazy (default), early
 *
 * * set: sets a property of an instance
 *   + attribute name   - name of the property to inject
 *   + attribute ref    - value to set is a reference to an instance by name
 *   + attribute text   - value to set is a string
 *   + attribute time   - value to set is a timespan
 *   + attribute number - value to set is an integer
 *   + attribute list   - value to set is a list of strings
 *   + attribute if-yes - optional condition to avoid applying the "set"
 *
 * * add: adds a value to a property list
 *   + attribute name   - name of the property to inject
 *   + attribute ref    - value to add is a reference to an instance by name
 *   + attribute text   - value to add is a string
 *   + attribute time   - value to add is a timespan
 *   + attribute number - value to add is an integer
 *   + attribute if-yes - optional condition to avoid applying the "add"
 *
 * * map: constructs a map of key-value pairs
 *   + attribute name   - name of the property to inject
 *   + attribute if-yes - optional constructs to avoid applying the "map"
 *
 *   * subelement: pair
 *     + attribute key  - key of the map pair
 *     + attribute text - value of the pair as string
 *
 * * alias: defines alias name of an existing instance
 *   + attribute name   - name of the alias (new instance name)
 *   + attribute ref    - name of an existing instance
 *
 * The attributes can contain ${property} strings which are expanded
 * automatically.
 *
 * No root element or parent entry is considered, so it is usually
 * necessary to pass a sub-configuration removing the initial prefix
 * by AbstractConfiguration::createView.
 *
 * Example configuration:
 *
 * <pre>
 * <factory>
 *	<instance name="main" class="BeeeOn::Main">
 *		<set name="userService" ref="userService" />
 *		<set name="listenPort"  number="8080" />
 *		<set name="timeout"     time="5 s" />
 *		<add name="listeners"   ref="loginListener" />
 *		<add name="listeners"   ref="errorListener" />
 *		<set name="whitelist"   list="192.168.5.5,192.168.15.1" />
 *	</instance>
 *
 *      <instance name="nameMap" class="BeeeOn::NameMap">
 *		<map name="mapping">
 *			<pair key="first" text="The First One" />
 *			<pair key="second" text="The Second One" />
 *		</map>
 *      </instance>
 *
 *	<instance name="userService" class="BeeeOn::UserService" />
 *	<instance name="loginListener" class="BeeeOn::LoginListener" />
 *	<instance name="stdoutErrorListener"
 * 			class="BeeeOn::StdoutErrorListener" />
 *	<alias name="errorListener" ref="stdoutErrorListener" />
 * </factory>
 *
 * DependencyInjector di(config.createView("factory"));
 * Poco::SharedPtr<Main> main = di.create<Main>("main");
 * </pre>
 */
class DependencyInjector : public Loggable {
public:
	typedef std::map<std::string, DIWrapper *> WrapperMap;
	typedef std::vector<DIWrapper *> WrapperVector;

	DependencyInjector(
		Poco::AutoPtr<Poco::Util::AbstractConfiguration> conf):
		m_conf(conf)
	{
		createEarly();
	}

	~DependencyInjector();

	DIWrapper *create(const std::string &name, bool disown = false);

	template <typename T>
	Poco::SharedPtr<T> create(const std::string &name, bool disown = false)
	{
		DIWrapper *wrapper = create(name, disown);
		if (wrapper == NULL)
			return NULL;

		return dynamic_cast<AbstractDIWrapper<T> &>(*wrapper).instance();
	}

	DIWrapper *find(const std::string &name);

	template <typename T>
	Poco::SharedPtr<T> find(const std::string &name)
	{
		DIWrapper *wrapper = find(name);
		if (wrapper == NULL)
			return NULL;

		return dynamic_cast<AbstractDIWrapper<T> &>(*wrapper).instance();
	}

private:
	/**
	 * Create instances that should be always created.
	 */
	void createEarly();

	void destroyOne(DIWrapper *one) const;
	void cleanup(const WrapperVector vec) const;
	WrapperVector tryDestroy(const WrapperVector vec) const;
	void destroyRest(const WrapperVector vec) const;

	DIWrapper *createNoAlias(
			const InstanceInfo &info,
			bool disown = false);
	DIWrapper *createNew(const InstanceInfo &info);
	DIWrapper *injectDependencies(
			const InstanceInfo &info,
			DIWrapper *target);
	void injectValue(const InstanceInfo &info,
			DIWrapper *target,
			const std::string &key,
			const std::string &name);
	bool tryInjectRef(const InstanceInfo &info,
			DIWrapper *target,
			const std::string &key,
			const std::string &name);
	bool tryInjectNumber(const InstanceInfo &info,
			DIWrapper *target,
			const std::string &key,
			const std::string &name);
	bool tryInjectText(const InstanceInfo &info,
			DIWrapper *target,
			const std::string &key,
			const std::string &name);
	bool tryInjectTime(const InstanceInfo &info,
			DIWrapper *target,
			const std::string &key,
			const std::string &name);
	bool tryInjectList(const InstanceInfo &info,
			DIWrapper *target,
			const std::string &key,
			const std::string &name);
	bool tryInjectMap(const InstanceInfo &info,
			DIWrapper *target,
			const std::string &key,
			const std::string &name);

private:
	WrapperMap m_set;
	WrapperVector m_free;
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> m_conf;
};

}

#endif
