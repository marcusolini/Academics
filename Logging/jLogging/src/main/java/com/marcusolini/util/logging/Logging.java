package com.marcusolini.util.logging;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.AccessDeniedException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.logging.*;


// TODO: Finish ability to re-configure independently during runtime.

public class Logging implements ILogging, Runnable {

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

    private LoggingManager loggingManager = null;

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
                    LogRecord logRecord = new LogRecord(logMessage.getLevel(), logMessage.getMessage());
                    logRecord.setThreadID(logMessage.getThreadId());
                    logRecord.setSourceMethodName(logMessage.method() + ":" + logMessage.line());
                    logger.log(logRecord);
                }
            }
        } catch (InterruptedException e) {
            if (null != logger) {
                logger.severe(LoggingErrorCode.E_LOGGER_QUEUE_TAKE_INTERRUPTED.toString());
            }
        }
    }

    // Logging

    public void info(String msg) {
        log(Level.INFO, msg);
    }
    public void warning(String msg) {
        log(Level.WARNING, msg);
    }
    public void severe(String msg) {
        log(Level.SEVERE, msg);
    }
    private void log(Level level, String msg) {
        //try {
            if ((null != logger) && (null != msg)) {
                String methodName = "Unknown";
                int lineNumber = 0;
                int threadId = (int)Thread.currentThread().getId();
                StackTraceElement[] stackTraceElements = Thread.currentThread().getStackTrace();
                if (4 <= stackTraceElements.length) {
                    methodName = (stackTraceElements[3].getMethodName()) != null ? stackTraceElements[3].getMethodName() : "Unknown";
                    lineNumber = stackTraceElements[3].getLineNumber();
                }
                LogMessage logMessage = new LogMessage(level, threadId, methodName, lineNumber, msg);

                //queue.put(logMessage);
                if (true == queue.offer(logMessage)) {
                    bQueuePutInterrupted = false;
                } else {
                    // Directly log once when queue put is interrupted.
                    if (false == bQueuePutInterrupted) {
                        bQueuePutInterrupted = true;
                        if (null != logger) {
                            logger.severe(LoggingErrorCode.E_LOGGER_QUEUE_PUT_INTERRUPTED.toString());
                        }
                    }
                }
            }
        //} catch (InterruptedException e) {
        //    // Directly log once when queue put is interrupted.
        //    if (false == bQueuePutInterrupted) {
        //        bQueuePutInterrupted = true;
        //        if (null != logger) {
        //            logger.severe(LoggingErrorCode.E_LOGGER_QUEUE_PUT_INTERRUPTED.toString());
        //        }
        //    }
        //}
    }

    // Add Logging Handler

    // Multiple calls will not be filtered
    public void addConsoleHandler() throws LoggingException {
        if (logger != null) {
            ConsoleHandler handler = new ConsoleHandler();
            LoggingFormatter loggingFormatter = new LoggingFormatter();
            handler.setFormatter(loggingFormatter);
            logger.addHandler(handler);
        }
    }

    // Multiple calls will not be filtered
    public void addFileHandler(String sFileName, boolean bAppend) throws LoggingException {
        try {
            if (logger != null) {
                FileHandler handler = new FileHandler(sFileName, bAppend);
                LoggingFormatter loggingFormatter = new LoggingFormatter();
                handler.setFormatter(loggingFormatter);
                logger.addHandler(handler);
            }
        } catch (IOException e) {
            throw new LoggingException(LoggingErrorCode.E_LOGGER_FILE_HANDLER_CREATION_FAILURE, e);
        }
    }

    // Enable/Disable Logging

    public void setLogOn() { setLogLevel(this.level);  }
    public void setLogOff() {
        if (logger != null) {
            this.level = logger.getLevel();
            logger.setLevel(Level.OFF);
        }
    }

    // Set Logging Levels

    public void setLogLevelAll() { setLogLevel(Level.ALL); }
    public void setLogLevelInfo() { setLogLevel(Level.INFO); }
    public void setLogLevelWarning() { setLogLevel(Level.WARNING); }
    public void setLogLevelSevere() { setLogLevel(Level.SEVERE); }
    private void setLogLevel(Level level) {
        if (logger != null) {
            this.level = level;
            logger.setLevel(this.level);
        }
    }

    public void flush() {
        if (logger != null) {
            Handler[] handlers = logger.getHandlers();
            for(Handler handle : handlers) {
                handle.flush();
            }
        }
    }

    // Configuration File (optional)
    public void setConfiguration(String fileName) throws LoggingException {
        try {
            if (null != fileName) {
                loggingManager = new LoggingManager(fileName);
            }
        } catch (LoggingException e) {
            throw e;
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
