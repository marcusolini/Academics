package com.marcusolini.util.logging;

public enum LoggingErrorCode {
    E_LOGGER_ALREADY_CREATED(1000, "Logger has already been created."),
    E_LOGGER_NOT_CREATED(1001, "Logger has not been created."),
    E_LOGGER_INVALID(1002, "Logger is invalid."),
    E_LOGGER_THREAD_INTERRUPTED(1003, "Logger thread has been interrupted."),
    E_LOGGER_QUEUE_PUT_INTERRUPTED(1004, "Logger queue put has been interrupted."),
    E_LOGGER_QUEUE_TAKE_INTERRUPTED(1004, "Logger queue take has been interrupted."),
    E_LOGGER_CONSOLE_HANDLER_CREATION_FAILURE(1005, "Logger file handler creation failure."),
    E_LOGGER_FILE_HANDLER_CREATION_FAILURE(1006, "Logger file handler creation failure."),
    ;

    private final int id;
    private final String msg;

    LoggingErrorCode(int id, String msg) {
        this.id = id;
        this.msg = msg;
    }

    public int getId() {
        return this.id;
    }

    public String getMsg() {
        return this.msg;
    }

    @Override
    public String toString() {
        return "LoggingErrorCode{" +
                "id=" + id +
                ", msg='" + msg + '\'' +
                '}';
    }
}
