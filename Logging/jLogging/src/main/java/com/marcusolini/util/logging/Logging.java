package com.marcusolini.util.logging;

import java.io.IOException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.logging.ConsoleHandler;
import java.util.logging.FileHandler;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Logging implements ILogging, Runnable{

    private static Logging ourInstance = new Logging();

    // Get Logger Instance - Call by all Components.
    public static Logging getInstance() {
        return ourInstance;
    }

    // Create Logger - Called only once by main executable
    public synchronized void createLogger(String sLogger) throws LoggingException {
        if (null == logger) {
            logger = Logger.getLogger(sLogger);
            if (null != logger) {
                logger.setUseParentHandlers(false);
                logger.setLevel(level);
            }
        } else {
            throw new LoggingException(LoggingErrorCode.E_LOGGER_ALREADY_CREATED);
        }
    }

    private Logger logger = null;
    private Level level = null;
    private Thread thread = null;
    private BlockingQueue<LogMessage> queue = null;
    private int queueCapacity = 1000;
    private boolean bQueuePutInterrupted = false;
    private Logging() {
        level = Level.ALL;
        queue = new LinkedBlockingDeque<>(queueCapacity);
        thread = new Thread(this);
        thread.start();
    }

    @Override
    public void run() {
        try {
            // TODO: Consider adding an interrupt
            while(true) {
                LogMessage logMessage = queue.take();
                if ((null != logger) && (null != logMessage)) {
                    logger.log(logMessage.getLevel(), logMessage.getMessage());
                }
            }
        } catch (InterruptedException e) {
            if (null != logger) {
                logger.severe(LoggingErrorCode.E_LOGGER_QUEUE_TAKE_INTERRUPTED.toString());
            }
        }
    }

    // Logging

    public void config(String msg) {
        log(Level.CONFIG, msg);
    }
    public void info(String msg) {
        log(Level.INFO, msg);
    }
    public void warning(String msg) {
        log(Level.WARNING, msg);
    }
    public void severe(String msg) {
        log(Level.SEVERE, msg);
    }
    public void log(Level level, String msg) {
        try {
            if ((null != logger) && (null != msg)) {
                LogMessage logMessage = new LogMessage(level, msg);
                queue.put(logMessage);
                bQueuePutInterrupted = false;
            }
        } catch (InterruptedException e) {
            // Directly log once when queue put is interrupted.
            if (false == bQueuePutInterrupted) {
                bQueuePutInterrupted = true;
                if (null != logger) {
                    logger.severe(LoggingErrorCode.E_LOGGER_QUEUE_PUT_INTERRUPTED.toString());
                }
            }
        }
    }

    // Add Logging Handler

    // TODO: Determine outcome if called more than once.
    public void addConsoleHandler() throws LoggingException {
        if (logger != null) {
            ConsoleHandler consoleHandler = new ConsoleHandler();
            logger.addHandler(consoleHandler);
        }
    }

    // TODO: Determine outcome if called more than once.
    public void addFileHandler(String sFileName, boolean bAppend) throws LoggingException {
        try {
            if (logger != null) {
                FileHandler fileHandler = new FileHandler(sFileName, bAppend);
                logger.addHandler(fileHandler);
            }
        } catch (IOException e) {
            throw new LoggingException(LoggingErrorCode.E_LOGGER_FILE_HANDLER_CREATION_FAILURE, e);
        }
    }

    // Enable/Disable Logging

    public void setLogOff() {
        if (logger != null) {
            this.level = logger.getLevel();
            logger.setLevel(Level.OFF);
        }
    }
    public void setLogOn() {
        setLogLevel(this.level);
    }

    // Set Logging Levels

    public void setLogLevelAll() {
        setLogLevel(Level.ALL);
    }
    public void setLogLevelConfig() {
        setLogLevel(Level.CONFIG);
    }
    public void setLogLevelInfo() {
        setLogLevel(Level.INFO);
    }
    public void setLogLevelWarning() {
        setLogLevel(Level.WARNING);
    }
    public void setLogLevelSevere() {
        setLogLevel(Level.SEVERE);
    }
    private void setLogLevel(Level level) {
        if (logger != null) {
            this.level = level;
            logger.setLevel(this.level);
        }
    }

    // Get Raw Logger
    public Logger getLogger() throws LoggingException {
        if (null == logger) {
            throw new LoggingException(LoggingErrorCode.E_LOGGER_INVALID);
        }
        return logger;
    }
}
