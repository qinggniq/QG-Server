//
// Created by wc on 1/15/19.
//

#include "HttpData.h"

namespace qg{
	MIMEType::mime_type_map MIMEType::mime_type_ = MIMEType::mime_type_map ();
	MIMEType::MIMEType () {}

	MIMEType::MIMEType (const qg::MIMEType &mime) {
		mime_type_ = MIMEType::mime_type_map (mime.mime_type_);
	}

	MIMEType::~MIMEType () {}


	void
	MIMEType::init () {
		mime_type_.emplace(".html", "text/html");
//		mime_type_[".avi"] = "video/x-msvideo";
//		mime_type_[".bmp"] = "image/bmp";
//		mime_type_[".c"] = "text/plain";
//		mime_type_[".doc"] = "application/msword";
//		mime_type_[".gif"] = "image/gif";
//		mime_type_[".gz"] = "application/x-gzip";
//		mime_type_[".htm"] = "text/html";
//		mime_type_[".ico"] = "application/x-ico";
//		mime_type_[".jpg"] = "image/jpeg";
//		mime_type_[".png"] = "image/png";
//		mime_type_[".txt"] = "text/plain";
//		mime_type_[".mp3"] = "audio/mp3";
//		mime_type_["default"] = "text/html";
	}

	MIMEType::mime_vt
	MIMEType::mime_item (qg::MIMEType::mime_kt &suffix) {
		auto it = MIMEType::mime_type_.find (suffix);
		if (it != MIMEType::mime_type_.end()) {
			return it->second;
		}else{
			return mime_vt ();
		}
	}

	HttpData::HttpData () {
			this->method_ = "";
			this->version_ = "";
			this->uri_ = "";
			this->header_ = HttpData::header_t ();
			this->body_ = HttpData::body_t ();
	}



	HttpData::~HttpData () {

	}

	HttpData::method_t
	HttpData::method () const {
		return this->method_;
	}

	void
	HttpData::set_methd (const qg::HttpData::method_t &method) {
		this->method_ = method;
	}

	HttpData::version_t
	HttpData::version () const {
		return this->version_;
	}

	void
	HttpData::set_version (const qg::HttpData::version_t &version) {
		this->version_ = version;
	}

	HttpData::uri_t
	HttpData::uri () const {
		return this->uri_;
	}

	inline
	void
	HttpData::set_uri (const qg::HttpData::uri_t &uri) {
		this->uri_ = uri;
	}

	inline
	void
	HttpData::set_query (const qg::HttpData::query_t &query) {
		//TODO (qinggniq) judge whether NULL
		this->query_ = query;
	}

	inline
	HttpData::query_vt
	HttpData::query_item (const qg::HttpData::query_vt &key) const {
		//TODO (qinggniq) fix the bug when key not in query
		auto it = query_.find (key);
		if (it != query_.end()) {
			return it->second;
		}else{
			return query_vt ();
		}
	}

	inline
	void
	HttpData::set_query_item (const qg::HttpData::query_kt &key, const qg::HttpData::query_vt &value) {
		//best practice to insert/update a key-value in c++.
		//https://stackoverflow.com/questions/14218042/most-efficient-way-to-assign-values-to-maps
		this->query_.emplace (key, value);
	}

	inline
	HttpData::header_vt
	HttpData::header_item (const qg::HttpData::header_kt &key) const {
		//TODO (qinggniq) fix the bug when key is not in header
		auto it = this->header_.find (key);
		if (it != this->header_.end()) {
			return it->second;
		}else{
			return header_vt ();
		}
	}

	void
	HttpData::set_header_item (const qg::HttpData::header_kt &key, const qg::HttpData::header_vt &value) {
		//best practice to insert/update a key-value in c++.
		//https://stackoverflow.com/questions/14218042/most-efficient-way-to-assign-values-to-maps

		this->header_.emplace (key, value);
	}

	inline
	void
	HttpData::set_header (const header_t &header) {
		//TODO (qinggniq) judge header (null ？)
		this->header_ = header;
	}

	inline
	HttpData::body_t
	HttpData::body () const {
		return this->body_;
	}

	inline
	void
	HttpData::set_body (const qg::HttpData::body_t &body) {
		//TODO (qinggniq) judge body (null ？)
		this->body_ = body;
	}


	qg_bool
	HttpData::Encoded () const{

		HttpData::header_t::const_iterator it = this->header_.find (HttpData::header_kt("Transfer-Encoding"));
		if (it != this->header_.end ()) {
			//TODO (qinggniq) find the real value of "Transfer-Encoding" field
			return it->second == qg_string("True");
		}
		return kFalse;
	}

	HttpData::header_t
	HttpData::header () const {
		return this->header_;
	}
} //namespace qg
