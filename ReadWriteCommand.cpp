#include "Interpreter.h"
#include "ReadWriteCommand.h"

ReadWriteCommand::ReadWriteCommand(const string& argument) : m_in(nullptr), m_reader(nullptr),
    m_inputFile(nullptr), m_inputString(nullptr) {
    setInputStreamFromArgument(argument);
}

void ReadWriteCommand::setInputStream(istream* newIn) {
    if (m_in == m_inputFile && m_inputFile) {
        if (m_inputFile->is_open()) m_inputFile->close();
        delete m_inputFile;
        m_inputFile = nullptr;
    }
    if (m_in == m_inputString && m_inputString) {
        delete m_inputString;
        m_inputString = nullptr;
    }
	delete m_reader;
    m_in = newIn;
    m_reader = new Reader(m_in);
}

void ReadWriteCommand::setInputFileStream(ifstream* newFin)
{
    try {
        if (m_in == &cin) {
            m_inputFile = newFin;
            m_in = m_inputFile;
            delete m_reader;
            m_reader = new Reader(m_in);
        }
        else throw ErrorWhileRedirecting();
	}
    catch (const ErrorWhileRedirecting& e) {
        cout << e.what() << endl;
    }

}

ReadWriteCommand::~ReadWriteCommand() {
    if (m_inputFile) {
        if (m_inputFile->is_open()) m_inputFile->close();
        delete m_inputFile;
        m_inputFile = nullptr;
    }
    if (m_inputString) {
        delete m_inputString;
        m_inputString = nullptr;
    }
    delete m_reader;
}

bool ReadWriteCommand::hasInput() const {
    return true;
}

void ReadWriteCommand::setInputStreamFromArgument(const string& argument)
{
    string content = "";
    if (argument.empty()) {
        m_in = &cin;
        m_reader = new ConsoleReader();
        return;
    }
    if (argument.front() == '"' && argument.back() == '"') {
        content = Interpreter::Instance()->getParser()->removeQuotes(argument);
    }
    if (argument.front() == '"' && argument.back() == '"' && content.back() != '"') {
        m_inputString = new istringstream(content);
		m_in = m_inputString;
    }
    else {
        m_inputFile = new ifstream(argument);
        if (m_inputFile->is_open()) {
            m_in = m_inputFile;
        }
        else {
            m_reader = nullptr;
            throw FileDoesNotExist();
        }
    }
    m_reader = new Reader(m_in);
}

Echo::Echo(const string& argument) : ReadWriteCommand(argument) {}

WordCount::WordCount(const string& option, const string& argument) : ReadWriteCommand(argument), 
    m_option(option), m_numOfChars(0), m_numOfWords(0) {}

void WordCount::processCommand() {
    if (!m_reader) return;
    string text = m_reader->readInput();
    try {
        if (m_option == "-w") {
            wordCount(text);
            (*m_out) << m_numOfWords << endl;
            return;
        }
        else if (m_option == "-c") {
            charCount(text);
            (*m_out) << m_numOfChars << endl;
            return;
        }
        else throw OptionDoesNotExist();
    }
    catch (const OptionDoesNotExist& e) {
        cout << e.what() << endl;
    }

}

void WordCount::charCount(const string& text) {
    for (char c : text) {
        m_numOfChars++;
    }
    return;
}

void WordCount::wordCount(const string& text) {
    istringstream stream(text);
    string word;
    while (stream >> word) {
        m_numOfWords++;
    }
    return;
}

int WordCount::getNumOfWords() const {
    return m_numOfWords;
}

int WordCount::getNumOfChars() const {
    return m_numOfChars;
}

void Echo::processCommand() {
    string text = m_reader->readInput();
    (*m_out) << text << endl;
    return;
}

Head::Head(string& option, const string& argument) : ReadWriteCommand(argument) {
    maxNumOfLines = parseOption(option);
}

void Head::processCommand() {
    string text = m_reader->readInput();
    istringstream stream(text);
    int cnt = 0;
    string line;
    while (cnt < maxNumOfLines && getline(stream, line)) {
        (*m_out) << line << endl;
        cnt++;
    }

}

int Head::parseOption(string& option) const {
    int number = 0;
    try {
        if (option.rfind("-n", 0) == 0) {
            option = option.substr(2);
            for (char c : option) {
                number = number * 10 + (c - '0');
            }
		}
        else throw OptionDoesNotExist();
	}
	catch (const OptionDoesNotExist& e) {
		cout << e.what() << endl;
    }
    return number;
}

Batch::Batch(const string& argument) : ReadWriteCommand(argument) {}

void Batch::processCommand() {
    string text = m_reader->readInput();
    istringstream stream(text);
    string line;
    while (getline(stream, line)) Interpreter::Instance()->executeCommand(line, m_out);

}

Tr::Tr(const string& argument, const string& what, const string& with) : ReadWriteCommand(argument), m_what(what), m_with(with) {}

void Tr::processCommand() {

    if (!m_what.empty()) {
        m_what = Interpreter::Instance()->getParser()->removeQuotes(m_what);
    }
    if (!m_with.empty()) {
        m_with = Interpreter::Instance()->getParser()->removeQuotes(m_with);
    }
    size_t pos = 0;
	string text = m_reader->readInput();
    while ((pos = text.find(m_what, pos)) != string::npos) {
        text.replace(pos, m_what.length(), m_with);
        pos += m_with.length();
    }
    (*m_out) << text << endl;
}
