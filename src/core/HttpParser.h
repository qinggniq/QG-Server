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
			typedef HttpData::uri_t uri_t;
			typedef HttpData::header_t header_t;
			typedef HttpData::body_t body_t;
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
			ok_t Parse (const msg_t &http_msg);
			ok_t Parse (qg_istream &stream);
			http_dt http_data () const;
			void Print () const;

		private:
			ok_t ParseStartLine (const msg_t &msg);
			ok_t ParseMethod (const msg_t &msg);
			ok_t ParseUri (const msg_t &msg);
			ok_t ParseQuery (const msg_t &msg);
			ok_t ParseVersion (const msg_t &msg);
			ok_t ParseHeader (const msg_t &msg);
			ok_t ParseHeader (qg_istream &istream);

			ok_t ParseBody (const msg_t &msg);
			ok_t ParseBody (qg_istream &istream);



			http_dt http_d_;

    };
} //namespace qg
#endif //QG_SERVER_REQUESTPARSER_H
