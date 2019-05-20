#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <opencv2/opencv.hpp>
#include <QSerialPort>
#include <QtDebug>
using namespace cv;
using namespace std;

CascadeClassifier fireCascade;
QSerialPort serial;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_monitor_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_SelectImage_clicked()
{
    mFilename=QFileDialog::getOpenFileName();
    QPixmap pic(mFilename);
    int w=ui->image_display->width();
    int h=ui->image_display->height();
    ui->image_display->setPixmap(pic.scaled(w,h,Qt::KeepAspectRatio));
}

void MainWindow::on_process_clicked()
{

    serial.setPortName("COM4");
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.open(QIODevice::ReadWrite);

    Mat img,img_gray;
    vector<Rect>fire;

    //-- 1. Load the cascades
    if( !fireCascade.load("cascade.xml"))
    {
        printf("--(!)Error loading\n");
        //        return -1;
    }
    while(!serial.isOpen()) serial.open(QIODevice::ReadWrite);
    if (serial.isOpen() && serial.isWritable())
    { qDebug() << "Serial is open";

        img=imread(mFilename.toStdString());
        //    imshow("input",img);waitKey(20);
        cv::resize(img, img,Size(400,300));
        //imshow("india",img);//waitKey(0);

        cvtColor(img, img_gray, COLOR_RGB2GRAY);
        equalizeHist(img_gray, img_gray);

        fireCascade.detectMultiScale( img_gray, fire, 1.1, 9);
        int j,max_width = 0;

        for(int index = 0 ; index < fire.size() ; index++)
        {
            if(fire[index].width > max_width){
                j = index;
                max_width = fire[j].width;
            }
        }


        //  rectangle(img,fire[j],Scalar(0,0,255),2);

        //imshow("fire",img);
        //        waitKey(0);

        Mat thr=img(fire[j]);
        //    imshow("cropped",thr);
        inRange( thr,Scalar(2,200,240),Scalar(110,255,255),thr);
        //    imshow("fired",thr);
        //         waitKey(0);
        Mat white;
        findNonZero(thr,white);
        cout<<"rows"<<white.rows<<endl;
        float perc= (white.rows/float(thr.rows*thr.cols))*100;
        cout<<"percentage"<<perc<<endl;
        string fires="Fire Detected";
        //        QTextToSpeech *m_speech = new QTextToSpeech();
        if(perc>=0.01)
        {
            cv::putText(img, fires, cv::Point(20, 50), cv::FONT_HERSHEY_COMPLEX_SMALL,3, cv::Scalar(0, 255, 0), 1, CV_32S);
            //            m_speech->say("fire ahead");
            ui->lineEdit->QLineEdit::setText("Fire Detected");

            cout<<"fireDetected"<<endl;
            serial.write("1");

        }
        else {
            ui->lineEdit->QLineEdit::setText("No Fire Detected");
        }

        //    imshow("final",img);
        waitKey(0);
    }
}

void MainWindow::on_video_clicked()
{
    mFilename2=QFileDialog::getOpenFileName();
    QPixmap pic(mFilename2);


    serial.setPortName("COM4");
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.open(QIODevice::ReadWrite);

    Mat img,img_gray;
    vector<Rect>fire;

    //-- 1. Load the cascades
    if( !fireCascade.load("cascade.xml"))
    {
        printf("--(!)Error loading\n");
        //        return -1;
    }


    while(!serial.isOpen()) serial.open(QIODevice::ReadWrite);
    if (serial.isOpen() && serial.isWritable())
    { qDebug() << "Serial is open";

    VideoCapture vid(mFilename2.toStdString());
    while (vid.read(img)) {

        cv::resize(img, img,Size(400,300));
        //imshow("india",img);//waitKey(0);

        cvtColor(img, img_gray, COLOR_RGB2GRAY);
        equalizeHist(img_gray, img_gray);

        fireCascade.detectMultiScale( img_gray, fire, 1.1, 9);
        int j,max_width = 0;

        for(int index = 0 ; index < fire.size() ; index++)
        {
            if(fire[index].width > max_width){
                j = index;
                max_width = fire[j].width;
            }
        }


        //  rectangle(img,fire[j],Scalar(0,0,255),2);

        //imshow("fire",img);
        //        waitKey(0);

        Mat thr=img(fire[j]);
        //        imshow("cropped",thr);
        inRange( thr,Scalar(2,200,240),Scalar(110,255,255),thr);
        //    imshow("fired",thr);
        //         waitKey(0);
        Mat white;
        findNonZero(thr,white);
        cout<<"rows"<<white.rows<<endl;
        float perc= (white.rows/float(thr.rows*thr.cols))*100;
        cout<<"percentage"<<perc<<endl;
        string fires="Fire Detected";
        if(perc>=0.01)
        {
            cv::putText(img, fires, cv::Point(20, 50), cv::FONT_HERSHEY_COMPLEX_SMALL,3, cv::Scalar(0, 255, 0), 1, CV_32S);

            cout<<"fireDetected"<<endl;
            serial.write("1");
            serial.waitForBytesWritten(1000);
        }
        else {
            cout<<"No Fire Detected"<<endl;
        }
        imshow("final",img);
        waitKey(10);
    }
        }

}
