/*
 * boostblue.h
 *
 *  Created on: 30/05/2014
 *      Author: Jonathan
 */

#ifndef BOOSTBLUE_H_
#define BOOSTBLUE_H_

#include <winsock2.h>
#include <boost/asio.hpp> // include boost
#include <boost/ref.hpp>
using namespace::boost::asio;  // save tons of typing
#include <iostream>

class boostblue
{
public:
    /**
     * Constructor.
     * \param port device name, example "/dev/ttyUSB0" or "COM4"
     * \param baud_rate communication speed, example 9600 or 115200
     * \throws boost::system::system_error if cannot open the
     * serial device
     */
    boostblue(std::string port, unsigned int baud_rate)
    : io(), serial(io,port)
    {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }

    /**
     * Write a string to the serial device.
     * \param s string to write
     * \throws boost::system::system_error on failure
     */
    void writeString(std::string s)
    {
        boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
    }

private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
};



#endif /* BOOSTBLUE_H_ */
