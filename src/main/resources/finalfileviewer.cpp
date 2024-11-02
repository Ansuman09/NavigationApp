#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;


typedef pair<int,int> Pair;
typedef pair<double,pair<int,int>> pPair;

struct cell{
    int parent_i,parent_j;

    double f,g,h;
};

struct hash_pair{
    template <class T1,class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto hash1=hash<T1>{}(p.first);
        auto hash2=hash<T2>{}(p.second);

        return hash1^hash2;
    }
};

double distanceBetweenPoints(Pair pointA,Pair pointB){
    return sqrt(pow((pointA.first-pointB.first),2)+pow(pointA.second-pointB.second,2));
    
}

bool samePair(Pair p,Pair q){
    if(p.first==q.first && p.second==q.second){
        return true;
    }
    else{
        return false;
    }
};

vector<Pair> getNeighbours(Pair &pair,Pair *allPairs,int sizeOfAllPairs){
    vector<Pair> resultNeighbours;
    for (int i=0;i<sizeOfAllPairs;i++){
        Pair possiblePair=allPairs[i];
                
        if (!samePair(pair,possiblePair) && (distanceBetweenPoints(pair,possiblePair)<=1.45 )){
            resultNeighbours.push_back(possiblePair);
        }
    }
    return resultNeighbours;
};

void mapResultSet(unordered_map<Pair,cell,hash_pair> cellDetails,Pair dest,cv::Mat &image){
    int row=dest.first;
    int col=dest.second;

    std::ofstream outputFile("output.txt");


    stack<Pair> path;
    while (!(cellDetails[make_pair(row,col)].parent_i==row && cellDetails[make_pair(row,col)].parent_j==col)){
        path.push(make_pair(row,col));
        
        //process the parents
        int new_row=cellDetails[make_pair(row,col)].parent_i;
        int new_col=cellDetails[make_pair(row,col)].parent_j;
        row=new_row;
        col=new_col;
    };

    Pair prev=path.top();

        if (outputFile.is_open()){
            outputFile << "[";
        while (!path.empty()){
        Pair p = path.top();

        //draw path by joining nodes
        cv::line(image,cv::Point(prev.first,prev.second),cv::Point(p.first,p.second),cv::Scalar(234,0,0),2,cv::LINE_AA);

        //draw nodes leading to destination
        cv::Point currPoint(p.first,p.second);
        
        prev=p;        
        // cout<<i<<","<<j<<"\n";
        int radius=2;
        cv::Scalar color(0,255,0);
        int thickness=-1;

        cv::circle(image,currPoint,radius,color,thickness);
        path.pop();
        //map path
        char buffer[100];
        if (path.empty()){
            sprintf(buffer, "{\"xcor\" : %d, \"ycor\" : %d}", p.first, p.second);
            std::string path(buffer);
            outputFile << path;
            continue;
        }
        sprintf(buffer, "{\"xcor\" : %d, \"ycor\" : %d},", p.first, p.second);
        std::string path(buffer);
        outputFile << path;
        
    }
        outputFile<<"]";
        outputFile.close();
    
    }
    else{
        cout<<"Error opening file\n";
    }
    
};

//check if destignation is reached
bool reachedDestigntaion(Pair src,Pair dest){
    if (src.first==dest.first && src.second==dest.second){
        return true;
    }
    return false;
}


void aStarSearch(Pair start,Pair dest,unordered_map<Pair,vector<Pair>,hash_pair> umap,vector<Pair> listOfNodePairs,cv::Mat &image){

    
    //initialize closed set 
    unordered_map<Pair,bool,hash_pair> closedList;

    //initialize cellDetails
    unordered_map<Pair,cell,hash_pair> cellDetails;

    for(Pair currPair:listOfNodePairs){
        // Pair currPair=totalPairs[i];

        cellDetails[currPair].parent_i=-1;
        cellDetails[currPair].parent_j=-1;
        cellDetails[currPair].f=FLT_MAX;
        cellDetails[currPair].g=FLT_MAX;
        cellDetails[currPair].h=FLT_MAX;


        closedList[currPair]=false;
    }

    //initialize openset;
    set<pPair> openSet;

    openSet.insert(make_pair(0,make_pair(start.first,start.second)));
    
    //intialize start node
    cellDetails[start].parent_i=start.first;
    cellDetails[start].parent_j=start.second;
    cellDetails[start].f=0.0;
    cellDetails[start].g=0.0;
    cellDetails[start].h=0.0;

    while (!openSet.empty()){
        pPair currElement=*openSet.begin();

        Pair currPair=currElement.second;
        int currf=currElement.first;

        openSet.erase(openSet.begin());

        closedList[currPair]=true;

        for (Pair nextPair:umap[currPair]){

            if (reachedDestigntaion(nextPair,dest)){
                cellDetails[nextPair].parent_i=currPair.first;
                cellDetails[nextPair].parent_j=currPair.second;
                cout<<"Destination is reached \n";
                mapResultSet(cellDetails,dest,image);
                return;
            }

            else if (closedList[nextPair]==false){
                float fNew,gNew,hNew;

                gNew=cellDetails[currPair].f+distanceBetweenPoints(currPair,nextPair);
                hNew=distanceBetweenPoints(nextPair,dest);
                fNew=gNew+hNew;

                if (cellDetails[nextPair].f==FLT_MAX || cellDetails[nextPair].f>fNew){
                    openSet.insert(make_pair(fNew,make_pair(nextPair.first,nextPair.second)));
                
                    cellDetails[nextPair].parent_i=currPair.first;
                    cellDetails[nextPair].parent_j=currPair.second;
                    cellDetails[nextPair].f=fNew;
                    cellDetails[nextPair].g=gNew;
                    cellDetails[nextPair].h=hNew;
                }
            }   

        }
        
    }
    cout << "Path not found";
    return;

};

void onMouse(int event,int x,int y, int flags, void* param){
    cv::Mat &matImage=*((cv::Mat*)param);

    if(event==cv::EVENT_LBUTTONDBLCLK){
        short val=matImage.at< short >(y,x);
        cout << "x=" << x << "y="<< y <<"val="<<val<<endl;
    }
};




int main(){
    string filePath;
    cout<<"Where is the file ?\n";
    cin>>filePath;

    cv::Mat image= cv::imread(filePath);
    if (image.empty()) {
        std::cout << "Could not read the image" << std::endl;
        return 1;
    }
    //resize image to 350/350
    cv::Mat resizedImage;

    cv::resize(image,resizedImage,cv::Size(350,350));

    cv::Mat blurredImage;
    
    
    cv::GaussianBlur(resizedImage,blurredImage,cv::Size(3,3),0);
    cv::Mat edgeDetect;
    cv::Canny(blurredImage,edgeDetect,50,150);

    vector<cv::Vec4i> linesP;

    cv::HoughLinesP(edgeDetect,linesP,1,CV_PI/180,50,50,10);

    // for (size_t i=0;i<linesP.size();i++){
    //     cv::Vec4i l=linesP[i];
    //     cv::line(image,cv::Point(l[0],l[1]),cv::Point(l[2],l[3]),cv::Scalar(0,0,234),2,cv::LINE_AA);
    // } 

    vector<Pair> inBetweenLines;
 
    //plotting points
    for (size_t i=0;i<linesP.size();i++){
        cv::Vec4i l=linesP[i];

        //break the line to 100 nodes

        for(int i=1;i<20;i++){
            float t=(i/100.0f)*5;

            int xCor = static_cast<int>(l[0] + t * (l[2] - l[0]));
            int yCor = static_cast<int>(l[1] + t * (l[3] - l[1]));
        
            inBetweenLines.push_back(make_pair(xCor,yCor));
            
            
        }
        
        inBetweenLines.push_back(make_pair(l[0],l[1]));
        inBetweenLines.push_back(make_pair(l[2],l[3]));

    }
    
    // for (auto currPair:inBetweenLines){
    //     int i=currPair.first;
    //     int j=currPair.second;

    //     cv::Point currPoint(i,j);
    //     // cout<<i<<","<<j<<"\n";
    //     int radius=2;
    //     cv::Scalar color(0,255,0);
    //     int thickness=-1;

    //     cv::circle(resizedImage,currPoint,radius,color,thickness);
    // }
    
    unordered_map<Pair,vector<Pair>,hash_pair> umapForNodeAndNeighbors;
    //intialisze start
    Pair userStart;
    cout << "What is the start X cor?\t";
    cin >> userStart.first;
    cout << "What is the start Y cor?\t";
    cin >> userStart.second;

    

    // userStart(make_pair(51,91));
    
    cv::circle(resizedImage,cv::Point(userStart.first,userStart.second),3,cv::Scalar(255,0,0),3);
    double minStartNodeToAvailableNodeDist=FLT_MAX;

    //initialize to dest
    Pair dest;
    // make_pair(175,224)
    cout << "What is the Dest X cor?\t";
    cin >> dest.first;
    cout << "What is the Dest Y cor?\t";
    cin >> dest.second;

    
    cv::circle(resizedImage,cv::Point(dest.first,dest.second),3,cv::Scalar(255,0,0),3);
    double minDestNodeToAvailableNodeDist=FLT_MAX;
    
    bool assignedDest=false;
    for (auto currPair:inBetweenLines){
        vector<Pair> neighBourNodeList;
        for (auto possibleNeihbourNodePair:inBetweenLines){
            //accept as neigh if next node in reachable distance
            if (distanceBetweenPoints(currPair,possibleNeihbourNodePair)<15 && !samePair(currPair,possibleNeihbourNodePair)){
                neighBourNodeList.push_back(possibleNeihbourNodePair);
            }
        }

        double startNodeToAvailNodeDistance=distanceBetweenPoints(currPair,userStart);
        if (startNodeToAvailNodeDistance<10 && minStartNodeToAvailableNodeDist>startNodeToAvailNodeDistance){
            cout<<"executed"<<currPair.first<<","<<currPair.second<<endl;
            userStart=currPair;
            minStartNodeToAvailableNodeDist=startNodeToAvailNodeDistance;
        }

        double destNodeToAvailNodeDistance=distanceBetweenPoints(currPair,dest);
        if (destNodeToAvailNodeDistance<20 && minDestNodeToAvailableNodeDist>destNodeToAvailNodeDistance){
            dest=currPair;
            // assignedDest=true;
            minDestNodeToAvailableNodeDist=destNodeToAvailNodeDistance;
        }
        umapForNodeAndNeighbors[currPair]=neighBourNodeList;
    }


    std::cout<<userStart.first<<","<<userStart.second<<std::endl;
    std::cout<<dest.first<<","<<dest.second<<std::endl;

    aStarSearch(userStart,dest,umapForNodeAndNeighbors,inBetweenLines,resizedImage);
    std::cout<<"execution completed"<<endl;
    // cv::imshow("edged image",edgeDetect);
    // cv::imshow("image line detected",resizedImage);
    // cv::setMouseCallback("edged image",onMouse,&image);
    // cv::waitKey(0);

    return 0;
}
