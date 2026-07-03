

#include "include/hifu_service.h"

#include "include/hifu_servlet.h"
#include "include/hifu_robot.h"
#include "include/hifu_common.h"
#include "include/hifu_jpeg.h"

#include "json/reader.h"

#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool reply(acl::HttpServletRequest& req,
						  acl::HttpServletResponse& res, 
						  int status, 
 						  const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	acl::string  buf;
	buf.vformat(fmt, ap);
	va_end(ap);
	bool keep_alive = req.isKeepAlive();
	res.setStatus(status)
		.setKeepAlive(keep_alive)
		.setContentType("text/html; charset=utf-8")
		.setContentLength(buf.length());
	return res.write(buf) && keep_alive;
}


HttpService::HttpService(void)
{
    m_Sqlite = NULL;
}

HttpService::~HttpService(void)
{
}

bool HttpService::thread_on_read(acl::socket_stream* stream)
{

	HttpServlet* servlet = (HttpServlet*) stream->get_ctx();
	if (servlet == NULL)
		logger_fatal("servlet null!");

	return servlet->doRun("127.0.0.1:11211", stream);
}

bool HttpService::thread_on_accept(acl::socket_stream* stream)
{
	logger("connect from %s, fd: %d", 
		stream->get_peer(true),
		stream->sock_handle());
	
	stream->set_rw_timeout(5);

	HttpServlet* servlet = new HttpServlet(*this);
	stream->set_ctx(servlet);

	return true;
}

bool HttpService::thread_on_timeout(acl::socket_stream* stream)
{
	logger("disconnect from %s, fd: %d", stream->get_peer(),
		stream->sock_handle());
	return false;
}

void HttpService::thread_on_close(acl::socket_stream* stream)
{
	logger("disconnect from %s, fd: %d", stream->get_peer(),
		stream->sock_handle());

	HttpServlet* servlet = (HttpServlet*) stream->get_ctx();
	if (servlet)
		delete servlet;
}

void HttpService::thread_on_init()
{
	
}

void HttpService::thread_on_exit()
{
}

void HttpService::proc_pre_jail()
{
}

void HttpService::proc_on_init()
{
    _buf = new unsigned char[1024*1024*2];
    m_Sqlite = new SqliteDatabase;
    m_Sqlite->initial();
}

void HttpService::proc_on_exit()
{
    delete[]_buf;
}

bool HttpService::doPost(acl::HttpServletRequest& req,
				         acl::HttpServletResponse& res)
{
	const char *reqPath = req.getPathInfo();
    if (strcmp(reqPath, "/checkConnection") == 0)
    {
        return reply(req, res, 200, "{\"status\":\"%s\"}", "connection is ok !");
    }
    else if (strcmp(reqPath, "/robotControl") == 0)
	{
        const char *axis = req.getParameter("axis");
        if (axis == NULL)
        {
            return reply(req, res, 500, "{\"status\":\"%s\"}", "axis null !");
        }
        int iAxis = strToInt(axis);

        const char *position = req.getParameter("position");
        if (position==NULL)
        {
            return reply(req, res, 500, "{\"status\":\"%s\"}", "position null !");
        }
        int pos = strToInt(position);

        // const char *speed = req.getParameter("speed");
        HIFURobot robot;  
        if (robot.setPos(iAxis, pos))
        {
            return reply(req, res, 200, "{\"status\":\"%s\"}", reqPath);
        }
        else
        {
            return reply(req, res, 200, "{\"status\":\"%s\"}", "set position error !");
        }
	}
    else if (strcmp(reqPath, "/getRealtimeImage")==0)
    {
        int size;

        pthread_mutex_lock(&mutex);
        read_jpeg_file("E:\\realtime.jpg", _buf,size);
        res.setContentType("image/BMP")
            .setKeepAlive(req.isKeepAlive())
            .setChunkedTransferEncoding(true)
            .setContentLength(size)
            .write(_buf,size);

        pthread_mutex_unlock(&mutex);

        return res.write(NULL, 0);
    }
    else if (strcmp(reqPath, "/getPatientInfo")==0)
    {
        if (m_Sqlite==NULL)
        {
            return reply(req, res, 500, "{\"status\":\"%s\"}", "Sqlite null !");
        }
        else
        {
            std::string command = "select * from PatientInfo";
            std::string json;
            m_Sqlite->query(command.c_str(),json);
            return reply(req, res, 200, "{\"PatientInfo\":%s}", json.c_str());
        }
    }
    else if (strcmp(reqPath, "/updatePatientInfo") == 0)
    {
        if (m_Sqlite == NULL)
        {
            return reply(req, res, 200, "{\"status\":\"%s\"}", "Sqlite null !");
        }
        else
        {

            const char * id = req.getParameter("id");
            if (id==NULL)
            {
                return reply(req, res, 500, "{\"status\":\"%s\"}", "update patient info id null !");
            }
            const char* name = req.getParameter("name");
            if (name == NULL)
            {
                return reply(req, res, 500, "{\"status\":\"%s\"}", "update patient info name null !");
            }

            std::string command = "update PatientInfo set name = \"";
            command.append(name);
            command.append("\"");

            const char* sex = req.getParameter("sex");
            if (sex!=NULL)
            {
                command.append(", sex = \"");
                command.append(sex);
                command.append("\"");
            }
   
            const char* age = req.getParameter("age");
            if (age!=NULL)
            {
                command.append(", age = \"");
                command.append(age);
                command.append("\"");
            }

            const char* mobile = req.getParameter("mobile");
            if (mobile != NULL)
            {
                command.append(", mobile = \"");
                command.append(mobile);
                command.append("\"");
            }

            const char* deepth = req.getParameter("deepth");
            if (deepth != NULL)
            {
                command.append(", deepth = \"");
                command.append(deepth);
                command.append("\"");
            }

            const char* x = req.getParameter("x");
            if (x != NULL)
            {
                command.append(", size_x = \"");
                command.append(x);
                command.append("\"");
            }

            const char* y = req.getParameter("y");
            if (y != NULL)
            {
                command.append(", size_y = \"");
                command.append(y);
                command.append("\"");
            }

            const char* z = req.getParameter("z");
            if (z != NULL)
            {
                command.append(", size_z = \"");
                command.append(z);
                command.append("\"");
            }

            const char* address = req.getParameter("address");
            if (address != NULL)
            {
                command.append(", address = \"");
                command.append(address);
                command.append("\"");
            }

            command.append(" where id = \"");
            command.append(id);
            command.append("\"");

            if (m_Sqlite->update(command.c_str()))
            {
                return reply(req, res, 200, "{\"status \":\"%s\"}", "update patient info ok");
            }
            else
            {
                return reply(req, res, 200, "{\"status \":\"%s\"}", "update patient info failed");
            }
        }
    }
    else if (strcmp(reqPath, "/getReport") == 0)
    {
        if (m_Sqlite == NULL)
        {
            return reply(req, res, 200, "{\"status\":\"%s\"}", "Sqlite null !");
        }
        else
        {
            const char * id = req.getParameter("id");
            if (id == NULL)
            {
                return reply(req, res, 500, "{\"status\":\"%s\"}", "id name null !");
            }

            std::string command = "select * from PatientInfo where id = \"";
            command.append(id);
            command.append("\"");
            std::string json;

            std::string temp;
            std::string command1 = "select * from Report where pid = \"";
            command1.append(id);
            command1.append("\"");
            if (!m_Sqlite->query(command.c_str(),command1.c_str(),temp))
            {
                return reply(req, res, 500, "{\"status\":\"%s\"}", command.c_str());
            }
            else
            {
                json.append(temp.c_str());
            }

            return reply(req, res, 200, "{\"PatientInfo\":%s}", json.c_str());
        }
    }
    else if (strcmp(reqPath, "/checkLoginInfo") == 0)
    {
        if (m_Sqlite == NULL)
        {
            return reply(req, res, 200, "{\"status\":\"%s\"}", "Sqlite null !");
        }
        else
        {
            const char * doctor = req.getParameter("doctor");
            if (doctor==NULL)
            {
                return reply(req, res, 500, "{\"status\":\"%s\"}", "doctor name null !");
            }

            const char* password = req.getParameter("password");
            if (password==NULL)
            {
                return reply(req, res, 500, "{\"status\":\"%s\"}", "doctor name null !");
            }

            std::string command = "select userName,pw from LoginInfo ";
            command.append("where userName = \"");
            command.append(doctor);
            command.append("\"");
            if (m_Sqlite->checkUserPassword(command.c_str(),password))
            {
                return reply(req, res, 200, "{\"check user password \":\"%s\"}", "check ok");
            }
            else
            {
                return reply(req, res, 200, "{\"check user password \":\"%s\"}", "check failed");
            }      
        }
    }

    std::string info(req.getRequestUri());
    info.append("  unknown error !");
    return reply(req, res, 500, "{\"status\":\"%s\"}", info);
}
