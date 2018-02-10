#include <new>

#ifdef __GLIBC__
#include <execinfo.h>
#include <unistd.h>
#endif

#include "util/Backtrace.h"

using namespace std;
using namespace BeeeOn;

Backtrace::Backtrace(bool empty)
{
	if (empty) {
#ifdef __GLIBC__
		m_backtrace_size = 0;
#endif
		return;
	}

#ifdef __GLIBC__
	m_backtrace_size = backtrace(m_backtrace,
			sizeof(m_backtrace) / sizeof(m_backtrace[0]));
#endif
}

Backtrace::~Backtrace()
{
}

unsigned int Backtrace::size() const
{
#ifdef __GLIBC__
	return m_backtrace_size;
#else
	return 0;
#endif
}

void Backtrace::fatal() const
{
	// malloc for backtrace has failed?
	// there is something really very very bad
	dump(STDOUT_FILENO);
	throw bad_alloc();
}

string Backtrace::at(unsigned int i) const
{
#ifdef __GLIBC__
	if (i >= m_backtrace_size)
		return "";

	char **b = backtrace_symbols(m_backtrace + i, 1);
	if (b == NULL)
		fatal();

	const string text(b[0]);
	free(b);
	return text;
#else
	return "";
#endif
}

string Backtrace::toString(const string &indent) const
{
#ifdef __GLIBC__
	if (m_backtrace_size == 0)
		return "";

	char **b = backtrace_symbols(m_backtrace, m_backtrace_size);
	if (b == NULL)
		fatal();

	string text;
	for (unsigned int i = 0; i < m_backtrace_size; ++i) {
		text += indent;
		text += b[i];
		text += "\n";
	}

	free(b);
	return text;
#else
	return "";
#endif
}

void Backtrace::dump(int fd) const
{
#ifdef __GLIBC__
	backtrace_symbols_fd(m_backtrace, m_backtrace_size, fd);
#else
#define NO_BACKTRACE_MSG "(no backtrace available)\n";
	write(fd, NO_BACKTRACE_MSG, sizeof(NO_BACKTRACE_MSG));
#endif
	fsync(fd);
}

#ifdef __GLIBC__
namespace BeeeOn {

/**
 * @brief Call the backtrace() function to make it
 * async-signal-safe in the future. Calling the
 * backtrace() leads to load the appropriate library
 * via dlopen() which calls malloc(). This way, any
 * other call to the backtrace() function is safe.
 */
class GlibcBacktraceInit {
public:
	GlibcBacktraceInit() {
		void *top[1];
		backtrace(top, 1);
	}
};

static volatile GlibcBacktraceInit glibcBacktraceInitializer;
}
#endif
