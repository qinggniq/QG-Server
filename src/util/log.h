//
// Created by wc on 1/18/19.
//

#ifndef PROJECT_LOG_H
#define PROJECT_LOG_H
#include <sstream>
#include "type.h"
#ifdef BOOST_NO_CXX11_HDR_CHRONO
#include <chrono>
#endif
/*
 * At first I have wanted to use Single Pattern to implement the Log Class.
 * But after I read many arguments of "Whether We should use 'Single Pattern'",
 * I found I was a fan of the "non-single-mode". So I give up using Single
 * Pattern to implement the Log Class.
 *
 */

#ifdef BOOST_NO_CXX11_HDR_CHRONO

namespace {
	inline
	qg_string
	NowTime () {
		auto now = std::chrono::system_clock::now ();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		return qg_string(std::ctime());
	}
}
#endif


enum LogLevel {
	LOGINFO=0,
	LOGWARN,
	LOGWRROR,
	LOGDEBUG,
	LOGDEBUG1,
	LOGDEBUG2,
	LOGDEBUG3,
	LOGDEBUG4
};



class Log {
public:
	typedef std::ostringstream ostream_t;

public:
	Log () ;
	virtual ~Log () ;
	ostream_t& get (LogLevel level=LOGINFO) ;

	static LogLevel& level ();

protected:
	std::ostringstream ostream;
private:
	Log (const Log&) ;
	Log& operator = (const Log&);

private:
	LogLevel level_;
};


Log::ostream_t&
Log::get(LogLevel level)
{
	this->ostream << "- " << NowTime();
	this->ostream << " " << (level) << ": ";
	this->ostream << qg::qg_string(level > LOGDEBUG ? 0 : level - LOGDEBUG, '\t');
	this->level_ = level;
	return this->ostream;
}


Log::~Log()
{
	if (this->level_ >= Log::level())
	{
		this->ostream << std::endl;
		fprintf(stderr, "%s", this->ostream.str().c_str());
		fflush(stderr);
	}
}
#endif //PROJECT_LOG_H
