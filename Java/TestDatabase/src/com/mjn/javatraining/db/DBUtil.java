package com.mjn.javatraining.db;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class DBUtil {
    public static void processException(SQLException e) {
        System.out.println("Error messsage: " + e.getMessage());
        System.out.println("Error code: " + e.getErrorCode());
        System.out.println("SQL state: " + e.getSQLState());
    }
}
