package com.web.navigation.entity;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
public class DataToSend {
    private int xCor; // Change to xCor
    private int yCor; // Change to yCor

    public DataToSend() {
    }

    public int getXCor() {
        return xCor;
    }

    public void setXCor(int xCor) {
        this.xCor = xCor;
    }

    public int getYCor() {
        return yCor;
    }

    public void setYCor(int yCor) {
        this.yCor = yCor;
    }
}
