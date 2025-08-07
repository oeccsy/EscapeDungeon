#include "ConnectLevel.h"

ConnectLevel::ConnectLevel()
{
	server.InitSocket();
	server.Bind();
	server.Listen();

	FD_ZERO(&server.readSet);
	FD_SET(server.listenSocket, &server.readSet);
}

ConnectLevel::~ConnectLevel()
{
}

void ConnectLevel::BeginPlay()
{
	super::BeginPlay();
}

void ConnectLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void ConnectLevel::Render()
{
	super::Render();
}

void ConnectLevel::SendPlayersData()
{
}