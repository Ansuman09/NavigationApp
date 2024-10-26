package com.web.navigation.entity;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class DataToSend {
    
    private int xCor;
    private int yCor;
    
    public DataToSend(int xCor,int yCor){
        this.xCor=xCor;
        this.yCor=yCor;
    }
}