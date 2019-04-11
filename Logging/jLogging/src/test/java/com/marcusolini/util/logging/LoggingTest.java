package com.marcusolini.util.logging;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

/**
 * Unit test for simple App.
 */
public class LoggingTest
{
    @Test
    public void basicLoggingTest()
    {
        ILogging logging = null;
        final String LOGGERNAME = "com.marcusolini.util.logging";
        final String LOGFILE = "loggingtest.log";
        boolean bAppend = true;

        try {
            logging = Logging.getInstance();
            logging.createLogger(LOGGERNAME);
            logging.setLogLevelAll();
            logging.addConsoleHandler();
            logging.addFileHandler(LOGFILE, bAppend);
        } catch (LoggingException e) {
            logging.severe("Exception" + e.getMessage());
        }

        logging.info( "Logging test begin...");
        logging.info( "...Logging test end.");

    }
}
