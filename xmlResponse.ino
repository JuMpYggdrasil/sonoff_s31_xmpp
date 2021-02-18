void xmlResponse(void) {
    char responseBuffer[500];

    if (XMPP_8_2.ping_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'/>"), attributeTo.c_str(), attributeFrom.c_str(), attributeId.c_str());
        client.client.write(responseBuffer);
        //Serial.println(responseBuffer);
        Serial.println(F("pong"));

        XMPP_8_2.ping_flag = false;
    }
    if (IEC61850_8_2.getDataValue_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><read><listOfAccessResult><AccessResult><success><integer>%d</integer></success></AccessResult></listOfAccessResult></read></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo.c_str(), attributeFrom.c_str(), attributeId.c_str(), InvokeID.c_str(), singleIntForRead);
        client.client.write(responseBuffer);
        Serial.println(F("get"));

        IEC61850_8_2.getDataValue_flag = false;
    }
    if (IEC61850_8_2.setDataValue_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><write><CHOICE><success/></CHOICE></write></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo.c_str(), attributeFrom.c_str(), attributeId.c_str(), InvokeID.c_str());
        client.client.write(responseBuffer);
        Serial.println(F("set"));

        IEC61850_8_2.setDataValue_flag = false;
    }
    if (IEC61850_8_2.controlSelect_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><read><listOfAccessResult><AccessResult><success><structure><Data><visible-string>%s</visible-string></Data></structure></success></AccessResult></listOfAccessResult></read></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo.c_str(), attributeFrom.c_str(), attributeId.c_str(), InvokeID.c_str(), ControlResponseString.c_str());
        client.client.write(responseBuffer);
        //Serial.println(responseBuffer);
        Serial.println(F("CO sel"));

        IEC61850_8_2.controlSelect_flag = false;
    }
    if (IEC61850_8_2.controlSelectWithValue_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><write><CHOICE><success/></CHOICE></write></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo.c_str(), attributeFrom.c_str(), attributeId.c_str(), InvokeID.c_str());
        client.client.write(responseBuffer);
        Serial.println(F("CO selVal"));

        IEC61850_8_2.controlSelectWithValue_flag = false;
    }
    //if (IEC61850_8_2.controlCancel_flag) {
    //    sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><write><CHOICE><success/></CHOICE></write></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo.c_str(), attributeFrom.c_str(), attributeId.c_str(), InvokeID.c_str());
    //    client.client.write(responseBuffer);
    //    Serial.println(F("CANCEL"));
    //
    //    IEC61850_8_2.controlCancel_flag = false;
    //}
    //if (IEC61850_8_2.controlOperate_flag) {
    //    sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><write><CHOICE><success/></CHOICE></write></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo.c_str(), attributeFrom.c_str(), attributeId.c_str(), InvokeID.c_str());
    //    client.client.write(responseBuffer);
    //    Serial.println(F("OPER"));
    //
    //    IEC61850_8_2.controlOperate_flag = false;
    //}
    if (IEC61850_8_2.getLDDir_flag) {
        sprintf_P(responseBuffer, PSTR("<iq from='%s' to='%s' id='%s' type='result'><query><confirmed-ResponsePDU><invokeID>%s</invokeID><ConfirmedServiceResponse><getNameList moreFollows=\"false\"><listOfIdentifier><Identifier>IEDNameLDINst</Identifier><Identifier>CONTROL</Identifier></listOfIdentifier></getNameList></ConfirmedServiceResponse></confirmed-ResponsePDU></query></iq>"), attributeTo.c_str(), attributeFrom.c_str(), attributeId.c_str(), InvokeID.c_str());
        client.client.write(responseBuffer);
        Serial.println(F("LDDir"));

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
