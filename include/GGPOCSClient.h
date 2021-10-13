#ifndef _GGPOCSClient_H
#define _GGPOCSClient_H

#include <nicenet/nicenet.h>
class CS;
class GGPOCSClient :public nicehero::TcpSessionC
{
public:
	virtual void close();
	CS* m_parent;
};

#endif
