package com.mjn.javatraning.vehicle;

public class Car extends Vehicle {

    public Car(int id, boolean isHandicapped) {
        super(id, VehicleType.CAR, VehicleSize.MEDIUM, isHandicapped);
    }
}