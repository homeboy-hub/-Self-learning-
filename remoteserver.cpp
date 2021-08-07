/*
 * 程序名：remoteserver.cpp，此程序将从客户端接收到的数据信息提取并存放到数据库的表中
 * 作者：张鸿银
 * 时间：2021/8/7
 */

#include "_freecplus.h"
#include "_ooci.h"

typedef struct role
{
	int id;
	char name[32];
	double score;
}role;

int main(int argc,char *argv[])
{
	CTcpServer TcpServer;   // 创建服务端对象。

	if (TcpServer.InitServer(5000)==false) // 初始化TcpServer的通信端口。
	{
		printf("TcpServer.InitServer(5000) failed.\n"); return -1;
	}

	if (TcpServer.Accept()==false)   // 等待客户端连接。
	{
		printf("TcpServer.Accept() failed.\n"); return -1;
	}

	printf("客户端(%s)已连接。\n",TcpServer.GetIP());

	char strbuffer[1024];  // 存放数据的缓冲区。

	memset(strbuffer,0,sizeof(strbuffer));
	TcpServer.Read(strbuffer,20); // 接收客户端发过来的请求报文。
	printf("接收：%s\n",strbuffer);

	// 解析数据并放入暂存结构体
	role tmprole;
	memset(tmprole.name,0,sizeof(tmprole.name));
	GetXMLBuffer(strbuffer,"id",&tmprole.id);
	GetXMLBuffer(strbuffer,"name",tmprole.name,31);
	GetXMLBuffer(strbuffer,"score",&tmprole.score);

	connection conn; // 数据库连接类

	// 登录数据库，返回值：0-成功，其它-失败。
	// 失败代码在conn.m_cda.rc中，失败描述在conn.m_cda.message中。
	if (conn.connecttodb("scott/tiger@snorcl11g_142","Simplified Chinese_China.ZHS16GBK")!=0)
	{
		printf("connect database failed.\n%s\n",conn.m_cda.message); return -1;
	}

	sqlstatement stmt(&conn); // 操作SQL语句的对象。

	stmt.prepare("insert into Role(id,name,score) \
			values(:1,:2,:3)");

	stmt.bindin(1,&tmprole.id);
	stmt.bindin(2,tmprole.name,32);
	stmt.bindin(3,&tmprole.score);

	// 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
	// 失败代码在stmt.m_cda.rc中，失败描述在stmt.m_cda.message中。
	if (stmt.execute() != 0)
	{
		printf("stmt.execute() failed.\n%s\n%s\n",stmt.m_sql,stmt.m_cda.message); return -1;
	}

	memset(strbuffer,0,sizeof(strbuffer));
	sprintf(strbuffer,"成功插入了%ld条记录。\n",stmt.m_cda.rpc); // stmt.m_cda.rpc是本次执行SQL影响的记录数

	//printf("result : id = %d name = %s score = %3.1f\n",tmprole.id,\
	//tmprole.name,tmprole.score);
	TcpServer.Write(strbuffer);     // 向客户端回应报文。

	printf("insert table Role ok.\n");

	conn.commit(); // 提交数据库事务

	return 0;
}
