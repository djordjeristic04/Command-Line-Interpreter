# Command Line Interpreter

## Project Description

This repository contains a custom command-line interpreter (CLI) implemented in C++, developed as a coursework project for the Object-Oriented Programming course at the School of Electrical Engineering, University of Belgrade (academic year 2024/25).

The interpreter reads commands from standard input (or from a batch file) and executes them one by one, supporting argument parsing, quoted string arguments, input/output redirection, pipes between commands, and custom error reporting with precise error-location markers.

## Key Features

- **Custom Command Set:** Implements `echo`, `prompt`, `time`, `date`, `touch`, `truncate`, `rm`, `wc`, `tr`, `head`, and `batch`, each with its own argument and option handling.
- **I/O Redirection & Pipes:** Supports input redirection (`<`), output redirection and append (`>`, `>>`), and chaining multiple commands together with pipes (`|`).
- **Batch Execution:** The `batch` command reads and executes a sequence of command lines from a file, with independent error handling per line.
- **Lexical & Syntax Validation:** Detects invalid characters and malformed command lines, reporting the exact position of the error to the user.
- **Custom Exception Hierarchy:** A dedicated set of exception classes (`FileDoesNotExist`, `UnknownCommand`, `ErrorWhileOpeningFile`, etc.) is used for structured, descriptive error handling.

## Design & Architecture

The project is built around a clear object-oriented class hierarchy:

- **`Command`** — abstract base class for all commands, defining the common interface (`processCommand()`, stream setters).
- **`WriteCommand : Command`** — for commands that only produce output (e.g., `Time`, `Date`).
- **`ReadWriteCommand : WriteCommand`** — for commands that consume an input stream and produce output (e.g., `Echo`, `WordCount`, `Tr`, `Head`, `Batch`).
- **`Reader` / `ConsoleReader` / `FileReader`** — abstracts line-by-line input, whether from the console or a file.
- **`Parser`** — tokenizes raw input lines into structured `ParsedCommand` objects, handling quoted arguments and redirection/pipe syntax.
- **`Interpreter`** — a singleton that drives the main read-parse-execute loop, using a factory method (`fabricCommand`) to instantiate the correct `Command` subclass for each parsed line.

This design keeps command-specific logic isolated in individual classes, while shared behavior (stream handling, output writing) is factored into intermediate base classes, avoiding code duplication across commands with similar I/O needs.

## Project Structure

- `main.cpp` — program entry point.
- `Interpreter.h / .cpp` — main execution loop, command dispatch, and cleanup.
- `Parser.h / .cpp` — lexical analysis and tokenization of command lines.
- `Reader.h / .cpp` — input abstraction (console vs. file).
- `Command.h / .cpp` — base `Command` class and simple no-I/O commands (`Touch`, `Exit`, `Prompt`, `Truncate`, `Remove`).
- `WriteCommand.h / .cpp` — output-only commands (`Time`, `Date`).
- `ReadWriteCommand.h / .cpp` — input/output commands (`Echo`, `WordCount`, `Tr`, `Head`, `Batch`).
- `Exceptions.h` — custom exception classes used throughout the project.

## How to Run

1. Clone the repository.
2. Open the project in your preferred C++ IDE (e.g., Visual Studio) or compile it directly with a C++ compiler that supports C++11 or later, for example:
   ```
   g++ -std=c++17 *.cpp -o interpreter
   ```
3. Run the compiled executable and enter commands interactively at the prompt (`$` by default), or run a batch file with:
   ```
   batch filename.txt
   ```

## Note

This project was developed as an individual coursework assignment. Since the assignment specification may be reused in future course iterations, please do not copy this code for your own submission if you are a student currently working on the same assignment.

## Author

Đorđe Ristić — Undergraduate student, School of Electrical Engineering, University of Belgrade.
