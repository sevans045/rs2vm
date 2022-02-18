#ifndef MUMBLE_RCSERVER_H
#define MUMBLE_RCSERVER_H

#include <cstdint>

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThreadPool>

enum class EPacketID
{
	None						= 0,
	PlayerMeta 					= 1,
	PlayerStoppedSpeaking       = 2,
	PlayerSpeaking              = 3,
	PlayerSpeakingSpatialized   = 4,
	PlayerDisconnected 			= 5,
	PlayerSpawned 				= 6,
	PlayerDied 					= 7,
	PlayerSquadChanged 			= 8,
	PlayerTeamChanged 			= 9,
};

enum class ETeam
{
	Axis 	= 0,
	Allies 	= 1,
	None    = 255,
};

class Packet
{
public:
	Packet(): id(EPacketID::None) {};
	virtual ~Packet() = default;
	inline static uint32_t minDataSize() {return HEADER_SIZE;};

public:
	static constexpr uint8_t HEADER_SIZE = sizeof(uint8_t) * 2;
	EPacketID id;
};

class PlayerMeta: public Packet
{
public:
	inline static uint32_t minDataSize();

public:
	ETeam team;
	uint8_t squad;
	uint32_t playerId;
	uint64_t uniqueNetId;
};

class PlayerStoppedSpeaking: public Packet{};
class PlayerSpeaking: public Packet{};
class PlayerSpeakingSpatialized: public Packet{};
class PlayerDisconnected: public Packet{};
class PlayerSpawned: public Packet{};
class PlayerDied: public Packet{};
class PlayerSquadChanged: public Packet{};
class PlayerTeamChanged: public Packet{};

/// Rising Storm 2: Vietnam dedicated server remote control.
class RCServer: public QTcpServer
{
	Q_OBJECT

public:
	explicit RCServer(QObject* parent = nullptr):
		  QTcpServer(parent)
	{}

protected:
	void incomingConnection(qintptr socketDescriptor) override;

public slots:
	void startServer();
	void closeServer();
};

class RCPacketHandler : public QObject
{
	Q_OBJECT

public:
	explicit RCPacketHandler(qintptr socketDescriptor, QObject* parent = nullptr);

private:
	static inline PlayerMeta handlePlayerMeta(QDataStream& dataStream);
	static inline PlayerStoppedSpeaking handlePlayerStoppedSpeaking(QByteArray& data);
	static inline PlayerSpeaking handlePlayerSpeaking(QByteArray& data);
	static inline PlayerSpeakingSpatialized handlePlayerSpeakingSpatialized(QByteArray& data);
	static inline PlayerDisconnected handlePlayerDisconnected(QByteArray& data);
	static inline PlayerSpawned handlePlayerSpawned(QByteArray& data);
	static inline PlayerDied handlePlayerDied(QByteArray& data);
	static inline PlayerSquadChanged handlePlayerSquadChanged(QByteArray& data);
	static inline PlayerTeamChanged handlePlayerTeamChanged(QByteArray& data);

private slots:
	void readSocketData();
	void disconnected();

signals:
	void resultReady();
	void error(QTcpSocket::SocketError socketError);

private:
	QTcpSocket* sock = nullptr;
};

// TODO: read socket, parse bytes and forward packet object
//       to appropriate handler?
// TODO: rename? RCIOTask? RCSocketTask?
class RCPacketTask: public QObject, public QRunnable
{
	Q_OBJECT

};

#endif // MUMBLE_RCSERVER_H
