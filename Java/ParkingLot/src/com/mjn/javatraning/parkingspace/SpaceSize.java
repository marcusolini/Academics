package com.mjn.javatraning.parkingspace;

public enum SpaceSize {
    SMALL ("Small"), MEDIUM ("Medium"), LARGE ("Large");

    private String nameAsString;

    private SpaceSize(String nameAsString) {
        this.nameAsString = nameAsString;
    }

    @Override
    public String toString() {
        return this.nameAsString;
    }
}

