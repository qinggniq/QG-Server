//
// Created by wc on 1/23/19.
//

#ifndef QG_SERVER_EVENT_LOOP_H
#define QG_SERVER_EVENT_LOOP_H

#include "../util/type.h"
#include <memory>

namespace qg{
	using mask_t = qg_int;
	template <class T>
	struct AfterSleepProc {
		void operator () (const T& t) {
			//TODO (qinggniq) the process which should be executed before/after Event avoked
		}
	};

	template <class T, typename id_t, class ClientData>
	struct FileProc {
		void operator () (const T& t, id_t id, const ClientData& client_data, mask_t mask) {
			//TODO (qinggniq) the process which should be executed before/after Event avoked
		}
	};

	template <class T, typename id_t, class ClientData>
	struct TimerProc {
		void operator () (const T& t, id_t id, const ClientData& client_data) {
			//TODO (qinggniq) the process which should be executed before/after Event avoked
		}
	};

	class IOEvent {
	public:

	};

	class TimerEvent {

	};

	class FiredEvent {

	};


	class EventLoop {
		public:
		typedef TimerEvent timer_event_t;
		typedef std::shared_ptr<TimerEvent> timer_event_pt;
		typedef IOEvent io_event_t;
		typedef std::shared_ptr<IOEvent> io_event_pt;
		typedef FiredEvent fired_event_t;
		typedef std::shared_ptr<FiredEvent> fired_event_pt;
		typedef qg_longlong eid_t;
		typedef qg_int esize_t;
		typedef qg_bool eflag_t;
		typedef qg::AfterSleepProc<EventLoop &> sleep_proc_ft;  //maybe should be point?

		EventLoop () = default;
		~EventLoop () = default;



		private:
		esize_t max_fd_;
		esize_t set_size_;
		eid_t time_event_next_id;
		qg_time_t last_time_;
		timer_event_pt ptime_events_;
		io_event_pt pio_events_;
		fired_event_pt pfired_events_;

		eflag_t stop_;
		sleep_proc_ft before_sleep_proc_;
		sleep_proc_ft after_sleep_proc_;


	};


}
#endif //QG_SERVER_EVENT_LOOP_H
