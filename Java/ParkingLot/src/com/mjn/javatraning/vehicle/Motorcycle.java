package com.mjn.javatraning.vehicle;

public class Motorcycle extends Vehicle {

    public Motorcycle(int id, boolean isHandicapped) {
        super(id, VehicleType.MOTORCYCLE, VehicleSize.SMALL, isHandicapped);
    }
}
