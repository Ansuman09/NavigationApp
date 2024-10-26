package com.web.navigation.service;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.nio.file.Files;
import java.nio.file.Paths;

import org.springframework.stereotype.Service;

import com.web.navigation.entity.DataToSend;

@Service
public class NavigationServiceImpl implements NavigationService{
    private final String fileDirectoryToUse="/home/ansuman/Documents/c++_learn/";
    
    @Override
    public String executeNavigation(DataToSend startPoint, DataToSend destPoint) {
     
        String DataFilePathToWrite=fileDirectoryToUse+"input.txt";
        String DataFilePathToRead = fileDirectoryToUse+"output.txt";
        String result="";
        try{
        BufferedWriter writer=new BufferedWriter(new FileWriter(DataFilePathToWrite));
        System.out.println(Integer.toString(startPoint.getXCor()));
        writer.write(Integer.toString(startPoint.getXCor()));
        writer.newLine();
        writer.write(Integer.toString(startPoint.getYCor()));
        writer.newLine();
        writer.write(Integer.toString(destPoint.getXCor()));
        writer.newLine();
        writer.write(Integer.toString(destPoint.getYCor()));
        System.out.println("Lines written");
        // writer.write(startPoint.getYcor());
        writer.close();
        
        ProcessBuilder builder = new ProcessBuilder(fileDirectoryToUse+"finalfileviewer");

        builder.redirectInput(new File(DataFilePathToWrite));
        builder.redirectOutput(new File(DataFilePathToRead));
        Process process = builder.start();
        

        process.waitFor();
        System.out.println("Got Output");
        String resultFilePath = DataFilePathToRead;
        result = new String(Files.readAllBytes(Paths.get(resultFilePath)));
        }


        catch (Exception e){
            System.out.println(e);
        }

        return result;
    }
}
