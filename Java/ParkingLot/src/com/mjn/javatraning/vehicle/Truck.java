package com.mjn.javatraning.vehicle;

public class Truck extends Vehicle {

    public Truck(int id, boolean isHandicapped) {
        super(id, VehicleType.TRUCK, VehicleSize.LARGE, isHandicapped);
    }
}