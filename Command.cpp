#include "Command.h"
#include "Reader.h"
#include "Exceptions.h"
#include "Interpreter.h"

Command::Command() {}

Exit::Exit() {}

Touch::Touch(const string& filename) : m_filename(filename) {}

Command::~Command() {}

void Command::setOutputStream(ostream* newOut) {}

void Command::setInputStream(istream* newIn) {}

void Command::setInputFileStream(ifstream* newFin) {}

void Touch::processCommand() {
    try {
        ifstream fileCheck(m_filename);
        if (fileCheck.good()) {
            throw FileAlreadyExists();
        }
        else {
            ofstream file(m_filename);
            if (!file) throw ErrorWhileMakingFile(m_filename);
        }
    }
    catch (const ErrorWhileMakingFile& e) {
        cout << e.what() << endl;
    }
    catch (const FileAlreadyExists& e) {
        cout << e.what() << endl;
    }
}
   

void Exit::processCommand() {}

bool Exit::isExit() const {
    return true;
}

bool Command::isExit() const {
    return false;
}

bool Command::hasInput() const {
    return false;
}

bool Command::hasOutput() const {
    return false;
}

Prompt::Prompt(string& prompt, const string& argument) : currentPrompt(prompt) , m_prompt(argument) {}

void Prompt::processCommand()
{
    if (m_prompt != "") {
        if (m_prompt.front() == '"' && m_prompt.back() == '"') { //Processing if the argument is quoted
            m_prompt = Interpreter::Instance()->getParser()->removeQuotes(m_prompt);
        }
        currentPrompt = m_prompt + " ";
    }
    return;
}

Truncate::Truncate(const string& filename) : m_filename(filename) {}

void Truncate::processCommand() {
    if (m_filename.empty()) {
        return;
    }
    ifstream fileCheck(m_filename);
    if (fileCheck.good()) {
        ofstream file(m_filename, ios::trunc);
        if (!file) {
            throw ErrorWhileOpeningFile(m_filename);
        }
    }
    else throw FileDoesNotExist();
    return;
}


Remove::Remove(const string& filename) : Command(), m_filename(filename) {}

void Remove::processCommand() {
    try {
        if (m_filename.empty()) return;
        ifstream fileCheck(m_filename);
        if (fileCheck.good()) {
            fileCheck.close(); // Close file before deleting
            if (remove(m_filename.c_str()) == 0); // Try to remove the file
            else throw ErrorWhileRemovingFile(m_filename);
        }
        else throw FileDoesNotExist();
    }
    catch (const ErrorWhileRemovingFile& e) {
        cout << e.what() << endl;
    }
}
