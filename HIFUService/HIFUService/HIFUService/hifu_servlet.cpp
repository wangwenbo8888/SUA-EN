
#include "include/hifu_servlet.h"
#include "include/hifu_service.h"

HttpServlet::HttpServlet(HttpService &_httpDataSer4)
: httpDataSer4(_httpDataSer4)
{
}

HttpServlet::~HttpServlet(void)
{
	acl::log::msg1("HttpServlet::~HttpServlet");
}

bool HttpServlet::doGet(acl::HttpServletRequest&req, 
						  acl::HttpServletResponse&resp)
{
	return httpDataSer4.doPost(req, resp);
}

bool HttpServlet::doPost(acl::HttpServletRequest& req, 
						   acl::HttpServletResponse& resp)
{
	return httpDataSer4.doPost(req, resp);
}

bool HttpServlet::doUnknown(acl::HttpServletRequest&, 
							  acl::HttpServletResponse& res)
{
	res.setStatus(400);
	res.setContentType("text/html; charset=");

	if (res.sendHeader() == false)
		return false;

	acl::string buf("<root error='unkown request method' />\r\n");
	(void) res.getOutputStream().write(buf);
	return false;
}

bool HttpServlet::doError(acl::HttpServletRequest&, 
							acl::HttpServletResponse&)
{
	return false;
}
