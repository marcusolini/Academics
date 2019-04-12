package com.marcusolini.util.logging;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.logging.LogManager;

public class LoggingManager extends LogManager {

    LoggingManager(String fileName) throws LoggingException {
        try {
            if (null != fileName) {
                FileInputStream fileInputStream = null;
                fileInputStream = new FileInputStream(fileName);
                readConfiguration(fileInputStream);
            } else {
                throw new LoggingException(LoggingErrorCode.E_LOGGER_MANAGER_CONFIG_FILE_FAILURE);
            }
        } catch (SecurityException e) {
            throw new LoggingException(LoggingErrorCode.E_LOGGER_MANAGER_CONFIG_FILE_FAILURE, e);
        } catch (FileNotFoundException e) {
            throw new LoggingException(LoggingErrorCode.E_LOGGER_MANAGER_CONFIG_FILE_FAILURE, e);
        } catch (IOException e) {
            throw new LoggingException(LoggingErrorCode.E_LOGGER_MANAGER_CONFIG_FILE_FAILURE, e);
        }
    }

    @Override
    public void readConfiguration(InputStream ins) throws IOException, SecurityException {
        super.readConfiguration(ins);
    }
}
