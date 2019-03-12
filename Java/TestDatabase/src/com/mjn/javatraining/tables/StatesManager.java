package com.mjn.javatraining.tables;

import com.mjn.javatraining.db.ConnectionManager;
import com.mjn.javatraining.db.DBType;
import com.mjn.javatraining.db.DBUtil;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class StatesManager {

    private static Connection conn = ConnectionManager.getInstance().getConnection();

    public static void displayAllRows() {

        String sql = "SELECT state FROM states";

        try (Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql);
        ){

            System.out.println("States Table:");
            while (rs.next()) {
                StringBuffer buffer = new StringBuffer();
                buffer.append(rs.getString("stateId"));
                System.out.println(buffer.toString());
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void displayData(ResultSet rs) throws SQLException {
        while (rs.next()) {
            String stateFullName =
                    rs.getString("stateId") + ": " + rs.getString("statename");
            System.out.println(stateFullName);
        }
    }
}
