#include <MowayduinoIo.h>

#ifdef SOCKET
#include <WiShield.h>
#else
#include <WiServerMoway.h>
#endif


#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2


unsigned char local_ip[] = {192,168,1,25};	// IP address of mOwayduino
unsigned char gateway_ip[] = {192,168,1,1};	// router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = {"*****************"};		// Replace "****" for your net name (max 32 bytes)

unsigned char security_type = 2;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"*****************"};	// Replace "****" for your net password (max 64 characters)

// WEP 128-bit keys
// sample HEX keys
prog_uchar wep_keys[] PROGMEM = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,	// Key 0
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 1
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Key 2
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	// Key 3
				};

// setup the wireless mode
// WIRELESS_MODE_INFRA - connect to AP
// WIRELESS_MODE_ADHOC - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;
// End of wireless configuration parameters ----------------------------------------


#define STOP            5
#define MOVE_STRAIGHT   1
#define MOVE_LEFT       2
#define MOVE_RIGHT      3
#define MOVE_BACK       4


#ifdef TWITTER_EXAMPLE

/************* Supertweet configuration parameters ******************/

long updateTime = 0;
extern void tweet_write();
extern void tweet_return(char* data, int len);


const char supertweetauth[] = "*********************";    // Replace '****' for "user:password" of supertweet in Base64
// IP Address for www.supertweet.net  
uint8 supertweetip[] = {50,18,251,175};
// A request that sends mOwayduino light sensor value to "supertweet"
POSTrequest twittermowayduino(supertweetip, 80, "api.supertweet.net", "/1.1/statuses/update.json",tweet_write);

void begintwitter()
{
  twittermowayduino.setAuth((char *)supertweetauth);
  // Have the processData function called when data is returned by the server
  twittermowayduino.setReturnFunc(tweet_return);
}

/************* END Supertweet configuration parameters ******************/
#endif //TWITTER_EXAMPLE



#ifdef WEB_SERVER

extern boolean mowayduinoPage(char* URL);

const char EXT_SERVER[] = "http://mowayduino.com/server/";
const char LOCAL_IP[] = "http://192.168.1.25/";
const char REDIRECT_META[] = "<meta http-equiv=\"Refresh\" content=\"0;url=/\">";
const char HEAD[] = "<html><head>";
const char END_HEAD[]= "</head>";
const char BODY[]= "<body>";
const char END[]= "<body></html>";
const char END_DIV[] = "</div>";

#endif



void beginmowayduinoIoT (void)
{
  digitalWrite(CS_EXP,HIGH);
  //reset Wifi Module
  digitalWrite(CE_EXP,LOW);
  delay(100);
  digitalWrite(CE_EXP,HIGH);
  //Wifi Module Init
  delay(1000);
  
#ifdef WEB_SERVER
  WiServer.init(mowayduinoPage);
#endif

#ifdef TWITTER_EXAMPLE
  WiServer.init(NULL);
  begintwitter();
#endif

#ifdef SOCKET
    WiFi.init();
#endif

}



