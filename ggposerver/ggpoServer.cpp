// asio_server.cpp : Defines the entry point for the console application.
//

#include <nicenet/nicenet.h>
#include <ggponet.h>
#include <GGPOCSMessage.hpp>

#include <unordered_map>

class MyClient :public nicehero::TcpSessionS
{
public:
	void close();
	std::string m_playerID;
	std::string m_room;
};

std::unordered_map<std::string,MyClient*> players;

class MyServer :public nicehero::TcpServer
{
public:
	MyServer(const std::string& ip, ui16 port)
		:TcpServer(ip, port)
	{}
	virtual nicehero::TcpSessionS* createSession();
};

nicehero::TcpSessionS* MyServer::createSession()
{
	return new MyClient();
}


class MyKcpSession : public nicehero::KcpSessionS
{
public:
};
class MyKcpServer :public nicehero::KcpServer
{
public:
	MyKcpServer(const std::string& ip, ui16 port)
		:KcpServer(ip, port)
	{}
	virtual nicehero::KcpSessionS* createSession();
};

nicehero::KcpSessionS* MyKcpServer::createSession()
{
	return new MyKcpSession();
}


int main(int argc, char* argv[])
{
	bool v6 = (argc > 1 && std::string(argv[1]) == "v6") ? true : false;
	nicehero::start(true);
	std::string listenIP = "0.0.0.0";
	if (v6)
	{
		listenIP = "::";
	}
	nicehero::HttpServer httpServer(listenIP,8080);
	httpServer.addHandler("/", [] HTTP_HANDLER_PARAMS{
		res->write("hello world");
	});
	httpServer.start();
	MyServer tcpServer(listenIP, 7000);
	tcpServer.accept();
	MyKcpServer kcpServer(listenIP, 7001);
 	kcpServer.accept();
	nicehero::joinMain();
	
	return 0;
}

TCP_SESSION_COMMAND(MyClient, GGPO_CS_MESSAGE_ID)
{
	GGPOCSMessage d;
	*msg >> d;
	if (d.fromPlayerID == "")
	{
		return true;
	}
	MyClient* c = dynamic_cast<MyClient*>(session.get());
	if (!c)
	{
		return true;
	}
	if (c->m_playerID == "")
	{
		c->m_playerID = d.fromPlayerID;
		players[d.fromPlayerID] = c;
		return true;
	}
	if (c->m_playerID != d.fromPlayerID)
	{
		return true;
	}
	auto it = players.find(d.playerID);
	if (it == players.end())
	{
		return true;
	}
	it->second->sendMessage(d);
	return true;
}

void MyClient::close()
{
	players.erase(m_playerID);
	nicehero::TcpSessionS::close();
}

