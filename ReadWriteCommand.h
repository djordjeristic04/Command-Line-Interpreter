#ifndef READWRITECOMMAND_H
#define READWRITECOMMAND_H

#include "Reader.h"
#include "WriteCommand.h"

class ReadWriteCommand : public WriteCommand { //Class with both input and output streams
public:
	ReadWriteCommand(const string& argument);
	void setInputStream(istream* newIn) override;
	void setInputFileStream(ifstream* newFin) override;
	virtual ~ReadWriteCommand();
	bool hasInput() const override;
	void setInputStreamFromArgument(const string& argument);
protected:
	istream* m_in;
	ifstream* m_inputFile;
	istringstream* m_inputString;
	Reader* m_reader;
};

class Echo : public ReadWriteCommand {
public:

	Echo(const string& argument);
	void processCommand() override;

};

class WordCount : public ReadWriteCommand {
public:
	WordCount(const string& option, const string& argument);

	void processCommand() override;
	void charCount(const string& text);
	void wordCount(const string& text);
	int getNumOfWords() const;
	int getNumOfChars() const;

private:

	string m_option;
	int m_numOfWords;
	int m_numOfChars;

};

class Tr : public ReadWriteCommand {
public:
	Tr(const string& argument, const string& what, const string& with);
	void processCommand() override;

private:
	string m_what;
	string m_with;
};

class Head : public ReadWriteCommand {
public:

	Head(string& option, const string& argument);
	void processCommand() override;
private:

	int parseOption(string& option) const;
	int maxNumOfLines;
};

class Batch : public ReadWriteCommand {
public:
	Batch(const string& argument);
	void processCommand() override;
private:
};
#endif // READWRITECOMMAND_H

