package com.mjn.javatraning.parkingspace;

public class ParkingSpace implements IParkingSpace{

    private int number;
    private SpaceSize size;
    private SpaceType type;
    private boolean isAvailable;

    public ParkingSpace(int number, SpaceSize size, SpaceType type, boolean isAvailable) {
        this.number = number;
        this.size = size;
        this.type = type;
        this.isAvailable = isAvailable;
    }
    public int getNumber() {
        return number;
    }
    public SpaceSize getSize() {
        return size;
    }
    public SpaceType getType() {
        return type;
    }
    public boolean isAvailable() {
        return isAvailable;
    }
    public void setAvailable() {
        isAvailable = true;
    }
    public void setUnAvailable() {
        isAvailable = false;
    }
    public boolean isHandicapped() {
        return (type == SpaceType.HANDICAPPED);
    }
}
