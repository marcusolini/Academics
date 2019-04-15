package com.marcusolini.util.logging;

import java.util.logging.Level;
import java.util.logging.Logger;

public interface ILogging {

    // Create Logger - Called only once by main executable
    public void createLogger(String sLogger) throws LoggingException;

    // Add Logging Handler - Called by main executable
    public void addConsoleHandler() throws LoggingException;
    public void addFileHandler(String sFileName, boolean bAppend) throws LoggingException;

    // Logging
    public void info(String msg);
    public void warning(String msg);
    public void severe(String msg);

    // Enable/Disable Logging
    public void setLogOn();
    public void setLogOff();

    // Set Logging Levels
    public void setLogLevelAll();
    public void setLogLevelInfo();
    public void setLogLevelWarning();
    public void setLogLevelSevere();

    public void flush();

    // Configuration File (optional)
    public void setConfiguration(String fileName) throws LoggingException;

    // Get Raw Logger
    public Logger getLogger() throws LoggingException;
}
