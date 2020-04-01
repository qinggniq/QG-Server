//
// Created by 王聪 on 2020/4/1.
//
#include "../http_context.h"
#include <iostream>
const char *test_cases[]{
    "GET /test HTTP/1.1\r\n"
    "User-Agent: curl/7.18.0 (i486-pc-linux-gnu) libcurl/7.18.0 OpenSSL/0.9.8g "
    "zlib/1.2.3.3 libidn/1.1\r\n"
    "Host: 0.0.0.0=5000\r\n"
    "Accept: */*\r\n"
    "\r\n",

    "GET /favicon.ico HTTP/1.1\r\n"
    "Host: 0.0.0.0=5000\r\n"
    "User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9) "
    "Gecko/2008061015 Firefox/3.0\r\n"
    "Accept: "
    "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    "Accept-Language: en-us,en;q=0.5\r\n"
    "Accept-Encoding: gzip,deflate\r\n"
    "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
    "Keep-Alive: 300\r\n"
    "Connection: keep-alive\r\n"
    "\r\n",


    "GET /forums/1/topics/2375?page=1#posts-17408 HTTP/1.1\r\n"
    "\r\n",

    "GET /get_one_header_no_body HTTP/1.1\r\n"
    "Accept: */*\r\n"
    "\r\n",
};

using namespace qg;
int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  HTTPContext *context = new HTTPContext;
  std::cout<< test_cases[0] << std::endl;
  for (int i = 0; i < 3; ++i) context->execute(test_cases[i], strlen(test_cases[i]));
}
