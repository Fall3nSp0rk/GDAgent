// Global variable class
// where global variables for program are kept

#ifndef __GLOBAL_H
#define __GLOBAL_H
class globals {
	public:
		globals();
		std::string g_logfile;
		std::string g_cfg;
		int g_lport; // listen port
		int g_ll; // global logging level variable
	private:
		void readCfg();
};

extern globals glob;
#endif
