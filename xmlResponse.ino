void xmlResponse(void) {
    char responseBuffer[550];

    if (XMPP_8_2.ping_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'/>"), attributeTo, attributeFrom, attributeId);
        client.client.write(responseBuffer);
        //Serial.println(responseBuffer);
        DEBUG_MSG_F("pong");
        DEBUG_MSG_F("\n");

        XMPP_8_2.ping_flag = false;
    }
    
    if (IEC61850_8_2.getDataValue_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><read><listOfAccessResult><AccessResult><success><integer>%d</integer></success></AccessResult></listOfAccessResult></read></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo, attributeFrom, attributeId, InvokeID, singleIntForRead);
        client.client.write(responseBuffer);
        DEBUG_MSG_F("get");
        DEBUG_MSG_F("\n");

        IEC61850_8_2.getDataValue_flag = false;
    }
    if (IEC61850_8_2.setDataValue_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><write><CHOICE><success/></CHOICE></write></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo, attributeFrom, attributeId, InvokeID);
        client.client.write(responseBuffer);
        DEBUG_MSG_F("set");
        DEBUG_MSG_F("\n");

        IEC61850_8_2.setDataValue_flag = false;
    }
    if (IEC61850_8_2.controlSelect_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><read><listOfAccessResult><AccessResult><success><structure><Data><visible-string>%s</visible-string></Data></structure></success></AccessResult></listOfAccessResult></read></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo, attributeFrom, attributeId, InvokeID, ControlResponseString.c_str());
        client.client.write(responseBuffer);
        //Serial.println(responseBuffer);
        DEBUG_MSG_F("CO sel");
        DEBUG_MSG_F("\n");

        IEC61850_8_2.controlSelect_flag = false;
    }
    if (IEC61850_8_2.controlSelectWithValue_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><write><CHOICE><success/></CHOICE></write></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo, attributeFrom, attributeId, InvokeID);
        client.client.write(responseBuffer);
        DEBUG_MSG_F("CO selVal");
        DEBUG_MSG_F("\n");

        IEC61850_8_2.controlSelectWithValue_flag = false;
    }
    //if (IEC61850_8_2.controlCancel_flag) {
    //    sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><write><CHOICE><success/></CHOICE></write></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo, attributeFrom, attributeId, InvokeID);
    //    client.client.write(responseBuffer);
    //    Serial.println(F("CANCEL"));
    //
    //    IEC61850_8_2.controlCancel_flag = false;
    //}
    //if (IEC61850_8_2.controlOperate_flag) {
    //    sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><write><CHOICE><success/></CHOICE></write></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo, attributeFrom, attributeId, InvokeID);
    //    client.client.write(responseBuffer);
    //    Serial.println(F("OPER"));
    //
    //    IEC61850_8_2.controlOperate_flag = false;
    //}
    if (IEC61850_8_2.getLDDir_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><getNameList moreFollows=\"false\"><listOfIdentifier><Identifier>IEDNameLDINst</Identifier><Identifier>CONTROL</Identifier></listOfIdentifier></getNameList></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo, attributeFrom, attributeId, InvokeID);
        client.client.write(responseBuffer);
        DEBUG_MSG_F("LDDir");
        DEBUG_MSG_F("\n");

        IEC61850_8_2.getLDDir_flag = false;
    }
}
void SBO_CO_norm_secure_state_machine_run(void) {
    switch (SBO_CO_norm_secure_state)
    {
        case Unselected:

            break;
        case PerformTest:

            break;
        case Ready:

            break;
        case WaitForActivationTime:

            break;
        case WaitForExecution:

            break;
        case Operate:

            break;
        default:

            break;
    }
}
void SBO_CO_enha_secure_state_machine_run(void) {
    switch (SBO_CO_enha_secure_state)
    {
        case Unselected:

            break;
        case Ready:

            break;
        case WaitForActivationTime:

            break;
        case PerformTest:

            break;
        case WaitForExecution:

            break;
        case Operate:

            break;
        default:

            break;
    }
}
