## This is a guide to setup the application in a containerized environment

## frontend for this app is at navigation-app-frontend.

## First compile the java app using maven
### if you have maven installed in your pc use 
mvn clean package

## Use the docker file to build the image. The image will install opencv and we will use that for executing out program.
docker build -t opencv-cpp .

## Now build the container up exposing port 8080 
docker run -p 8080:8080 --name opencv-test opencv-cpp

## Troubleshoot if the finalfileviewer does not execute recompile it inside the container using the below command
#### get into container 
docker exec -it openc-test bash

fileExecution command with opencv4:
g++ finalfileviewer.cpp -o finalfileviewer `pkg-config --cflags opencv4` `pkg-config --libs opencv4`


