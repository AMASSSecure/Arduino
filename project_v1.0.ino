#define CUSTOM_SETTINGS
#define INCLUDE_INTERNET_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_GPS_SHIELD
#define INCLUDE_CAMERA_SHIELD
#define INCLUDE_FACEBOOK_SHIELD
#define INCLUDE_ACCELEROMETER_SENSOR_SHIELD
#define INCLUDE_PHONE_SHIELD
#define INCLUDE_SMS_SHIELD
#define INCLUDE_TWITTER_SHIELD
#define INCLUDE_NFC_SHIELD
/* Include 1Sheeld library. */
#include <OneSheeld.h>

boolean isMessageSent = false;
boolean isPictureSent = false;
boolean isPhoneCall = false;
boolean remoteShutdown = false;
const int ledPin =  13;
/* Create an Http request with openweathermap.org api url. */
HttpRequest request("http://rhino-guard.000webhostapp.com/firebaseTest.php");
HttpRequest request2("http://rhino-guard.000webhostapp.com/requestFire.php");
void setup() 
{  
  /* Start communication. */
  OneSheeld.begin();
  /* LED PIN 13 remote shutdown*/
  pinMode(ledPin, OUTPUT);
  /* Subscribe to success callback for the request. */
  request2.setOnSuccess(&onSuccess);
  /* Subscribe to json value replies. */
  request2.setOnFailure(&onFailure);
  // subscribe 
  //request2.getResponse().setOnNextResponseBytesUpdate(&onBytesUpdate);
  /* Subscribe to response errors. */
  request2.getResponse().setOnError(&onResponseError);
  /* Subscribe to Internet errors. */
  Internet.setOnError(&onInternetError);
 // NFC KEY FOB!
  //NFC.setOnNewTagScanned(&tagFunction);
  // LED
  pinMode(ledPin, OUTPUT);
}

void loop(){   
  request2.addParameter("kill","");
  Internet.performGet(request2); 
  OneSheeld.delay(1000);

  float latitude = GPS.getLatitude();
  char latitudeArray[9];
  dtostrf(latitude,2,7,latitudeArray);
  //snprintf(latitudeArray, 11, "%f", latitude);
  request.addParameter("lat",latitudeArray);
  Terminal.println(latitude);
  float longitude = GPS.getLongitude();
  Terminal.println(longitude);
  char longitudeArray[9];
  dtostrf(longitude,2,7,longitudeArray);
  request.addParameter("long", longitudeArray);
  Internet.performGet(request);
  Terminal.println(longitudeArray);

     
   if(AccelerometerSensor.getX() > 3 )
   {
      if (!isMessageSent)
      {
        // SMS.send("0873256693","Your car is moving and possibly being stolen!");
         isMessageSent = true;
       }
         if(!isPictureSent)
         {
         Camera.setFlash(ON);
         Camera.frontCapture();
         Twitter.tweetLastPicture("Picture of a thief!");
          Facebook.postLastPicture("Picture of a thief");
         isPictureSent = true;
      }
         if(!isPhoneCall){
         Phone.call("0873256693");
         isPhoneCall=true;
         }
     
     request.addParameter("moving","yes");
     Internet.performGet(request);
     
     }else{
        request.addParameter("moving","no");
        Internet.performGet(request);
     }
 
}


void onSuccess(HttpResponse & response)
{
//  if (!strcmp("y",response.getBytes())){
  if(!strstr(response.getBytes(),"y")){
    digitalWrite(ledPin,HIGH);
    Terminal.println("light on");
  }
  else 
  {
     digitalWrite(ledPin,LOW);
     Terminal.println("light off ");
  }
  
  if(!response.isSentFully())
  {       
     // Ask for the next 64 bytes. 
     response.getNextBytes();
  }
}

void onFailure(HttpResponse & response){
  Terminal.println(response.getStatusCode());
  Terminal.println(response.getBytes());
}

void onResponseError(int errorNumber)
{
  /* Print out error Number.*/
  Terminal.print("Response error:");
  switch(errorNumber)
  {
    case INDEX_OUT_OF_BOUNDS: Terminal.println("INDEX_OUT_OF_BOUNDS");break;
    case RESPONSE_CAN_NOT_BE_FOUND: Terminal.println("RESPONSE_CAN_NOT_BE_FOUND");break;
    case HEADER_CAN_NOT_BE_FOUND: Terminal.println("HEADER_CAN_NOT_BE_FOUND");break;
    case NO_ENOUGH_BYTES: Terminal.println("NO_ENOUGH_BYTES");break;
    case REQUEST_HAS_NO_RESPONSE: Terminal.println("REQUEST_HAS_NO_RESPONSE");break;
    case SIZE_OF_REQUEST_CAN_NOT_BE_ZERO: Terminal.println("SIZE_OF_REQUEST_CAN_NOT_BE_ZERO");break;
    case UNSUPPORTED_HTTP_ENTITY: Terminal.println("UNSUPPORTED_HTTP_ENTITY");break;
    case JSON_KEYCHAIN_IS_WRONG: Terminal.println("JSON_KEYCHAIN_IS_WRONG");break;
  }
}

void onInternetError(int requestId, int errorNumber)
{
  /* Print out error Number.*/
  Terminal.print("Request id:");
  Terminal.println(requestId);
  Terminal.print("Internet error:");
  switch(errorNumber)
  {
    case REQUEST_CAN_NOT_BE_FOUND: Terminal.println("REQUEST_CAN_NOT_BE_FOUND");break;
    case NOT_CONNECTED_TO_NETWORK: Terminal.println("NOT_CONNECTED_TO_NETWORK");break;
    case URL_IS_NOT_FOUND: Terminal.println("URL_IS_NOT_FOUND");break;
    case ALREADY_EXECUTING_REQUEST: Terminal.println("ALREADY_EXECUTING_REQUEST");break;
    case URL_IS_WRONG: Terminal.println("URL_IS_WRONG");break;
  }
}
