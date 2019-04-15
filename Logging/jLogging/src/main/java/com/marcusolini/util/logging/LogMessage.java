package com.marcusolini.util.logging;

import java.util.logging.Level;

public class LogMessage {
    private int threadId;
    private Level level;
    private String message;

    LogMessage(Level level, int threadId, String message) {
        this.level = level;
        this.threadId = threadId;
        this.message = message;
    }

    public Level getLevel() {
        return level;
    }
    public int getThreadId() {
        return threadId;
    }
    public String getMessage() {
        return message;
    }
}
