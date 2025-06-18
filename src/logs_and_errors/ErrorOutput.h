#ifndef ERROROUTPUT_H
#define ERROROUTPUT_H

#include <string>
#include <exception>
#include "Logger.h"

class ErrorOutput {
public:
    /**
     * @brief Constructor met logger referentie
     * @pre logger moet geldig zijn
     */
    explicit ErrorOutput(Logger& logger);

    /**
     * @brief Verwerkt een foutmelding en logt deze
     * @param message Het foutbericht
     * @param context Optionele context informatie
     * @post error bericht wordt gelogd met LogLevel::ERROR
     */
    void handleError(const std::string& message, const std::string& context = "");

    /**
     * @brief Verwerkt een exception en logt deze
     * @param e De exception
     * @param context Optionele context informatie
     * @post exception wordt gelogd met LogLevel::ERROR
     */
    void handleException(const std::exception& e, const std::string& context = "");

    /**
     * @brief Logt een waarschuwing
     * @param message Het waarschuwingsbericht
     * @param context Optionele context informatie
     * @post bericht wordt gelogd met LogLevel::WARNING
     */
    void handleWarning(const std::string& message, const std::string& context = "");

    /**
     * @brief Logt informatie
     * @param message Het informatiebericht
     * @param context Optionele context informatie
     * @post bericht wordt gelogd met LogLevel::INFO
     */
    void handleInfo(const std::string& message, const std::string& context = "");

    /**
     * @brief Vervanger voor std::cerr usage
     * @param message Het te loggen bericht
     * @post bericht wordt gelogd als ERROR, programma blijft draaien
     */
    void logError(const std::string& message);

private:
    Logger& logger;
    std::string formatMessage(const std::string& message, const std::string& context) const;
};

#endif // ERROROUTPUT_H