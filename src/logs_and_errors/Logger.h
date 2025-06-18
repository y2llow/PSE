#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <iostream>

class Logger {
public:
    enum class LogLevel { INFO, WARNING, ERROR };

    /**
     * @brief Constructor voor Logger
     * @param filename Pad naar logbestand
     * @param consoleOutput Of output ook naar console moet
     * @param minLevel Minimum log level om te tonen
     * @param clearFile Of het bestand leeg gemaakt moet worden (true) of append (false)
     * @pre `filename` moet een geldig pad naar een bestand zijn.
     * @post logfile is open en klaar om logs te schrijven.
     */
    explicit Logger(const std::string& filename = "logs/output.txt", bool consoleOutput = true, LogLevel minLevel = LogLevel::INFO, bool clearFile = true);

    /**
     * @brief Destructor sluit het logbestand
     */
    ~Logger();

    /**
     * @brief Schrijft een bericht naar het logbestand en optioneel console.
     * @param message Het te loggen bericht
     * @param level Het log niveau
     * @post Bericht is toegevoegd aan het tekstbestand en/of console
     */
    void log(const std::string& message, LogLevel level = LogLevel::INFO);

    /**
     * @brief Forceert het schrijven van alle gebufferde data
     */
    void flush();

    /**
     * @brief Controleert of de logger operationeel is
     */
    bool isOperational() const;

    /**
     * @brief Zet console output aan/uit
     */
    void setConsoleOutput(bool enable);

    /**
     * @brief Zet het minimum log level
     */
    void setMinLogLevel(LogLevel level);

private:
    std::ofstream logfile;
    bool consoleOutput;
    LogLevel minLevel;
    std::string getLogLevelString(LogLevel level) const;
    std::string getCurrentTimestamp() const;
};

#endif // LOGGER_H