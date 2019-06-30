//
// Created by wc on 5/7/19.
//
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <fstream>
#include <boost/iostreams/device/mapped_file.hpp>

#include "http_handler.h"
#include "rwtool.h"
#include "socket.h"
#include "event_handler.h"
#include "config.h"
#include "reactor.h"

namespace qg{

namespace intern{
enum WriteMethod{
  kSendFile,
  kRawWrite,
  kMmapC,
  kMmapCc
};
qg_int QGSendFile(qg_fd_t fd, qg_string& file, WriteMethod method) {
  if (method == kSendFile) {
	qg_fd_t _fd = open(file.c_str(),O_RDONLY);
	struct stat stat_buf;
	fstat(_fd, &stat_buf);
	sendfile(_fd, fd, nullptr, stat_buf.st_size);
	close(_fd);
  }else if (method == kRawWrite) {

  } else if (method == kMmapC) {
	qg_fd_t _fd = open(file.c_str(),O_RDONLY);
	struct stat stat_buf;
	fstat(_fd, &stat_buf);
    void *void_addr = (mmap(nullptr, stat_buf.st_size, PROT_READ, MAP_PRIVATE, _fd, 0));
    close(_fd);
    if (void_addr == (void *)(-1)) {
      munmap(void_addr, stat_buf.st_size);
      return -1;
    }
    char *src_addr = static_cast<char*>(void_addr);
    QGWriteN(fd, src_addr, stat_buf.st_size);
    munmap(void_addr, stat_buf.st_size);
  }
  return 0;

}

};

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
						 conn_status_(ConnectionStatus::kConnected),
						 keep_alive_(false),
						 error_(false){

  handler_->enableAll();
  handler_->SetReadCallBack(std::bind(&HttpHandler::handleRead, this));
  handler_->SetWriteCallBack(std::bind(&HttpHandler::handleWrite, this));
  //handler_->SetErrorCallBack(std::bind(&HttpHandler::handleError, this));
  dispatcher->registerHandler(handler_);
}

void HttpHandler::handleRead() {
  qg_string data;
  auto& ev = handler_->GetREvents();
  auto ret = QGReadN(fd_, data, qg::kBufferSize);
  if (ret < 0) {
    //TODO logit
    //handleError();
    error_ = true;
    return;
  } else if (ret == 0) {
    conn_status_ = ConnectionStatus ::kDisConnected;
    ev &= ~EventMode ::kEventRead;
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
    error_ = true;
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
	    error_ = true;
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

	/*
	 * copy from https://stackoverflow.com/questions/17925051/fast-textfile-reading-in-c
	 * author sehe
	 */
//	boost::iostreams::mapped_file mmap(request_data_->file(), boost::iostreams::mapped_file::readonly);
//	if (mmap.const_data() == nullptr) {
//	  response_.clear();
//	  //TODO maybe is read error
//	  handleError(StatusCode::NOT_FOUND, "File Not Found");
//	  return;
//	}
//	response_ += mmap.const_data();
//	mmap.close();

  }
}

void HttpHandler::handleWrite() {
  //TODO log it; Maybe I should refator the logic
  /*
   * 1. save data to response_;
   * 2. save fileName and write when handleWrite;
   */
  if (!error_ && conn_status_ != ConnectionStatus::kDisConnected) {
    auto &ievents = handler_->GetIEvents();
	if (QGWriteN(fd_, this->response_, this->response_.length()) <0) {
	  error_ = true;
	  ievents = 0;
	}else{
	  if (config_->detail.sendfile) {
		if (intern::QGSendFile(fd_, this->request_data_->file(), intern::WriteMethod::kSendFile) == -1) {
		  error_ = true;
		  handleError(StatusCode::NOT_FOUND, "File Not Found");
		}
	  } else{
		if (intern::QGSendFile(fd_, this->request_data_->file(), intern::WriteMethod::kMmapC) == -1) {
		  error_ = true;
		  handleError(StatusCode::NOT_FOUND, "File Not Found");
		}
	  }
	}
	if (!this->response_.empty()) {
	  ievents != EventMode ::kEventWrite;
	}
  }
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
  header_buff += "Server: QG Server\r\n";
  header_buff += "\r\n";
  sprintf(send_buff, "%s", header_buff.c_str());
  QGWriteN(fd_, send_buff, strlen(send_buff));
  sprintf(send_buff, "%s", body_buff.c_str());
  QGWriteN(fd_, send_buff, strlen(send_buff));
}

} //namespace qg