#include "Command.h"
#include "Exceptions.h"
#include "Parser.h"

#include <fstream>
#include <set>

Parser::Parser() :  m_reader(new ConsoleReader()) {}

Parser::~Parser() {
    delete m_reader;
}

Reader* Parser::getReader() const {
    return m_reader;
}

vector<string> Parser::parseLine(const string& line) const {

    string token;
    vector<string> tokens;
    reportUnexpectedCharacters(line);
    bool insideQuotes = false; //Initially, as we go through the line, we are not in quotes.

    for (int i = 0; i < line.size(); i++) {
        char c = line[i];
        if (c == '"') {
            insideQuotes = !insideQuotes; //If a quotation mark is encountered, the state is changed.
            if (insideQuotes || !token.empty()) {
                token += c;
            }
        }
        else if (c == ' ' && !insideQuotes) { //True when we encounter blank space outside of quotation marks.
            if (!token.empty()) {
                tokens.push_back(token); //If the token is not empty, it inserts it into the vector.
                token.clear();
            }
        }
        else if (c == '|' && !insideQuotes) {
            tokens.push_back(token);
            tokens.push_back("|");
            token.clear();
        }
        else {
            token += c;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }
    while (tokens.size() < 3) {
        tokens.push_back("");
    }
    
    return tokens;

}


string Parser::removeQuotes(string str) const {
    int count = 0;
    for (size_t i = 0; i < str.size() && count < 2; ++i) {
        if (str[i] == '"') {
            str.erase(i, 1);
            i--;
            count++;
        }
    }
    return str;
}

vector<ParsedCommand*> Parser::tokenize(const vector<string>& tokens) const {
    vector<ParsedCommand*> commands;
    ParsedCommand* current = new ParsedCommand();
	current->name = tokens[0]; // first token is the command name

    for (size_t i = 1; i < tokens.size(); i++) {
        string token = tokens[i];
        if (token == "|") {
			commands.push_back(current);
			current = new ParsedCommand(); // create a new command for the next part of the pipeline
            current->name = tokens[++i];
			continue; // skip the pipe token
        }
        // >> filename
        if (token == ">>" && i + 1 < tokens.size()) {
            current->appendMode = true; // append = true;
            current->outputFileName = tokens[++i];
        }
        else if (token.rfind(">>", 0) == 0) {
            current->appendMode = true; // append = true;
            current->outputFileName = token.substr(2);
        }
        // > filename
        else if (token == ">" && i + 1 < tokens.size()) {
            current->appendMode = false; // append = false;
            current->outputFileName = tokens[++i];
        }
        // >filename
        else if (token[0] == '>') {
            current->appendMode = false; // append = false;
            current->outputFileName = token.substr(1);
        }
        // < filename
        else if (token == "<" && i + 1 < tokens.size()) {
            current->inputFileName = tokens[++i];
        }
        // <filename
        else if (token[0] == '<' && token.size() > 1) {
            current->inputFileName = token.substr(1);
        }
        else if (token[0] == '-') {
            current->option = token;
        }
        else {
            current->arguments.push_back(token);
        }
    }
	if (current->arguments.size() == 0) current->arguments.push_back("");
    commands.push_back(current);
    return commands;
}

void Parser::reportUnexpectedCharacters(const string& input) const {
    const set<char> invalidChars = { '&', '*', '+', '?', '#', '@', '!' }; //set of invalid chars
	bool insideQuotes = false;
    string markerLine(input.size(), ' ');
    int count = 0;
    for (int i = 0; i < input.size(); i++) {
		char c = input[i];
		if (c == '"') {
			insideQuotes = !insideQuotes;
		}
        if (invalidChars.count(c) && !insideQuotes) {
            markerLine[i] = '^';
            count++;

        }

    }
    if (!count) return;
    throw UnexpectedCharacters(input, markerLine);
}
