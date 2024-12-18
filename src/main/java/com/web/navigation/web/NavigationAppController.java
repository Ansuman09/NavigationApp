package com.web.navigation.web;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import com.fasterxml.jackson.databind.ObjectMapper;

import com.web.navigation.entity.DataToSend;
import com.web.navigation.service.NavigationService;

import jakarta.servlet.http.HttpServletRequest;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

@CrossOrigin("http://localhost:3000")
@RestController
@RequestMapping("/api/navigation")
public class NavigationAppController {
    
    @Autowired
    NavigationService navigationService;

    @PostMapping("/findpath/{mapname}")
    public ResponseEntity<String> getMethodName(@RequestBody List<DataToSend> points,@PathVariable String mapname) {
        DataToSend startPoint=points.get(0);
        DataToSend endPoint=points.get(1);

        for (DataToSend point : points){
            System.out.println(point.getXCor());
            System.out.println(point.getYCor());
        }
        String fileExecution=navigationService.executeNavigation(startPoint, endPoint,mapname);
        return new ResponseEntity<>(fileExecution,HttpStatus.OK);
    }

    @GetMapping("/getval")
    public ResponseEntity<String> testingread() {
        
        return new ResponseEntity<>(navigationService.testingRead(),HttpStatus.OK);
    }
        
}
