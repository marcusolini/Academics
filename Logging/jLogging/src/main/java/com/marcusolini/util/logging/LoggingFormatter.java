package com.marcusolini.util.logging;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Formatter;
import java.util.logging.LogRecord;

public class LoggingFormatter extends Formatter {
    private static final String TIMESTAMP_PATTERN = "yyyy-MM-dd'T'HH:mm:ss.SSSXXX";

    @Override
    public synchronized String format(LogRecord record) {
        return String.format("[%1$06d] [%2$s] [%3$-7s] [%4$s] [%5$s]\n",
                record.getThreadID(),
                new SimpleDateFormat(TIMESTAMP_PATTERN).format(new Date(record.getMillis())),
                record.getLevel().getName(),
                record.getSourceMethodName(),
                formatMessage(record));
    }
}
