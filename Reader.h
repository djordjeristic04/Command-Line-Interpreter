#ifndef READER_H
#define READER_H

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Reader {
public:
	Reader(istream* input);
	virtual ~Reader();
	virtual bool isEnd() const;
	virtual string readInput();
	virtual string readLine() const;
	virtual string ignoreRest(const string& input) const;

protected:
	istream* m_input;
	const int lineLimit = 512;
};

class ConsoleReader : public Reader {
public:

	ConsoleReader();

};

class FileReader : public Reader {
public:

	FileReader(const string& filepath);
	~FileReader();

private:
	ifstream* m_file;
};
#endif
