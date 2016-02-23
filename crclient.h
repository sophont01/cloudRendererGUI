#ifndef CRCLIENT_H
#define CRCLIENT_H
#include <string>
#include <iostream>
#include <QObject>
#include<QPixmap>
#include <QDebug>


#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include<boost/thread.hpp>
#include<turbojpeg.h>

using boost::asio::ip::udp;

#define ID 0
#define FRAME 1
#define MODEL 3

#define CONNECT '0'
#define RENDER '1'
#define EXIT 'X'


class CRClient:public QObject
{
    Q_OBJECT
public:
    //explicit CRClient(QObject *parent = 0);
    CRClient ();
    ~CRClient(){
        m_ioWorkThread->join();
    }
    void run () { io_service.run(); }
 /*   void run () {
        m_ioServiceWork = new boost::asio::io_service::work(io_service);
        m_ioWorkThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));
        //   io_service.work();
    }*/
    std::string connect (std::string _ip, std::string _port);
    void render (std::string model, std::string fps, std::string width, std::string height);

signals:
    void updateFrameBuffer_signal(QPixmap*);
    void exitRenderer();

private:
    void connectRenderer ();


    void rlisten ();
    void sendIOtoRenderer (std::string io);

    void handle_rreceive (const boost::system::error_code&, std::size_t);
    void handle_rsend (boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t);

    bool connection;

    std::string renderer_ip;
    std::string renderer_port;

    boost::asio::io_service io_service, rio_service;
    udp::resolver resolver, rresolver;
    udp::socket socket, rsocket;
    udp::endpoint remote_endpoint, renderer_endpoint;
    boost::array<char, 1024> recv_rbuffer;
    boost::asio::io_service::work* m_ioServiceWork;
    boost::thread* m_ioWorkThread;
    tjhandle jpeg;
    int pixelformat,
    subsamp,
    flags;
    int width, height, pitch;
    QPixmap* myFrame;



};

#endif // CRCLIENT_H
