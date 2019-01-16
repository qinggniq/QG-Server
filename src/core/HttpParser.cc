//
// Created by wc on 1/15/19.
//

#include "HttpParser.h"

namespace qg{
HttpParser::HttpParser () {
	this->http_d = HttpParser::http_dt();
}

HttpParser::~HttpParser () {}

HttpParser::ok_t
HttpParser::parse (msg_t &http_msg) {
	ok_t s;
	return s;
}

HttpParser::http_dt
HttpParser::http_data () const {
	return this->http_d;
}

HttpParser::ok_t
HttpParser::parse_start_line (qg::HttpParser::msg_t &start_msg) {
	ok_t s;
	return s;
}

HttpParser::ok_t
HttpParser::parse_header (qg::HttpParser::msg_t &header_msg) {
	ok_t s;
	return s;
}

HttpParser::ok_t
HttpParser::parse_body (qg::HttpParser::msg_t &body_msg) {
	ok_t s;
	return s
}
} //namespace qg