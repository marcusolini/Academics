package com.marcusolini.util.logging;

import static org.junit.Assert.assertTrue;

import org.junit.Before;
import org.junit.Test;

/**
 * Unit test for simple App.
 */
public class LoggingTest
{
    protected static String loggerName = null;
    protected static String logfileName = null;

    @Before
    public void setUp() {
    }

    @Test
    public void basicLoggingTest()
    {
        ILogging logging = null;
        boolean bAppend = false;
        boolean bUsingConfigFile = true;

        try {
            loggerName = new Object(){}.getClass().getEnclosingMethod().getName();
            logfileName = loggerName + ".log";

            logging = Logging.getInstance();
            logging.createLogger(loggerName);

            try {
                logging.setConfiguration("loggingtest.properties");
                bUsingConfigFile = true;
            } catch (LoggingException e) {
                bUsingConfigFile = false;
                logging.severe("Exception" + e.getMessage());
            }

            if (false == bUsingConfigFile) {
                try { logging.addConsoleHandler(); }
                catch (LoggingException e) { logging.severe("Exception" + e.getMessage()); }

                try { logging.addFileHandler(logfileName, bAppend); }
                catch (LoggingException e) { logging.severe("Exception" + e.getMessage()); }

                logging.setLogLevelAll();
            }
        } catch (LoggingException e) {
            logging.severe("Exception" + e.getMessage() );
        }

        logging.setLogLevelAll();
        logging.info( "Starting Tests...");
        logging.info( "LogLevel-All: INFO message IS displayed.");
        logging.warning( "LogLevel-All: WARNING message IS displayed.");
        logging.severe( "LogLevel-All: SEVERE message IS displayed.");
        logging.flush();
        pause(300);

        logging.setLogLevelSevere();
        logging.info( "LogLevel-Severe: INFO message NOT displayed.");
        logging.warning( "LogLevel-Severe: WARNING message NOT displayed.");
        logging.severe( "LogLevel-Severe: SEVERE message IS displayed.");
        logging.flush();
        pause(300);

        logging.setLogOff();
        logging.info( "LogLevel-Severe-Off: INFO message NOT displayed.");
        logging.warning( "LogLevel-Severe-Off: WARNING message NOT displayed.");
        logging.severe( "LogLevel-Severe-Off: SEVERE message NOT displayed.");
        logging.flush();
        pause(300);

        logging.setLogOn();
        logging.info( "LogLevel-Severe-On: INFO message NOT displayed.");
        logging.warning( "LogLevel-Severe-On: WARNING message NOT displayed.");
        logging.severe( "LogLevel-Severe-On: SEVERE message IS displayed.");
        logging.flush();
        pause(300);

        logging.setLogLevelAll();
        logging.info( "...Ending Tests.");
        logging.flush();
        pause(300);
    }

    public static void pause(int milliseconds) {
        try { Thread.sleep(milliseconds); }
        catch (InterruptedException e) {}
    }
}
