package com.web.navigation.service;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import javax.xml.crypto.Data;

import org.springframework.stereotype.Service;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;
import com.web.navigation.entity.DataToSend;

@Service
public class NavigationServiceImpl implements NavigationService{
    private final String fileDirectoryToUse="";
    private final String mapLocation="/home/ansuman/Downloads";
    
    @Override
    public String executeNavigation(DataToSend startPoint, DataToSend destPoint,String mapname) {
        
        String DataFilePathToWrite=fileDirectoryToUse+"input.txt";
        String DataFilePathToRead = fileDirectoryToUse+"output.txt";
        String result="";    
        try{
        BufferedWriter writer=new BufferedWriter(new FileWriter(DataFilePathToWrite));
        System.out.println(Integer.toString(startPoint.getXCor()));
        System.out.println(mapLocation+"/"+mapname);
        writer.write(mapLocation+"/"+mapname);
        writer.newLine();
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
        
        ProcessBuilder builder = new ProcessBuilder("./finalfileviewer");

        builder.redirectInput(new File(DataFilePathToWrite));
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

    @Override
    public String testingRead() {
        String result = "";
        ObjectMapper objectMapper = new ObjectMapper();
    
        try {
            String resultFilePath = fileDirectoryToUse + "output.txt";
            result = new String(Files.readAllBytes(Paths.get(resultFilePath)));
    
            // Parse the result into a List of Coordinates
            List<DataToSend> coordinates = objectMapper.readValue(result, new TypeReference<List<DataToSend>>() {});
    
            // Example: Print the coordinates
            for (DataToSend coordinate : coordinates) {
                System.out.println("xCor: " + coordinate.getXCor() + ", yCor: " + coordinate.getYCor());
            }
    
        } catch (Exception e) {
            e.printStackTrace();
        }
    
        return result;
    }
}
