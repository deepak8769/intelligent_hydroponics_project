#include <Camera.h>
#include <SDHCI.h>
#include <stdio.h>

int out_width =128;
int out_height  =212;
 int lefttop_x  =90;
int lefttop_y  =5;
 int rightbottom_x =217;
 int rightbottom_y =216;
 
int take_picture_count =0;
int reference_height  = 0; 
int Height=0;
float ACY=0;
bool booli  = true;

int reference = 0;
int count=0;
bool Light_status= false;
bool I_turned_on= false;

#define BLYNK_TEMPLATE_ID "TMPLLKKDe96S"
#define BLYNK_TEMPLATE_NAME "Intelligent Hydroponics"
#define BLYNK_AUTH_TOKEN "LsnVHD63CAEbiD_e1wL6zd0xezPbJrBR"
#define BLYNK_PRINT Serial
#define ESP8266_BAUD 115200
//
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(12,13);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "4GWIFI_69976";
char pass[] = "12345678";

ESP8266 wifi(&EspSerial);
BlynkTimer timer;

SDClass  theSD;
unsigned createMask(unsigned a, unsigned b)
{
   unsigned r = 0;
   for (unsigned i=a; i<=b; i++)
       r |= 1 << i;
  return r;
}
void printError(enum CamErr err)
{
  Serial.print("Error: ");
  switch (err)
    {
      case CAM_ERR_NO_DEVICE:
        Serial.println("No Device");
        break;
      case CAM_ERR_ILLEGAL_DEVERR:
        Serial.println("Illegal device error");
        break;
      case CAM_ERR_ALREADY_INITIALIZED:
        Serial.println("Already initialized");
        break;
      case CAM_ERR_NOT_INITIALIZED:
        Serial.println("Not initialized");
        break;
      case CAM_ERR_NOT_STILL_INITIALIZED:
        Serial.println("Still picture not initialized");
        break;
      case CAM_ERR_CANT_CREATE_THREAD:
        Serial.println("Failed to create thread");
        break;
      case CAM_ERR_INVALID_PARAM:
        Serial.println("Invalid parameter");
        break;
      case CAM_ERR_NO_MEMORY:
        Serial.println("No memory");
        break;
      case CAM_ERR_USR_INUSED:
        Serial.println("Buffer already in use");
        break;
      case CAM_ERR_NOT_PERMITTED:
        Serial.println("Operation not permitted");
        break;
      default:
        break;
    }
}

int is_button_on=0;
BLYNK_WRITE(V20)
{
   is_button_on= param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
}
BLYNK_WRITE(V19)
{
   reference_height= param.asInt();
   booli = true;// assigning incoming value from pin V1 to a variable

  // process received value
}
BLYNK_WRITE(V9)
{
   int a = param.asInt();
   if (a==1){
    Light_status = true;
   }
   else {
    Light_status=false;
   }
 

}
BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V9);  // will cause BLYNK_WRITE(V0) to be executed
}



void CamCB(CamImage img)
{
//   Blynk.run();
 
  

  if (!img.isAvailable()) {Serial.println("Image is not available. Try again");return;}
  CamImage small;
  CamErr err = img.clipAndResizeImageByHW(small, lefttop_x, lefttop_y,
                    rightbottom_x, rightbottom_y, out_width, out_height);
  if (!small.isAvailable())
  {Serial.println("Error in .clipAndResizeImageByHW: " + String(err)); return;}
  

  if (I_turned_on==true){
    Blynk.virtualWrite(V9,0);
    I_turned_on=false;
  }

float R = 0; 
  float G =0;
  float B = 0;
  float Y = 0;
  
  int n = 0;


  Blynk.syncVirtual(V9);  // will cause BLYNK_WRITE(V0) to be executed
  delay(500);
              
  CamErr error1 = small.convertPixFormat(CAM_IMAGE_PIX_FMT_RGB565);
  if (small.getPixFormat()!=CAM_IMAGE_PIX_FMT_RGB565){
  }
  uint16_t* imgbuf = (uint16_t*)small.getImgBuff(); 
   

    Serial.print(small.getImgSize(), DEC);
      Serial.print(" , ");

      Serial.print("buff size= ");
      Serial.print((unsigned long)small.getImgBuffSize(), DEC);
      Serial.println("");

      Serial.println(out_height);
      Serial.println(out_width);
      String image_arr[out_height];


      int I = 0; 
      int J = 0;

int Min_i = 1000;
unsigned b = createMask(0,4);
unsigned blue_value   ;
          
          unsigned g =createMask(5,10) ;
          unsigned green_value ;
          unsigned r= createMask(11,15) ;
          unsigned red_value;
   byte red =0;
   byte green=0;
   byte blue  =0;

if (reference==0){
  Serial.println("in the referencee");
  if (is_button_on==1){
    String temp= "";
    for (n = 0; n < out_height*out_width; ++n) {
          blue_value = (b & imgbuf[n]);
          green_value = (g & imgbuf[n])>>5;
          red_value = (r & imgbuf[n])>>11;
          red   = red_value << 3;
          green = green_value << 2;
          blue  = blue_value << 3;

          R = (float)red/255;
          G= (float)green/255;
          B = (float)blue/255;
          Y = B*B + 3*R*R - 4*G*G; // tolerence function
          if (Y < 0.8  )
          {
            temp+='0';
          }
          else{
            temp+='1';
          }

    if (J==out_width-1){
      J = 0;
      image_arr[I] = temp;
      temp="";
      I++; 
    }
    else{     
        J++;
    }   
}
Serial.println("checking for reference");
delay(2000);


for (int j =0 ;j<out_width ;j++){
    for (int i = 5 ;i<out_height;i++){

            if (image_arr[i][j]=='0' ){
                  int z = i;
                  for (int a =i ; a<i+4 && a<out_height;a++){
                    if (image_arr[a][j]==1){
                              i=a;
                              break;}
                    z = a;
                  }
                        
                  if (z==i+3){
                        Min_i = min(Min_i,i);
                        break;}
      
    }
            
  
}
}
reference_height = Min_i;
Blynk.virtualWrite(V19, reference_height);

Blynk.virtualWrite(V20,0);
booli=true;

  }
  else {

    
    Serial.println("Taking the real_reference");
   
    if (Blynk.connected())
{
  Blynk.syncVirtual(V19);  // will cause BLYNK_WRITE(V0) to be executed
}
else {
  booli=false;
}
delay(2000);
Serial.println("takedn from server ");
Serial.println(reference_height);

  }
  reference++;
  

  

   
    
}

  else {
  Serial.println("in the else");
  for (n = 0; n < out_height*out_width; ++n) {


    String temp= "";
    for (n = 0; n < out_height*out_width; ++n) {
          blue_value = (b & imgbuf[n]);
          green_value = (g & imgbuf[n])>>5;
          red_value = (r & imgbuf[n])>>11;
          red   = red_value << 3;
          green = green_value << 2;
          blue  = blue_value << 3;

          R = (float)red/255;
          G= (float)green/255;
          B = (float)blue/255;
          Y = B*B + 3*R*R - 4*G*G; // tolerence function
          if (Y < 0.8  )
          {
            temp+='0';
          }
          else{
            temp+='1';
          }

    if (J==out_width-1){
      J = 0;
      image_arr[I] = temp;
      temp="";
      I++; 
    }
    else{     
        J++;
    }   
}
                       
for (int j =0 ;j<out_width ;j++){
    for (int i = 5 ;i<out_height;i++){

            if (image_arr[i][j]=='0' ){
                  int z = i;
                  for (int a =i ; a<i+4 && a<out_height;a++){
                    if (image_arr[a][j]==1){
                              i=a;
                              break;}
                    z = a;
                  }
                        
                  if (z==i+3){
                        Min_i = min(Min_i,i);
                        break;}
      
    }
            
  
}
}
                          
       }
  }
            Serial.print("I am printing the reference value: ");
            Serial.print(reference_height);
            Serial.println(" ");
            Height = Min_i-reference_height;
            ACY = -0.1492*Height+23;
            Serial.print("I am printing the Height Value: ");
            Serial.print(ACY);
            Serial.println(" " );
            if (booli){
              if (Light_status){
              Blynk.virtualWrite(V13,ACY);
              }
            }
            

            
            
            if (take_picture_count<7){
              
              delay(5000);
              Serial.println("Going to next picture");
            }
            else {
              
             
              count=0;
              for (int i =0 ; i<50;i++){
                Blynk.run();
                
                
                delay(6000);
                count++;
                Blynk.virtualWrite(V18,count);
              }
            }
            take_picture_count++;
            Blynk.virtualWrite(V17,take_picture_count);
            if (booli){
             
              Blynk.virtualWrite(V19, reference_height);
              if (!Light_status){
                Blynk.virtualWrite(V15,"Light_is_off");
              }
              else{
                String new_str = "New Lable is:-";
              new_str+=String(reference_height);
              Blynk.virtualWrite(V15,new_str);
              }
              
            
            }
            else {
              Blynk.virtualWrite(V15,"Set the new ref");
            }
            
           
              
               
            
            
            
            
          
            

            

}
void setup() {

  Serial.begin(115200);
  EspSerial.begin(ESP8266_BAUD);///////////////
  Blynk.begin(auth, wifi, ssid, pass);
 CamErr err;
Serial.println("Please Put the reference Scale");
digitalWrite(LED0, HIGH);
delay(3000);
digitalWrite(LED1, HIGH);
delay(3000);
digitalWrite(LED2, HIGH);
delay(3000);
digitalWrite(LED3, HIGH);
delay(3000);
    digitalWrite(LED0, LOW);
    delay(300);
    digitalWrite(LED1, LOW);
    delay(300);
    digitalWrite(LED2, LOW);
    delay(300);
    digitalWrite(LED3, LOW);
    delay(300);
//   Initialize camera
  theCamera.begin();
  theCamera.startStreaming(true, CamCB);
  // confirmation for the call for camera
  Serial.println("Camera is operating fine");
  Serial.println("Set Auto white balance parameter");
  err = theCamera.setAutoWhiteBalanceMode(CAM_WHITE_BALANCE_AUTO);
  if (err != CAM_ERR_SUCCESS){printError(err);}

}

void loop() {
  
Blynk.run();

  Serial.println("In the loop");

sleep(INT_MAX);
}
