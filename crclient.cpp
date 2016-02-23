#include "crclient.h"



CRClient::CRClient ()
    : socket(io_service), rsocket(io_service),
      resolver(io_service), rresolver(io_service)
{
    connection = false;
    myFrame = new QPixmap;
    width = 800;
    height = 600;
    jpeg = tjInitDecompress();
    pixelformat = TJPF_RGBA;
    flags = 0;
    pitch = width * tjPixelSize[pixelformat];
}

std::string CRClient::connect (std::string _ip, std::string _port)
{
    udp::resolver::query query(udp::v4(), _ip.c_str(), _port.c_str());
    remote_endpoint = *resolver.resolve(query);
    socket.open(udp::v4());

    boost::array<char, 1> send_buf = {{ CONNECT }};
    socket.send_to(boost::asio::buffer(send_buf), remote_endpoint);

    boost::array<char, 512> server_details;
    udp::endpoint sender_endpoint;
    std::size_t len = socket.receive_from(boost::asio::buffer(server_details), sender_endpoint);

    std::cout << "ServerDetails:" << std::endl;
    std::cout.write(server_details.data(), len);
    return std::string(server_details.data(),len);
}

void CRClient::render (std::string model, std::string fps, std::string width, std::string height)
{
    const int mssg_len = model.length() + 3;
    char _fps = (atoi(fps.c_str()));
    std::string mssg = "1" + fps + model;
    std::cout << "Send RENDER " << mssg << std::endl;
    //	boost::array<char, 128> send_buf(mssg.data());
    //	boost::shared_ptr<std::string> mssg_ptr(&mssg);
    //	socket.async_send_to(boost::asio::buffer(*mssg_ptr), remote_endpoint,
    //			boost::bind(&CRClient::handle_rsend, this, mssg_ptr,
    //				boost::asio::placeholders::error,
    //				boost::asio::placeholders::bytes_transferred));

    socket.send_to(boost::asio::buffer(mssg), remote_endpoint);

    boost::array<char, 512> renderer_details;
    udp::endpoint sender_endpoint;
    std::size_t len = socket.receive_from(boost::asio::buffer(renderer_details), sender_endpoint);

    std::cout << "RendererDetails:" << std::endl;
    std::cout.write(renderer_details.data(), len);
    std::cout << std::endl;

    std::stringstream ss(std::string(renderer_details.data(), len));
    ss >> renderer_ip >> renderer_port;

    connectRenderer ();
}

void CRClient::connectRenderer ()
{
    udp::resolver::query query(udp::v4(), renderer_ip.c_str(), renderer_port.c_str());
    renderer_endpoint = *rresolver.resolve(query);
    rsocket.open(udp::v4());

    rlisten ();
    //io_service.run();

    char conn = CONNECT;
    boost::shared_ptr<std::string> send_buf(new std::string(&conn));
    //	while (!connection) {
    rsocket.async_send_to(boost::asio::buffer(*send_buf), renderer_endpoint,
                          boost::bind(&CRClient::handle_rsend, this, send_buf,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
    //	}

    std::cout << "Renderer Connection status " << connection << std::endl;
    run();
}

void CRClient::rlisten ()
{
    qDebug() << "Listening";
    rsocket.async_receive_from (
                boost::asio::buffer(recv_rbuffer), renderer_endpoint,
                boost::bind(&CRClient::handle_rreceive, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void CRClient::sendIOtoRenderer (std::string io)
{
    boost::shared_ptr<std::string> send_buf(new std::string(io));
    rsocket.async_send_to(boost::asio::buffer(*send_buf), renderer_endpoint,
                          boost::bind(&CRClient::handle_rsend, this, send_buf,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
}



void CRClient::handle_rsend (boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t)
{
}

void CRClient::handle_rreceive (const boost::system::error_code& error,
                                std::size_t bytes_transferred)
{
    std::cout << "IN" << std::endl;
    if (!error || error == boost::asio::error::message_size)
    {
        switch(recv_rbuffer.data()[ID])
        {
        case CONNECT:
            connection = true;
            std::cout << "Connected ! Rendering is about to begin !!" << std::endl;
            break;
        case RENDER:
            std::cout << "Rendered Frame " << (int)(recv_rbuffer.data()[FRAME]) << std::endl;
#if 0
        {
            unsigned char* jpegstream = new unsigned char[(int)bytes_transferred-2];
            unsigned long jpegsize = (unsigned long)(bytes_transferred - 2);
            //                              tjDecompressHeader2(jpeg, jpegstream, jpegsize, &width, &height, &subsamp);
            //                              int pitch = width * tjPixelSize[pixelformat];
            unsigned char* data = new unsigned char [width*height*tjPixelSize[pixelformat]];
            tjDecompress2(jpeg, jpegstream, jpegsize, data, width, pitch, height, pixelformat, flags);
            //QImage tempQimage(data,width,height,QImage::Format_RGBA8888);
            QImage tempQImage(data,width,height,QImage::Format_RGBA8888);
            myFrame->fromImage(tempQImage);
            //myFrame->fromImage(QImage::QImage(data,width,height,QImage::Format_RGBA8888));
            delete [] jpegstream;
        }
#endif
            qDebug() << "Emiting";
//            emit updateFrameBuffer_signal(myFrame);
            qDebug() << "Emitted";
            break;
        case EXIT:
            std::cout << "Exiting " << std::endl;
            return;
        default:
            std::cout << "Failed" << std::endl;
            break;
        }
        qDebug() << "Listen Again";
        //rlisten ();
    }
    rlisten ();
}

