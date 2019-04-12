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
        boolean bAppend = false;

        try {
            logging = Logging.getInstance();
            logging.createLogger(LOGGERNAME);

            try { logging.addConsoleHandler(); }
            catch (LoggingException e) { logging.severe("Exception" + e.getMessage()); }

            try { logging.addFileHandler(LOGFILE, bAppend); }
            catch (LoggingException e) { logging.severe("Exception" + e.getMessage()); }
            try { logging.addFileHandler(LOGFILE, bAppend); }
            catch (LoggingException e) { logging.severe("Exception" + e.getMessage()); }

            logging.setLogLevelAll();
        } catch (LoggingException e) { logging.severe("Exception" + e.getMessage() ); }

        logging.setLogLevelAll();
        logging.info( "Starting Tests...");
        logging.info( "LogLevel-All: INFO message IS displayed.");
        logging.warning( "LogLevel-All: WARNING message IS displayed.");
        logging.severe( "LogLevel-All: SEVERE message IS displayed.");
        logging.flush();
        try { Thread.sleep(300); }
        catch (InterruptedException e) {e.printStackTrace();}

        logging.setLogLevelSevere();
        logging.info( "LogLevel-Severe: INFO message NOT displayed.");
        logging.warning( "LogLevel-Severe: WARNING message NOT displayed.");
        logging.severe( "LogLevel-Severe: SEVERE message IS displayed.");
        logging.flush();
        try { Thread.sleep(300); }
        catch (InterruptedException e) {e.printStackTrace();}

        logging.setLogOff();
        logging.info( "LogLevel-Severe-Off: INFO message NOT displayed.");
        logging.warning( "LogLevel-Severe-Off: WARNING message NOT displayed.");
        logging.severe( "LogLevel-Severe-Off: SEVERE message NOT displayed.");
        logging.flush();
        try { Thread.sleep(300); }
        catch (InterruptedException e) {e.printStackTrace();}

        logging.setLogOn();
        logging.info( "LogLevel-Severe-On: INFO message NOT displayed.");
        logging.warning( "LogLevel-Severe-On: WARNING message NOT displayed.");
        logging.severe( "LogLevel-Severe-On: SEVERE message IS displayed.");
        logging.flush();
        try { Thread.sleep(300); }
        catch (InterruptedException e) {e.printStackTrace();}

        logging.setLogLevelAll();
        logging.info( "...Ending Tests.");
        logging.flush();
        try { Thread.sleep(300); }
        catch (InterruptedException e) {e.printStackTrace();}
    }
}
