package com.marcusolini.util.logging;

import java.util.logging.Level;

public class LogMessage {
    private Level level;
    private String message;

    LogMessage(Level level, String message) {
        this.level = level;
        this.message = message;
    }
    public Level getLevel() {
        return level;
    }
    public void setLevel(Level level) {
        this.level = level;
    }
    public String getMessage() {
        return message;
    }
    public void setMessage(String message) {
        this.message = message;
    }
}
