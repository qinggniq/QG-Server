//
// Created by wc on 1/15/19.
//

#include "http_parser.h"

#include "../util/type.h"

#include <iostream>


namespace qg{

	http_parser::http_parser () = default;

	http_parser::~http_parser () = default ;

	http_parser::ok_t
	http_parser::Parse (const msg_t &http_msg) {
		return kOk;
	}

	http_parser::ok_t
	http_parser::Parse (qg_istream &stream) {
		qg_string line;
		method_t method;
		version_t version;
		qg_string query_s;
		header_t header;
		body_t body;
		
		uri_t uri;

		getline (stream, line);
		size_t method_end = qg_string::npos;

		if ((method_end = line.find (' ')) != qg_string::npos) {
			method = line.substr (0, method_end);
			
			qg::qg_sizt_t  query_start = qg_string::npos;
			qg::qg_sizt_t  uri_and_query_end = qg_string::npos;
			qg::qg_sizt_t line_size = line.size ();

			for (qg::qg_sizt_t i = method_end + 1; i < line_size; ++i) {
				if (line[i] == '?' && i < line_size - 1) {
					query_start = i;
				} else if (line[i] == ' '){
					uri_and_query_end = i;
					break;
				}
			}
			//
			if (uri_and_query_end != qg_string::npos) {
				if (query_start != qg_string::npos) {
					uri = line.substr (method_end + 1, query_start);
					query_s = line.substr (query_start, uri_and_query_end - method_end);
				} else {
					uri = line.substr (method_end + 1, uri_and_query_end - method_end);
				}
				
				qg::qg_sizt_t version_end = qg_string::npos;
				if ((version_end = line.find ('/', uri_and_query_end + 1)) != qg_string::npos) {
					if(line.compare(uri_and_query_end + 1, version_end - uri_and_query_end - 1, "HTTP") != 0)
						return kError;
					version = line.substr(version_end + 1, line.size() - version_end - 1);
				} else {
					return kError;
				}

				this->http_d_.set_methd (method);
				this->http_d_.set_version (version);
				this->http_d_.set_uri (uri);


				return this->ParseHeader (stream) &&this->ParseQuery (query_s) ;
			}
			
		}
		return kOk;

	}


	http_parser::http_dt
	http_parser::http_data () const {
		return this->http_d_;
	}

	http_parser::ok_t
	http_parser::ParseStartLine (const http_parser::msg_t &start_msg) {
		ok_t s = -1;

		return kOk;
	}
	http_parser::ok_t
	http_parser::ParseMethod (const http_parser::msg_t &msg) {
		return kOk;
	}

	http_parser::ok_t
	http_parser::ParseUri (const qg::http_parser::msg_t &msg) {
		return kOk;
	}

	http_parser::ok_t
	http_parser::ParseQuery (const qg::http_parser::msg_t &msg) {

		return kOk;
	}

	http_parser::ok_t
	http_parser::ParseVersion (const qg::http_parser::msg_t &msg) {
		return kOk;
	}


	http_parser::ok_t
	http_parser::ParseHeader (const qg::http_parser::msg_t &header_msg) {
		ok_t s = kOk;
		return s;

	}

	http_parser::ok_t
	http_parser::ParseHeader (qg::qg_istream &istream) {
		qg_string line;
		header_t header;
		while (getline (istream, line)) {
			qg_sizt_t key_end;
			if ((key_end = line.find (":")) != qg_string::npos && line.length ()) {
				this->http_d_.set_header_item (line.substr (0, key_end), line.substr (key_end + 1, line.length ()));
			}
		}
		
		return this->ParseBody (istream);
	}

	http_parser::ok_t
	http_parser::ParseBody (const qg::http_parser::msg_t &body_msg) {
		return kOk;
	}

	http_parser::ok_t
	http_parser::ParseBody (qg::qg_istream &istream) {
		if (((this->http_d_).Encoded () )== kOk) {
			return kOk;
			//TODO(qinggniq) implement the encode/decode function
		} else {
			return kOk;
			//TODO(qinggniq) implement the body parse function
		}
		return kOk;
	}

	void
	http_parser::Print () const {
		std::cout << "Http Data:" << std::endl;
		std::cout << "Http Method:" << this->http_d_.method () << std::endl;
		std::cout << "Http Uri:" << this->http_d_.uri () << std::endl;
		std::cout << "Http Version:" << this->http_d_.version () << std::endl;
		std::cout << "Http Headers:" << std::endl;
		http_data::header_t header = this->http_d_.header ();
		for (auto it = header.begin (); it != header.end(); it++) {
			std::cout << it->first << " : " << it->second << std::endl;
		}
		std::cout << "Http Data End!" << std::endl;
	}
} //namespace qg