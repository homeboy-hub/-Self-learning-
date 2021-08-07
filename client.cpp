/*
 * 程序名：client.cpp，此程序为将本地数据文件的内容上传到远端数据库的客户端
 * 作者：张鸿银
 * 日期：2021/8/7
 */

#include "_freecplus.h"

int main(int argc,char *argv[])
{

	FILE *fp=0;
	char strBuffer[1024];    // 存放数据的缓冲区。

	if ( (fp=FOPEN("testdata.xml","r"))==0)
	{
		printf("FOPEN(testdata.xml) %d:%s\n",errno,strerror(errno)); return -1;
	}

	memset(strBuffer,0,sizeof(strBuffer));
	FGETS(fp,strBuffer,1024);

	fclose(fp);

	CTcpClient TcpClient;   // 创建客户端的对象。

	if (TcpClient.ConnectToServer("192.168.150.142",5000) == false) // 向服务端发起连接请求。
	{
		printf("TcpClient.ConnectToServer(\"192.168.150.142\",5000) failed.\n"); return -1;
	}

	printf("发送：%s\n",strBuffer);
	TcpClient.Write(strBuffer);    // 向服务端发送请求报文。

	memset(strBuffer,0,sizeof(strBuffer));
	TcpClient.Read(strBuffer,20);  // 接收服务端的回应报文。
	printf("接收：%s\n",strBuffer);


	// 程序直接退出，析构函数会释放资源。
}
