package com.example.databasetest;

import org.apache.derby.jdbc.EmbeddedDataSource;

import javax.crypto.*;
import javax.crypto.spec.SecretKeySpec;
import java.io.*;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.security.GeneralSecurityException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.sql.*;
import java.util.Base64;
import java.util.Properties;
import java.util.Random;
import java.util.logging.FileHandler;
import java.util.logging.Level;
import java.util.logging.Logger;

public class DatabaseTest {

    private static final String DB_PASSWORD = "dbPassword";
    private static final String DB_USER = "dbUser";
    private static final String DB_URL = "dbUrl";
    private static final String DB_PROPERTIES = "db.properties";
    private String dbUrl;
    private String dbUser;
    private String dbPasword;
    private Properties globalProps;

    public void writeProperites() throws IOException {
        Properties prop = new Properties();
        prop.setProperty("dbUrl", "localhost"); // jdbc:derby:test.db;create=true
        prop.setProperty("dbUser", "username");
        prop.setProperty("dbPassword", "password");

        try (OutputStream out = new FileOutputStream(DB_PROPERTIES)) {
            prop.store(out, "Database Properties File");
        }
    }

    public void getProperites() throws IOException {
        try (InputStream in = new FileInputStream(DB_PROPERTIES)) {
            Properties prop = new Properties();
            prop.load(in);
            dbUrl = prop.getProperty(DB_URL);
            dbUser = prop.getProperty(DB_USER);
            dbPasword = prop.getProperty(DB_PASSWORD);
        }
    }

    public void readProperites() throws IOException {
        try (InputStream in = new FileInputStream(DB_PROPERTIES)) {
            globalProps = new Properties();
            globalProps.load(in);
        }
    }

    public Connection connectToDatabase() throws SQLException {
        DriverManager.registerDriver(new org.apache.derby.jdbc.EmbeddedDriver());

        Properties connectionProps = new Properties();
        connectionProps.put("user", globalProps.getProperty(DB_USER));
        connectionProps.put("password", globalProps.getProperty(DB_PASSWORD));

        Connection conn = DriverManager.getConnection(globalProps.getProperty(DB_URL), connectionProps);
        return conn;
    }

    // Connection pooling
    public Connection connectUsingDataSource() throws SQLException {
        EmbeddedDataSource ds = new org.apache.derby.jdbc.EmbeddedDataSource();

        ds.setDatabaseName("test.db;create=true");
        ds.setUser(globalProps.getProperty(DB_USER));
        ds.setPassword(globalProps.getProperty(DB_PASSWORD));
        Connection conn = ds.getConnection();

        conn.setAutoCommit(true);

        return conn;
    }

    public void createDatabaseTable(Connection conn) throws SQLException {
        try (Statement s = conn.createStatement()) {
            s.executeUpdate("CREATE TABLE testdata(" +
                    "num INT, " +
                    "dt TIMESTAMP, " +
                    "txt VARCHAR(256))");
            s.close();
        }
    }

    public void addData(Connection conn) throws SQLException {
        String sql = "INSERT INTO testdata VALUES(?,?,?)";

        try (PreparedStatement ps = conn.prepareStatement(sql) ){
            Random rnd = new Random();
            long timenow = System.currentTimeMillis();

            for (int i = 0; i <10; i++) {
                int num = rnd.nextInt(1000);
                long time = timenow - (num ^ 2);

                ps.setInt(1, num);
                ps.setTimestamp(2, new Timestamp(time));
                ps.setString(3, time + ": " + (new Date(time)));
                ps.executeUpdate();
            }
        }
    }

    public void readData(Connection conn) throws SQLException {
        try (Statement query = conn.createStatement()) {
            String sql = "SELECT * FROM testdata";
            query.setFetchSize(100);

            try (ResultSet rs = query.executeQuery(sql)) {
                while (!rs.isClosed() && rs.next()) {
                    int num = rs.getInt(1);
                    Timestamp dt = rs.getTimestamp(2);
                    String txt = rs.getString(3);
                    System.out.println(num + "; " + dt + "; " + txt);
                }
            }
        }
    }

    public void closeDatabaseConnection(Connection conn) throws SQLException {
        conn.close();
/*
        try {
            DriverManager.getConnection("jdbc:derby:test.db;shutdown=true");
        } catch (SQLException sqe) {
            if ((sqe.getErrorCode() == 4500) || (sqe.getErrorCode() == 5000)) {
                // ignore...
            } else {
                throw sqe;
            }
        }
        */
    }

    public void testConnection() throws IOException, SQLException {
        readProperites();;
        //Connection conn = connectToDatabase();
        Connection conn = connectUsingDataSource();

        try {
            createDatabaseTable(conn);
            addData(conn);
        } catch (SQLException e) {
            System.out.println("Databse table already exists");
        }

        readData(conn);
        closeDatabaseConnection(conn);
    }

    public SecretKey generateNewKey() throws GeneralSecurityException {
        KeyGenerator keyGenerator = KeyGenerator.getInstance("AES");
        keyGenerator.init(128);
        SecretKey secretKey = keyGenerator.generateKey();
        return secretKey;
    }

    private SecretKey buiildKey() throws GeneralSecurityException {
        byte[] keyBytes = new byte[16];
        SecretKey secretKey = null;

        // OPTION 1 (previous secretKey.getEncoded))
        keyBytes = new byte[] {-89, -99, 101, -9, 80, 28, 25, -111,
                                -121, -40, 14, 53, 54, 2, 53, 24};
        secretKey = new SecretKeySpec(keyBytes, "AES");

        // OPTION 2 (known String)
        try {
            String pwdString = "asfkjkl;j;jkdjfa";
            byte[] pwBytes = pwdString.getBytes("UTF-8");
            System.arraycopy(pwBytes, 0, keyBytes, 0, Math.min(keyBytes.length, pwBytes.length));
            secretKey = new SecretKeySpec(keyBytes, "AES");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        // OPTION 3 (known random seed)
        long secretSeed = 123456789;
        KeyGenerator keyGenerator = KeyGenerator.getInstance("AES");
        SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
        random.setSeed(secretSeed);
        keyGenerator.init(128, random);
        secretKey = keyGenerator.generateKey();

        return secretKey;
    }

    private String encryptString(String plainText, SecretKey secretKey) throws GeneralSecurityException, UnsupportedEncodingException {
        Cipher  cipher = Cipher.getInstance(secretKey.getAlgorithm());
        cipher.init(Cipher.ENCRYPT_MODE, secretKey);

        byte[] textBytes = plainText.getBytes("UTF-8");
        byte[] encryptedBytes = cipher.doFinal(textBytes);

        Base64.Encoder encoder = Base64.getEncoder();
        String encryptedText = encoder.encodeToString(encryptedBytes);
        return encryptedText;
    }

    private char[] decryptStringf(String encryptedText, SecretKey secretKey) throws NoSuchPaddingException, NoSuchAlgorithmException, BadPaddingException, IllegalBlockSizeException, InvalidKeyException {
        Base64.Decoder decoder = Base64.getDecoder();
        byte[] encryptedBytes = decoder.decode(encryptedText);

        Cipher cipher = Cipher.getInstance(secretKey.getAlgorithm());
        cipher.init(Cipher.DECRYPT_MODE, secretKey);

        byte[] decryptedBytes = cipher.doFinal(encryptedBytes);
        Charset utf8 = StandardCharsets.UTF_8;
        CharBuffer decryptedChars = utf8.decode(ByteBuffer.wrap(decryptedBytes));

        return decryptedChars.array();
    }

    public void KeyTest() {
        SecretKey secretKey = null;
        try {
            secretKey = buiildKey();
            String encrypted = encryptString("password", secretKey);
            char[] decrypted = decryptStringf(encrypted, secretKey);
            System.out.println("encrypted password: " + encrypted);
            System.out.println("decrypted password: " + new String(decrypted));
        } catch (GeneralSecurityException e) {
            e.printStackTrace();
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        DatabaseTest test = new DatabaseTest();
        Logger logger = Logger.getLogger("com.example.databasetest");

        FileHandler handler = null;
        try {
            handler = new FileHandler("mylog.xml");
            logger.addHandler(handler);
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            test.writeProperites();
            test.testConnection();
        } catch (IOException | SQLException e) {
            logger.log(Level.WARNING, "Error testing connection", e);
            //e.printStackTrace();
        }
    }
}
