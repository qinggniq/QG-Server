//
// author:qinggniq
// email:livewithblank@gmail.com
// Created by wc on 1/15/19.
//
#pragma once
#ifndef QG_SERVER_HTTPDATA_H
#define QG_SERVER_HTTPDATA_H


#include <unordered_map>
#include <memory>

#include "../util/type.h"

// ----------------------------------------------------------------------
// Here we define the HttpData Class to represent the Http Header & Body.
// -----------------------------------------------------------------------


// Http has many text meaning definition.
// such that ctl := <the character which >128 >
// Http Http Content (from rfc2616):
// Http = Http-Line
//           *(( general header | Http-header | entity-header ) CRLF)*
//           CRLF
//           [ message-body ]

// Http-Line = Method SP Http-URI SP HTTP-Version CRLF
// Method = "OPTIONS"|"GET"|"POST"|"PUT"... (QG-Server only supports the "GET", "POST", "HEAD" methods.)
//
namespace qg{

	const qg_string GET = "GET";
	const qg_string HEAD = "HEAD";
	const qg_string POST = "POST";

	class MIMEType{
		public:
		typedef qg_string mime_vt;
		typedef qg_string mime_kt;
		typedef std::unordered_map<mime_kt, mime_vt> mime_type_map;
		public:
		MIMEType ();
		MIMEType (const MIMEType&);
		~MIMEType ();
		static void init ();
		static mime_vt mime_item (mime_kt &suffix);
		public:
		static mime_type_map mime_type_;
	};

    class HttpData{
    	public:
			typedef qg_string method_t;
			typedef qg_string version_t;
			typedef qg_string uri_t;
			typedef qg_string body_t;
			typedef std::unordered_map<qg_string, qg_string> header_t;
			typedef std::unordered_map<qg_string, qg_string> query_t;
			typedef qg_string query_vt;
			typedef qg_string query_kt;
			typedef qg_string header_vt;
			typedef qg_string header_kt;

			public:
			HttpData (
					const method_t &method,
					const version_t &version,
					const uri_t &uri,
					const query_t &query,
					const header_t &header,
					const body_t &body):
					method_ (method),
					version_ (version),
					uri_ (uri),
					query_ (query),
					header_ (header),
					body_ (body) {}
			HttpData ();
			~HttpData ();

			version_t version () const;
			void set_version (const version_t &version) ;
			method_t method () const ;
			void set_methd (const method_t &method) ;
			uri_t uri () const ;
			void set_uri (const uri_t &uri) ;
			void set_query (const query_t &query) ;
			header_t header () const;
			query_vt query_item (const query_vt &) const;
			void set_query_item (const query_kt &key, const query_vt &value);
			void set_header (const header_t &header) ;
			void set_header_item (const header_kt &key, const header_vt &value);
			header_vt header_item (const header_kt &) const;
			void set_body (const body_t &body);
			body_t body () const;
			qg_bool Encoded () const;


			private:
			method_t method_;
			version_t version_;
			uri_t uri_;
			query_t query_;
			header_t header_;
			body_t body_;
    };
} //namespace qg

#endif //QG_SERVER_HTTPDATA_H
