// This #include statement was automatically added by the Spark IDE.
#include "FreenodeIRC.h"

TCPClient tcpClient;

// 174.143.119.91 = asimov.freenode.net
Freenode irc(&tcpClient, { 174, 143, 119, 91 }, 6667, "my-sparkbot");

void setup() {
    Serial.begin(115200);
    delay(7000);
    
    irc.useSerialDebug(true);
    irc.connect();
    
    Spark.function("ircJoin", ircJoin);
    Spark.function("ircMsg", ircMsg);
    Spark.function("ircLeave", ircLeave);
    Spark.function("ircQuit", ircQuit);
}

void loop() {
    irc.poll();
}


int ircJoin(String str) {
    if(irc.joinChannel(str))
        return 1;
    else
        return -1;
}


int ircMsg(String str) {
    str.replace("%20", " ");
    
    if(irc.sendMessage(str.substring(0, str.indexOf(";")), str.substring(str.indexOf(";")+1)))
        return 1;
    else
        return -1;
}


int ircLeave(String str) {
    str.replace("%20", " ");
    
    if(irc.leaveChannel(str.substring(0, str.indexOf(";")), str.substring(str.indexOf(";")+1)))
        return 1;
    else
        return -1;
}


int ircQuit(String str) {
    if(irc.quit(str))
        return 1;
    else
        return -1;
}
