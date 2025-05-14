package com.web.navigation.web;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

import com.fasterxml.jackson.databind.ObjectMapper;

import com.web.navigation.entity.DataToSend;
import com.web.navigation.service.NavigationService;

import jakarta.servlet.http.HttpServletRequest;

import java.io.BufferedReader;
import java.io.File;
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

@CrossOrigin("https://localhost:3000")
@RestController
@RequestMapping("/api/navigation")
public class NavigationAppController {
    private static final String UPLOAD_DIR = "/app/uploads/";

    @Autowired
    NavigationService navigationService;

    @PostMapping("/findpath/{mapname}")
    public ResponseEntity<String> getMethodName(@RequestBody List<DataToSend> points, @PathVariable String mapname) {
        DataToSend startPoint = points.get(0);
        DataToSend endPoint = points.get(1);

        for (DataToSend point : points) {
            System.out.println(point.getXCor());
            System.out.println(point.getYCor());
        }
        String fileExecution = navigationService.executeNavigation(startPoint, endPoint, mapname);
        return new ResponseEntity<>(fileExecution, HttpStatus.OK);
    }

    @GetMapping("/getval")
    public ResponseEntity<String> testingread() {

        return new ResponseEntity<>(navigationService.testingRead(), HttpStatus.OK);
    }

    @PostMapping("/upload")
    public ResponseEntity<String> uploadImage(@RequestParam("image") MultipartFile file) {
        try {
            // Create the upload dir if it doesn't exist
            File uploadDir = new File(UPLOAD_DIR);
            if (!uploadDir.exists()) {
                uploadDir.mkdirs();
            }

            // Save the file
            System.out.println(file.getOriginalFilename());
            String filePath = UPLOAD_DIR + file.getOriginalFilename();
            file.transferTo(new File(filePath));

            return new ResponseEntity<>("File uploaded successfully: " + filePath, HttpStatus.OK);
        } catch (IOException e) {
            e.printStackTrace();
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body("Upload failed");
        }
    }

}
