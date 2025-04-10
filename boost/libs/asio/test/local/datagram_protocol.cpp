//
// datagram_protocol.cpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include <boost/asio/local/datagram_protocol.hpp>

#include <cstring>
#include <boost/asio/io_context.hpp>
#include "../unit_test.hpp"

//------------------------------------------------------------------------------

// local_datagram_protocol_socket_compile test
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The following test checks that all public member functions on the class
// local::datagram_socket::socket compile and link correctly. Runtime failures
// are ignored.

namespace local_datagram_protocol_socket_compile {

void connect_handler(const boost::system::error_code&)
{
}

void send_handler(const boost::system::error_code&, std::size_t)
{
}

void receive_handler(const boost::system::error_code&, std::size_t)
{
}

void test()
{
#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
  using namespace boost::asio;
  namespace local = boost::asio::local;
  typedef local::datagram_protocol dp;

  try
  {
    io_context ioc;
    const io_context::executor_type ioc_ex = ioc.get_executor();
    char mutable_char_buffer[128] = "";
    const char const_char_buffer[128] = "";
    socket_base::message_flags in_flags = 0;
    socket_base::send_buffer_size socket_option;
    socket_base::bytes_readable io_control_command;
    boost::system::error_code ec;

    // basic_datagram_socket constructors.

    dp::socket socket1(ioc);
    dp::socket socket2(ioc, dp());
    dp::socket socket3(ioc, dp::endpoint(""));
    int native_socket1 = ::socket(AF_UNIX, SOCK_DGRAM, 0);
    dp::socket socket4(ioc, dp(), native_socket1);

    dp::socket socket5(ioc_ex);
    dp::socket socket6(ioc_ex, dp());
    dp::socket socket7(ioc_ex, dp::endpoint(""));
    int native_socket2 = ::socket(AF_UNIX, SOCK_DGRAM, 0);
    dp::socket socket8(ioc_ex, dp(), native_socket2);

    // I/O object functions.

    dp::socket::executor_type ex = socket1.get_executor();
    (void)ex;

    // basic_socket functions.

    dp::socket::lowest_layer_type& lowest_layer = socket1.lowest_layer();
    (void)lowest_layer;

    socket1.open(dp());
    socket1.open(dp(), ec);

    int native_socket3 = ::socket(AF_UNIX, SOCK_DGRAM, 0);
    socket1.assign(dp(), native_socket3);
    int native_socket4 = ::socket(AF_UNIX, SOCK_DGRAM, 0);
    socket1.assign(dp(), native_socket4, ec);

    bool is_open = socket1.is_open();
    (void)is_open;

    socket1.close();
    socket1.close(ec);

    dp::socket::native_handle_type native_socket5 = socket1.native_handle();
    (void)native_socket5;

    socket1.cancel();
    socket1.cancel(ec);

    bool at_mark1 = socket1.at_mark();
    (void)at_mark1;
    bool at_mark2 = socket1.at_mark(ec);
    (void)at_mark2;

    std::size_t available1 = socket1.available();
    (void)available1;
    std::size_t available2 = socket1.available(ec);
    (void)available2;

    socket1.bind(dp::endpoint(""));
    socket1.bind(dp::endpoint(""), ec);

    socket1.connect(dp::endpoint(""));
    socket1.connect(dp::endpoint(""), ec);

    socket1.async_connect(dp::endpoint(""), connect_handler);

    socket1.set_option(socket_option);
    socket1.set_option(socket_option, ec);

    socket1.get_option(socket_option);
    socket1.get_option(socket_option, ec);

    socket1.io_control(io_control_command);
    socket1.io_control(io_control_command, ec);

    dp::endpoint endpoint1 = socket1.local_endpoint();
    (void)endpoint1;
    dp::endpoint endpoint2 = socket1.local_endpoint(ec);
    (void)endpoint2;

    dp::endpoint endpoint3 = socket1.remote_endpoint();
    (void)endpoint3;
    dp::endpoint endpoint4 = socket1.remote_endpoint(ec);
    (void)endpoint4;

    socket1.shutdown(socket_base::shutdown_both);
    socket1.shutdown(socket_base::shutdown_both, ec);

    // basic_datagram_socket functions.

    socket1.send(buffer(mutable_char_buffer));
    socket1.send(buffer(const_char_buffer));
    socket1.send(null_buffers());
    socket1.send(buffer(mutable_char_buffer), in_flags);
    socket1.send(buffer(const_char_buffer), in_flags);
    socket1.send(null_buffers(), in_flags);
    socket1.send(buffer(mutable_char_buffer), in_flags, ec);
    socket1.send(buffer(const_char_buffer), in_flags, ec);
    socket1.send(null_buffers(), in_flags, ec);

    socket1.async_send(buffer(mutable_char_buffer), send_handler);
    socket1.async_send(buffer(const_char_buffer), send_handler);
    socket1.async_send(null_buffers(), send_handler);
    socket1.async_send(buffer(mutable_char_buffer), in_flags, send_handler);
    socket1.async_send(buffer(const_char_buffer), in_flags, send_handler);
    socket1.async_send(null_buffers(), in_flags, send_handler);

    socket1.send_to(buffer(mutable_char_buffer),
        dp::endpoint(""));
    socket1.send_to(buffer(const_char_buffer),
        dp::endpoint(""));
    socket1.send_to(null_buffers(),
        dp::endpoint(""));
    socket1.send_to(buffer(mutable_char_buffer),
        dp::endpoint(""), in_flags);
    socket1.send_to(buffer(const_char_buffer),
        dp::endpoint(""), in_flags);
    socket1.send_to(null_buffers(),
        dp::endpoint(""), in_flags);
    socket1.send_to(buffer(mutable_char_buffer),
        dp::endpoint(""), in_flags, ec);
    socket1.send_to(buffer(const_char_buffer),
        dp::endpoint(""), in_flags, ec);
    socket1.send_to(null_buffers(),
        dp::endpoint(""), in_flags, ec);

    socket1.async_send_to(buffer(mutable_char_buffer),
        dp::endpoint(""), send_handler);
    socket1.async_send_to(buffer(const_char_buffer),
        dp::endpoint(""), send_handler);
    socket1.async_send_to(null_buffers(),
        dp::endpoint(""), send_handler);
    socket1.async_send_to(buffer(mutable_char_buffer),
        dp::endpoint(""), in_flags, send_handler);
    socket1.async_send_to(buffer(const_char_buffer),
        dp::endpoint(""), in_flags, send_handler);
    socket1.async_send_to(null_buffers(),
        dp::endpoint(""), in_flags, send_handler);

    socket1.receive(buffer(mutable_char_buffer));
    socket1.receive(null_buffers());
    socket1.receive(buffer(mutable_char_buffer), in_flags);
    socket1.receive(null_buffers(), in_flags);
    socket1.receive(buffer(mutable_char_buffer), in_flags, ec);
    socket1.receive(null_buffers(), in_flags, ec);

    socket1.async_receive(buffer(mutable_char_buffer), receive_handler);
    socket1.async_receive(null_buffers(), receive_handler);
    socket1.async_receive(buffer(mutable_char_buffer), in_flags,
        receive_handler);
    socket1.async_receive(null_buffers(), in_flags, receive_handler);

    dp::endpoint endpoint;
    socket1.receive_from(buffer(mutable_char_buffer), endpoint);
    socket1.receive_from(null_buffers(), endpoint);
    socket1.receive_from(buffer(mutable_char_buffer), endpoint, in_flags);
    socket1.receive_from(null_buffers(), endpoint, in_flags);
    socket1.receive_from(buffer(mutable_char_buffer), endpoint, in_flags, ec);
    socket1.receive_from(null_buffers(), endpoint, in_flags, ec);

    socket1.async_receive_from(buffer(mutable_char_buffer),
        endpoint, receive_handler);
    socket1.async_receive_from(null_buffers(),
        endpoint, receive_handler);
    socket1.async_receive_from(buffer(mutable_char_buffer),
        endpoint, in_flags, receive_handler);
    socket1.async_receive_from(null_buffers(),
        endpoint, in_flags, receive_handler);
  }
  catch (std::exception&)
  {
  }
#endif // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
}

} // namespace local_datagram_protocol_socket_compile

//------------------------------------------------------------------------------

BOOST_ASIO_TEST_SUITE
(
  "local/datagram_protocol",
  BOOST_ASIO_COMPILE_TEST_CASE(local_datagram_protocol_socket_compile::test)
)
