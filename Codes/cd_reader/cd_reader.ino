#include <Wire.h>
#include <SD.h>
const int chipSelect = 8;
#define FILE "log.txt"
String cmd;
int dist,speed;
int cmdNo = 0;
int k=0;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  SDinit();
   dist = 50;
  speed = 70;
}

void loop() {
  cmd = "s";
  for(int i=0;i<5;i++){
    cmdNo = cmdNo + 1;
    SDwrite();
    speed = speed + 1;
    dist = dist + 1;
  }
  for(int i=0;i<5;i++){
    SDread();
  }
  delay(10000);
  // put your main code here, to run repeatedly:

}

void SDinit(){
  Serial.print("Initialising SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Init failed, or not present");
    return;
  }
  Serial.println("Card initialised.");   
   File dataFile = SD.open(FILE, FILE_WRITE);
  if (SD.exists(FILE)){
    // Delete existing file
    Serial.print(FILE);
    Serial.println(" exists, deleting");
    SD.remove(FILE);
    Serial.println();
  }
  else 
  {
    Serial.print(FILE);
    Serial.println(" not found!");
  }
  dataFile.close();
}
void SDwrite(){
  int temp = comToInt();
File dataFile = SD.open(FILE, FILE_WRITE);
  dataFile.print(cmdNo);
  dataFile.print(",");
  dataFile.print(temp);
  dataFile.print(",");
  dataFile.print(speed);
  dataFile.print(",");
  dataFile.print(dist);
  dataFile.print("\n");
  dataFile.close();
  //Serial.print("Seconds is :");
  //Serial.println(cmdNo);
}
int comToInt(){
    if(cmd == "s" || cmd == "S"){
     return 1; 
    }else if(cmd == "F" || cmd == "f"){
     return 2;   
    }else if(cmd == "B" || cmd == "b"){
     return 3;   
    }else if(cmd == "R" || cmd == "r"){
     return 4;    
    }else if(cmd == "L" || cmd == "l"){
     return 5;
    }
}
void SDread(){
  int i=0;
  long value =0;
   byte readByte = 0;
  String completeData = "";
  File dataFile = SD.open(FILE, FILE_READ);
     if (dataFile){   
        // File available, read file until no data available
        while(dataFile.available()) {
          readByte = dataFile.read();
          completeData += (char)readByte;
        }
        dataFile.close();
     }
     while(i !=4){
      String temp;
      int j=k;
      while(1){
        if(completeData.charAt(j) == ',' || completeData.charAt(j) == '\n'){
          break;
        }
        j = 1 + j;
      }
      temp = completeData.substring(k,j);
      value = temp.toInt();
          if(i == 0){
            cmdNo = (int)value;
          }
          if(i == 1){
            cmd = (String)value;
          }
          if(i == 2){
            dist = value;
          }
          if(i == 3){
            speed = (int)value;
          }
          i = 1 + i;
          k = j + 1;
     }
     Serial.print("k is: ");
     Serial.println(k);
     Serial.print("cmdNO SD : ");
     Serial.println(cmdNo);
     Serial.print("cmd : ");
     Serial.println(cmd);
    Serial.print("dist is : ");
     Serial.println(dist);
    Serial.print("speed is : ");
     Serial.println(speed); 
     readByte = 0;
     completeData = "";

}

