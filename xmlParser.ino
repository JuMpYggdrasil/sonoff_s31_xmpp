void xmlParser() {//xmppStanza string parse
    boolean attributeBool;
    const char* attributeCString;
    uint32_t  attributeUint32;

    XMLDocument xmlDocument;

    DEBUG_MSG(ESP.getFreeHeap(), DEC);
    DEBUG_MSG(F(" --> "));
    // check format that can parser & parse
    if (xmlDocument.Parse(xmppStanza.c_str()) != XML_SUCCESS) {
        DEBUG_MSG(F("Error parsing\n"));
        return;
    }

    DEBUG_MSG(ESP.getFreeHeap(), DEC);
    DEBUG_MSG(F("\n"));

    XMLElement* root = xmlDocument.RootElement();//iq, presence, message

    if (NULL != root) {
        const char* rootName = root->Name();

        if (strcmp(rootName, "iq") == 0) { //iq stanza
            if (root->QueryStringAttribute("type", &attributeCString) != XML_SUCCESS) {
                DEBUG_MSG(F("type:-\n"));
            } else {
                DEBUG_MSG(F("type: "));//IEC61850-8-2 APPENDIX D
                DEBUG_MSG(attributeCString);
                DEBUG_MSG(F("\n"));

                if (strcmp(attributeCString, "get") == 0) {
                    XMPP_8_2.type_get_flag = true;
                    XMLElement* unknownElement = root->FirstChildElement();
                    XMLElement* envelopElement = NULL;
                    if (NULL != unknownElement) {
                        const char* uTagName = unknownElement->Name();
                        if (strcmp(uTagName, "query") == 0) {
                            envelopElement = unknownElement->FirstChildElement(confirmed_RequestPDU);//"confirmed_RequestPDU"
                        } else if (strcmp(uTagName, confirmed_RequestPDU) == 0) {
                            envelopElement = root->FirstChildElement(confirmed_RequestPDU);//"confirmed_RequestPDU"
                        } else if (strcmp(uTagName, "ping") == 0) {
                            XMPP_8_2.ping_flag = true;
                        } else {
                            DEBUG_MSG(F("not confirmed_RequestPDU\n"));
                        }
                        if (NULL != envelopElement) {
                            XMLElement* serviceElement = envelopElement->FirstChildElement(ConfirmedServiceRequest)->FirstChildElement();
                            if (NULL != serviceElement) {
                                parseInvokeId(envelopElement);
                                const char* strTagName = serviceElement->Name();
                                if (strcmp(strTagName, "read") == 0) {
                                    XMPP_8_2.read_flag = true;
                                    XMLElement* domain_specific = serviceElement->FirstChildElement(variableAccessSpecification)->FirstChildElement(listOfVariable)->FirstChildElement("SEQUENCE")->FirstChildElement(variableSpecification)->FirstChildElement("name")->FirstChildElement("domain-specific");

                                    parseDomainId(domain_specific);
                                    parseItemId(domain_specific);

                                    if (serviceElement->QueryBoolAttribute(specificationWithResult, &attributeBool) != XML_SUCCESS) {
                                        DEBUG_MSG(F("Could not obtain the attribute\n"));
                                    } else {
                                        DEBUG_MSG(F("attr: "));
                                        DEBUG_MSG(attributeBool);
                                        DEBUG_MSG(F("\n"));
                                    }
                                } else if (strcmp(strTagName, "getNameList") == 0) {
                                    XMLElement* domainSpecificElement = serviceElement->FirstChildElement("objectScope")->FirstChildElement("domainSpecific");
                                    if (NULL != domainSpecificElement) {
                                        const char* itemId = domainSpecificElement->GetText();
                                        ItemId = String(itemId);
                                        DEBUG_MSG(ItemId);
                                        DEBUG_MSG(F("\n"));
                                        XMPP_8_2.domainSpecific_flag = true;
                                    }
                                } else if (strcmp(strTagName, "getVariableAccessAttributes") == 0) {
                                    DEBUG_MSG(F("wrong iq type"));
                                } else if (strcmp(strTagName, "write") == 0) {
                                    DEBUG_MSG(F("wrong iq type"));
                                } else if (strcmp(strTagName, "defineNamedVariableList") == 0) {
                                    DEBUG_MSG(F("wrong iq type"));
                                } else if (strcmp(strTagName, "deleteNamedVariableList") == 0) {
                                    DEBUG_MSG(F("wrong iq type"));
                                } else {
                                    DEBUG_MSG(F("unknown serviceElement"));
                                    DEBUG_MSG(F("\n"));
                                }
                            }//if (NULL != serviceElement) {
                        } //if (NULL != envelopElement) {
                    }
                } else if (strcmp(attributeCString, "set") == 0) { //get set result error
                    XMPP_8_2.type_set_flag = true;
                    XMLElement* unknownElement = root->FirstChildElement();
                    XMLElement* envelopElement = NULL;
                    if (NULL != unknownElement) {
                        const char* uTagName = unknownElement->Name();
                        if (strcmp(uTagName, "query") == 0) {
                            envelopElement = unknownElement->FirstChildElement(confirmed_RequestPDU);//"confirmed_RequestPDU"
                        } else if (strcmp(uTagName, "cRP") == 0) {
                            envelopElement = root->FirstChildElement("cRP");//"confirmed_RequestPDU"
                        } else {
                            DEBUG_MSG(F("not confirmed_RequestPDU"));
                        }//confirmed_RequestPDU Element

                        if (NULL != envelopElement) {
                            XMLElement* serviceElement = envelopElement->FirstChildElement(ConfirmedServiceRequest)->FirstChildElement();
                            if (NULL != serviceElement) {
                                parseInvokeId(envelopElement);

                                const char* strTagName = serviceElement->Name();
                                if (strcmp(strTagName, "write") == 0) {
                                    XMPP_8_2.write_flag = true;
                                    XMLElement* domain_specific = serviceElement->FirstChildElement(variableAccessSpecification)->FirstChildElement(listOfVariable)->FirstChildElement("SEQUENCE")->FirstChildElement(variableSpecification)->FirstChildElement("name")->FirstChildElement("domain-specific");

                                    parseDomainId(domain_specific);//Store data in string DomainId,and set flag XMPP_8_2.domain_id_flag
                                    parseItemId(domain_specific);//Store data in string ItemId,and set flag XMPP_8_2.item_id_flag

                                    XMLElement* variableElement = serviceElement->FirstChildElement("listOfData")->FirstChildElement("Data")->FirstChildElement();
                                    if (NULL != variableElement) {
                                        const char* variableName = variableElement->Name();
                                        if (strcmp(variableName, "integer") == 0) {
                                            const char* dataInteger = variableElement->GetText();
                                            VariableDataString = String(dataInteger);
                                            DEBUG_MSG(VariableDataString);

                                            //else if (strcmp(variableName, "float") == 0) {
                                            //else if (strcmp(variableName, "boolean") == 0) {
                                            //else if (strcmp(variableName, "bit-string") == 0) {
                                            //else if (strcmp(variableName, "utc-time") == 0) {
                                        } else if (strcmp(variableName, "structure") == 0) {
                                            //XMLElement* arrayElement = variableElement->FirstChildElement();
                                            //XMLElement* arrayElement = getElementByName(xmlDocument, "octet-string");
                                            //if (NULL != arrayElement) {
                                            //const char* octet = arrayElement->GetText();
                                            //InvokeID = String(octet);
                                            //Serial.println(octet);
                                            //} else {
                                            //Serial.println(F("octet not found"));
                                            //}
                                            DEBUG_MSG(F("struct found"));
                                            DEBUG_MSG(F("\n"));
                                            DEBUG_MSG(ESP.getFreeHeap(), DEC);
                                            DEBUG_MSG(F("\n"));
                                        }//strcmp(variableName,
                                    }//NULL != variableElement
                                } else if (strcmp(strTagName, "getNameList") == 0) {
                                    DEBUG_MSG(F("wrong iq type"));
                                } else if (strcmp(strTagName, "read") == 0) {
                                    DEBUG_MSG(F("wrong iq type"));
                                } else {
                                    DEBUG_MSG(F("unknown serviceElement"));
                                    DEBUG_MSG(F("\n"));
                                }
                            }//serviceElement
                        }//envelopElement
                    }//NULL != unknownElement

                    DEBUG_MSG(F("type set"));
                    DEBUG_MSG(F("\n"));
                } else if (strcmp(attributeCString, "result") == 0) {
                    XMPP_8_2.type_result_flag = true;

                    DEBUG_MSG(F("type result"));
                    DEBUG_MSG(F("\n"));
                } else { //error
                    DEBUG_MSG(F("type error"));
                    DEBUG_MSG(F("\n"));
                }
            }

            if (root->QueryStringAttribute("id", &attributeCString) != XML_SUCCESS) {
                DEBUG_MSG(F("id:-"));
                DEBUG_MSG(F("\n"));
                attributeId = "";
            } else {
                DEBUG_MSG(F("id: "));
                DEBUG_MSG(attributeCString);
                DEBUG_MSG(F("\n"));
                attributeId = attributeCString;
            }

            if (root->QueryStringAttribute("to", &attributeCString) != XML_SUCCESS) {
                DEBUG_MSG(F("to:-"));
                DEBUG_MSG(F("\n"));
                attributeTo = "";
            } else {
                DEBUG_MSG(F("to: "));
                DEBUG_MSG(attributeCString);
                DEBUG_MSG(F("\n"));
                attributeTo = String(attributeCString);
            }

            if (root->QueryStringAttribute("from", &attributeCString) != XML_SUCCESS) {
                DEBUG_MSG(F("from:-"));
                DEBUG_MSG(F("\n"));
                attributeFrom = "";
            } else {
                DEBUG_MSG(F("from: "));
                DEBUG_MSG(attributeCString);
                DEBUG_MSG(F("\n"));
                attributeFrom = String(attributeCString);
            }

        } else if (strcmp(rootName, "presence") == 0) {//presence stanza
            DEBUG_MSG(F("get presence"));
            DEBUG_MSG(F("\n"));
        } else if (strcmp(rootName, "message") == 0) {//message stanza
            DEBUG_MSG(F("get message"));
            DEBUG_MSG(F("\n"));
        } else { //not standard stanza
            DEBUG_MSG(F("not standard stanza"));
            DEBUG_MSG(F("\n"));
        }
    } else {
        //check for XML Stream Features
        DEBUG_MSG(F("parse unknown: "));
    }
    //Serial.println(ESP.getFreeHeap(), DEC);
    xmlDocument.Clear();
    //Serial.println(ESP.getFreeHeap(), DEC);
    // == remove this because memory full error(29) ==
    //    XMLPrinter printer;
    //    xmlDocument.Print(&printer);
    //    Serial.println();
    //    Serial.println(printer.CStr());
}

void parseDomainId(XMLElement * domain_specific_x) {
    if (NULL != domain_specific_x) {
        XMLElement* domainIdElement = domain_specific_x->FirstChildElement("domainId");
        if (NULL != domainIdElement) {
            const char* domainId = domainIdElement->GetText();
            DomainId = String(domainId);
            DEBUG_MSG(DomainId);
            DEBUG_MSG(F("\n"));
            XMPP_8_2.domain_id_flag = true;
        }
    }
}
void parseItemId(XMLElement * domain_specific_x) {
    if (NULL != domain_specific_x) {
        XMLElement* itemIdElement = domain_specific_x->FirstChildElement("itemId");
        if (NULL != itemIdElement) {
            const char* itemId = itemIdElement->GetText();
            ItemId = String(itemId);
            DEBUG_MSG(ItemId);
            DEBUG_MSG(F("\n"));
            XMPP_8_2.item_id_flag = true;
        }
    }
}
void parseInvokeId(XMLElement * envelop_x) {
    XMLElement* invokeIdElement = envelop_x->FirstChildElement("invokeID");
    if (NULL != invokeIdElement) {
        const char* invokeID = invokeIdElement->GetText();
        InvokeID = String(invokeID);
        DEBUG_MSG(InvokeID);
        DEBUG_MSG(F("\n"));
    } else {
        DEBUG_MSG(F("query not found"));
        DEBUG_MSG(F("\n"));
    }
}


//
//XMLElement * getElementByName(XMLDocument & doc, String elemt_value) {
//
//    XMLElement * elem = doc.RootElement(); //Tree root
//    while (elem) {
//        if (String(elem -> Name()) != elemt_value) return (elem);
//        /*elem = elem->NextSiblingElement();*/
//        if (elem -> FirstChildElement()) {
//            elem = elem -> FirstChildElement();
//        } else if (elem -> NextSiblingElement()) {
//            elem = elem -> NextSiblingElement();
//        } else {
//            while (!elem -> Parent() -> NextSiblingElement()) {
//                if (elem -> Parent() -> ToElement() == doc.RootElement()) {
//                    return NULL;
//                }
//                elem = elem -> Parent() -> NextSiblingElement();
//            }
//        }
//    }
//    return (NULL);
//}
