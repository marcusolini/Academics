package com.mjn.javatraining.db;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class ConnectionManager implements AutoCloseable {
    private static ConnectionManager instance = new ConnectionManager();

    public static ConnectionManager getInstance() {
        return instance;
    }

    private static final String USERNAME = "dbuser";
    private static final String PASSWORD = "dbuser";
    private static final String H_CONN_STRING =
            "jdbc:hsqldb:data/explorecalifornia";   // HyperSql uses files in the data directory
    private static final String M_CONN_STRING =
            "jdbc:mysql://localhost/explorecalifornia";  // MySql uses a server

    //private DBType dbType = DBType.MYSQL;
    private DBType dbType = DBType.HSQLDB;

    private Connection conn = null;

    public DBType getDbType() {
        return dbType;
    }

    public void setDbType(DBType dbType) {
        this.dbType = dbType;
    }

    private ConnectionManager() {
    }

    private boolean openConnection() {
        System.out.println("Opening connection");
        try {
            if (conn != null) {
                close();
            }

            switch (dbType) {
                case MYSQL:
                    //Class.forName("comm.mysql.jdbc.Driver"); // for java 6-
                    conn = DriverManager.getConnection(M_CONN_STRING, USERNAME, PASSWORD);
                    return true;
                case HSQLDB:
                    conn = DriverManager.getConnection(H_CONN_STRING, USERNAME, PASSWORD);
                    return true;
                default:
                    return false;
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public synchronized Connection getConnection() {
        System.out.println("Getting connection");
        if (conn == null) {
            if (openConnection()) {
                System.out.println("Connection opened");
            } else {
                return null;
            }
        }
        return conn;
    }

    @Override
    public void close() throws Exception {
        System.out.println("Closing connection");
        if (conn != null) {
            try {
                conn.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
            conn = null;
        }
    }
}
