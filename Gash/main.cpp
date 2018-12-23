#include "base/GPath.h"
#include <conio.h>
#include "base/GUrlParts.h"
#include <unordered_map>
#include "base/reflection/GUserTypeRegister.h"
#include "base/GWin32BaseTypeRegistry.h"
#include "base/GValue.h"
#include "base/GValueConverter.h"
#include "base/reflection/GField.h"
#include "base/reflection/GFunction.h"
#include "base/reflection/GFunctionTraits.h"
#include "base/reflection/GClassRegistry.h"
#include "base/GShell.h"
#include "base/GRegex.h"
#include "base/net/GHttpClient.h"
#include "base/stream/GStream.h"
#include "base/stream/GFileStream.h"
#include "base/io/GBufferIO.h"
#include "base/stream/GBufferStream.h"
#include "base/stream/GIOStream.h"

class A: public GObject
{
public:
	A()
	{
		memset(&a, 0, sizeof(a));
	}

	int test(const RECT& rc, int val)
	{
		printf("test(), %d\n", val);
		return rc.top;
	}
	RECT a;
};

int main()
{
	Win32BaseTypeRegister();

// 	GClassRegistry<A>("A")
// 		.addField("a", &A::a)
// 		.addDefaultConstructor()
// 		.addFunction("test", &A::test)
// 		.end();
// 
// 	GClassPtr pClass = _reflectionManager->findClass("A");
// 	GObjectPtr pObject = pClass->newInstance();
// 	pClass->setValue("a", pObject, GValueConverter<RECT>()({5, 6, 7, 8}));
// 	RECT rc;
// 	pClass->getValue("a", pObject).getUserData(&rc, sizeof(rc));
// 	
// 	RECT rrc = { 5,6,7,8 };
// 	pClass->call<const RECT&>("test", pObject, rrc, 5);
// 
// 	GFileIO file;
// 	if (!file.open(L"E:/text.txt", GFileIO::Operation::Create, GFileIO::ModeReadWrite))
// 	{
// 		printf("创建文件失败");
// 		return -1;
// 	}
// 	file.write(u8"Test测试用数据");
// 	GString str = file.readAll().toString();
// 	printf("%s\n", str.toAnsi().data());
// 	file.close();
	
// 	GFile::WriteFile(L"E:/test.txt", "测试api");
// 
// 	GTLSSocket s;
// 	s.enableFlags(GTLSSocket::SkipCertificationCheck);
// 	if (!s.connect(L"http://www.baidu.com"))
// 	{
// 		printf("连接失败");
// 	}
// 	system("chcp 65001");
// 	GHttpClient httpClient;
// 	//httpClient.skipCertificationCheck(true);
// 	httpClient.setUrl(L"https://www.baidu.com");
// 	httpClient.setHeader(L"Accept", L"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
// 	httpClient.setHeader(L"Accept-Encoding", L"gzip, deflate, br");
// 	httpClient.setHeader(L"Accept-Language", L"zh-CN,zh;q=0.9,en;q=0.8,gl;q=0.7,fr;q=0.6,zh-TW;q=0.5,lb;q=0.4,ja;q=0.3");
// 	httpClient.setHeader(L"Upgrade-Insecure-Requests", L"1");
// 	httpClient.setHeader(L"Cookie", L"BAIDUID=10D38B6D55BE9369FDDB61DBB2466E69:FG=1; BIDUPSID=10D38B6D55BE9369FDDB61DBB2466E69; PSTM=1490709583; BDUSS=1BuNnREUm15bUVTam9vQ3FpcW9Lc2p0bXo5eXNsdVBBYXhhNHJVYk1uVWJ3QTFaSUFBQUFBJCQAAAAAAAAAAAEAAABC6nw4amtsbGxpagAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABsz5lgbM-ZYd; BD_UPN=12314753; pgv_pvi=9137525760; BD_HOME=1; delPer=0; BD_CK_SAM=1; PSINO=6; H_PS_PSSID=28040_1444_25809_21116_18559_28018_26350_27245_27509; BDRCVFR[feWj1Vr5u3D]=I67x6TjHwwYf0; H_PS_645EC=7d98wTOgoBI%2BmlFNTqJw%2BHhOCzA4J4QsGdHkwYeyUE1Jx4gQq5qMQxIf5x0v3NAzQVtq; BDSVRTM=143");
// 	httpClient.setHeader(L"UserAgent", L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36");
// 	httpClient.setHeader(L"Cache-Control", L"max-age=0");
// 	httpClient.setDebug(true);
// 	httpClient.setFollowLocation(true);
// 	if (!httpClient.execute())
// 	{
// 		printf("链接失败: %s\n", httpClient.getErrorMessage().toUtf8().data());
// 	}

	GBufferStreamPtr buffer = new GBufferStream();
	buffer << 5;
	GFileOutputStreamPtr pStream = new GFileOutputStream(L"E:/test.txt");
	for (size_t i = 0; i < 4096; ++i)
	{
		char buf[16] = {};
		size_t _size = sprintf_s(buf, "%zu,", i);
		buffer << buf;
	}
	GIOStreamPtr pIOStream = new GIOStream();
	buffer->pipe(pIOStream);

	_getch();

// 	char buf[1024] = {};
// 	size_t readSize = 0;
// 	while ((readSize = buffer.read(buf, readSize)) != 0)
// 	{
// 		buf[readSize] = 0;
// 		printf("%s\n", buf);
// 	}
/*	_getch();*/
// 	GOutputStreamPtr pOutputStream = new GFileOutputStream(L"E:/test.txt");
// 	pOutputStream->write(GDataArray(L"测试文本", GDataArray::EncodingUtf8));
// 	pOutputStream->flush();

	return 0;
}