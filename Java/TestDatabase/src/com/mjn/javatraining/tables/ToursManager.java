package com.mjn.javatraining.tables;

import com.mjn.javatraining.db.ConnectionManager;
import com.mjn.javatraining.db.DBType;
import com.mjn.javatraining.db.DBUtil;

import java.math.BigDecimal;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.NumberFormat;

public class ToursManager {

    private static Connection conn = ConnectionManager.getInstance().getConnection();

    public static void displayAllRows() {

        final String sql = "SELECT tourId, tourName, price FROM tours";

        try (Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql);
        ){

            System.out.println("Tours Table:");
            while (rs.next()) {
                StringBuffer buffer = new StringBuffer();
                buffer.append("Tour " + rs.getInt("tourId") + ": ");
                buffer.append(rs.getString("tourName"));
                System.out.println(buffer.toString());

                double price = rs.getDouble("price");
                NumberFormat nf = NumberFormat.getCurrencyInstance();
                String formattedPrice = nf.format(price);
                buffer.append(" (" + formattedPrice + ")");

                System.out.println(buffer.toString());
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }

    }

    public static void  displayData(ResultSet rs) throws SQLException {

        int nRows = 0;

        rs.last();
        nRows = rs.getRow();

        if (0 == nRows) {
            System.out.println("No tours were found");
        }
        else {
            System.out.println("Number of tours: " + nRows);

            rs.beforeFirst();

            while (rs.next()) {
                StringBuffer buffer = new StringBuffer();

                int tourId = rs.getObject("tourId", Integer.class);
                String tourName = rs.getObject("tourName", String.class);
                BigDecimal price = rs.getObject("price", BigDecimal.class);
                //double price = rs.getObject("price", Double.class);

                //int tourId = rs.getInt("tourId");
                //String tourName = rs.getString("tourName");
                //double price = rs.getDouble("price");

                buffer.append("Tour " + tourId + ": ");
                buffer.append(tourName);

                NumberFormat nf = NumberFormat.getCurrencyInstance();
                String formattedPrice = nf.format(price);

                buffer.append(" (" + formattedPrice + ")");
                System.out.println(buffer.toString());
            }
        }
    }
}
