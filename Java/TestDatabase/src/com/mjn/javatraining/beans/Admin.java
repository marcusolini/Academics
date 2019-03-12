package com.mjn.javatraining.beans;

//import java.io.Serializable;

public class Admin {
//public class Admin implements Serializable {

    private int adminId;
    private String userName;
    private String password;

    public int getAdminId() {
        return adminId;
    }
    public void setAdminId(int adminId) {
        this.adminId = adminId;
    }
    public String getUserName() {
        return userName;
    }
    public void setUserName(String userName) {
        this.userName = userName;
    }
    public String getPassword() {
        return password;
    }
    public void setPassword(String password) {
        this.password = password;
    }
}