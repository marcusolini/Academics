package com.marcusolini.util.logging;

import java.util.logging.Level;

public class LogMessage {
    private Level level;
    private int threadId;
    private String method;
    private int line;
    private String message;

    LogMessage(Level level, int threadId, String method, int line, String message) {
        this.level = level;
        this.threadId = threadId;
        this.method = method;
        this.line = line;
        this.message = message;
    }

    public Level getLevel() {
        return level;
    }
    public int getThreadId() {
        return threadId;
    }
    public String method() {
        return method;
    }
    public int line() {
        return line;
    }
    public String getMessage() {
        return message;
    }
}
