//
// author:qinggniq
// email:livewithblank@gmail.com
// Created by wc on 1/15/19.
//
#pragma once
#ifndef QG_SERVER_HTTPDATA_H
#define QG_SERVER_HTTPDATA_H


#include <unordered_map>
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
				static mime_vt mime_item (mime_kt &suffix) const;
    private:
        static mime_type_map mime_type_;
    };

    class HttpData{
		public:
			typedef qg_string method_t;
			typedef qg_string version_t;
			typedef qg_string url_t;
			typedef qg_string body_t;
			typedef std::unordered_map<qg_string, qg_string> header_t;
			typedef std::unordered_map<qg_string, qg_string> query_t;
			typedef qg_string query_vt;
			typedef qg_string query_kt;
			typedef qg_string header_vt;
			typedef qg_string header_kt;
    public:
			HttpData (const method_t method,
								const version_t version,
								const url_t url,
								const query_t query,
								const header_t header,
								const body_t body):
					method_ (method),
					version_ (version),
					url_ (url),
					query_ (query),
					header_ (header),
					body_ (body) {}
			;
			HttpData ();
			~HttpData ();

			version_t version () const;
			method_t method () const ;
			url_t url () const ;
			query_vt query_item (const query_vt &) const;
			header_t header () const;
			header_vt header_item (const header_kt &) const;
			body_t body() const;
    private:
			method_t method_;
      version_t version_;
      url_t url_;
      query_t query_;
      header_t header_;
      body_t body_;
    };
} //namespace qg

#endif //QG_SERVER_HTTPDATA_H
