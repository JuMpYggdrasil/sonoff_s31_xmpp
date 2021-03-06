void xmlManage(char* _xmppStanza, int _xmppStanzaLength) {//

    boolean controlObject_flag = false;
    if (XMPP_8_2.type_get_flag) {
        if (XMPP_8_2.domainSpecific_flag) {//getNameList
            if (DomainId == LD1) {
                IEC61850_8_2.getLDDir_flag = true;
            } else if (DomainId == LD2) {
                IEC61850_8_2.getLDDir_flag = true;
            } else {
                if (DomainId.startsWith(LD1)) {

                } else if (DomainId.startsWith(LD2)) {

                }
            }
        }
        if (XMPP_8_2.read_flag) {//Read
            if (XMPP_8_2.domain_id_flag && XMPP_8_2.item_id_flag) {
                if (ItemId.indexOf("$CO$") >= 0) {//Control class model 20.3 select
                    String CO_controlObjectRef;
                    if (ItemId.startsWith(LD1LN1)) {
                        //CO_controlObjectRef => <LDname>/<LNname>$CO$<DOname>
                        LDname = DomainId;
                        LNname = getSplitValue(ItemId, '$', 0);
                        String functionalConstrain = getSplitValue(ItemId, '$', 1);
                        String DOname = getSplitValue(ItemId, '$', 2);
                        controlObjAttribute = getSplitValue(ItemId, '$', 3);
                        controlObjSubAttribute = getSplitValue(ItemId, '$', 4);
                        //String controlObjSubAttributeType = getSplitValue(ItemId, '$', 5);//f
                        CO_controlObjectRef = LDname + "/" + LNname + "$" + functionalConstrain + "$" + DOname;

                        controlObject_flag = true;
                    } else  if (ItemId.startsWith(LD2LN1)) {
                        //CO_controlObjectRef => <LDname>/<LNname>$CO$<DOname>
                        LDname = DomainId;
                        LNname = getSplitValue(ItemId, '$', 0);
                        String functionalConstrain = getSplitValue(ItemId, '$', 1);
                        String DOname = getSplitValue(ItemId, '$', 2);
                        controlObjAttribute = getSplitValue(ItemId, '$', 3);
                        controlObjSubAttribute = getSplitValue(ItemId, '$', 4);
                        //String controlObjSubAttributeType = getSplitValue(ItemId, '$', 5);//f
                        CO_controlObjectRef = LDname + "/" + LNname + "$" + functionalConstrain + "$" + DOname;

                        controlObject_flag = true;
                    }
                    if (controlObject_flag) {
                        if (controlObjAttribute == "SBO") {
                            IEC61850_8_2.controlSelect_flag = true;
                            ControlResponseString = CO_controlObjectRef;
                        } else {
                            ControlResponseString = "";
                        }
                    }
                } else {//DataObject 13.2 getDataValues
                    if (DomainId.startsWith(LD1)) {
                        if (ItemId.startsWith(LD1LN1)) {
                            singleIntForRead = 2;
                        } else {
                            singleIntForRead = 1;
                        }
                        IEC61850_8_2.getDataValue_flag = true;
                    } else if (DomainId.startsWith(LD2)) {
                        if (ItemId.startsWith(LD2LN1)) {
                            singleIntForRead = 2;
                        } else {
                            singleIntForRead = 1;
                        }
                        IEC61850_8_2.getDataValue_flag = true;
                    }
                }

                XMPP_8_2.domain_id_flag = false;
                XMPP_8_2.item_id_flag = false;
            }
            XMPP_8_2.read_flag = false;
        }
        //  if (XMPP_8_2.getVariableAccessAttributes_flag) {//getVariableAccessAttributes
        //      XMPP_8_2.getVariableAccessAttributes_flag = false;
        //  }
        //  if (XMPP_8_2.getNamedVariableListAttributes_flag) {//getNamedVariableListAttributes
        //      XMPP_8_2.getNamedVariableListAttributes_flag = false;
        //  }
        //  if (XMPP_8_2.getNamedTypeAttributes_flag) {//getNamedTypeAttributes
        //      XMPP_8_2.getNamedTypeAttributes_flag = false;
        //  }
        //  if (XMPP_8_2.readJournal_flag) {//readJournal
        //      XMPP_8_2.readJournal_flag = false;
        //  }
        //  if (XMPP_8_2.fileRead_flag) {//fileRead
        //      XMPP_8_2.fileRead_flag = false;
        //  }
        //  if (XMPP_8_2.fileDirectory_flag) {//fileDirectory
        //      XMPP_8_2.fileDirectory_flag = false;
        //  }
        //  if (XMPP_8_2.obtainFile_flag) {//obtainFile
        //      XMPP_8_2.obtainFile_flag = false;
        //  }
        //  if (XMPP_8_2.extendedGetNameList_flag) {//extendedGetNameList_flag
        //      XMPP_8_2.extendedGetNameList_flag = false;
        //  }

        XMPP_8_2.type_get_flag = false;
    }
    if (XMPP_8_2.type_set_flag) {
        if (XMPP_8_2.write_flag) {
            if (XMPP_8_2.domain_id_flag && XMPP_8_2.item_id_flag) {
                if (ItemId.indexOf("$CO$") >= 0) {//Control class model 20.4 select with value
                    String CO_controlObjectRef;
                    if (ItemId.startsWith(LD1LN1)) {
                        //CO_controlObjectRef => <LDname>/<LNname>$CO$<DOname>
                        LDname = DomainId;
                        LNname = getSplitValue(ItemId, '$', 0);
                        String functionalConstrain = getSplitValue(ItemId, '$', 1);
                        String DOname = getSplitValue(ItemId, '$', 2);
                        controlObjAttribute = getSplitValue(ItemId, '$', 3);
                        controlObjSubAttribute = getSplitValue(ItemId, '$', 4);
                        //String controlObjSubAttributeType = getSplitValue(ItemId, '$', 5);//f
                        CO_controlObjectRef = LDname + "/" + LNname + "$" + functionalConstrain + "$" + DOname;

                        controlObject_flag = true;
                    } else  if (ItemId.startsWith(LD2LN1)) {
                        //CO_controlObjectRef => <LDname>/<LNname>$CO$<DOname>
                        LDname = DomainId;
                        LNname = getSplitValue(ItemId, '$', 0);
                        String functionalConstrain = getSplitValue(ItemId, '$', 1);
                        String DOname = getSplitValue(ItemId, '$', 2);
                        controlObjAttribute = getSplitValue(ItemId, '$', 3);
                        controlObjSubAttribute = getSplitValue(ItemId, '$', 4);
                        //String controlObjSubAttributeType = getSplitValue(ItemId, '$', 5);//f
                        CO_controlObjectRef = LDname + "/" + LNname + "$" + functionalConstrain + "$" + DOname;

                        controlObject_flag = true;
                    }
                    if (controlObject_flag) {//iec61850-8-2 P.133 table.67
                        if (controlObjAttribute == "SBOw") {
                            IEC61850_8_2.controlSelectWithValue_flag = true;
                        } else if (controlObjAttribute == "Cancel") {
                            IEC61850_8_2.controlCancel_flag = true;
                        } else if (controlObjAttribute == "Oper") {
                            IEC61850_8_2.controlOperate_flag = true;
                        }
                    }
                } else {//DataObject 13.3 setDataValues
                    if (DomainId.startsWith(LD1)) {
                        if (ItemId.startsWith(LD1LN1)) {
                            //??? = VariableDataString.toInt();
                            DEBUG_MSG(VariableDataString.toInt());
                            DEBUG_MSG_F("\n");
                        }
                        IEC61850_8_2.setDataValue_flag = true;
                    } else if (DomainId.startsWith(LD2)) {
                        if (ItemId.startsWith(LD2LN1)) {
                            //??? = VariableDataString.toInt();
                            DEBUG_MSG(VariableDataString.toInt());
                            DEBUG_MSG_F("\n");
                        }
                        IEC61850_8_2.setDataValue_flag = true;
                    }
                }
                XMPP_8_2.domain_id_flag = false;
                XMPP_8_2.item_id_flag = false;
            }
            XMPP_8_2.write_flag = false;
        }
        //  if (XMPP_8_2.definedNamedVariableList_flag) {//definedNamedVariableList
        //      XMPP_8_2.definedNamedVariableList_flag = false;
        //  }
        //  if (XMPP_8_2.deleteNamedVariableList_flag) {//deleteNamedVariableList
        //      XMPP_8_2.deleteNamedVariableList_flag = false;
        //  }
        //  if (XMPP_8_2.fileOpen_flag) {//fileOpen
        //      XMPP_8_2.fileOpen_flag = false;
        //  }
        //  if (XMPP_8_2.fileClose_flag) {//fileClose_flag
        //      XMPP_8_2.fileClose_flag = false;
        //  }
        //  if (XMPP_8_2.fileDelete_flag) {//fileDelete_flag
        //      XMPP_8_2.fileDelete_flag = false;
        //  }

        XMPP_8_2.type_set_flag = false;
    }
    if (struct_data_flag) {//get hidden message
        if (controlObject_flag) {//check both CO and struct
            if (*(_xmppStanza + _xmppStanzaLength + 1) != '\0') {
                DEBUG_MSG_F("listOfData structure: ");
                DEBUG_MSG(_xmppStanza + _xmppStanzaLength + 1);
                DEBUG_MSG_F("\n");

                XMLDocument xmlDocument;

                DEBUG_MSG(ESP.getFreeHeap(), DEC);
                DEBUG_MSG_F(" ==> ");
                // check format that can parser & parse
                //if (xmlDocument.Parse(xmppStanza.c_str()) != XML_SUCCESS) {
                if (xmlDocument.Parse(_xmppStanza + _xmppStanzaLength + 1) != XML_SUCCESS) {
                    DEBUG_MSG_F("Error parsing\n");
                    return;
                }
                DEBUG_MSG(ESP.getFreeHeap(), DEC);
                DEBUG_MSG_F("\n");

                XMLElement* root = xmlDocument.RootElement();

                xmlDocument.Clear();
            }
        }
        struct_data_flag = false;
    }
    ESP.wdtFeed();
}
