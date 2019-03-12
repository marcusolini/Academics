package com.mjn.javatraning.parkinglot;

import com.mjn.javatraning.parkingspace.SpaceSize;
import com.mjn.javatraning.parkingspace.SpaceType;
import com.mjn.javatraning.vehicle.*;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

public class Main {

    public static void buildParkingLot(ParkingLot parkingLot) {
        parkingLot.addSpaces(9, SpaceSize.SMALL, SpaceType.REGULAR);
        parkingLot.addSpaces(24, SpaceSize.MEDIUM, SpaceType.REGULAR);
        parkingLot.addSpaces(5, SpaceSize.MEDIUM, SpaceType.HANDICAPPED);
        parkingLot.addSpaces(10, SpaceSize.LARGE, SpaceType.REGULAR);
    }

    public static void outputParkingLotInfo(ParkingLot parkingLot) {
        System.out.println("Total Spaces: " + parkingLot.getTotalSpaces());
        System.out.println("Total Available Spaces: " + parkingLot.getTotalAvailableSpacees());
        System.out.println("Total Unavailable Spaces: " + parkingLot.getTotalUnavailableSpacees());
        System.out.println("Is Full: " + parkingLot.isFull());
        System.out.println("Is Empty: " + parkingLot.isEmpty());
        System.out.println("Is Available: " + parkingLot.isAvailable());
    }

    public static void buildVehiclesList(Queue<Vehicle> vehicleList) {
        int id = 1;
        // CARS - HANDICAPPED
        for (int i = 0; i < 6; i++, id++) {
            Vehicle vehicle = new Car(id, true);
            vehicleList.add(vehicle);
        }
        // TRUCKS
        for (int i = 0; i < 8; i++, id++) {
            Vehicle vehicle = new Truck(id, false);
            vehicleList.add(vehicle);
        }
        // CARS
        for (int i = 0; i < 3; i++, id++) {
            Vehicle vehicle = new Car(id, false);
            vehicleList.add(vehicle);
        }
        // TRUCK - HANDICAPPED
        for (int i = 0; i < 1; i++, id++) {
            Vehicle vehicle = new Truck(id, true);
            vehicleList.add(vehicle);
        }
        // TRUCK
        for (int i = 0; i < 1; i++, id++) {
            Vehicle vehicle = new Truck(id, false);
            vehicleList.add(vehicle);
        }
        // TRUCK - HANDICAPPED
        for (int i = 0; i < 1; i++, id++) {
            Vehicle vehicle = new Truck(id, true);
            vehicleList.add(vehicle);
        }
        // MOTO
        for (int i = 0; i < 3; i++, id++) {
            Vehicle vehicle = new Motorcycle(id, false);
            vehicleList.add(vehicle);
        }
        // CAR
        for (int i = 0; i < 1; i++, id++) {
            Vehicle vehicle = new Car(id, false);
            vehicleList.add(vehicle);
        }

        System.out.println("Total Vehicles: " + vehicleList.size());
    }

    public static void parkVehicles(ParkingLot parkingLot, Queue<Vehicle> vehicleList) {

        for( Vehicle vehicle : vehicleList) {
            System.out.println("Parking - "
                    + " ID:" + vehicle.getId()
                    + " Type:" + vehicle.getType()
                    + " Size:" + vehicle.getSize()
                    + " Handicapped:"+ vehicle.isHandicapped()   );

            if (false == parkingLot.parkVehicle(vehicle)) {
                System.err.println("Unable to park vehicle");
            }
        }
    }

    public static void main(String[] args) {
        ParkingLot parkingLot = new ParkingLot();
        buildParkingLot(parkingLot);

        Queue<Vehicle> vehicleList = new LinkedList<>();
        buildVehiclesList(vehicleList);

        outputParkingLotInfo(parkingLot);

        parkVehicles(parkingLot, vehicleList);

        outputParkingLotInfo(parkingLot);

    }
}
