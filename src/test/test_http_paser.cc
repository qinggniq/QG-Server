//
// Created by wc on 1/21/19.
//
#include "http_parser.h"
#include <iostream>
#include <sstream>

using namespace qg;

	void TestReqParser (qg::qg_istream &istream) {
		RequestParser reqp = RequestParser ();
		reqp.Parse (istream);
		reqp.Print ();
	}


	int main () {
		qg::qg_string s ("GET /hello.htm HTTP/1.1\n"
										 "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n"
										 "Host: www.tutorialspoint.com\n"
										 "Accept-Language: en-us\n"
										 "Accept-Encoding: gzip, deflate\n"
										 "Connection: Keep-Alive\n");
		std::istringstream is (s);
		TestReqParser (is);

		return 0;
	}

