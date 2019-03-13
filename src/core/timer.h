//
// Created by wc on 3/6/19.
//

#ifndef SRC_TIMER_H
#define SRC_TIMER_H

#include "type.h"
#include "time_stamp.h"
#include "noncopyable.h"


namespace qg{
 class Timer : public noncopyable {
 public:
  	Timer();
  	Timer(TimeStamp expiration, qg_double interval) :
  		expiration_(expiration),
  		interval_(interval),
  		repeat_(interval > 0.0),
  		sequence_(0)	// TODO(qinggniq) the sequence of Timer shoule be generated
  						// atomaticaly.
  		{}
  	void handle();
	TimeStamp expiration() const {return expiration_;}
	qg_bool repeat() const {return repeat_;}
	qg_int64_t sequence() const {return sequence_;}

	void Restart(TimeStamp now);
 private:
	TimeStamp expiration_;
	const qg_double interval_;
	const qg_bool repeat_;
	const qg_int64_t sequence_;

	//TODO(qinggniq) add Atom
};

}//namespace qg
#endif //SRC_TIMER_H
