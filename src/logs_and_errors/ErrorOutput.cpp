#include "ErrorOutput.h"

ErrorOutput::ErrorOutput(Logger& logger)
        : logger(logger) {}

void ErrorOutput::handleError(const std::string& message, const std::string& context) {
    std::string formattedMessage = formatMessage("FOUT: " + message, context);
    logger.log(formattedMessage, Logger::LogLevel::ERROR);
}

void ErrorOutput::handleException(const std::exception& e, const std::string& context) {
    std::string formattedMessage = formatMessage("EXCEPTION: " + std::string(e.what()), context);
    logger.log(formattedMessage, Logger::LogLevel::ERROR);
}

void ErrorOutput::handleWarning(const std::string& message, const std::string& context) {
    std::string formattedMessage = formatMessage("WAARSCHUWING: " + message, context);
    logger.log(formattedMessage, Logger::LogLevel::WARNING);
}

void ErrorOutput::handleInfo(const std::string& message, const std::string& context) {
    std::string formattedMessage = formatMessage(message, context);
    logger.log(formattedMessage, Logger::LogLevel::INFO);
}

void ErrorOutput::logError(const std::string& message) {
    // Direct vervanger voor std::cerr - geen "FOUT:" prefix
    logger.log(message, Logger::LogLevel::ERROR);
}

std::string ErrorOutput::formatMessage(const std::string& message, const std::string& context) const {
    if (context.empty()) {
        return message;
    }
    return "[" + context + "] " + message;
}