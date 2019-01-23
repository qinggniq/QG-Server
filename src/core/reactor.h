//
// Created by wc on 1/23/19.
//

/*
 * I decide use Reactor Design Pattern to implement the multiple
 * connections.
 * Here I cite many open source implement of Reactor Design Pattern such as
 * https://github.com/song0071000/Reactor_Implemention,
 * https://github.com/justinzhu/reactor,
 * and the redis implemention.
 * */


#ifndef QG_SERVER_REACTOR_H
#define QG_SERVER_REACTOR_H


#include "../util/type.h"
#include <memory>

namespace qg{
	typedef qg_uint event_t;

	enum EventMode{
		kEventRead=0x01,
		kEventWrite=0x02,
		kEventError=0x03,
		kEventMask=0xff
	};

	typedef qg_int handle_t;


	/* You could not implement the read/write handle of file describer,
	 * but you must implement the error handle.
   *
	 */
	class EventHandler {
	public:
		virtual handle_t GetHandle () const = 0;
		virtual void HandleRead () {}
		virtual void HandleWrite () {}
		virtual void HandleError () = 0;

	protected:
		EventHandler () = default;
		virtual ~EventHandler () = 0;
	};

	class ReactorImplementation;
	class Reactor {
	public:
		typedef std::shared_ptr<EventHandler> event_handler_pt;
		typedef std::shared_ptr<ReactorImplementation> reactor_impl_pt;

		Reactor () ;
		~Reactor () ;
		qg_int RegisterHandler (event_handler_pt &eh, event_t ev);
		qg_int RemoveHandle (event_handler_pt &eh, event_t ev);
		void HandleEvents ();
		//

	private:
		Reactor (const Reactor &);
		Reactor &operator=(const Reactor &);

	private:
		reactor_impl_pt reactor_impl_;
	};

} //namespace qg
#endif //QG_SERVER_REACTOR_H
