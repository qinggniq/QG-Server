//
// Created by wc on 1/21/19.
//
//#include "http_parser.h"
#include <iostream>
#include <sstream>

//	void TestReqParser (qg::qg_istream &istream) {
//		RequestParser reqp = RequestParser ();
//		reqp.Parse (istream);
//		reqp.Print ();
//	}

struct Test {
  int a;
  int b;
};

int main() {
  /*qg::qg_string s ("GET /hello.htm HTTP/1.1\n"
                                                                   "User-Agent:
  Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n" "Host:
  www.tutorialspoint.com\n" "Accept-Language: en-us\n" "Accept-Encoding: gzip,
  deflate\n" "Connection: Keep-Alive\n");

  std::istringstream is (s);
  TestReqParser (is);*/

  Test s({1, 2});
  std::cout << s.a << s.b << std::endl;
  return 0;
}
