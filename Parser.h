#ifndef PARSER_H
#define PARSER_H

#include "Command.h"
#include "Reader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Interpreter;
using namespace std;

class Parser {
public:
	
	Reader* getReader() const;
	vector<string> parseLine(const string& line) const;
	string removeQuotes(string str) const;
	vector<ParsedCommand*> tokenize(const vector<string>& tokens) const;
	void reportUnexpectedCharacters(const string& input) const;
	friend class Interpreter;

protected:
	Parser();
	~Parser();
private:
	
	Reader* m_reader;
};

#endif