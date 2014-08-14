// asio.h
//
// written using examples at: 
// http://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp
//

using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;





void handle_data( std::vector<int>& buffer_data );
int charToInt( const char c );

void session( socket_ptr sock );

void server( boost::asio::io_service& io_service, short port );

