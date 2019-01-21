//
// Created by wc on 1/21/19.
//
#include "../core/HttpParser.h"
#include <iostream>
#include <sstream>
namespace qg{
	void TestHttpParser (qg::qg_istream &istream) {
		HttpParser htpp = HttpParser ();
		htpp.Parse (istream);
		htpp.Print ();
	}


	int main () {
		qg::qg_string s ("Get http://qinggniq.com/blod 1.1Http");
		std::istringstream is (s);
		TestHttpParser (is);

		return 0;
	}
}
