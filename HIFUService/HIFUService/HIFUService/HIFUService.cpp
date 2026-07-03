// HIFUService.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "include/hifu_service.h"

int _tmain(int argc, char* argv[])
{

	// 初始化 acl 库
	acl::acl_cpp_init();

	HttpService& ms = acl::singleton2<HttpService>::get_instance();

	// 开始运行
	acl::log::stdout_open(true);  // 日志输出至标准输出
	const char* addr = "0.0.0.0:8838";
	printf("listen on: %s\r\n", addr);
	if (argc >= 3)
		ms.run_alone(addr, argv[2], 0, 100);  // 单独运行方式
	else
		ms.run_alone(addr, NULL, 0, 100);  // 单独运行方式

	printf("Enter any key to exit now\r\n");
	getchar();

	return 0;
}

