#include <iostream>
#include <cmath>
#include <list>
#include <queue>
#include "trackInit.h"


#define ERODE_SIZE 6
#define MIN_CENTER_DIS 300   // two different lines' minimum center distance
#define MIN_ADJDELTA 30     // two different lines' minimum angle difference
#define PI 3.14159

using namespace std;
using namespace cv;




vector<Point> borderPoint; //border selected by user
Mat perspectiveMatrix;  //for changing perspective
vector<Vec4i> trackLines;
deque<Point> turningPos;

void help()
{
    cout << "\ts - quit the program\n"
         "\ti - initialize the map\n"
         "\tr - run car\n"
         "\tq - finish map\n"
         "To initialize tracking, select the object with mouse\n";
}


int genTrack(VideoCapture& cap){

    Mat orgMap;
    cap >> orgMap;

    //change perspective
    Mat mapProcessed = changePerspective(orgMap);

    //binarize the map
    Mat mapBinarized = binarize(mapProcessed);
    imshow("mapBinarized",mapBinarized);
    cout<<"The binaried map is displayed. Press any key to continue"<<endl;
    waitKey(0);

    //sharpen the track
    Mat mapSharpend = sharpen(mapBinarized);
    imshow("mapSharpend",mapSharpend);
    cout<<"The sharpened map is displayed. Press any key to continue"<<endl;
    waitKey(0);

    //use Hough line transformation to detect the lines
    HoughLinesP(mapSharpend,trackLines,0.5, CV_PI/90,50,50,50);
    drawHoughLines(mapProcessed);

    //analyze the hough lines and  generate the turning point sequence
    //analyze the global trackLines, store the output in turningPos queue
    genTurningPos(mapProcessed); 

    for(int i=0;i<turningPos.size();i++)
        circle(mapProcessed, turningPos[i], 10, Scalar(255,255), 3, 8, 0 );

    imshow("Track lines and turning positions",mapProcessed);

    cout<<"The track lines and turning positions are displayed. Press any key to continue"<<endl;
    waitKey(0);

    return 0;
}


Mat changePerspective(Mat org){
    cout<<"\nPlease mark the border of the track."<<endl;
    cout<<"left top point > right top point > right bottom point > left bottom point\n"<<endl;
    namedWindow("changePerspective",CV_WINDOW_AUTOSIZE); //create a window to show the image
    setMouseCallback("changePerspective",onMouse,0);//call onMouse if the user clicks the mouse
    imshow("changePerspective",org);

    waitKey(0);

    Point2f srcPoints[4];
    Point2f dstPoints[4];

    srcPoints[0] = borderPoint[0];
    srcPoints[1] = borderPoint[1];
    srcPoints[2] = borderPoint[2];
    srcPoints[3] = borderPoint[3];

    int height = org.size().height;
    int width = org.size().width;

    dstPoints[0] = Point(0,0);
    dstPoints[1] = Point(width,0);
    dstPoints[2] = Point(width,height);
    dstPoints[3] = Point(0,height);


    perspectiveMatrix = getPerspectiveTransform(srcPoints,dstPoints);
    Mat finalTrack = org.clone();
    warpPerspective(org,finalTrack,perspectiveMatrix,finalTrack.size(),INTER_LINEAR, BORDER_CONSTANT);

    return finalTrack;
}



void onMouse(int event, int x, int y, int flags, void* param){
    Point pt;

    if(event == CV_EVENT_LBUTTONDOWN){
        pt = Point(x,y);
        cout<<"x:"<<x<<" y:"<<y<<endl;
        borderPoint.push_back(pt);

        if(borderPoint.size()>=4){
            cout<<"Selecting border points finished."<<endl;
            destroyWindow("changePerspective"); //it will expire waitKey(0)
        }
    }
}


Mat binarize(Mat src){
    Mat tmpdst(src.size().height,src.size().width,CV_8U);
    Mat dst(src.size().height,src.size().width,CV_8U);

    cvtColor(src, tmpdst, CV_BGR2GRAY);
    threshold(tmpdst,dst,150, 255, THRESH_BINARY|THRESH_OTSU);

    return dst;
}


Mat sharpen(Mat src){
    Mat tmpdst(src.size().height,src.size().width,CV_8U);
    Mat dst(src.size().height,src.size().width,CV_8U);
    threshold(src, tmpdst, 128, 255, THRESH_BINARY_INV);  //revert the color

    Mat erodeStruct = getStructuringElement(MORPH_RECT,Size(ERODE_SIZE,ERODE_SIZE));
    erode(tmpdst,dst,erodeStruct);
    Canny(dst, dst, 50, 150, 3);  //detect the edges

    return dst;
}


void drawHoughLines(Mat& cdst){
    cout<<"\nDetected "<<trackLines.size()<<" raw track lines"<<endl;
    for( size_t i = 0; i < trackLines.size(); i++ ){
        Vec4i l = trackLines[i];
        line(cdst,Point(l[0], l[1]),Point(l[2],l[3]),Scalar(0,100,100),1,CV_AA);
        }
}



void genTurningPos(Mat& sourceMap){

    int i,minx_point_index,line_num = trackLines.size();
    double minx;

    //trackLines[i][0],[i][2] is x ; [i][1],[i][3] is y
    if(line_num>0){
        minx = trackLines[0][0];
        minx_point_index = 0;
    }

    for(i=0;i<line_num;i++){
        if(trackLines[i][0]<minx){
            minx = trackLines[i][0];
            minx_point_index = i;
        }
        if(trackLines[i][2]<minx){
            minx = trackLines[i][2];
            minx_point_index = i;
        }
    }

    Point startPoint,secondPoint;
    Vec4i currentLine = trackLines[minx_point_index];

    startPoint = Point(currentLine[0],currentLine[1]);
    secondPoint = Point(currentLine[2],currentLine[3]);

    if(startPoint.x>secondPoint.x){
        Point tmp = startPoint;
        startPoint = secondPoint;
        secondPoint = tmp;
    }

    //add the start point to the turningPos queue
    turningPos.push_back(startPoint);

    vector<Vec4i> reducedLines;
    reducedLines.push_back(currentLine);

    for(unsigned i=0; i<trackLines.size(); i++){
        bool same=false;
        for(unsigned j=0; j<reducedLines.size(); j++)
            same += isSameLine(trackLines[i],reducedLines[j]);

        if(!same)
            reducedLines.push_back(trackLines[i]);
    }

    for(i=0;i<reducedLines.size();i++)
        line(sourceMap,Point(reducedLines[i][0], reducedLines[i][1]),Point(reducedLines[i][2],reducedLines[i][3]),Scalar(0,255,255),1,CV_AA);

    cout<<"\nAfter reduction, there are "<<reducedLines.size()<<" trackLines"<<endl;
    Point currentPoint(secondPoint.x,secondPoint.y);

    //erase the first line after current point and current line is saved
    reducedLines.erase(reducedLines.begin());
    int trackNum = reducedLines.size();

    //find the turning point sequence
    for (int j=0;j<trackNum;j++){

        int nextIndex = 0;
        double min_pointDist = xyPointDist(currentPoint.x,currentPoint.y,reducedLines[0][0],reducedLines[0][1]);
        for(i=0;i<reducedLines.size();i++){
            double tmpdst1 = xyPointDist(currentPoint.x,currentPoint.y,reducedLines[i][0],reducedLines[i][1]);
            double tmpdst2 = xyPointDist(currentPoint.x,currentPoint.y,reducedLines[i][2],reducedLines[i][3]);
            if(tmpdst1<min_pointDist){
                min_pointDist = tmpdst1;
                nextIndex = i;
            }
            if(tmpdst2<min_pointDist){
                min_pointDist = tmpdst2;
                nextIndex = i;
            }
        }
        Vec4i nextLine = reducedLines[nextIndex];
        reducedLines.erase(reducedLines.begin()+nextIndex);
        Point intersection = getIntersec(currentLine,nextLine);

        turningPos.push_back(intersection);
        if(xyPointDist(intersection.x,intersection.y,nextLine[0],nextLine[1])<xyPointDist(intersection.x,intersection.y,nextLine[2],nextLine[3])) 
            currentPoint = Point(nextLine[2],nextLine[3]);
        else
            currentPoint = Point(nextLine[0],nextLine[1]);

        currentLine = nextLine;
    }

    //the last point
    turningPos.push_back(currentPoint);
}


bool isSameLine(Vec4i line1,Vec4i line2){
    bool flag1 = false;
    bool flag2 = false;

    double k1 = (line1[3]-line1[1])/(line1[2]-line1[0]);
    double k2 = (line2[3]-line2[1])/(line2[2]-line2[0]);

    double delta = (atan(k1)-atan(k2))*180/PI;
    if(abs(delta)<MIN_ADJDELTA)
        flag1 = true;

    double x1 = (line1[0]+line1[2])/2;
    double y1 = (line1[1]+line1[3])/2;
    double x2 = (line2[0]+line2[2])/2;
    double y2 = (line2[1]+line2[3])/2;
    double dx = x1-x2;
    double dy = y1-y2;
    double distance = sqrt(dx*dx+dy*dy);
    if(distance<MIN_CENTER_DIS)
        flag2 = true;

    return (flag1&&flag2);
}


Point getIntersec(Vec4i l1,Vec4i l2){
    Vec4d l1b = l1;
    Vec4d l2b = l2;
    double k1 = (l1b[3]-l1b[1])/(l1b[2]-l1b[0]);
    double k2 = (l2b[3]-l2b[1])/(l2b[2]-l2b[0]);
    double b1 = l1b[1]-k1*l1b[0];
    double b2 = l2b[1]-k2*l2b[0];

    double ins_x = (b2-b1)/(k1-k2);
    double ins_y = k1*ins_x+b1;

    return Point(ins_x,ins_y);
}


double xyPointDist(double x1,double y1, double x2, double y2){
    double dx = x1-x2;
    double dy = y1-y2;
    return sqrt(dx*dx+dy*dy);
}




void drawCircle(Mat &input, const vector<Vec3f> &circles){
    for(int i=0; i<circles.size(); i++){
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(input, center, radius, Scalar(255,0,0), 3, 8, 0 );
    }
}