#include <Wire.h>
#include <SD.h>
const int chipSelect = 8;
#define FILE "log.txt"
String completeData = "";
String cmd ;
int dist=0,speed=0;
int cmdNo = 0;
int k=0;
int pointer;
bool backtrack = false;
bool flag = false;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  SDinit();
   //dist = 50;
  //speed = 70;
  flag = true;
  SDwrite();
  flag = false;
}

void loop() {
  if(backtrack == false){
    cmdFromPi();
    SDwrite();
    flag = false;
    //comToAr();
  }else{
    Serial.println("ENter backtrack");
    SdLastPointer();
    Serial.println(cmdNo);
    while(cmdNo != 1){
      
      SDBack();
      delay(1000);
    //  comToAr();
    }
    if(cmdNo == 1){
      Serial.println("FINISH");
      delay(1000000);
    }
    delay(1000);
  }
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
  if(flag == false){
    return;
  }
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
  cmdNo = cmdNo + 1;
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
    }else if(cmd == "<"){
      return 1000;
    }else if(cmd == "M" || cmd == "m"){
      return 1001;
    }
}
void SDread(){
  int i=0;
  long value =0;
   byte readByte = 0;
 completeData = "";
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
//     Serial.print("k is: ");
//     Serial.println(k);
     Serial.print(cmdNo);
     Serial.print(", ");
     Serial.print(cmd);
    Serial.print(",");
     Serial.print(dist);
    Serial.print(",");
     Serial.println(speed); 
     readByte = 0;
     completeData = "";

}
void SdLastPointer(){
  int i=0;
  int value =0;
   byte readByte = 0;
  completeData = "";
  String temp;
  File dataFile = SD.open(FILE, FILE_READ);
     if (dataFile){   
        // File available, read file until no data available
        while(dataFile.available()) {
          readByte = dataFile.read();
          completeData += (char)readByte;
        }
        dataFile.close();
     }
  while(value != 1000){
  int j= pointer;
    while(1){
          if(completeData.charAt(j) == ',' || completeData.charAt(j) == '\n'){
            break;
          }
          j = 1 + j;
        }
   temp = completeData.substring(pointer,j);
   value = temp.toInt();
   pointer = j+ 1;
  }
  while(completeData.charAt(pointer) != '\n'){
    pointer = pointer - 1;
  }
  pointer = pointer - 1;
//  Serial.print("pointer is: "); Serial.println(pointer);
//  Serial.print("value at pointer is: "); Serial.println(completeData.charAt(pointer));
  
}
void SDBack(){
    int i=0;
  int value =0;
   byte readByte = 0;
  String temp;
  flag = true;
  while(i!=4){
    int j = pointer;
    while(1){
          if(completeData.charAt(j) == ',' || completeData.charAt(j) == '\n'){
            break;
          }
          j = j - 1;
        }
      temp = completeData.substring(j+1,pointer+1);
      value = temp.toInt();
       if(i == 0){
            speed = value;
          }
          if(i == 1){
            dist = value;
          }
          if(i == 2){
            cmd = value;
          }
          if(i == 3){
            cmdNo = (int)value;
          }
   i = 1 + i;     
  pointer = j - 1;
//    Serial.print("pointer after a loop is: "); Serial.println(pointer);
//  Serial.print("value at pointer is: "); Serial.println(completeData.charAt(pointer));
  }
     Serial.print(cmdNo);
     Serial.print(", ");
     Serial.print(cmd);
    Serial.print(",");
     Serial.print(dist);
    Serial.print(",");
     Serial.println(speed);
}

void cmdFromPi(){
        if (Serial.available() > 0) {
                flag = true;
                // read the incoming byte:
                String incomingByte = Serial.readString();
                String temp = incomingByte.substring(0,1);
                Serial.println(temp);
                cmd = temp;
                if(temp == "S" || temp =="s" ||temp =="G" || temp =="g" || temp == "M" || temp == "m" || temp == "<"){
                  if(temp == "<"){
                    backtrack = true;
                  }
                  return;
                }
                int j=2,k=2;
                for(int i=0;i<2;i++){
                  while(1){
                    if(incomingByte.charAt(j) == ','){
                      break;
                    }
                    j = 1 + j;
                  }
                  temp = incomingByte.substring(k,j);
                  int num = temp.toInt();
                  if(i == 0){
                    dist = num;
                  }else if(i == 1){
                    speed = num;
                  }
                  
                  j = j + 1;
                  k=j;
                }
                Serial.println(dist);
                Serial.println(speed);
                
                //Serial.println(incomingByte);
        }
}

//void comToAr(){
//  if (flag == true && backtrack == false) {
//    if (cmd == "s" || cmd == "S") {
//        stop();
//    } else if (cmd == "G" || cmd == "g" ){
//        sendStatus();
//    } else if (cmd == "F" || cmd == "f"){
//        forward((float) dist, (float) speed);
//        
//    } else if (cmd == "B" || cmd == "b"){
//        reverse((float) dist, (float) speed);
//    } else if (cmd == "R" || cmd == "r"){
//        right((float) dist, (float) speed);
//    } else if (cmd == "L" || cmd == "l"){
//       left((float) dist, (float) speed);
//    } else if (cmd == "M" || cmd == "m"){
//        stop();
//        // blink LED 
//     }
//    SDwrite(); // Save to file on SDCard
//    // todo : mark, clr
//    flag = false;
// 
//}

