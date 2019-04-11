package com.marcusolini.util.logging;

public class LoggingException extends Exception {

    private int errorCode;
    private String errorMsg;

    public LoggingException(LoggingErrorCode code) {
        super(code.toString());
        this.errorMsg = code.getMsg();
        this.errorCode = code.getId();
    }

    public LoggingException(LoggingErrorCode code, Throwable err) {
        super(code.toString(), err);
        this.errorMsg = code.getMsg();
        this.errorCode = code.getId();
    }

    public int getErrorCode() {
        return errorCode;
    }

    public String getErrorMsg() {
        return errorMsg;
    }
}