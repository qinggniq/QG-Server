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
        typedef qg_string header_vt;
        typedef qg_string header_kt;
    public:
        HttpData (method_t method,
									version_t version,
                  url_t url,
									header_t header,
									body_t body);
        HttpData ();
        ~HttpData ();

        version_t version () const;
        method_t method () const ;
        url_t url () const ;
        header_t header () const;
        header_vt header_item (header_kt) const;
        body_t body() const;
    private:
        method_t method_;
        version_t version_;
        url_t url_;
        header_t header_;
        body_t body_;
    };
} //namespace qg

#endif //QG_SERVER_HTTPDATA_H
