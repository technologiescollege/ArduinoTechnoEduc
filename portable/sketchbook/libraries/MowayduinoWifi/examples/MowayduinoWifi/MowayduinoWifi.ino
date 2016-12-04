/*
 * mOwayduinoWifi example library
 */
 
//#define TWITTER_EXAMPLE
#define WEB_SERVER
/* IMPORTANT  */ /* COMMENT #define APP_SOCKAPP and UNCOMMENT #define APP_WISERVER 
in apps-conf.h in arduino/libraries/MowayduinoWifi folder */

//#define SOCKET 
/* IMPORTANT  */ /* UNCOMMENT #define APP_SOCKAPP and COMMENT #define APP_WISERVER 
in apps-conf.h in arduino/libraries/MowayduinoWifi folder 
UNCOMMENT #define socket in socketapp.c  */


#include <MowayduinoRobot.h>
#include <SPI.h>
#include <EEPROM.h>
#include "MowayduinoWifi.h"

#ifdef SOCKET
#include <WiShield.h>
#else
#include <WiServerMoway.h>
#endif

mowayduinorobot Mowayduino;
int move;
int red=0, green=0, blue=0;

#ifdef TWITTER_EXAMPLE

// This function writes tweet when twittermowayduino.submit() is called 
void tweet_write() {
  WiServer.print("status=Mowayduino light sensor: ");
  WiServer.print(Mowayduino.readLight());
}


// Function that prints data from tweet return
void tweet_return(char* data, int len) {
  // Print the data returned by the server
  while (len-- > 0) {
    Serial.print(*(data++));
  } 
}

#endif

#ifdef WEB_SERVER

/******************  Redirect to / when command are sent  ******************/
void redirect()
{
  WiServer.print(HEAD);
  WiServer.print(REDIRECT_META);
  WiServer.print(END_HEAD);
  WiServer.print(END);
}



/******************  HTML Page serving function  ******************/
// This is our page serving function that generates web pages
boolean mowayduinoPage(char* URL) 
{
  // Check if the requested URL matches "/"
  if (strcmp(URL, "/r") == 0) {
    // Use WiServer's print and println functions to write out the page content
    red^=1;
    redirect();
    return true;
  }
  else if (strcmp(URL, "/b") == 0) {
    // Use WiServer's print and println functions to write out the page content
    blue^=1;
    redirect();
    return true;
  }
  else if (strcmp(URL, "/g") == 0) {
    // Use WiServer's print and println functions to write out the page content
    green^=1;
    redirect();
    return true;
  }

  else if (strcmp(URL, "/up") == 0) {
    // Use WiServer's print and println functions to write out the page content
    move=MOVE_STRAIGHT;
    redirect();
    return true;
  }

  else if (strcmp(URL, "/left") == 0) {
    // Use WiServer's print and println functions to write out the page content
    move=MOVE_LEFT;
    redirect();
    return true;
  }
  else if (strcmp(URL, "/right") == 0) {
    // Use WiServer's print and println functions to write out the page content
    move=MOVE_RIGHT;
    redirect();
    return true;
  }
  else if (strcmp(URL, "/down") == 0) {
    // Use WiServer's print and println functions to write out the page content
    move=MOVE_BACK;
    ;
    redirect();
    return true;
  }
  else if (strcmp(URL, "/stop") == 0) {
    // Use WiServer's print and println functions to write out the page content
    move=STOP;
    redirect();
    return true;
  }


  else if (strcmp(URL, "/") == 0)
  {
    WiServer.print(HEAD);
    WiServer.println("<title>Web Server mOwayduino</title>");
    WiServer.println("<link rel=\"stylesheet\" href=\"http://mowayduino.com/server/server.css\">");
    WiServer.println("<base href=\"");
    WiServer.print(EXT_SERVER);
    WiServer.print("\">");

    //Remove line to disable autoupdate
    WiServer.print("<meta http-equiv=refresh content=4>");
    WiServer.print(END_HEAD);
    WiServer.println(BODY);
    WiServer.println("<div><img src=\"logo.png\" hspace=\"10\" vspace=\"10\">");
    WiServer.print("<div id=\"dc\">");
    WiServer.print("<a href=\"");
    WiServer.print(LOCAL_IP);
    WiServer.print("left\"><img src=\"left.jpg\" id=\"left\"></a>");
    WiServer.print("<a href=\"");
    WiServer.print(LOCAL_IP);
    WiServer.print("right\"><img src=\"right.jpg\" id=\"right\"></a>");
    WiServer.print("<a href=\"");
    WiServer.print(LOCAL_IP);
    WiServer.print("down\"><img src=\"back.jpg\" id=\"down\"></a>");
    WiServer.print("<a href=\"");
    WiServer.print(LOCAL_IP);
    WiServer.print("up\"><img src=\"st.jpg\" id=\"up\"></a>");
    WiServer.print("<a href=\"");
    WiServer.print(LOCAL_IP);
    WiServer.print("stop\"><img src=\"stop.jpg\" id=\"stop\"></a></div>");
    WiServer.println("<div id=\"dm\"><img src=\"mOwayduino.png\" id=\"imo\">");
    WiServer.print("<div id=\"s1\">");
    WiServer.print(Mowayduino.Obstacle[IR_LL]);
    WiServer.print(END_DIV);
    WiServer.print("<div id=\"s2\">");
    WiServer.print(Mowayduino.Obstacle[IR_CL]);
    WiServer.print(END_DIV);
    WiServer.print("<div id=\"s3\">");
    WiServer.print(Mowayduino.Obstacle[IR_CR]);
    WiServer.print(END_DIV);
    WiServer.print("<div id=\"s4\">");
    WiServer.print(Mowayduino.Obstacle[IR_LR]);
    WiServer.print(END_DIV);
    WiServer.print("<a href=\"http://192.168.1.25/r\"><img src=\"rd.png\" id=\"imgR\"></a>");
    WiServer.print("<a href=\"http://192.168.1.25/g\"><img src=\"gd.png\" id=\"imgG\"></a>");
    WiServer.print("<a href=\"http://192.168.1.25/b\"><img src=\"bd.png\" id=\"imgB\"></a>");
    WiServer.println("<div id=\"luz\">");
    WiServer.print(Mowayduino.readLight(),DEC);
    WiServer.print(END_DIV);
    WiServer.println("<div id=\"light\">Light");
    WiServer.print(END_DIV);
    WiServer.print(END_DIV);
    WiServer.print(END_DIV);
    WiServer.print(END_DIV);
    WiServer.println(END);   

    return true;
  }
  else
    return false;
}

#endif





void setup() {
  Mowayduino.beginMowayduino();
  Mowayduino.Blueon();
  beginmowayduinoIoT();
  Mowayduino.Blueoff();
  Mowayduino.Greenon();
  
  Serial.begin(57600);
  //while (!Serial);
}


void loop(){

#ifdef TWITTER_EXAMPLE
  // Check if it's time to get an update
  if (millis() >= updateTime) {
    twittermowayduino.submit();    
    // Get another update one minute from now
    updateTime += 60000;
  }
  Mowayduino.Stop();
#endif

#ifdef SOCKET
  WiFi.run();
   if (red==1)
   {
    Mowayduino.Redon();
    Mowayduino.Greenoff();
    Mowayduino.Blueoff();
   }
   else if (green==1)
   {
    Mowayduino.Redoff();
    Mowayduino.Greenon();
    Mowayduino.Blueoff();
   }
   else if (blue==1)
   {
    Mowayduino.Redoff();
    Mowayduino.Greenoff();
    Mowayduino.Blueon();
   }

#else
  // Run WiServer
  WiServer.server_task();
  delay(10);
#endif


#ifdef WEB_SERVER
  if (blue==1)
    Mowayduino.Blueon();
  else
    Mowayduino.Blueoff();
  if (red==1)
    Mowayduino.Redon();
  else
    Mowayduino.Redoff();
  if (green==1)
    Mowayduino.Greenon();
  else
    Mowayduino.Greenoff();
  if (move == STOP)
  {
    Mowayduino.Stop();
    move=0;
  }
  if (move == MOVE_STRAIGHT)
  {
    Mowayduino.Straight(100);
    delay(2000);
    Mowayduino.Stop();
    move=0;
  }
  if (move == MOVE_LEFT)
  {
    Mowayduino.TurnLeft(100,90);
    move=0;
  }
  if (move == MOVE_RIGHT)
  {
    Mowayduino.TurnRight(100,90);
    move=0;
  }
  if (move == MOVE_BACK)
  {
    Mowayduino.Back(100);
    delay(2000);
    Mowayduino.Stop();
    move=0;
  }

  Mowayduino.readAcc();
  Mowayduino.readObstacle();
#endif
  
}



