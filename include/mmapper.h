#ifndef __MMAPPER_H
#define __MMAPPER_H
typedef std::pair<int, int> TPCodeUidPair;
typedef std::pair<int, int> RgCodeUidPair;
typedef std::pair<int, int> DLpair;

class mmapper {
	public:
		mmapper();
		std::string getKeyFromMap( const int &key1, const int &key2, const int &mid );
		static std::map<RgCodeUidPair, std::string> siteMap;
		static std::map<TPCodeUidPair, std::string> cachedData;
		static std::map<DLpair, std::string> Ddlabel;
	private:
		boost::mutex mapmute_;
		void createTypeMap();
		void createSiteMap();
		void createDriveLetterMap();
};
extern mmapper mapp;
#endif
