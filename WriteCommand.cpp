#include "WriteCommand.h"

WriteCommand::WriteCommand() : m_out(&cout) {}

void WriteCommand::setOutputStream(ostream* newOut) {
    if (m_out != &cout) delete m_out;
    m_out = newOut;
}

WriteCommand::~WriteCommand() {
    if (m_out != &cout) {
        delete m_out;
		m_out = nullptr;
    }
}

bool WriteCommand::hasOutput() const {
    return true;
}

Time::Time() : m_timeFormat(nullptr), m_currTime(time(nullptr)) {
    if (localtime_s(&m_locTime, &m_currTime) == 0) m_timeFormat = new char[20];
}

Time::~Time() {
    delete[] m_timeFormat;
}

void Time::processCommand() {
    strftime(m_timeFormat, 20, "%H:%M:%S", &m_locTime);
    (*m_out) << m_timeFormat << endl;
}

Date::Date() : Time() {}

void Date::processCommand() {
    strftime(m_timeFormat, 20, "%d.%m.%Y.", &m_locTime);
    (*m_out) << m_timeFormat << endl;
}
