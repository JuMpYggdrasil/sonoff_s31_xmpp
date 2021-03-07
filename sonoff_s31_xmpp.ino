//Sketch uses 524708 bytes (50%) of program storage space. Maximum is 1044464 bytes.
//Global variables use 32356 bytes (39%) of dynamic memory, leaving 49564 bytes for local variables. Maximum is 81920 bytes.

// input[126] <iq type="get" id="967-873" to="esp@xmpp.egat.co.th/8n1x08ixsd" from="xmpp.egat.co.th"><query xmlns="jabber:iq:version"/></iq>
// open debug serial
// 15088 --> 6688
// correct bug authen2 in xmppclient library
// 14536 --> 6168
// improve authen2 in xmppclient library
// 15032 --> 6664
// improve USE_IEC61850_8_2_REDUCTION_FORM
// 16040 --> 7672
// include web server
// 13320 --> 4952
// reduce string size from 2000 to 1500 xmppStanza.reserve(1500); *may be to 1300
// 13832 --> 5464
// use F() and String(F("")).c_str()
// 14984 --> 6616
// add module OTA
// 14872 --> 6504,14632 --> 6264
// edit pbkdf2
// 14856 --> 6488
// set fingerprint to PROGMEM
// 14976 --> 6608
// change string to array of char
// 15088 --> 6720         (worse(not worst) case[761] 14864 --> 1672)
// add some debug fn
// 14888 --> 6520
// change xmppStanza STRING to charArray
// 15144 --> 6776
// edit webserver page
// 15160 --> 6792,15176 --> 6808
// add wifimanager with comment out debug
// 13944 --> 5576
// improve init2 in xmppclient library
// 15504 --> 7136
// improve response,stateAction,authen2 in xmppclient library
// 15576 --> 7208
// random number array to progmem in xmppclient library
// 15640 --> 7272
// add web page
// 13112 --> 4744,13064 --> 4696
// try to optimize all things.
// throw the towel. give up!!!!!
// *remove every thing about SPIFFS
// 16440 --> 8072            (worse(not worst) case[363+325=688] 15856 --> 3096)
//
// *note: need to disable firewall on openfire server side
// Extensible Messaging and Presence Protocol (XMPP)
// is a protocol for streaming Extensible Markup Language (XML) elements
// in order to exchange structured information in close to real time
// between any two network endpoints.
//
// XML stream acts as an envelope for all the XML stanzas sent during a session
//  |--------------------|
//  | <stream>           |
//  |--------------------|
//  | <presence>         |
//  |   <show/>          |
//  | </presence>        |
//  |--------------------|
//  | <message to='foo'> |
//  |   <body/>          |
//  | </message>         |
//  |--------------------|
//  | <iq to='bar'>      |
//  |   <query/>         |
//  | </iq>              |
//  |--------------------|
//  | ...                |
//  |--------------------|
//  | </stream>          |
//  |--------------------|

#define USE_WEB_SERVER
#define USE_WIFI_CONNECT
#define USE_CSE7766
//#define USE_FTP_SERVER//suck ram alot--not use
//#define USE_REMOTE_DEBUG//suck ram alot--not use

#ifdef USE_CSE7766
#define DEBUG_MSG(...)
#define DEBUG_MSG_F(...)
#else
#define DEBUG_MSG(...) Serial.print( __VA_ARGS__ )
#define DEBUG_MSG_F(...) Serial.print(F( __VA_ARGS__ ))
#endif


//#include "C:\Users\chuwl\Documents\Arduino\libraries\arduino-base64-master\Base64.h"
//https://github.com/adamvr/arduino-base64
#include <ESP8266WiFi.h>

#if  defined (USE_WEB_SERVER) || defined (USE_WIFI_CONNECT)
#include <WiFiClient.h>//==========================================
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>//=========================================
#include <ESP8266HTTPUpdateServer.h>
#endif

#ifdef USE_WIFI_CONNECT
#include "WiFiConnect.h"
#endif

#ifdef USE_CSE7766
#include "CSE7766.h"
#define PUSHBUTTON_PIN   0
#define RELAY_PIN       12//relay(active high) include red led(active high)
#define LED_PIN         13//blue led(active high/->>low???)
#endif

#include <XMPPClient.h>
#include <tinyxml2.h>
//#include "FS.h"

#define USERNAME "esp"//"esp@XMPP.egat.co.th"  "esp"
#define DOMAIN "XMPP.egat.co.th"
#define PASSWORD "esp"
#define RESOURCE "8n1x08ixsd"

#define USERNAMERECEIVE "admin@XMPP.egat.co.th" //"tmntestxmpp@tmn.egat.co.th"
//#define USERNAMERECEIVE "ied1@eureka.egat.co.th/converse.js-108786251"//ied1@eureka.egat.co.th/converse.js-108786251"


//*****************************************************************************************
// IEC61850 Data Model
//*****************************************************************************************
//IED  LD  LN    DO     DA
//IED1
//IED1_LD1
//IED1_LD1_LLN0
//IED1_LD1_LLN0_LLName
//IED1_LD1_LLN0_Mod //mode
//IED1_LD1_LLN0_Beh
//IED1_LD1_LLN0_Health
//IED1_LD1_LLN0_NamePlt
//IED1_LD1_LPHD1
//IED1_LD1_LPHD1_LLName
//IED1_LD1_LPHD1_PhyNam
//IED1_LD1_LPHD1_PhyHealt
//IED1_LD1_LPHD1_Proxy
//IED1_LD1_GGIO1


#define LD1 "IEDNameLDINst"     // Specify domainId DER1LD1
#define LD1LN1 "LLN0" // Specify itemId --> Logical Node + Data Attribute ZBAT1$ST$BatSt
#define LD1LN1PACK "LLN0$ST$Beh$stVal" // Specify itemId --> Logical Node + Data Attribute ZBAT1$ST$BatSt

#define LD2 "CONTROL"
#define LD2LN1 "GGIO1"
#define LD2LN1PACK "GGIO1$CO$DPCSO1"


//*****************************************************************************************

//*****************************************************************************************
// MAP IEC61850 Model to Physical Server
//*****************************************************************************************
#define LNofBatStatusPin 0

//*****************************************************************************************


//*****************************************************************************************
// Reduction form
//*****************************************************************************************
#ifdef USE_IEC61850_8_2_REDUCTION_FORM//define in "xmppclient.h"

#define confirmed_RequestPDU_tag String(F("cRP")).c_str()
#define invokeID_tag String(F("ivID")).c_str()
#define ConfirmedServiceRequest_tag String(F("CSR")).c_str()
#define variableAccessSpecification_tag String(F("vAS")).c_str()
#define variableSpecification_tag String(F("vS")).c_str()
//#define alternateAccess_tag String(F("aA")).c_str()
#define specificationWithResult_tag String(F("sWR")).c_str()
//#define variableListName_tag String(F("vLN")).c_str()
#define listOfVariable_tag String(F("lOV")).c_str()
#define objectScope_tag String(F("oS")).c_str()
#define domainSpecific_tag String(F("dS")).c_str()
#define SEQUENCE_tag String(F("SEQ")).c_str()
#define domain_specific_tag String(F("ds")).c_str()
#define name_tag String(F("name")).c_str()
#define listOfData_tag String(F("lOD")).c_str()
#define Data_tag String(F("Data")).c_str()
#define domainId_tag String(F("dId")).c_str()
#define itemId_tag String(F("iId")).c_str()

#else

#define confirmed_RequestPDU_tag "confirmed_RequestPDU"
#define invokeID "invokeID"
#define ConfirmedServiceRequest_tag "ConfirmedServiceRequest"
#define variableAccessSpecification_tag "variableAccessSpecification"
#define variableSpecification_tag "variableSpecification"
//#define alternateAccess_tag "alternateAccess"
#define specificationWithResult_tag "specificationWithResult"
//#define variableListName_tag "variableListName"
#define listOfVariable_tag "listOfVariable"
#define objectScope_tag "objectScope"
#define domainSpecific_tag "domainSpecific"
#define SEQUENCE_tag "SEQUENCE"
#define domain_specific_tag "domain-specific"
#define name_tag "name"
#define listOfData_tag "listOfData"
#define Data_tag "Data"
#define domainId "domainId"
#define itemId "itemId"


#endif

//*****************************************************************************************

#ifdef USE_WEB_SERVER
const char* host = "esp8266";
#endif

#ifndef USE_WIFI_CONNECT
const char* ssid     = "JUMP";
const char* password = "025260652";
//const char* ssid     = "D-Link 531/70";
//const char* password = "531/70Ideo";
#endif

// Update these with values suitable for your network.

byte xmppServer[] = {192, 168, 1, 38};

using namespace tinyxml2;

// # Data String Variable
// ## iq stanza
char attributeFrom[64];
char attributeTo[64];
char attributeId[64];

String DomainId;
String ItemId;
char InvokeID[16];

String LDname, LNname;

String VariableDataString;//other xmpp client write
//String StructerDataString;
int singleIntForRead;//other xmpp client read
String ControlResponseString;

String controlObjAttribute;//length <10
String controlObjSubAttribute;//length <10

// ## constant variable inmemory declare
// ###  webpage structure (to access use FPSTR())
const char WEB_HEAD[] PROGMEM = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
const char WEB_LOAD_SCRIPT[] PROGMEM = "<script src=\"highcharts.js\"></script>";
const char WEB_STYLE[] PROGMEM = "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">";
const char WEB_BODY_START[] PROGMEM = "</head><body>";
const char WEB_SIDENAV[] PROGMEM = "<div class=\"sidenav\"><a href=\"/\">Home</a><a href=\"/info\">Info</a><a href=\"/graph\">Graph</a><a href=\"/update\">OTA</a></div>";
const char WEB_CONTENT_START[] PROGMEM = "<div class=\"content\"><h2><span style=\"color: maroon\">I</span>ED</h2>";
const char WEB_BODY_HTML_END[] PROGMEM = "</div></body></html>";//with end content
const char WEB_SCRIPT_START[] PROGMEM = "</div></body><script>";//with end content
const char WEB_SCRIPT_HTML_END[] PROGMEM = "</script></html>";

File fsUploadFile;

#ifdef USE_CSE7766
CSE7766 cse7766;
#endif

#ifdef USE_WEB_SERVER
ESP8266WebServer server(80);//=====================================
ESP8266HTTPUpdateServer httpUpdater;

static const char text_plain[] PROGMEM = "text/plain";

#endif


typedef struct
{
    boolean ping_flag = false;
    boolean domain_id_flag = false;
    boolean item_id_flag = false;

    boolean type_get_flag = false;//iq stanza type
    // Service Element (for get)
    boolean read_flag = false;
    boolean domainSpecific_flag = false;
    //  get - unused
    //      boolean getVariableAccessAttributes_flag = false;
    //      boolean getNamedVariableListAttributes_flag = false;
    //      boolean getNamedTypeAttributes_flag = false;
    //      boolean readJournal_flag = false;
    //      boolean fileRead_flag = false;
    //      boolean fileDirectory_flag = false;
    //      boolean obtainFile_flag = false;
    //      boolean extendedGetNameList_flag = false;
    boolean type_set_flag = false;//iq stanza type
    // Service Element (for set)
    boolean write_flag = false;
    //  set - unused
    //      boolean definedNamedVariableList_flag = false;
    //      boolean deleteNamedVariableList_flag = false;
    //      boolean fileOpen_flag = false;
    //      boolean fileClose_flag = false;
    //      boolean fileDelete_flag = false;
    boolean type_result_flag = false;//iq stanza type

} XMPP_t;
XMPP_t XMPP_8_2;

boolean struct_data_flag = false;

typedef struct
{
    boolean getLDDir_flag = false;//11
    boolean getLNDir_flag = false;//12.2
    //boolean getAllDataValues_flag = false;//12.3
    boolean getDataValue_flag = false;//13.2
    boolean setDataValue_flag = false;//13.3
    //boolean getDataDirectory_flag = false;//13.4
    //boolean getDataDefinition_flag = false;//13.5
    boolean controlSelect_flag = false;//20.3
    boolean controlSelectWithValue_flag = false;//20.4
    boolean controlCancel_flag = false;//20.5
    boolean controlOperate_flag = false;//20.6
} IEC61850_8_2_t;
IEC61850_8_2_t IEC61850_8_2;



//String xmppStanza;
const int xmppStanzaLenMax = 1400;
//char xmppStanza[xmppStanzaLenMax];

//======================
// CtrlModels definition

typedef enum {
    Unselected,
    Ready,
    PerformTest,
    //waitForSelection,
    WaitForExecution,
    //WaitForChange,//in 61850-7-2 table.47 P.157 but mms don't
    WaitForActivationTime,
    Operate,
    //MirrorBlockedCmd,
    //EndSelection
} CO_State_t;


//CO_State_t status_only
//CO_State_t DIR_CO_norm_secure;//        case:1
CO_State_t SBO_CO_norm_secure;//Sel       case:2
//CO_State_t DIR_CO_enha_secure;          case:3
CO_State_t SBO_CO_enha_secure;//SelVal    case:4
uint8_t SBO_CO_norm_secure_state = Unselected;
uint8_t SBO_CO_enha_secure_state = Unselected;



//======================
#ifdef USE_WIFI_CONNECT
WiFiConnect wc;

void startWiFi(void) {
    //wc.resetSettings(); //helper to remove the stored wifi connection, comment out after first upload and re upload
    /*
        AP_NONE = Continue executing code
        AP_LOOP = Trap in a continuous loop - Device is useless
        AP_RESET = Restart the chip
        AP_WAIT  = Trap in a continuous loop with captive portal until we have a working WiFi connection
    */
    if (!wc.autoConnect()) { // try to connect to wifi
        /* We could also use button etc. to trigger the portal on demand within main loop */
        wc.startConfigurationPortal(AP_WAIT);//if not connected show the configuration portal
    }
}
#endif


XMPPClient client(xmppServer, 5223);//5223

void setup()
{
    SetupStringReserve();//set to avoid heap crash
    ESP.wdtDisable();

#ifdef USE_CSE7766
    cse7766.setRX(1);
    cse7766.begin();// will initialize serial to 4800 bps
#else
    Serial.begin(9600);
#endif

    pinMode(LNofBatStatusPin, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);//turn off red led

    doConnect();
    DEBUG_MSG_F("\ninit x\n");
#ifdef USE_IEC61850_8_2_REDUCTION_FORM
    DEBUG_MSG_F("\nUSE_IEC61850_8_2_REDUCTION_FORM\n");
#else
    DEBUG_MSG_F("\nnot USE_IEC61850_8_2_REDUCTION_FORM\n");
#endif
    client.client.setTimeout(80);

#ifdef USE_WEB_SERVER
    MDNS.begin(host);
    MDNS.addService(String(F("http")).c_str(), String(F("tcp")).c_str(), 80);
    //MDNS.addService(String(F("telnet")).c_str(), String(F("tcp")).c_str(), 23);

    //bool spiffsResult = SPIFFS.begin();

    server.on(String(F("/")).c_str(), HTTP_GET, handleRoot);
    //    server.on(String(F("/deleteAll")).c_str(), HTTP_GET, []() {
    //        //delete file??
    //        server.send_P(200, text_plain, PSTR("delete all"));
    //    });

    //    server.serveStatic(String(F("/log.txt")).c_str(), SPIFFS, String(F("/log.txt")).c_str());
    //    server.serveStatic(String(F("/config.txt")).c_str(), SPIFFS, String(F("/config.txt")).c_str());
    //    server.serveStatic(String(F("/style.css")).c_str(), SPIFFS, String(F("/style.css")).c_str());

    //    // ##### file upload
    //    server.on(String(F("/upload")).c_str(), HTTP_POST, []() {
    //        server.send_P(200, text_plain, PSTR("upload ok"));
    //    }, handleFileUpload);


#ifdef USE_CSE7766
    server.on(String(F("/on")).c_str(), HTTP_GET/*HTTP_POST*/, []() {
        digitalWrite(RELAY_PIN, HIGH);
        server.send(204);
    });
    server.on(String(F("/off")).c_str(), HTTP_GET/*HTTP_POST*/, []() {
        digitalWrite(RELAY_PIN, LOW);
        server.send(204);
    });
    // ##### ajax xmlHttpRequest
    server.on(String(F("/xVal")).c_str(), HTTP_GET, []() {//using AJAX
        String xValue;
        if (digitalRead(RELAY_PIN)) {
            xValue.concat(F("on"));
        } else {
            xValue.concat(F("off"));
        }
        xValue.concat(F(","));
        xValue.concat(String(cse7766.getVoltage()));
        xValue.concat(F(","));
        xValue.concat(String(cse7766.getCurrent()));
        xValue.concat(F(","));
        xValue.concat(String(cse7766.getActivePower()));
        xValue.concat(F(","));
        xValue.concat(String(cse7766.getApparentPower()));
        xValue.concat(F(","));
        xValue.concat(String(cse7766.getReactivePower()));
        xValue.concat(F(","));
        xValue.concat(String(cse7766.getPowerFactor()));
        xValue.concat(F(","));
        xValue.concat(String(cse7766.getEnergy()));
        xValue.concat(F(","));
        xValue.concat(WiFi.SSID());

        server.send(200, "text/plain", xValue);//(comma format)
    });
#endif


    // ##### file not found
    server.onNotFound(handleNotFound);
    httpUpdater.setup(&server);

    server.begin();


#endif

#ifdef USE_WIFI_CONNECT
    startWiFi();
#endif

    digitalWrite(RELAY_PIN, HIGH);//turn on red led

}

void loop() {
    //===================================
    //       Parser 61850 Part
    //===================================
    doConnect();
    //===================================


    //===================================
    //       Parser 61850 Part
    //===================================
    unsigned long t1 = millis();

    char *xmppStanza = (char *)malloc(xmppStanzaLenMax * sizeof(char));
    if (xmppStanza != NULL) {
        int xmppStanzaLength = client.getAllData(xmppStanza, xmppStanzaLenMax);
        if (xmppStanzaLength > 0) {
            DEBUG_MSG_F("\n");
            DEBUG_MSG_F("input[");
            DEBUG_MSG(xmppStanzaLength);
            DEBUG_MSG_F("] ");
            DEBUG_MSG(xmppStanza);
            DEBUG_MSG_F("\n");
            DEBUG_MSG_F("\nHeap:");
            DEBUG_MSG(ESP.getFreeHeap(), DEC);
            DEBUG_MSG_F("\n");


            if (xmppStanzaLength < 1000) {//current max 934 Control Select with value
                xmlParser(xmppStanza, xmppStanzaLength);
                xmlManage(xmppStanza, xmppStanzaLength);
                xmlResponse();
            } else {
                DEBUG_MSG_F("\nToo Huge\n");
            }
            free(xmppStanza);

            unsigned long t2 = millis();
            DEBUG_MSG_F("t:");
            DEBUG_MSG(t2 - t1);
            DEBUG_MSG_F("\n");
        } else if (xmppStanzaLength < 0) {
            DEBUG_MSG_F("too long command\n");
        } else {
            free(xmppStanza);
        }
    } else {
        free(xmppStanza);
    }
    //===================================


    //===================================
    //       Action Part
    //===================================
#ifdef USE_CSE7766
    cse7766.handle();// CSE7766 handle
    ESP.wdtFeed();
#endif




    //===================================


#ifdef USE_WEB_SERVER
    server.handleClient();
    ESP.wdtFeed();
    MDNS.update();
    ESP.wdtFeed();
#endif

    ESP.wdtFeed();
    yield();// Give a time for ESP
}
void doConnect()
{
#ifdef USE_WIFI_CONNECT
    if (WiFi.status() != WL_CONNECTED) {
        ESP.wdtDisable();
        if (!wc.autoConnect()) wc.startConfigurationPortal(AP_WAIT);
    }
#else
    if (WiFi.status() != WL_CONNECTED)
    {
        ESP.wdtDisable();
        DEBUG_MSG_F("Starting wifi\n");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            DEBUG_MSG_F(".");
        }
        DEBUG_MSG_F("\nWiFi connected\n");
        DEBUG_MSG_F("\nIP address: ");
        DEBUG_MSG(WiFi.localIP());
        DEBUG_MSG_F("\n");
    }
#endif


    if (!client.client.connected())
    {
        //client.client.stopAll();
        DEBUG_MSG_F("Connecting to xmpp\n");
        delay(500);
        /* Connect to the XMPP server mads@skythree.inventit.dk */
        if (client.connect(USERNAME, DOMAIN, RESOURCE, PASSWORD))
        {
            /* Connected, let everyone   know that we're online */
            client.sendPresence();
            client.sendMessage(USERNAMERECEIVE, "I am online");
            ESP.wdtEnable(WDTO_8S);
        }
    }
    ESP.wdtFeed();
}
String getSplitValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void SetupStringReserve(void) {
    controlObjAttribute.reserve(16);
    controlObjSubAttribute.reserve(16);

    DomainId.reserve(64);
    ItemId.reserve(64);

    LDname.reserve(16);
    LNname.reserve(16);

    VariableDataString.reserve(16);
    ControlResponseString.reserve(64);
}
