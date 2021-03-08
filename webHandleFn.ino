void handleRoot() {
    //    String rootPage = "";
    //    rootPage.concat(FPSTR(WEB_HEAD));
    //    rootPage.concat(FPSTR(WEB_STYLE));
    //    rootPage.concat(FPSTR(WEB_BODY_START));
    //    rootPage.concat(FPSTR(WEB_SIDENAV));
    //    rootPage.concat(FPSTR(WEB_CONTENT_START));
    //    rootPage.concat(F("<div style=\"font-weight:bold\">Upload</div>"));
    //    rootPage.concat(F("<form action='/upload' method='post' enctype='multipart/form-data'>"));
    //    rootPage.concat(F("<input type='file' name='uploadBtn'>"));
    //    rootPage.concat(F("<input class='button' type='submit'>"));
    //    rootPage.concat(F("</form>"));
    //
    //    rootPage.concat(FPSTR(WEB_BODY_HTML_END));
    //
    //    server.send(200, "text / html", rootPage);

    server.send_P(200, text_plain, PSTR("IED (support IEC61850 on ESP8266)"));
}
void handleNotFound() {
    //    String message = F("File Not Found\n\n");
    //    message.concat(F("URI: "));
    //    message.concat(server.uri());
    //    message.concat(F("\nMethod: "));
    //    message.concat((server.method() == HTTP_GET) ? "GET" : "POST");
    //    message.concat(F("\nArguments: "));
    //    message.concat(server.args());
    //    message.concat(F("\n"));
    //    for (uint8_t i = 0; i < server.args(); i++) {
    //        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    //    }
    //    server.send(404, "text/plain", message);
    server.send_P(404, text_plain, PSTR("File Not Found"));
}
void handleTime() {
    time_t now = time(nullptr);//now type is long,%ld
    char nowCString[20];
    sprintf(nowCString, "%ld", now);
    //            debugV("epoch: %s", nowCString);//epoch:unix timestamp
    //            // struct tm* p_tm = localtime(&now);
    //            // debugD("t: %s", asctime (p_tm));//human readable

    server.send(200, "text/plain", nowCString);
}
//void handleFileUpload() {
//    //if (server.uri() != "/info") return;
//
//    HTTPUpload& upload = server.upload();
//    if (upload.status == UPLOAD_FILE_START) {
//        String filename = upload.filename;
//        if (!filename.startsWith("/")) filename = "/" + filename;
//        //DBG_OUTPUT_PORT.print("handleFileUpload Name: "); DBG_OUTPUT_PORT.println(filename);
//        fsUploadFile = SPIFFS.open(filename, "w");
//        filename = String();
//    } else if (upload.status == UPLOAD_FILE_WRITE) {
//        //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
//        if (fsUploadFile)
//            fsUploadFile.write(upload.buf, upload.currentSize);
//    } else if (upload.status == UPLOAD_FILE_END) {
//        if (fsUploadFile)
//            fsUploadFile.close();
//        //DBG_OUTPUT_PORT.print("handleFileUpload Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
//    }
//
//    String uploadPage = "";
//    uploadPage.concat(FPSTR(WEB_HEAD));
//    uploadPage.concat(FPSTR(WEB_STYLE));
//    uploadPage.concat(FPSTR(WEB_BODY_START));
//    uploadPage.concat(FPSTR(WEB_SIDENAV));
//    uploadPage.concat(FPSTR(WEB_CONTENT_START));
//
//    uploadPage.concat(F("<div>file upload ok</div>"));
//    uploadPage.concat(FPSTR(WEB_BODY_HTML_END));
//
//    server.send(200, "text/html", uploadPage);
//
//}
