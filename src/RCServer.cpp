#include <iostream>
#include <cstdint>
#include <sstream>

#include <QDataStream>
#include <QtEndian>

#include "RCServer.h"

constexpr int BUFFER_SIZE = 255;

QDataStream& operator>>(QDataStream& in, EPacketID& packetId)
{
	return in >> reinterpret_cast<uint8_t&>(packetId);
}

std::ostream& operator<<(std::ostream& os, EPacketID& packetId)
{
	switch (packetId)
	{
		case EPacketID::None:
			return os << "EPacketID::None";
		case EPacketID::PlayerMeta:
			return os << "EPacketID::PlayerMeta";
		case EPacketID::PlayerStoppedSpeaking:
			return os << "EPacketID::PlayerStoppedSpeaking";
		case EPacketID::PlayerSpeaking:
			return os << "EPacketID::PlayerSpeaking";
		case EPacketID::PlayerSpeakingSpatialized:
			return os << "EPacketID::PlayerSpeakingSpatialized";
		case EPacketID::PlayerDisconnected:
			return os << "EPacketID::PlayerDisconnected";
		case EPacketID::PlayerSpawned:
			return os << "EPacketID::PlayerSpawned";
		case EPacketID::PlayerDied:
			return os << "EPacketID::PlayerDied";
		case EPacketID::PlayerSquadChanged:
			return os << "EPacketID::PlayerSquadChanged";
		case EPacketID::PlayerTeamChanged:
			return os << "EPacketID::PlayerTeamChanged";
	}
	return os << static_cast<uint32_t>(packetId);
}

QDataStream& operator<<(QDataStream& out, EPacketID& packetId)
{
	std::stringstream ss;
	ss << packetId;
	return out << ss.get();
}

QDebug operator<<(QDebug debug, EPacketID& packetId)
{
	std::stringstream ss;
	ss << packetId;
	return debug << ss.get();
}

void RCServer::incomingConnection(qintptr socketDescriptor)
{
	new RCPacketHandler(socketDescriptor, this);
}

void RCServer::startServer()
{
	listen(QHostAddress("localhost"), 10053);
}

void RCServer::closeServer()
{
	close();
}

RCPacketHandler::RCPacketHandler(
	qintptr socketDescriptor, QObject *parent): QObject(parent)
{
	sock = new QTcpSocket{};
	if (!sock->setSocketDescriptor(socketDescriptor))
	{
		emit error(sock->error());
		return;
	}

	connect(sock, &QTcpSocket::readyRead,
			this, &RCPacketHandler::readSocketData, Qt::DirectConnection);
	connect(sock, &QTcpSocket::disconnected,
			this, &RCPacketHandler::disconnected);
}

void RCPacketHandler::readSocketData()
{
	QByteArray data;
	data = sock->read(BUFFER_SIZE);

	QHostAddress peerAddr = sock->peerAddress();
	qWarning() << peerAddr << " data: " << data;

	if (data.size() < Packet::minDataSize())
	{
		qWarning() << "not enough data received";
		return;
	}

	QDataStream dataStream{data};
	// Sending from UnrealScript side in little endian.
	dataStream.setByteOrder(QDataStream::LittleEndian);

	uint32_t size;
	EPacketID packetId;
	// size = static_cast<uint32_t>(static_cast<uint8_t>(data[0]));a
	// packetId = static_cast<EPacketID>(static_cast<uint8_t>(data[1]));
	dataStream >> size;
	dataStream >> packetId;
	qWarning() << "device.size = " << dataStream.device()->size();
	qWarning() << "size = " << size;
	qWarning() << "packetId = " << packetId;

	switch (packetId)
	{
		case EPacketID::PlayerMeta:
			handlePlayerMeta(dataStream);
			break;
		case EPacketID::PlayerStoppedSpeaking:
			handlePlayerStoppedSpeaking(data);
			break;
		case EPacketID::PlayerSpeaking:
			handlePlayerSpeaking(data);
			break;
		case EPacketID::PlayerSpeakingSpatialized:
			handlePlayerSpeakingSpatialized(data);
			break;
		case EPacketID::PlayerDisconnected:
			handlePlayerDisconnected(data);
			break;
		case EPacketID::PlayerSpawned:
			handlePlayerSpawned(data);
			break;
		case EPacketID::PlayerDied:
			handlePlayerDied(data);
			break;
		case EPacketID::PlayerSquadChanged:
			handlePlayerSquadChanged(data);
			break;
		case EPacketID::PlayerTeamChanged:
			handlePlayerTeamChanged(data);
			break;
		case EPacketID::None:
		default:
			break;
	}
}

// TODO: don't return anything and handle the data right away.
PlayerMeta RCPacketHandler::handlePlayerMeta(QDataStream& dataStream)
{
	if (dataStream.device()->size() < PlayerMeta::minDataSize())
	{
		qWarning() << "not enough data for PlayerMeta";
		return {};
	}

	PlayerMeta pm;
	uint32_t netIdA;
	uint32_t netIdB;

	dataStream >> pm.team;
	dataStream >> pm.squad;
	dataStream >> pm.playerId;
	dataStream >> netIdA;
	dataStream >> netIdB;
	// (B << 32) | A
	pm.uniqueNetId = (static_cast<uint64_t>(netIdB) << 32) | netIdA;
	// dataStream >> pm.uniqueNetId;
//	pm.team = static_cast<ETeam>(static_cast<uint8_t>(data[2]));
//	pm.squad = static_cast<uint8_t>(data[3]);
//	pm.playerId = qFromBigEndian(
//		static_cast<uint32_t>(data[4])		  |
//		static_cast<uint32_t>(data[5]) << 8   |
//		static_cast<uint32_t>(data[6]) << 16  |
//		static_cast<uint32_t>(data[7]) << 24);
//	pm.uniqueNetId = qFromBigEndian(
//		(static_cast<uint64_t>(data[8])        |
//		 static_cast<uint64_t>(data[9])  << 8  |
//		 static_cast<uint64_t>(data[10]) << 16 |
//		 static_cast<uint64_t>(data[11]) << 24
//		 ) << 32 |
//		(static_cast<uint64_t>(data[12])       |
//		 static_cast<uint64_t>(data[13]) << 8  |
//		 static_cast<uint64_t>(data[14]) << 16 |
//		 static_cast<uint64_t>(data[15]) << 24
//		 ));

	qWarning() << "team          " << static_cast<uint8_t>(pm.team);
	qWarning() << "squad         " << pm.squad;
	qWarning() << "playerId      " << pm.playerId;
	qWarning() << "uniqueNetId   " << pm.uniqueNetId;

	// TODO: Do murmur specific things with PlayerMeta data...
	//		 emit result?

	return pm;
}

PlayerStoppedSpeaking RCPacketHandler::handlePlayerStoppedSpeaking(QByteArray &data)
{
	return {};
}

PlayerSpeaking RCPacketHandler::handlePlayerSpeaking(QByteArray &data)
{
	return {};
}

PlayerSpeakingSpatialized RCPacketHandler::handlePlayerSpeakingSpatialized(QByteArray &data)
{
	return {};
}

PlayerDisconnected RCPacketHandler::handlePlayerDisconnected(QByteArray &data)
{
	return {};
}

PlayerSpawned RCPacketHandler::handlePlayerSpawned(QByteArray &data)
{
	return {};
}

PlayerDied RCPacketHandler::handlePlayerDied(QByteArray &data)
{
	return {};
}

PlayerSquadChanged RCPacketHandler::handlePlayerSquadChanged(QByteArray &data)
{
	return {};
}

PlayerTeamChanged RCPacketHandler::handlePlayerTeamChanged(QByteArray &data)
{
	return {};
}

void RCPacketHandler::disconnected()
{

}

uint32_t PlayerMeta::minDataSize()
{
	return Packet::minDataSize()
		   + sizeof(uint8_t)
		   + sizeof(uint8_t)
		   + sizeof(uint32_t)
		   + sizeof(uint64_t);
}
