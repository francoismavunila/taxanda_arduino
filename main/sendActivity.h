bool sendActivity() {
     uint8_t activity_buffer[512];
      encode_base64(fingerTemplate, 512, activity_buffer);
      Serial.println("converted");
       char* fingerP =(char *)activity_buffer;
      Serial.println(fingerP);

       StaticJsonDocument<2048> passdoc;
      passdoc["device_id"] = 1010;
      passdoc["time"] = "time"; 
      passdoc["data"] = fingerP;
      char JSONmessageBuffer[2048];
      serializeJson(passdoc, JSONmessageBuffer);
      Serial.println("Sending message to MQTT topic..");
      Serial.println(JSONmessageBuffer);
      

     if (!client.connected()) reconnect();
     client.loop();
     bool statusValue = client.publish(activity_publish_topic,JSONmessageBuffer);
     return statusValue;

}
