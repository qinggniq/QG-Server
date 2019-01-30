//
// Created by wc on 1/18/19.
//

#ifndef PROJECT_LOG_H
#define PROJECT_LOG_H
#include <sstream>
#include "type.h"
#ifndef BOOST_NO_CXX11_HDR_CHRONO
#include <chrono>
#endif
/*
 * At first I have wanted to use Single Pattern to implement the Log Class.
 * But after I read many arguments of "Whether We should use 'Single Pattern'",
 * I found I was a fan of the "non-single-mode". So I give up using Single
 * Pattern to implement the Log Class.
 *
 */


#ifndef BOOST_NO_CXX11_HDR_CHRONO
namespace qg {
	inline
	qg_string
	NowTime () {
		auto now = std::chrono::system_clock::now ();
		std::time_t end_time = std::chrono::system_clock::to_time_t (now);
		return qg_string(std::ctime (&end_time));
	}
} // namespace qg
#else

namespace qg {
	inline
	qg_string
	NowTime () {
		return qg_string ("1970-1-1");
	}
}
#endif

namespace qg {
enum LogLevel {
  kLogInfo = 0,
  kLogWarn,
  kLogError,
  kLogDebug,
  kLogDebug1,
  kLogDebug2,
  kLogDebug3,
  kLogDebug4
};

class Logger {
 public:
  typedef std::ostringstream ostream_t;

 public:
  Logger();
  virtual ~Logger();
  ostream_t &Get(LogLevel level = kLogInfo);

  static LogLevel ReportingLevel();

 protected:
  std::ostringstream ostream;
 private:
  Logger(const Logger &);
  Logger &operator=(const Logger &);

 private:
  LogLevel level_;
};

LogLevel
Logger::ReportingLevel() {
  return level_;
}
Logger::ostream_t &
Logger::Get(LogLevel level) {
  this->ostream << "- " << qg::NowTime();
  this->ostream << " " << (level) << ": ";
  this->ostream << qg::qg_string(level > kLogDebug ? 0 : level - kLogDebug, '\t');
  this->level_ = level;
  return this->ostream;
}

Logger::~Logger() {
  if (this->level_ >= Logger::ReportingLevel()) {
	this->ostream << std::endl;
	fprintf(stderr, "%s", this->ostream.str().c_str());
	fflush(stderr);
  }
}

extern LogLevel log_level;

#define Log (level) \
do {\
if (level > log_level) ; \
else Logger().Get(level)\
}while (0)

}//namespace qg
#endif //PROJECT_LOG_H
