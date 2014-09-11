#include <boost/asio/io_service.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "pool.h"

tpool::tpool( std::size_t tpool_size ) : work_( io_service_ ), available_( tpool_size ) {
	for ( std::size_t i = 0; i < tpool_size; ++i ){
		threads_.create_thread( boost::bind( &boost::asio::io_service::run, &io_service_ ) );
	}
}

tpool::~tpool() {
	io_service_.stop();
	try {
		threads_.join_all();
	}
	catch( ... ) {}
}
/* void tpool::run_task( Task task ) {
	boost::unique_lock< boost::mutex > lock( mutex_ );
	if( 0 < available_ ) {
		--available_;
		io_service_.post( boost::bind( &tpool::wrap_task, this, boost::function< void() > ( task ) ) );
	}
} */

void tpool::wrap_task( boost::function< void() > task ) {
	// run the supplied task
	try {
		task();
	} // suppress exceptions
	catch( ... ) {
	}
	boost::unique_lock< boost::mutex > lock( mutex_ );
	++available_;
}
tpool dbpool( 50 );
