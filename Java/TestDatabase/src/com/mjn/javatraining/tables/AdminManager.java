package com.mjn.javatraining.tables;

import com.mjn.javatraining.beans.Admin;
import com.mjn.javatraining.db.ConnectionManager;
import com.mjn.javatraining.db.DBType;
import com.mjn.javatraining.db.DBUtil;

import java.sql.*;

public class AdminManager {

    private static Connection conn = ConnectionManager.getInstance().getConnection();

    public static boolean updateInline(Admin bean) throws SQLException {
        String sql = "SELECT * FROM admin WHERE adminId = ?";

        ResultSet rs = null;

        try (PreparedStatement stmt = conn.prepareStatement(
                     sql,
                     ResultSet.TYPE_SCROLL_INSENSITIVE,
                     ResultSet.CONCUR_UPDATABLE);
        ) {
            //stmt.setString(1, bean.getUserName());
            //stmt.setString(2, bean.getPassword());
            stmt.setInt(1, bean.getAdminId());

            rs = stmt.executeQuery();

            if (rs.next()) {
                rs.updateString("username", bean.getUserName());
                rs.updateString("password", bean.getPassword());
                rs.updateRow();
                return true;
            } else {
                return false;
            }
        } catch (SQLException e) {
            System.err.println(e);
            return false;
        } finally {
            if (rs != null) {
                rs.close();
            }
        }
    }

    public static boolean delete(int adminId) {
        String sql = "DELETE FROM admin WHERE adminId = ?";

        try (PreparedStatement stmt = conn.prepareStatement(sql);
        ) {

            stmt.setInt(1, adminId);

            int affected = stmt.executeUpdate();
            if (affected == 1) {
                return true;
            } else {
                return false;
            }
        } catch (SQLException e) {
            System.err.println(e);
            return false;
        }
    }

    public static boolean update(Admin bean) {
        String sql = "UPDATE admin SET " +
                "userName = ?, password = ? " +
                "WHERE adminId = ?";

        try (PreparedStatement stmt = conn.prepareStatement(sql);
        ) {
            stmt.setString(1, bean.getUserName());
            stmt.setString(2, bean.getPassword());
            stmt.setInt(3, bean.getAdminId());

            int affected = stmt.executeUpdate();
            if (affected == 1) {
                return true;
            } else {
                return false;
            }
        } catch (SQLException e) {
            System.err.println(e);
            return false;
        }
    }

    public static boolean insert(Admin bean) throws SQLException {
        String sql = "INSERT into admin (userName, password) " +
                "VALUES (?, ?)";

        ResultSet rsKeys = null;

        try (PreparedStatement stmt = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
        ){
            stmt.setString(1, bean.getUserName());
            stmt.setString(2, bean.getPassword());

            int affected = stmt.executeUpdate();
            if (affected == 1) {
                rsKeys = stmt.getGeneratedKeys();
                rsKeys.next();
                int newKey = rsKeys.getInt(1);
                bean.setAdminId(newKey);
            } else {
                System.err.println("No rows affected");
                return false;
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        } finally {
            if (rsKeys != null) {
                rsKeys.close();
            }
        }

        return true;
    }

    public static void displayAllRows() {

        String sql = "SELECT adminId, userName, password FROM admin";

        try (Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql);
            ){

            System.out.println("Admin Table:");
            while (rs.next()) {
                StringBuffer buffer = new StringBuffer();
                buffer.append(rs.getInt("adminId") + ": ");
                buffer.append(rs.getString("userName") + ": ");
                buffer.append(rs.getString("password"));
                System.out.println(buffer.toString());
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static Admin getRow(int adminId) throws SQLException {
        String sql = "SELECT * FROM admin WHERE adminId = ?";

        ResultSet rs = null;

        try (PreparedStatement stmt = conn.prepareStatement(sql);
        ){
            stmt.setInt(1, adminId);
            rs = stmt.executeQuery();

            if (rs.next()) {
                Admin bean = new Admin();
                bean.setAdminId(adminId);
                bean.setUserName(rs.getString("userName"));
                bean.setPassword(rs.getString("password"));
                return bean;
            } else {
                return null;
            }
        } catch (SQLException e) {
            System.err.println(e);
            return null;
        } finally {
            if (rs != null) {
                rs.close();
            }
        }
    }
}
