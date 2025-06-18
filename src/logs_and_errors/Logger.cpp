#include "Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

Logger::Logger(const std::string& filename, bool consoleOutput, LogLevel minLevel, bool clearFile)
        : consoleOutput(consoleOutput), minLevel(minLevel) {

    // Zorg ervoor dat de directory bestaat
    std::filesystem::path filePath(filename);
    if (filePath.has_parent_path()) {
        std::filesystem::create_directories(filePath.parent_path());
    }

    // Open in write mode (overschrijven) of append mode
    if (clearFile) {
        logfile.open(filename, std::ios::out); // Overschrijf bestand
    } else {
        logfile.open(filename, std::ios::app); // Append aan bestand
    }

    if (!logfile.is_open()) {
        std::cerr << "KRITIEKE FOUT: Kon logbestand niet openen: " << filename << std::endl;
        std::cerr << "Logger zal alleen naar console schrijven." << std::endl;
    }
}

Logger::~Logger() {
    if (logfile.is_open()) {
        logfile.close();
    }
}

void Logger::log(const std::string& message, LogLevel level) {
    // Check of dit level gelogd moet worden
    if (level < minLevel) {
        return;
    }

    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = getLogLevelString(level);
    std::string fullMessage = "[" + timestamp + "] " + levelStr + " " + message;

    // Schrijf naar bestand als het open is
    if (logfile.is_open()) {
        logfile << fullMessage << std::endl;
        logfile.flush(); // Zorg voor onmiddellijke schrijving
    }

    // Schrijf naar console als ingeschakeld
    if (consoleOutput) {
        if (level == LogLevel::ERROR) {
            std::cerr << fullMessage << std::endl;
        } else {
            std::cout << fullMessage << std::endl;
        }
    }
}

void Logger::flush() {
    if (logfile.is_open()) {
        logfile.flush();
    }
    std::cout.flush();
    std::cerr.flush();
}

bool Logger::isOperational() const {
    return logfile.is_open() || consoleOutput;
}

void Logger::setConsoleOutput(bool enable) {
    consoleOutput = enable;
}

void Logger::setMinLogLevel(LogLevel level) {
    minLevel = level;
}

std::string Logger::getLogLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::INFO: return "[INFO]";
        case LogLevel::WARNING: return "[WARNING]";
        case LogLevel::ERROR: return "[ERROR]";
        default: return "[UNKNOWN]";
    }
}

std::string Logger::getCurrentTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    std::stringstream ss;
    ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}