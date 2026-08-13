#include "Reader.h"

#include <iostream>
#include <sstream>

using namespace std;

Reader::Reader(istream* input) : m_input(input) {}

Reader::~Reader(){}

bool Reader::isEnd() const {
	return m_input->eof();
}

string Reader::readInput() {
	string line;
	string text;
	while (getline(*m_input, line)) {
		if (!text.empty()) {
			text += "\n";
		}
		text += line;
	}
	//When it encounters EOF, it deletes it so that it does not affect the end of the entire program, but the end of the input argument.
	if (this->isEnd()) { 
		(*m_input).clear();
	}
	return text;
}

string Reader::readLine() const {
	string input;
	getline(cin, input);
	if (input.size() > lineLimit) { //lineLimit const in reader = 512
		input = ignoreRest(input); //if the input is larger than 512 characters, the remaining ones are not taken into account
	}
	
	return input;
}

string Reader::ignoreRest(const string& input) const {
	istringstream stream(input);
	char* list = new char[lineLimit + 1];
	stream.get(list, lineLimit + 1);
	return list;
}

ConsoleReader::ConsoleReader() : Reader(&cin) {}


FileReader::FileReader(const string& filepath) : m_file(new ifstream(filepath)), Reader(m_file) {}

FileReader::~FileReader() {
	((ifstream*)m_input)->close();
	delete m_input;
}
