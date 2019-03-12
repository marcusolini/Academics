package com.mjn.javatraning.vehicle;

public abstract class Vehicle {

    int         id;
    VehicleType type;
    VehicleSize size;
    boolean isHandicapped;

    public Vehicle(int id, VehicleType type, VehicleSize size, boolean isHandicapped) {
        this.id = id;
        this.type = type;
        this.size = size;
        this.isHandicapped = isHandicapped;
    }
    public int getId() {
        return id;
    }
    public VehicleType getType() {
        return type;
    }
    public VehicleSize getSize() {
        return size;
    }
    public boolean isHandicapped() {
        return isHandicapped;
    }
}
