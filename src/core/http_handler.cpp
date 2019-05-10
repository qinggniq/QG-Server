//
// Created by wc on 5/7/19.
//
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <boost/iostreams/device/mapped_file.hpp>
#include "http_handler.h"
#include "rwtool.h"

namespace qg{
const int DEFAULT_KEEP_ALIVE_TIME = 5*60*1000;
HttpHandler::HttpHandler(dispatcher_pt dispatcher,
	                     qg_fd_t fd,
	                     config_pt config) :
	                     dispatcher_(std::move(dispatcher)),
	                     fd_(fd),
	                     request_data_(new RequestData()),
	                     request_parser_(new RequestParser(request_data_)),
						 handler_(new EventHandler(dispatcher, fd)),
						 config_(std::move(config)),
						 keep_alive_(false){

  handler_->enableAll();
  handler_->SetReadCallBack(std::bind(&HttpHandler::handleRead, this));
  handler_->SetWriteCallBack(std::bind(&HttpHandler::handleWrite, this));
  //handler_->SetErrorCallBack(std::bind(&HttpHandler::handleError, this));
  dispatcher->registerHandler(handler_);
}

void HttpHandler::handleRead() {
  events_t& ev = handler_->GetREvents();
  qg_string data;

  if (QGReadN(fd_, data, qg::kBufferSize) < 0) {
    //TODO logit
    //handleError();
    return;
  }
  qg_istringstream istream(data);

  if (!request_parser_->Parse(istream)) {
    //TODO log

    switch (request_parser_->error_code()) {
      case ErrorCode::kFileNotFound:
		handleError(StatusCode ::NOT_FOUND, "File Not Found");
		break;
		case ErrorCode ::kQueryError: case ErrorCode ::kBodyError:
		case ErrorCode ::kHeaderError: case ErrorCode ::kProtoclError:
		  handleError(StatusCode::BAD_REQUEST, "Bad Request");
    }
    return;
  }
  qg_string response;
  if (request_data_->method() == "POST") {

  } else if (request_data_->method() == "GET" || request_data_->method() == "HEAD") {
	qg_string header;
	header += request_data_->version() + " 200 OK\r\n";
	if((request_data_->header_item("Connection") == "Keep-Alive" ||
		request_data_->header_item("Connection") == "keep-alive"))
	{
	  keep_alive_ = true;
	  header += qg_string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout="
	  	+ std::to_string(config_->detail.keep_alive_timeout) + "\r\n";
	}
	qg_size_t dot_pos = request_data_->file().find('.');
	qg_string file_type;
	if (dot_pos == qg_string::npos) {
	  file_type = MIMEType::mime_item("default");
	}else{
	  file_type = MIMEType::mime_item(request_data_->file().substr(0, dot_pos));
	  if (file_type.empty()) {
	    handleError(StatusCode::BAD_REQUEST, "File Type Not Supported");
	    return;
	  }
	}

	struct stat sbuf;
	if (stat(request_data_->file().c_str(), &sbuf) < 0)
	{
	  handleError(StatusCode ::NOT_FOUND, "File Not Found!");
	  return ;
	}
	header += "Content-Type: " + file_type + "\r\n";
	header += "Content-Length: " + std::to_string(sbuf.st_size) + "\r\n";
	header += "Server: QG Server\r\n";

	header += "\r\n";
	response_ += header;

	if (request_data_->method() == "HEAD")
	  return ;

	if (config_->detail.sendfile) {
	  return ;
	}
	/*
	 * copy from https://stackoverflow.com/questions/17925051/fast-textfile-reading-in-c
	 * author sehe
	 */
	boost::iostreams::mapped_file mmap(request_data_->file(), boost::iostreams::mapped_file::readonly);
	if (mmap.const_data() == nullptr) {
	  response_.clear();
	  //TODO maybe is read error
	  handleError(StatusCode::NOT_FOUND, "File Not Found");
	  return;
	}
	response_ += mmap.const_data();
	mmap.close();
  }
}

void HttpHandler::handleWrite() {
  ResponseData red;

}

void HttpHandler::handleError(qg_int err_num, qg_string &&note) {

  note = " " + note;
  char send_buff[4096];
  qg_string body_buff, header_buff;
  body_buff += "<html><title>Error</title>";
  body_buff += "<body bgcolor=\"ffffff\">";
  body_buff += std::to_string(err_num) + note;
  body_buff += "<hr><em> QG Server</em>\n</body></html>";

  header_buff += "HTTP/1.1 " + std::to_string(err_num) + note + "\r\n";
  header_buff += "Content-Type: text/html\r\n";
  header_buff += "Connection: Close\r\n";
  header_buff += "Content-Length: " + std::to_string(body_buff.size()) + "\r\n";
  header_buff += "Server: QG Server\r\n";;
  header_buff += "\r\n";
  sprintf(send_buff, "%s", header_buff.c_str());
  QGWriteN(fd_, send_buff, strlen(send_buff));
  sprintf(send_buff, "%s", body_buff.c_str());
  QGWriteN(fd_, send_buff, strlen(send_buff));
}

} //namespace qg