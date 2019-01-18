//
// Created by wc on 1/15/19.
//

#include "HttpData.h"

namespace qg{
	MIMEType::MIMEType () {}

	MIMEType::MIMEType (const qg::MIMEType &mime) {
		mime_type_ = MIMEType::mime_type_map (mime.mime_type_);
	}

	MIMEType::~MIMEType () {}

	static
	void
	MIMEType::init () {
		mime_type_[".html"] = "text/html";
		mime_type_[".avi"] = "video/x-msvideo";
		mime_type_[".bmp"] = "image/bmp";
		mime_type_[".c"] = "text/plain";
		mime_type_[".doc"] = "application/msword";
		mime_type_[".gif"] = "image/gif";
		mime_type_[".gz"] = "application/x-gzip";
		mime_type_[".htm"] = "text/html";
		mime_type_[".ico"] = "application/x-ico";
		mime_type_[".jpg"] = "image/jpeg";
		mime_type_[".png"] = "image/png";
		mime_type_[".txt"] = "text/plain";
		mime_type_[".mp3"] = "audio/mp3";
		mime_type_["default"] = "text/html";
	}
	static
	MIMEType::mime_vt
	MIMEType::mime_item (qg::MIMEType::mime_kt &suffix) const {
		return mime_type_[suffix];
	}

	HttpData::HttpData () {
			this->method_ = "";
			this->version_ = "";
			this->url_ = "";
			this->header_ = HttpData::header_t ();
			this->body_ = HttpData::body_t ();
	}



	HttpData::~HttpData () {

	}

	HttpData::method_t
	HttpData::method () const {
		return this->method_;
	}

	HttpData::version_t
	HttpData::version () const {
		return this->version_;
	}

	HttpData::url_t
	HttpData::url () const {
		return this->url_;
	}

	HttpData::query_vt
	HttpData::query_item (const qg::HttpData::query_vt &key) const {
		return this->query_[key];
	}

	HttpData::header_vt
	HttpData::header_item (const qg::HttpData::header_kt &key) const {
		return this->header_[key];
	}

	HttpData::header_t
	HttpData::header () const {
		return this->header_;
	}

	HttpData::body_t
	HttpData::body () const {
		return this->body_;
	}

} //namespace qg
