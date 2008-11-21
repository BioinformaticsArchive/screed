#include "dbwrite.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]){
	fstream theFile;
	char a;
	string line, name, desc, dna;
	long long filepos;
    int multiplier;

    multiplier = 2; // Determines how many big hash file will be to index file
    if(argc == 3){
        multiplier = atol(argv[2]);
        if(multiplier <= 0){
            cerr << "ERROR: CAN'T CREATE DB OF 0 OR NEGATIVE HASH LENGTH\n";
            exit(1);
        }
    }
	dbwrite db(argv[1], 'a', multiplier);
	if(!db.is_open()){
		cerr << "ERROR: DATABASE FILES ARE NOT OPEN\n";
		exit(1);
	}
	theFile.open(argv[1], fstream::in);
	while(!theFile.eof()){

		//Increment the file position one to skip the '>' in front of
		//the name
		filepos = theFile.tellg();
		theFile.seekg(filepos+1);
		theFile >> name >> ws; // Pull in the line and skip whitespace

		theFile >> desc;
		theFile.get(a);
		while(a != '\n'){ // Appends the rest of description
			desc.push_back(a);
			theFile.get(a);

			//This makes sure that the file being read in is still
			//valid
			if(!theFile.good()){
				cerr << "ERROR: The stream is corrupt. " <<
					"Did you open the correct file?\n" <<
					"Check to make sure the file isn't " <<
					"empty or an invalid fasta file\n";
				db.close();
				exit(1);
			}
		}

		dna.clear();
		a = '0';
		while((a != '>') && (!theFile.eof())){
			// Another file-validity check
			if(!theFile.good()){
				cerr << "ERROR: The stream is corrupt. " <<
					"Did you open the correct file?\n" <<
					"Check to make sure the file isn't " <<
					"empty or an invalid fasta file\n";
				db.close();
				exit(1);
			}
			theFile >> line >> ws;
			dna.append(line);

			a = theFile.peek();
		}

		db.writeFirst(name);
		db.writeLine(name);
		db.writeLine(desc);
		db.writeLine(dna);
		if(db.fail() == true){
			cerr << "ERROR: One of the database file streams is "<<
				"corrupt. Do you have enough diskspace?\n";
			db.close();
			exit(1);
		}
	}
	theFile.close();
    db.hash2Disk();
	cout << "Database saved in " << argv[1] << "_seqdb2\n";
	cout << "Index saved in " << argv[1] << "_seqdb2_idx\n";
	cout << "Hash saved in " << argv[1] << "_seqdb2_hash\n";

	return 0;
}