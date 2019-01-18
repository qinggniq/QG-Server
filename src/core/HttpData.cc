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

	HttpData::method_t&
	HttpData::method () const {
		return this->method_;
	}

	void
	HttpData::set_methd (const qg::HttpData::method_t &method) {
		this->method_ = method;
	}

	HttpData::version_t&
	HttpData::version () const {
		return this->version_;
	}

	void
	HttpData::set_version (const qg::HttpData::version_t &version) {
		this->version_ = version;
	}

	HttpData::url_t&
	HttpData::url () const {
		return this->url_;
	}

	void
	HttpData::set_url (const qg::HttpData::url_t &url) {
		this->url_ = url;
	}

	void
	HttpData::set_query (const qg::HttpData::query_t &query) {
		//TODO (qinggniq) judge whether NULL
		this->query_ = query;
	}

	HttpData::query_vt&
	HttpData::query_item (const qg::HttpData::query_vt &key) const {
		//TODO (qinggniq) fix the bug when key not in query
		return this->query_[key];
	}

	void
	HttpData::set_query_item (const qg::HttpData::query_kt &key, const qg::HttpData::query_vt &value) {
		//best practice to insert/update a key-value in c++.
		//https://stackoverflow.com/questions/14218042/most-efficient-way-to-assign-values-to-maps
		this->query_.emplace (key, value);
	}

	HttpData::header_vt&
	HttpData::header_item (const qg::HttpData::header_kt &key) const {
		//TODO (qinggniq) fix the bug when key is not in header
		return this->header_[key];
	}

	void
	HttpData::set_header_item (const qg::HttpData::header_kt &key, const qg::HttpData::header_vt &value) {
		//best practice to insert/update a key-value in c++.
		//https://stackoverflow.com/questions/14218042/most-efficient-way-to-assign-values-to-maps

		this->header_.emplace (key, value);
	}
	void
	HttpData::set_header (const header_t &header) const {
		//TODO (qinggniq) judge header (null ？)
		this->header_ = header;
	}


	HttpData::body_t&
	HttpData::body () const {
		return this->body_;
	}

	void
	HttpData::set_body (const qg::HttpData::body_t &body) {
		//TODO (qinggniq) judge body (null ？)
		this->body_ = body;
	}
} //namespace qg
