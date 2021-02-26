//Sketch uses 505932 bytes (48%) of program storage space. Maximum is 1044464 bytes.
//Global variables use 31804 bytes (38%) of dynamic memory, leaving 50116 bytes for local variables. Maximum is 81920 bytes.
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
// reduce string size from 2000 to 1500 xmppStanza.reserve(1500); *may be to 1100
// 13832 --> 5464
// use F() and String(F("")).c_str()
// 14984 --> 6616
// add module OTA
// 14872 --> 6504,14632 --> 6264
// edit pbkdf2
// 14912 --> 6544

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
//#define USE_REMOTE_DEBUG//suck ram alot--not use

//#define DEBUG_MSG(...)
//#define DEBUG_MSG_F(...)
#define DEBUG_MSG(...) Serial.print( __VA_ARGS__ )
#define DEBUG_MSG_F(...) Serial.print(F( __VA_ARGS__ ))


//#include "C:\Users\chuwl\Documents\Arduino\libraries\arduino-base64-master\Base64.h"
//https://github.com/adamvr/arduino-base64
#include <ESP8266WiFi.h>


#ifdef USE_WEB_SERVER
#include <WiFiClient.h>//==========================================
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>//=========================================
#include <ESP8266HTTPUpdateServer.h>
#endif


#include <XMPPClient.h>
#include <tinyxml2.h>
#include "FS.h"
//#include "iec61850_server.h"

#define USERNAME "esp"//"esp@XMPP.egat.co.th"  "esp"
#define DOMAIN "XMPP.egat.co.th"
#define PASSWORD "esp"
#define RESOURCE "8n1x08ixsd"

#define USERNAMERECEIVE "admin@XMPP.egat.co.th" //"tmntestxmpp@tmn.egat.co.th"
//#define USERNAMERECEIVE "ied1@eureka.egat.co.th/converse.js-108786251"//ied1@eureka.egat.co.th/converse.js-108786251"


//*****************************************************************************************
// IEC61850 Data Model
//*****************************************************************************************
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
#ifdef USE_IEC61850_8_2_REDUCTION_FORM

#define confirmed_RequestPDU_tag String(F("cRP")).c_str()
#define ConfirmedServiceRequest_tag String(F("CSR")).c_str()
#define variableAccessSpecification_tag String(F("vAS")).c_str()
#define variableSpecification_tag String(F("vS")).c_str()
//#define alternateAccess_tag String(F("aA")).c_str()
#define specificationWithResult_tag String(F("sWR")).c_str()
#define listOfVariable_tag String(F("lOV")).c_str()
#define objectScope_tag String(F("objectScope")).c_str()
#define domainSpecific_tag String(F("domainSpecific")).c_str()
#define SEQUENCE_tag String(F("SEQUENCE")).c_str()
#define domain_specific_tag String(F("domain-specific")).c_str()
#define name_tag String(F("name")).c_str()
#define listOfData_tag String(F("listOfData")).c_str()
#define Data_tag String(F("Data")).c_str()

#else

#define confirmed_RequestPDU_tag "confirmed_RequestPDU"
#define ConfirmedServiceRequest_tag "ConfirmedServiceRequest"
#define variableAccessSpecification_tag "variableAccessSpecification"
#define variableSpecification_tag "variableSpecification"
//#define alternateAccess_tag "alternateAccess"
#define specificationWithResult_tag "specificationWithResult"
#define listOfVariable_tag "listOfVariable"
#define objectScope_tag "objectScope"
#define domainSpecific_tag "domainSpecific"
#define SEQUENCE_tag "SEQUENCE"
#define domain_specific_tag "domain-specific"
#define name_tag "name"
#define listOfData_tag "listOfData"
#define Data_tag "Data"

#endif

//*****************************************************************************************

#ifdef USE_WEB_SERVER
const char* host = "esp8266";
#endif
const char* ssid     = "JUMP";
const char* password = "025260652";
//const char* ssid     = "D-Link 531/70";
//const char* password = "531/70Ideo";

// Update these with values suitable for your network.

byte xmppServer[] = {192, 168, 1, 38};

int counter = 0;
long reconnectTimeout = 0;

using namespace tinyxml2;

// # Data String Variable
// ## iq stanza
String attributeFrom;
String attributeTo;
String attributeId;

String DomainId;
String ItemId;
String InvokeID;

String LDname;
String LNname;

String VariableDataString;//other xmpp client write
//String StructerDataString;
int singleIntForRead;//other xmpp client read
String ControlResponseString;

String controlObjAttribute;//length <10
String controlObjSubAttribute;//length <10

#ifdef USE_WEB_SERVER
ESP8266WebServer server(80);//=====================================
ESP8266HTTPUpdateServer httpUpdater;

void handleRoot() {
    server.send(200, "text/plain", "hello from esp8266!");
}
void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}
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

bool huge_struct_data_flag = false;
bool norm_struct_data_flag = false;

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

String xmppStanza;
//String firstXmlTag;

//======================
// CtrlModels definition

typedef enum {
    Unselected,
    Ready,
    PerformTest,
    //waitForSelection,
    WaitForExecution,
    WaitForChange,
    WaitForActivationTime,
    Operate,
    //MirrorBlockedCmd,
    //EndSelection
} CO_State_t;


//CO_State_t status_only
//CO_State_t DIR_CO_norm_secure;
CO_State_t SBO_CO_norm_secure;//Sel
//CO_State_t DIR_CO_enha_secure;
CO_State_t SBO_CO_enha_secure;//SelVal
uint8_t SBO_CO_norm_secure_state = Unselected;
uint8_t SBO_CO_enha_secure_state = Unselected;

//======================

XMPPClient client(xmppServer, 5223);      //5223

void setup()
{
    SetupStringReserve();//set to avoid heap crash
    Serial.begin(9600);
    pinMode(LNofBatStatusPin, INPUT);

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
    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    httpUpdater.setup(&server);

    server.begin();

    MDNS.addService(String(F("http")).c_str(), String(F("tcp")).c_str(), 80);
    //MDNS.addService(String(F("telnet")).c_str(), String(F("tcp")).c_str(), 23);
#endif


}

void loop() {
    /* Say hello! */
    doConnect();
    unsigned long t1 = millis();
    xmppStanza = client.getAllData();
    int xmppStanzaLength = xmppStanza.length();
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

//#if  defined (USE_XMPP_CLIENT_SERIAL_DEBUG) || defined (USE_REMOTE_DEBUG)
//#endif        
        if (xmppStanzaLength < 200) {//current max 934 Control Select with value
            xmlParser();
            xmlManage();
            xmlResponse();
        } else {
            DEBUG_MSG_F("\nHuge\n");
            //            if (extracthugeDataToFlash()) { //set huge_struct_data_flag
            //                xmlManage();
            //            }//if package is too large Don't support it.
        }
        unsigned long t2 = millis();
        DEBUG_MSG_F("t:");
        DEBUG_MSG(t2 - t1);
        DEBUG_MSG_F("\n");
    }
#ifdef USE_WEB_SERVER
    server.handleClient();//=======================================
    MDNS.update();
#endif

    yield();// Give a time for ESP
}


bool extracthugeDataToFlash(void) {
    String structDataString;

    int indexF = xmppStanza.indexOf(F("<listOfData><Data><structure>"));//29 count text
    int indexB = xmppStanza.indexOf(F("</structure></Data></listOfData>"));

    if ((indexF > 0) && (indexB > 0)) {
        structDataString = xmppStanza.substring(indexF + 29, indexB);
        xmppStanza.remove(indexF + 29, indexB - indexF - 29);
        xmppStanza.remove(indexF);

        DEBUG_MSG(SPIFFS.begin());
        DEBUG_MSG_F("\n");
        File dummyFile = SPIFFS.open("/file.txt", "w");
        if (dummyFile) {
            dummyFile.print(structDataString.c_str());
        }
        dummyFile.close();

        huge_struct_data_flag = true;
        return true;
    }

    return false;
}
void doConnect()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUG_MSG_F("Starting wifi\n");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            DEBUG_MSG_F(".");
        }
        DEBUG_MSG_F("\nWiFi connected\n");
        //DEBUG_MSG_F("\nIP address: ");
        //DEBUG_MSG(WiFi.localIP());
        //DEBUG_MSG_F("\n");
        //WiFi.hostname("esp8266.local");
    }

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
        }
    }
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

    attributeFrom.reserve(64);
    attributeTo.reserve(64);
    attributeId.reserve(64);

    DomainId.reserve(64);
    ItemId.reserve(64);
    InvokeID.reserve(16);


    LDname.reserve(16);
    LNname.reserve(16);

    VariableDataString.reserve(16);
    ControlResponseString.reserve(64);
    xmppStanza.reserve(1500);//******* 1100
    //firstXmlTag.reserve();
}
