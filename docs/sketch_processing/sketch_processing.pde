/*
    hoverboard-sidebboard-hack MPU6050 IMU - 3D Visualization Example
    Copyright (C) 2020-2021 Emanuel FERU
*/
import processing.serial.*;
import java.awt.event.KeyEvent;
import java.io.IOException;

Serial myPort;
float roll, pitch,yaw;
int idx = 0;
int inBytePrev;
short bufWord;  

void setup() {
  size (1400, 800, P3D);    
  printArray(Serial.list());                 // List all the available serial ports    
  myPort = new Serial(this, "COM5", 38400);   // starts the serial communication

}

void draw() {

  while (myPort.available() > 0) {      
    int inByte = myPort.read();
    bufWord = (short)(inBytePrev | (inByte << 8));
    idx++;      
    if(bufWord == -21846) {     // check START_FRAME = 0xAAAA
      idx = 0;         
    }
    if (idx == 2) {
       roll   = float(bufWord) / 100;
    }
    if (idx == 4) {
       pitch  = float(bufWord) / 100;
    }
    if (idx == 6) {
       yaw   = float(bufWord) / 100;
    }      
    inBytePrev = inByte;      
  }
    
  // println(bufWord); //<>//

  translate(width/2, height/2, 0);
  background(51);
  textSize(22);
  text("Roll: " + roll + "     Pitch: " + pitch + "     Yaw: " + yaw, -200, 300);
  
  // Rotate the object
  rotateX(radians(roll));
  rotateZ(radians(-pitch));
  rotateY(radians(yaw));
  
  // 3D 0bject
  
  // Draw box with text
  fill(35, 133, 54);     // Make board GREEN
  box (426, 30, 220);
  textSize(25);
  fill(255, 255, 255);
  text("MPU-6050 DMP DEMO", -150, 10, 111);
  
  // Add other boxes
  translate(-70, -18, -30);
  fill(100, 100, 100);
  box (20, 5, 20);     // MPU-6050
  
  translate(70, 0, 0);
  box (40, 5, 40);     // GD32
  
  translate(0, 0, -70);
  fill(255, 255, 255);
  box (50, 40, 15);     // USART Main
  
  translate(0, 0, 200);
  box (50, 40, 15);     // Color Led connector
  
  translate(-60, 0, 0);
  box (40, 40, 15);     // Blue Led connector

}
