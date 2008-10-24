#include <fstream>
#include <string>
#include <queue>
#include <list>
#include <fstream>

const char newline = '\n';
const char delim = '-';
const char space = ' ';

class dbwrite{
	private:
        std::queue<std::string, std::list<std::string> > Names4Hash;
		std::ofstream dbFile, idxFile;
        std::fstream hashFile;
        long long Recordlen; // The number of records read in

		void writeTop(char);

		bool open, failbit;

        long long hashFunct(std::string, long long);

	public:
		dbwrite(std::string, char);

		~dbwrite();

		void close();

		bool fail() const {return failbit;}

        bool hash2Disk();

		bool is_open() const {return open;}

		bool writeDelim();

		bool writeFirst(std::string);

		bool writeLine(std::string);
};
