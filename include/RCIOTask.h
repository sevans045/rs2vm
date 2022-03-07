#ifndef RS2VM_RCIOTASK_H
#define RS2VM_RCIOTASK_H

#include <QtCore>
#include <QRunnable>
#include <QTcpSocket>

namespace rs2vm
{

// TODO: read socket, parse bytes and forward packet object
//       to appropriate handler?
class RCIOTask : public QObject, public QRunnable
{
Q_OBJECT

public:
    explicit RCIOTask(qintptr socketDescriptor);

private:
    void run() override;

private slots:

    void readSocketData();

    void disconnected();

signals:

    void resultReady();

    void error(QTcpSocket::SocketError socketError);

private:
    qintptr socketDescriptor;
    QTcpSocket* sock = nullptr;
};

}

#endif //RS2VM_RCIOTASK_H
