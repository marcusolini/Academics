package com.mjn.javatraning.parkinglot;

import com.mjn.javatraning.parkingspace.IParkingSpace;
import com.mjn.javatraning.parkingspace.ParkingSpace;
import com.mjn.javatraning.parkingspace.SpaceSize;
import com.mjn.javatraning.parkingspace.SpaceType;
import com.mjn.javatraning.vehicle.Vehicle;
import com.mjn.javatraning.vehicle.VehicleSize;
import com.mjn.javatraning.vehicle.VehicleType;

import java.util.*;

public class ParkingLot {

    Map<Integer, IParkingSpace> allSpacesMap = new TreeMap<>();
    Map<Integer, Vehicle> parkingMap = new TreeMap<>();
    List<IParkingSpace> availableList = new ArrayList<>();

    synchronized void addSpaces(int spaces, SpaceSize size, SpaceType type) {
        if (spaces == 0) return;

        int spaceNumber = allSpacesMap.size()+1;
        for (int i = 0; i < spaces; i++, spaceNumber++)
        {
            IParkingSpace parkingSpace = new ParkingSpace(spaceNumber, size, type, true);
            allSpacesMap.put(spaceNumber, parkingSpace);
            availableList.add(parkingSpace);
        }
    }

    synchronized boolean parkVehicle(Vehicle vehicle) {
        if (true == isFull()) return false;

        boolean isParked = false;
        SpaceSize spaceSize = SpaceSize.SMALL;
        SpaceType spaceType = vehicle.isHandicapped() ? SpaceType.HANDICAPPED : SpaceType.REGULAR;

        switch (vehicle.getSize()) {
            case SMALL:
                spaceSize = SpaceSize.SMALL;
            break;
            case MEDIUM:
                spaceSize = SpaceSize.MEDIUM;
            break;
            case LARGE:
                spaceSize = SpaceSize.LARGE;
            break;
        }

        isParked = attemptToParkVehicle(spaceSize, spaceType, vehicle);

        if ((false == isParked) && SpaceType.HANDICAPPED == spaceType) {
            isParked = attemptToParkVehicle(spaceSize, SpaceType.REGULAR, vehicle);
        }

        return isParked;
    }

    synchronized private boolean attemptToParkVehicle(SpaceSize spaceSize, SpaceType spaceType, Vehicle vehicle) {
        IParkingSpace parkingSpace = null;

        switch (spaceSize) {
            case SMALL:
                parkingSpace = findSpace(SpaceSize.SMALL, spaceType);
                if (null != parkingSpace) {
                    parkVehicle(parkingSpace, vehicle);
                    return true;
                }
            case MEDIUM:
                parkingSpace = findSpace(SpaceSize.MEDIUM, spaceType);
                if (null != parkingSpace) {
                    parkVehicle(parkingSpace, vehicle);
                    return true;
                }
            case LARGE:
                parkingSpace = findSpace(SpaceSize.LARGE, spaceType);
                if (null != parkingSpace) {
                    parkVehicle(parkingSpace, vehicle);
                    return true;
                }
        }

        return false;
    }

    synchronized private void parkVehicle(IParkingSpace parkingSpace, Vehicle vehicle) {
        parkingSpace.setUnAvailable();
        availableList.remove(parkingSpace);
        parkingMap.put(parkingSpace.getNumber(), vehicle);
        System.out.println("id " + vehicle.getId()
                + " " + vehicle.getType()
                + " has parked in a " + parkingSpace.getSize()
                + " "
                + parkingSpace.getType()  );
    }

    synchronized boolean isFull() {
        return (availableList.size() == 0);
    }
    synchronized boolean isAvailable() {
        return (availableList.size() != 0);
    }
    synchronized boolean isEmpty() {
        return (availableList.size() == allSpacesMap.size());
    }
    synchronized int getTotalSpaces() {
        return allSpacesMap.size();
    }
    synchronized int getTotalAvailableSpacees() {
        return availableList.size();
    }
    synchronized int getTotalUnavailableSpacees() {
        return allSpacesMap.size() - availableList.size();
    }

    synchronized private IParkingSpace findSpace(SpaceSize size, SpaceType type) {
        IParkingSpace space = null;
        for (IParkingSpace s : availableList) {
            if (s.isAvailable() && s.getSize() == size && s.getType() == type) {
                space = s;
                break;
            }
        }
        return space;
    }

}
