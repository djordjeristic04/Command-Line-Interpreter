#ifndef COMMAND_H
#define COMMAND_H

#include "Exceptions.h"

#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct ParsedCommand { //plain data container, mediator between parser and command object
public:
	string name;                    
	string option;                 
	vector<string> arguments;      
	string inputFileName;       // if <input.txt exists 
	string outputFileName;     // if >output.txt or >>output.txt exists
	bool appendMode = false;  // true if >>
	ParsedCommand() = default;

};

class Command {
public:
	Command();
	virtual ~Command();
	virtual void setOutputStream(ostream* newOut);
	virtual void setInputStream(istream* newIn);
	virtual void setInputFileStream(ifstream* newFin);
	virtual void processCommand() = 0;
	virtual bool isExit() const;
	virtual bool hasInput() const;
	virtual bool hasOutput() const;

};

class Touch : public Command {
public:
	Touch(const string& filename);
	void processCommand() override;

protected:
	string m_filename;
};

class Exit : public Command {
public:
	Exit();
	void processCommand() override;
	bool isExit() const override;
};

class Prompt : public Command {
public:
	Prompt(string& prompt, const string& argument);
	void processCommand() override;
private:
	string& currentPrompt;
	string m_prompt;
};

class Truncate : public Command {
public:

	Truncate(const string& filename);
	void processCommand() override;

private:
	string m_filename;
};

class Remove : public Command {
public:

	Remove(const string& filename);
	void processCommand() override;

private:
	string m_filename;
};
#endif
