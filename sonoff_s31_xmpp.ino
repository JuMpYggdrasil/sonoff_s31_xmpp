//Sketch uses 453612 bytes (43%) of program storage space. Maximum is 1044464 bytes.
//Global variables use 31284 bytes (38%) of dynamic memory, leaving 50636 bytes for local variables. Maximum is 81920 bytes.
// input[126] <iq type="get" id="967-873" to="esp@xmpp.egat.co.th/8n1x08ixsd" from="xmpp.egat.co.th"><query xmlns="jabber:iq:version"/></iq>
// 15088 --> 6688
// 14536 --> 6168
// 15032 --> 6664
// 16040 --> 7672

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

//#define USE_WEB_SERVER true
#define DEBUG_MSG(...) Serial.print( __VA_ARGS__ )

//#include "C:\Users\chuwl\Documents\Arduino\libraries\arduino-base64-master\Base64.h"
//https://github.com/adamvr/arduino-base64
#include <ESP8266WiFi.h>


#ifdef USE_WEB_SERVER
#include <WiFiClient.h>//==========================================
#include <ESP8266WebServerSecure.h>
#include <ESP8266mDNS.h>//=========================================
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
ESP8266WebServerSecure server(443);//=====================================

// The certificate is stored in PMEM
static const uint8_t x509[] PROGMEM = {
    0x30, 0x82, 0x01, 0x3d, 0x30, 0x81, 0xe8, 0x02, 0x09, 0x00, 0xfe, 0x56,
    0x46, 0xf2, 0x78, 0xc6, 0x51, 0x17, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86,
    0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x30, 0x26, 0x31,
    0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x07, 0x45, 0x53,
    0x50, 0x38, 0x32, 0x36, 0x36, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55,
    0x04, 0x03, 0x0c, 0x09, 0x31, 0x32, 0x37, 0x2e, 0x30, 0x2e, 0x30, 0x2e,
    0x31, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x37, 0x30, 0x33, 0x31, 0x38, 0x31,
    0x34, 0x34, 0x39, 0x31, 0x38, 0x5a, 0x17, 0x0d, 0x33, 0x30, 0x31, 0x31,
    0x32, 0x35, 0x31, 0x34, 0x34, 0x39, 0x31, 0x38, 0x5a, 0x30, 0x26, 0x31,
    0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x07, 0x45, 0x53,
    0x50, 0x38, 0x32, 0x36, 0x36, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55,
    0x04, 0x03, 0x0c, 0x09, 0x31, 0x32, 0x37, 0x2e, 0x30, 0x2e, 0x30, 0x2e,
    0x31, 0x30, 0x5c, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
    0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x4b, 0x00, 0x30, 0x48, 0x02,
    0x41, 0x00, 0xc6, 0x72, 0x6c, 0x12, 0xe1, 0x20, 0x4d, 0x10, 0x0c, 0xf7,
    0x3a, 0x2a, 0x5a, 0x49, 0xe2, 0x2d, 0xc9, 0x7a, 0x63, 0x1d, 0xef, 0xc6,
    0xbb, 0xa3, 0xd6, 0x6f, 0x59, 0xcb, 0xd5, 0xf6, 0xbe, 0x34, 0x83, 0x33,
    0x50, 0x80, 0xec, 0x49, 0x63, 0xbf, 0xee, 0x59, 0x94, 0x67, 0x8b, 0x8d,
    0x81, 0x85, 0x23, 0x24, 0x06, 0x52, 0x76, 0x55, 0x9d, 0x18, 0x09, 0xb3,
    0x3c, 0x10, 0x40, 0x05, 0x01, 0xf3, 0x02, 0x03, 0x01, 0x00, 0x01, 0x30,
    0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b,
    0x05, 0x00, 0x03, 0x41, 0x00, 0x69, 0xdc, 0x6c, 0x9b, 0xa7, 0x62, 0x57,
    0x7e, 0x03, 0x01, 0x45, 0xad, 0x9a, 0x83, 0x90, 0x3a, 0xe7, 0xdf, 0xe8,
    0x8f, 0x46, 0x00, 0xd3, 0x5f, 0x2b, 0x0a, 0xde, 0x92, 0x1b, 0xc5, 0x04,
    0xc5, 0xc0, 0x76, 0xf4, 0xf6, 0x08, 0x36, 0x97, 0x27, 0x82, 0xf1, 0x60,
    0x76, 0xc2, 0xcd, 0x67, 0x6c, 0x4b, 0x6c, 0xca, 0xfd, 0x97, 0xfd, 0x33,
    0x9e, 0x12, 0x67, 0x6b, 0x98, 0x7e, 0xd5, 0x80, 0x8f
};

// And so is the key.  These could also be in DRAM
static const uint8_t rsakey[] PROGMEM = {
    0x30, 0x82, 0x01, 0x3a, 0x02, 0x01, 0x00, 0x02, 0x41, 0x00, 0xc6, 0x72,
    0x6c, 0x12, 0xe1, 0x20, 0x4d, 0x10, 0x0c, 0xf7, 0x3a, 0x2a, 0x5a, 0x49,
    0xe2, 0x2d, 0xc9, 0x7a, 0x63, 0x1d, 0xef, 0xc6, 0xbb, 0xa3, 0xd6, 0x6f,
    0x59, 0xcb, 0xd5, 0xf6, 0xbe, 0x34, 0x83, 0x33, 0x50, 0x80, 0xec, 0x49,
    0x63, 0xbf, 0xee, 0x59, 0x94, 0x67, 0x8b, 0x8d, 0x81, 0x85, 0x23, 0x24,
    0x06, 0x52, 0x76, 0x55, 0x9d, 0x18, 0x09, 0xb3, 0x3c, 0x10, 0x40, 0x05,
    0x01, 0xf3, 0x02, 0x03, 0x01, 0x00, 0x01, 0x02, 0x40, 0x35, 0x0b, 0x74,
    0xd3, 0xff, 0x15, 0x51, 0x44, 0x0f, 0x13, 0x2e, 0x9b, 0x0f, 0x93, 0x5c,
    0x3f, 0xfc, 0xf1, 0x17, 0xf9, 0x72, 0x94, 0x5e, 0xa7, 0xc6, 0xb3, 0xf0,
    0xfe, 0xc9, 0x6c, 0xb1, 0x1e, 0x83, 0xb3, 0xc6, 0x45, 0x3a, 0x25, 0x60,
    0x7c, 0x3d, 0x92, 0x7d, 0x53, 0xec, 0x49, 0x8d, 0xb5, 0x45, 0x10, 0x99,
    0x9b, 0xc6, 0x22, 0x3a, 0x68, 0xc7, 0x13, 0x4e, 0xb6, 0x04, 0x61, 0x21,
    0x01, 0x02, 0x21, 0x00, 0xea, 0x8c, 0x21, 0xd4, 0x7f, 0x3f, 0xb6, 0x91,
    0xfa, 0xf8, 0xb9, 0x2d, 0xcb, 0x36, 0x36, 0x02, 0x5f, 0xf0, 0x0c, 0x6e,
    0x87, 0xaa, 0x5c, 0x14, 0xf6, 0x56, 0x8e, 0x12, 0x92, 0x25, 0xde, 0xb3,
    0x02, 0x21, 0x00, 0xd8, 0x99, 0x01, 0xf1, 0x04, 0x0b, 0x98, 0xa3, 0x71,
    0x56, 0x1d, 0xea, 0x6f, 0x45, 0xd1, 0x36, 0x70, 0x76, 0x8b, 0xab, 0x69,
    0x30, 0x58, 0x9c, 0xe0, 0x45, 0x97, 0xe7, 0xb6, 0xb5, 0xef, 0xc1, 0x02,
    0x21, 0x00, 0xa2, 0x01, 0x06, 0xc0, 0xf2, 0xdf, 0xbc, 0x28, 0x1a, 0xb4,
    0xbf, 0x9b, 0x5c, 0xd8, 0x65, 0xf7, 0xbf, 0xf2, 0x5b, 0x73, 0xe0, 0xeb,
    0x0f, 0xcd, 0x3e, 0xd5, 0x4c, 0x2e, 0x91, 0x99, 0xec, 0xb7, 0x02, 0x20,
    0x4b, 0x9d, 0x46, 0xd7, 0x3c, 0x01, 0x4c, 0x5d, 0x2a, 0xb0, 0xd4, 0xaa,
    0xc6, 0x03, 0xca, 0xa0, 0xc5, 0xac, 0x2c, 0xe0, 0x3f, 0x4d, 0x98, 0x71,
    0xd3, 0xbd, 0x97, 0xe5, 0x55, 0x9c, 0xb8, 0x41, 0x02, 0x20, 0x02, 0x42,
    0x9f, 0xd1, 0x06, 0x35, 0x3b, 0x42, 0xf5, 0x64, 0xaf, 0x6d, 0xbf, 0xcd,
    0x2c, 0x3a, 0xcd, 0x0a, 0x9a, 0x4d, 0x7c, 0xad, 0x29, 0xd6, 0x36, 0x57,
    0xd5, 0xdf, 0x34, 0xeb, 0x26, 0x03
};

void handleRoot() {
    //digitalWrite(led, 1);
    server.send(200, "text/plain", "hello from esp8266!");
    //digitalWrite(led, 0);
}
void handleNotFound() {
    //digitalWrite(led, 1);
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
    //digitalWrite(led, 0);
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
    DEBUG_MSG(F("\ninit\n"));
#ifdef USE_IEC61850_8_2_REDUCTION_FORM
    DEBUG_MSG(F("\nUSE_IEC61850_8_2_REDUCTION_FORM\n"));
#else
    DEBUG_MSG(F("\nnot USE_IEC61850_8_2_REDUCTION_FORM\n"));
#endif
    client.client.setTimeout(80);

#ifdef USE_WEB_SERVER
    //    if (MDNS.begin("esp8266")) {
    //        DEBUG_MSG("MDNS responder started\n");
    //    }

    server.getServer().setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));

    server.on("/", handleRoot);//==================================

    server.on("/inline", []() {
        server.send(200, "text/plain", "this works as well");
    });

    server.onNotFound(handleNotFound);

    server.begin();//================================================

    //    MDNS.addService("http", "tcp", 80);
    //    MDNS.addService("telnet", "tcp", 23);
#endif

}

void loop() {
    /* Say hello! */
    doConnect();
    unsigned long t1 = millis();
    xmppStanza = client.getAllData();
    int xmppStanzaLength = xmppStanza.length();
    if (xmppStanzaLength > 0) {
        DEBUG_MSG(F("\n"));
        DEBUG_MSG(F("input["));
        DEBUG_MSG(xmppStanzaLength);
        DEBUG_MSG(F("] "));
        DEBUG_MSG(xmppStanza);
        DEBUG_MSG(F("\n"));
        xmlParser();

        DEBUG_MSG(F("\nHeap:"));
        DEBUG_MSG(ESP.getFreeHeap(), DEC);
        DEBUG_MSG(F("\n"));
        DEBUG_MSG(xmppStanzaLength);
        DEBUG_MSG(F("\n"));
        if (xmppStanzaLength < 900) {//current max 934 Control Select with value
            xmlManage();
        } else {
            //            if (extracthugeDataToFlash()) { //set huge_struct_data_flag
            //                xmlManage();
            //            }//if package is too large Don't support it.
        }
        xmlResponse();
        unsigned long t2 = millis();
        DEBUG_MSG(F("t:"));
        DEBUG_MSG(t2 - t1);
        DEBUG_MSG(F("\n"));
    }
#ifdef USE_WEB_SERVER
    server.handleClient();//=======================================
    //MDNS.update();
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
        DEBUG_MSG(F("\n"));
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
        DEBUG_MSG(F("Starting wifi\n"));
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            DEBUG_MSG(F("."));
        }
        DEBUG_MSG(F("\nWiFi connected\n"));
        //DEBUG_MSG(F("\nIP address: "));
        //DEBUG_MSG(WiFi.localIP());
        //DEBUG_MSG(F("\n"));
        //WiFi.hostname("esp8266.local");
    }

    if (!client.client.connected())
    {
        //client.client.stopAll();
        DEBUG_MSG(F("Connecting to xmpp\n"));
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
    xmppStanza.reserve(2000);
}
