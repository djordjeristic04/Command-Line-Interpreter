#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <iostream>

using namespace std;

class FileDoesNotExist : public exception {
public:
	FileDoesNotExist() : exception("Error. File doesn't exist.") {}
};

class UnknownCommand : public exception {
public:
	UnknownCommand() : exception("Unknown command: ") {}
};

class FileAlreadyExists : public exception {
public:
	FileAlreadyExists() : exception("Error. File already exists.") {}
};

class ErrorWhileMakingFile : public exception {
public:
	ErrorWhileMakingFile(const string& filename) : exception(("Error while making file: " + filename).c_str()) {}

};
class UnexpectedCharacters : public exception {
public:
	UnexpectedCharacters(const string& input, const string& markerLine) : exception("") {
		cout << "Error - unexpected characters:" << endl << input << endl << markerLine;
	};
};

class OptionDoesNotExist : public exception {
public:
	OptionDoesNotExist() : exception("Error. Option doesn't exist.") {}
};


class ExitCommand : public exception {
public:
	ExitCommand() {};
};

class ErrorWhileOpeningFile : public exception {
public:
	ErrorWhileOpeningFile(const string& filename) : exception(("Error while opening file: " + filename).c_str()) {}
};

class ErrorWhileRemovingFile : public exception {
public:
	ErrorWhileRemovingFile(const string& filename) : exception(("Error while removing file: " + filename).c_str()) {}
};

class ErrorWhileRedirecting : public exception {
public:
	ErrorWhileRedirecting() : exception("Error. Cannot be redirected.") {}
};
#endif