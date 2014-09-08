// tpool class
// It's always closed. :glasses:

#ifndef __POOL_H
#define __POOL_H 
class tpool {
	public:
		tpool( std::size_t tpool_size );
		~tpool();
		template< typename Task >
		void run_task( Task task ){
			boost::unique_lock< boost::mutex > lock( mutex_ );
			if( 0 < available_ ) {
				--available_;
				io_service_.post( boost::bind( &tpool::wrap_task, this, boost::function< void() > ( task ) ) );
			}
		}
	private:
		boost::asio::io_service io_service_;
		boost::asio::io_service::work work_;
		boost::thread_group threads_;
		std::size_t available_;
		boost::mutex mutex_;
		void wrap_task( boost::function< void() > task );
};
extern tpool dbpool;
#endif
