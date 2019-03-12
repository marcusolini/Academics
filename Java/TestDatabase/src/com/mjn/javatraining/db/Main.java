package com.mjn.javatraining.db;

import com.mjn.javatraining.beans.Admin;
import com.mjn.javatraining.tables.AdminManager;
import com.mjn.javatraining.tables.StatesManager;
import com.mjn.javatraining.tables.ToursManager;
import com.mjn.javatraining.util.InputHelper;

import javax.net.ssl.SSLException;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class Main {

    public static void testToursData() {
        try (ConnectionManager connectionManager = ConnectionManager.getInstance();
             Statement stmt = connectionManager.getConnection().createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_READ_ONLY);
             ResultSet rs = stmt.executeQuery("SELECT * FROM tours");
        ) {
            ToursManager.displayData(rs);
            rs.last();
            System.out.println("Number of rows: " + rs.getRow());
        } catch (SQLException e) {
            DBUtil.processException(e);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void testToursJavaLimitRowsData() throws SQLException {
        // Will still receive all rows data.
        ResultSet rs = null;

        try (ConnectionManager connectionManager = ConnectionManager.getInstance();
             Statement stmt = connectionManager.getConnection().createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_READ_ONLY);
        ) {
            stmt.setMaxRows(5);
            rs = stmt.executeQuery("SELECT * FROM tours");
            ToursManager.displayData(rs);
        } catch (SQLException e) {
            DBUtil.processException(e);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            rs.close();
        }
    }

    public static void testToursSqlLimitRowsData() {
        // Retrieves on specified rows. Paging data.
        try (ConnectionManager connectionManager = ConnectionManager.getInstance();
             Statement stmt = connectionManager.getConnection().createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_READ_ONLY);
             ResultSet rs = stmt.executeQuery(
                     "SELECT * FROM tours " +
                             "LIMIT 0, 5");
        ) {
            ToursManager.displayData(rs);
            rs.last();
            System.out.println("Number of rows: " + rs.getRow());
        } catch (SQLException e) {
            DBUtil.processException(e);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void testStatesData() {
        try (ConnectionManager connectionManager = ConnectionManager.getInstance();
             Statement stmt = connectionManager.getConnection().createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_READ_ONLY);
             ResultSet rs = stmt.executeQuery("SELECT stateId, stateName FROM states");
        ) {
            StatesManager.displayData(rs);
            rs.last();
            System.out.println("Number of rows: " + rs.getRow());

            rs.first();
            System.out.println("The first state is "+ rs.getString("stateName"));

            rs.last();
            System.out.println("The last state is "+ rs.getString("stateName"));

        } catch (SQLException e) {
            DBUtil.processException(e);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void testToursPromptPreparedStatementData() {
        final String SQL = "SELECT tourId, tourName, price FROM tours WHERE price <= ?";

        double maxPrice;
        try {
            maxPrice = InputHelper.getDoubleInput("Enter a maximum price: ");
        } catch (NumberFormatException e) {
            System.out.println("Error invalid number");
            return;
        }

        ResultSet rs = null;

        try (ConnectionManager connectionManager = ConnectionManager.getInstance();
             PreparedStatement stmt = connectionManager.getConnection().prepareStatement(
                     SQL,
                     ResultSet.TYPE_SCROLL_INSENSITIVE,
                     ResultSet.CONCUR_READ_ONLY);

        ) {
            stmt.setDouble(1, maxPrice);
            rs = stmt.executeQuery();
            ToursManager.displayData(rs);
        } catch (SQLException e) {
            DBUtil.processException(e);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void testToursStoredProcedureData() {
        final String SQL = "{call GetToursByPrice(?)}";

        double maxPrice;
        try {
            maxPrice = InputHelper.getDoubleInput("Enter a maximum price: ");
        } catch (NumberFormatException e) {
            System.out.println("Error invalid number");
            return;
        }

        ResultSet rs = null;

        try (ConnectionManager connectionManager = ConnectionManager.getInstance();
             CallableStatement stmt = connectionManager.getConnection().prepareCall(
                     SQL,
                     ResultSet.TYPE_SCROLL_INSENSITIVE,
                     ResultSet.CONCUR_READ_ONLY);

        ) {
            stmt.setDouble(1, maxPrice);
            rs = stmt.executeQuery();
            ToursManager.displayData(rs);
        } catch (SQLException e) {
            DBUtil.processException(e);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void testAdminBeanOutputData() {
        int adminId = 0;
        try {
            adminId = InputHelper.getIntegerInput("Select a row: ");
            Admin bean = AdminManager.getRow(adminId);
            if (bean == null) {
                System.err.println("No rows were found");
            } else {
                System.out.println("Admin id: " + bean.getAdminId());
                System.out.println("UserName id: " + bean.getUserName());
                System.out.println("Password id: " + bean.getPassword());
            }
        } catch (NumberFormatException | SQLException e) {
            System.out.println("Error invalid number");
            return;
        }
    }

    public static void testAdminBeanInputData() {
        int adminId = 0;
        try {
            AdminManager.displayAllRows();

            Admin bean = new Admin();

            System.out.println("Add new admin...");
            bean.setUserName(InputHelper.getInput("UserName: "));
            bean.setPassword(InputHelper.getInput("Password: "));

            boolean result = AdminManager.insert(bean);

            if (result == true) {
                System.out.println("New row with primary key " + bean.getAdminId() + " was inserted");
            }

            AdminManager.displayAllRows();

        } catch (NumberFormatException | SQLException e) {
            return;
        }
    }

    public static void testAdminBeanUpdateData() {
        int adminId = 0;
        try {
            AdminManager.displayAllRows();
            adminId = InputHelper.getIntegerInput("Select a Admin row to update: ");

            Admin bean = AdminManager.getRow(adminId);
            if (bean == null) {
                System.err.println("Row not found");
                return;
            }

            String passwordPrompt = InputHelper.getInput("Enter a new password: ");
            bean.setPassword(passwordPrompt);

            boolean result = AdminManager.update(bean);
            if (result == true) {
                System.out.println("Success");
            } else {
                System.err.println("Error");
            }

            AdminManager.displayAllRows();

        } catch (NumberFormatException | SQLException e) {
            return;
        }
    }

    public static void testAdminBeanDeleteData() {
        int adminId = 0;
        try {
            AdminManager.displayAllRows();
            adminId = InputHelper.getIntegerInput("Select a Admin row to delete: ");

            Admin bean = AdminManager.getRow(adminId);
            if (bean == null) {
                System.err.println("Row not found");
                return;
            }

            boolean result = AdminManager.delete(adminId);
            if (result == true) {
                System.out.println("Success");
            } else {
                System.err.println("Error");
            }

            AdminManager.displayAllRows();

        } catch (NumberFormatException | SQLException e) {
            return;
        }
    }

    public static void testAdminBeanInlineUpdateData() {
        int adminId = 0;
        try {
            AdminManager.displayAllRows();
            adminId = InputHelper.getIntegerInput("Select a Admin row to update: ");

            Admin bean = AdminManager.getRow(adminId);
            if (bean == null) {
                System.err.println("Row not found");
                return;
            }

            String passwordPrompt = InputHelper.getInput("Enter a new password: ");
            bean.setPassword(passwordPrompt);

            boolean result = AdminManager.updateInline(bean);
            if (result == true) {
                System.out.println("Success");
            } else {
                System.err.println("Error");
            }

            AdminManager.displayAllRows();

        } catch (NumberFormatException | SQLException e) {
            return;
        }
    }

    public static void testAdminBeanUpdateWithCommitData() {
        int adminId = 0;
        try {
            AdminManager.displayAllRows();
            adminId = InputHelper.getIntegerInput("Select a Admin row to update: ");

            Admin bean = AdminManager.getRow(adminId);
            if (bean == null) {
                System.err.println("Row not found");
                return;
            }

            String passwordPrompt = InputHelper.getInput("Enter a new password: ");
            bean.setPassword(passwordPrompt);

            // Create a transaction
            Connection conn = ConnectionManager.getInstance().getConnection();

            conn.setAutoCommit(false);

            boolean result = AdminManager.update(bean);
            if (result == true) {
                System.out.println("Success");
            } else {
                System.err.println("Error");
            }

            conn.commit();
            System.out.println("Transaction committed");
            //conn.rollback();
            //System.out.println("Transaction rollback");

            conn.setAutoCommit(true);

            AdminManager.displayAllRows();

        } catch (NumberFormatException | SQLException e) {
            return;
        }
    }

    public static void outputMetadata() throws SQLException {
        Connection conn = ConnectionManager.getInstance().getConnection();
        ResultSet rsTables = null;
        ResultSet rsColumns = null;
        List<String> tables = new ArrayList<>();

        try {
            DatabaseMetaData metaData = conn.getMetaData();
            String[] tableTypes = {"TABLE"};
            rsTables = metaData.getTables(null, "%", "%", tableTypes);

            while (rsTables.next()) {
                tables.add(rsTables.getString("TABLE_NAME"));
            }

            for (String tableName: tables) {
                System.out.println("Table: " + tableName);
                System.out.println("----------------");
                rsColumns = metaData.getColumns(null, "%", tableName, "%");
                while (rsColumns.next()) {
                    StringBuffer buffer = new StringBuffer();
                    buffer.append(rsColumns.getString("COLUMN_NAME"));
                    buffer.append(": ");
                    buffer.append(rsColumns.getString("TYPE_NAME"));
                    System.out.println(buffer.toString());
                }
                System.out.println("");
            }

        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            rsTables.close();
            rsColumns.close();
        }
    }

    public static void main(String[] args) throws Exception {

        try {
            //ConnectionManager.getInstance().setDbType(DBType.MYSQL);
            ConnectionManager.getInstance().setDbType(DBType.HSQLDB);

            //Main.testToursData();
            //Main.testStatesData();
            //Main.testToursJavaLimitRowsData();
            //Main.testToursSqlLimitRowsData();
            //Main.testToursPromptPreparedStatementData();
            //Main.testToursStoredProcedureData();
            //Main.testAdminBeanOutputData();
            //Main.testAdminBeanInputData();
            //Main.testAdminBeanUpdateData();
            //Main.testAdminBeanDeleteData();
            //Main.testAdminBeanInlineUpdateData();
            //Main.testAdminBeanUpdateWithCommitData();
            Main.outputMetadata();
        } catch (Exception e) {
            System.out.println("Exception" + e);
        }

        ConnectionManager.getInstance().close();
    }
}
