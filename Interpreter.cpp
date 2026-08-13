#include "Exceptions.h"
#include "Interpreter.h"

Parser* Interpreter::getParser() const {
    return m_parser;
}

Interpreter* Interpreter::Instance() {
	static Interpreter instance;
	return &instance;
}

void Interpreter::run() { 
    while (true) {
        try {
            cout << m_commandPrompt;
            string line = m_parser->getReader()->readLine();
            executeCommand(line, &cout);
        }
        catch (const ExitCommand&) {
            break;
        }
    }
    return;
}

void Interpreter::executeCommand(string& line, ostream* out) {
	vector<ParsedCommand*> parsedcoms;
    vector<string> tokens;
    vector<Command*> commands;
    int cnt = 0, i = 0;
    try {
        tokens = m_parser->parseLine(line);
		parsedcoms = m_parser->tokenize(tokens);
        for (auto* p : parsedcoms) {
            cnt++;
			Command* com = fabricCommand(p);
			commands.push_back(com);
        }

        for (i = 0; i < cnt; i++) {
            if (i > 0) {
				auto* ss = new stringstream();
				commands[i - 1]->setOutputStream(ss);
				commands[i]->setInputStream(ss);
            }
            if (i== 0 || i == commands.size() - 1) {
                assignStreams(commands[i], parsedcoms[i]);
            }
        }
        for (i = 0; i < cnt; i++) {
            if (commands[i]) {
                if (commands[i]->isExit()) {
					cleanup(commands, parsedcoms);
                    throw ExitCommand();
                }
                
                commands[i]->processCommand();
                
            }
        }
        cleanup(commands, parsedcoms);
    }
    catch (const UnknownCommand& e) {
        *(out) << e.what() << parsedcoms[cnt - 1]->name << endl;
        cleanup(commands, parsedcoms);
    }
    catch (const UnexpectedCharacters& e) {
        *(out) << e.what() << endl;
        cleanup(commands, parsedcoms);
    }
    catch (const ErrorWhileOpeningFile& e) {
        *(out) << e.what() << endl;
        cleanup(commands, parsedcoms);
    }
    catch (const FileDoesNotExist& e) {
        *(out) << e.what() << endl;
        cleanup(commands, parsedcoms);
    }

}

Command* Interpreter::fabricCommand(ParsedCommand* parsed) {
    string name = parsed->name;
    if (name == "time") {
        return new Time();
    }
    else if (name == "date") {
        return new Date();
    }
    else if (name == "echo") {
        return new Echo(parsed->arguments[0]);
    }
    else if (name == "touch") {
        return new Touch(parsed->arguments[0]);
    }
    else if (name == "wc") {
        return new WordCount(parsed->option, parsed->arguments[0]);
    }
    else if (name == "exit") {
        return new Exit();
    }
    else if (name == "prompt") {
        return new Prompt(m_commandPrompt, parsed->arguments[0]);
    }
    else if (name == "truncate") {
        return new Truncate(parsed->arguments[0]);
    }
    else if (name == "rm") {
        return new Remove(parsed->arguments[0]);
        }
    else if (name == "head") {
        return new Head(parsed->option, parsed->arguments[0]);
    }
    else if (name == "batch") {
        return new Batch(parsed->arguments[0]);
    }
    else if (name == "tr") {
        string argument, what, with;

        if (parsed->arguments[0].size() < 7) {
            argument = "";
			what = parsed->arguments[0];
            with = parsed->arguments.size() > 1 ? parsed->arguments[1] : "";
        }
        else {
			argument = parsed->arguments[0];
			what = parsed->arguments[1];
            with = parsed->arguments.size() > 2 ? parsed->arguments[2] : "";
		}

        return new Tr(argument, what, with);
    }
    else {
        throw UnknownCommand();
    }
}

Interpreter::~Interpreter() {
    delete m_parser;
}

void Interpreter::assignStreams(Command* com, ParsedCommand* parsed) const {
    if (com->hasInput() && !parsed->inputFileName.empty()) {
        ifstream* fin = new ifstream(parsed->inputFileName);
        if (fin->is_open()) {
            com->setInputFileStream(fin);
        }
        else {
            delete fin;
            throw ErrorWhileOpeningFile(parsed->inputFileName);
        }
            
    }
    if (com->hasOutput() && !parsed->outputFileName.empty()) {
        ofstream* fout = new ofstream(parsed->outputFileName, parsed->appendMode ? ios::app : ios::trunc);
        if (fout->is_open()) {
            com->setOutputStream(fout);
        }
        else {
			delete fout;
			throw ErrorWhileOpeningFile(parsed->outputFileName);
        }
    }  
}

void Interpreter::cleanup(vector<Command*>& commands, vector<ParsedCommand*>& parsedcoms) const {
	for (auto* cmd : commands) delete cmd;
	for (auto* pc : parsedcoms) delete pc;
}

Interpreter::Interpreter() : m_parser(new Parser()), m_commandPrompt("$ ") {}
