package com.mjn.javatraning.parkingspace;

public interface IParkingSpace {
    public int getNumber();
    public SpaceSize getSize();
    public SpaceType getType();
    public boolean isAvailable();
    public void setAvailable();
    public void setUnAvailable();
    public boolean isHandicapped();
}
