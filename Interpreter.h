#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"
#include "ReadWriteCommand.h"

#include <ctime>
#include <iostream>
#include <string>

using namespace std;


class Interpreter {

public:
	Parser* getParser() const;
	static Interpreter* Instance();
	void run();
	void executeCommand(string& line, ostream* out);
	Command* fabricCommand(ParsedCommand* parsed);
	void assignStreams(Command* com, ParsedCommand* parsed) const;
	void cleanup(vector<Command*>& commands, vector<ParsedCommand*>& parsedcoms) const; // Cleans up dynamically allocated commands and parsed commands.

	//Disables copy and move operations
	Interpreter(const Interpreter&) = delete;
	Interpreter& operator=(const Interpreter&) = delete;
	Interpreter(Interpreter&&) = delete;
	Interpreter& operator=(Interpreter&&) = delete;

protected:
	Interpreter();
	~Interpreter();
private:
	Parser* m_parser;
	string m_commandPrompt;
};
#endif // !INTERPRETER_H
