#define CUSTOM_SETTINGS
#define INCLUDE_INTERNET_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#include <OneSheeld.h>
#
// declaring an object from the HttpRequest with the URL of thing speak api
HttpRequest request ("https://api.thingspeak.com/channels/227743/fields/1.json?results=2");
HttpRequest request2("https://api.thingspeak.com/update.jsonapi_key=NSRVLUSDDWKJ4X4Efield1=73");
 
void setup()
{
  OneSheeld.begin();
  //request.addParameter("key","NSRVLUSDDWKJ4X4E"); // add your API key
  Internet.setOnError(&onInternetError);
}
 
void loop()
{
 int five = 5;     // convert from int to char array
  // add the value of the temperature as the other parameter for the URL
 // request.addParameter("field1",five);
 
  //Internet.performGet(request);                // Perform get to the request
  Internet.performGet(request2);
  Internet.performGet(request);
  Internet.performPost(request2);
  OneSheeld.delay(10000);                      // 10 sec delay between each reading and the other.
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

