#ifndef WRITECOMMAND_H
#define WRITECOMMAND_H

#include "Command.h"

class WriteCommand : public Command { //A class that has no input but has an output stream
public:
	WriteCommand();
	void setOutputStream(ostream* newOut) override;
	virtual ~WriteCommand();
	bool hasOutput() const override;
protected:
	ostream* m_out;

};

class Time : public WriteCommand {
public:
	Time();
	virtual ~Time();
	void processCommand() override;

protected:
	const time_t m_currTime;
	tm m_locTime{};
	char* m_timeFormat;
};

class Date : public Time {
public:
	Date();
	void processCommand() override;
};

#endif