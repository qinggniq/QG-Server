//
// Created by wc on 1/15/19.
//
#pragma once
#ifndef QG_SERVER_REQUESTPARSER_H
#define QG_SERVER_REQUESTPARSER_H

#include "../util/type.h"
#include "HttpData.h"

class HttpData;

namespace qg{

    class HttpParser{
		public:
			typedef HttpData::method_t method_t;
			typedef HttpData::version_t version_t ;
			typedef HttpData::url_t url_t;
			typedef HttpData http_dt;
			typedef qg_int ok_t;
			typedef qg_string msg_t;

			enum HeaderState
			{
					H_START = 0,
					H_KEY,
					H_COLON,
					H_SPACE_AFTER_COLON,
					H_VALUE,
					H_CR,
					H_LF,
					H_END_CR,
					H_END_LF
			};

			enum ParseState
			{
					PARSE_URL = 0,
					PARSE_HEADER,
					PARSE_BODY,
					PARSE_END,
					PARSE_ERROR
			};

		public:
			HttpParser ();
			~HttpParser ();
			ok_t parse (msg_t &http_msg);
			http_dt http_data () const;
			ok_t parse_start_line(msg_t &start_msg);
			ok_t parse_header (msg_t &header_msg);
			ok_t parse_body (msg_t &body_msg);

		private:
			http_dt http_d;

    };
} //namespace qg
#endif //QG_SERVER_REQUESTPARSER_H
