//
// Created by wc on 1/15/19.
//

#include "HttpParser.h"
#include "../util/type.h"

namespace qg{

	HttpParser::HttpParser () :
	http_d (HttpParser::http_dt ()) {

	}

	HttpParser::~HttpParser () {}

	HttpParser::ok_t
	HttpParser::parse (const msg_t &http_msg) {
		ok_t s;
		return s;
	}

	HttpParser::ok_t
	HttpParser::parse (qg_istream &stream) {
		qg_string line;
		method_t method;
		version_t version;
		qg_string query_s;
		header_t header;
		body_t body;
		
		url_t url;

		getline (stream, line);
		size_t method_end = qg_string::npos;

		if ((method_end = line.find (' ')) != qg_string::npos) {
			method = line.substr (0, method_end);
			
			qg::qg_sizt_t  query_start = qg_string::npos;
			qg::qg_sizt_t  url_and_query_end = qg_string::npos;
			qg::qg_sizt_t line_size = line.size ();

			for (qg::qg_sizt_t i = method_end + 1; i < line_size; ++i) {
				if (line[i] == '?' && i < line_size - 1) {
					query_start = i;
				} else if (line[i] == ' '){
					url_and_query_end = i;
					break;
				}
			}
			//
			if (url_and_query_end != qg_string::npos) {
				if (query_start != qg_string::npos) {
					url = line.substr (method_end + 1, query_start);
					query_s = line.substr (query_start, url_and_query_end);
				} else {
					url = line.substr (method_end + 1, url_and_query_end);
				}
				
				qg::qg_sizt_t version_end = qg_string::npos;
				if ((version_end = line.find ('/', url_and_query_end + 1)) != qg_string::npos) {
					if(line.compare(url_and_query_end + 1, version_end - url_and_query_end - 1, "HTTP") != 0)
						return ERROR;
					version = line.substr(version_end + 1, line.size() - version_end - 2);
				} else {
					return ERROR;
				}
				header = this->parse_header ()
			}
			
		}

	}
	HttpParser::http_dt
	HttpParser::http_data () const {
		return this->http_d;
	}

	HttpParser::ok_t
	HttpParser::parse_start_line (const HttpParser::msg_t &start_msg) {
		ok_t s = -1;

		return s;
	}
	HttpParser::ok_t
	HttpParser::parse_method (const HttpParser::msg_t &msg) {

	}

	HttpParser::ok_t
	HttpParser::parse_url (const qg::HttpParser::msg_t &msg) {

	}

	HttpParser::ok_t
	HttpParser::parse_query (const qg::HttpParser::msg_t &msg) {

	}

	HttpParser::ok_t
	HttpParser::parse_version (const qg::HttpParser::msg_t &msg) {

	}


	HttpParser::ok_t
	HttpParser::parse_header (const qg::HttpParser::msg_t &header_msg) {
		ok_t s = -1;
		return s;
	}

	HttpParser::ok_t
	HttpParser::parse_body (const qg::HttpParser::msg_t &body_msg) {
		ok_t s = -1;
		return s;
	}
} //namespace qg