package com.mjn.javatraning.vehicle;

public enum VehicleType {
    MOTORCYCLE ("Motorcycle"), CAR ("Car"), TRUCK ("Truck");

    private String nameAsString;

    VehicleType(String type) {
        this.nameAsString = type;
    }

    @Override
    public String toString() {
        return this.nameAsString;
    }
}
