#pragma once

#include "lib_acl.h"
#include "acl_cpp/lib_acl.hpp"

class HttpService;
class HttpServlet : public acl::HttpServlet
{
public:
	HttpServlet(HttpService &httpDataSer4);
	virtual ~HttpServlet(void);

	virtual bool doGet(acl::HttpServletRequest&, acl::HttpServletResponse&);

	virtual bool doPost(acl::HttpServletRequest&, acl::HttpServletResponse&);

	virtual bool doUnknown(acl::HttpServletRequest&, acl::HttpServletResponse&);

	virtual bool doError(acl::HttpServletRequest&, acl::HttpServletResponse&);

private:
	HttpService &httpDataSer4;

};

