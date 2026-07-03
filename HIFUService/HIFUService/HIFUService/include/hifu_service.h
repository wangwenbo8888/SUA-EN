#pragma once

#include "lib_acl.h"
#include "acl_cpp/lib_acl.hpp"

#include "include/hifu_database.h"


class HttpService : public acl::master_threads
{
public:

	HttpService(void);
	virtual ~HttpService(void);
	bool doPost(acl::HttpServletRequest& req,acl::HttpServletResponse& res);

private:

	virtual bool thread_on_read(acl::socket_stream* stream);
	virtual bool thread_on_accept(acl::socket_stream* stream);
	virtual bool thread_on_timeout(acl::socket_stream* stream);
	virtual void thread_on_close(acl::socket_stream* stream);
	virtual void thread_on_init();
	virtual void thread_on_exit();
	virtual void proc_pre_jail();
	virtual void proc_on_init();
	virtual void proc_on_exit();

    unsigned char* _buf;

    SqliteDatabase* m_Sqlite;

};

