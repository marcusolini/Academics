package com.mjn.javatraning.vehicle;

public enum VehicleSize {
    SMALL ("Small"), MEDIUM ("Medium"), LARGE ("Large");

    private String nameAsString;

    private VehicleSize(String nameAsString) {
        this.nameAsString = nameAsString;
    }

    @Override
    public String toString() {
        return this.nameAsString;
    }
}
