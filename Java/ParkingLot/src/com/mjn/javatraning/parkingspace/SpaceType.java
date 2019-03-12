package com.mjn.javatraning.parkingspace;

public enum SpaceType {
    REGULAR ("Regular"), HANDICAPPED ("Handicapped");

    private String nameAsString;

    private SpaceType(String nameAsString) {
        this.nameAsString = nameAsString;
    }

    @Override
    public String toString() {
        return this.nameAsString;
    }
}
