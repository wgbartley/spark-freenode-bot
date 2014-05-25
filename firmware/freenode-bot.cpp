#include "application.h"
#include "FreenodeIRC.h"

Freenode::Freenode(TCPClient * tcp, IPAddress server, uint16_t port, String nick) {
    _tcp = tcp;
    _server = server;
    _port = port;
    _nick = nick;
    
    generateUserString();
}


void Freenode::generateUserString() {
    _user = _nick+" "+_user_host+" "+_user_server+" : "+_user_name;
}


void Freenode::setUserName(String username) {
    _user_name = username;
    
    generateUserString();
}


void Freenode::setUserHost(String host) {
    _user_host = host;
    
    generateUserString();
}


void Freenode::setUserServer(String server) {
    _user_server = server;
    
    generateUserString();
}


void Freenode::useAutoLogin(bool autologin) {
    _autologin = autologin;
}


void Freenode::useOnboardLED(bool use_onboard_led) {
    _use_onboard_led = use_onboard_led;
}


void Freenode::useSerialDebug(bool use_serial_debug) {
    _use_serial_debug = use_serial_debug;
}


void Freenode::send(String str) {
    if(_use_serial_debug)
        Serial.println(" --> "+str);
    
    _tcp->println(str);
}


bool Freenode::connect() {
    if(_use_onboard_led==true)
        RGBcolor(0, 0, 255);
    
    if(_tcp->connect(_server, _port)) {
        _connected = true;
        
        if(_use_serial_debug==true)
            Serial.println(" ==> Connected");
        
        if(_use_onboard_led==true)
            RGBcolor(0, 255, 0);
    } else {
        _connected = false;
        
        if(_use_serial_debug==true)
            Serial.println(" ==> Connect failed");
        
        disconnected();
    }
    
    return _connected;
}


void Freenode::poll() {
    if(_tcp->available()) {
        char c = _tcp->read();
        _data.concat(c);
        
        if(_data.endsWith("\n")) {
            _data.trim();
    
            if(_use_serial_debug==true)        
                Serial.println(_data);
            
            
            // Auto-login
            if(_autologin==true) {
                // USER
                if(_data.indexOf("*** No Ident response")>=0) {
                    send("USER "+_user);
                    
                    sendNick();
                }
                
                // Search for nick already in use
                if(_data.indexOf("Nickname is already in use")>=0)
                    sendNick();
            }
            
            
            // Are we logged in and ready to do stuff?
            if(_data.indexOf("End of /MOTD command.")>=0)
                _is_ready = true;
            
            
            // Respond to PING
            if(_data.indexOf("PING")==0) {
                send("PONG "+_data.substring(6));
                
                // Blink yellow
                if(_use_onboard_led) {
                    RGBcolor(255, 255, 0);
                    delay(100);
                    RGBrevert();
                }
            }
            
            
            // Clear _data
            _data = "";
        }
    }
    
    
    if(!_tcp->connected() && _connected==true) {
        disconnected();
    }
}


void Freenode::disconnected() {
    if(_use_serial_debug==true)
        Serial.println(" ==> Disconnected");
        
    _tcp->stop();
    _connected = false;
    _is_ready = false;
    
    if(_use_onboard_led==true) {
        RGBcolor(255, 0, 0);
    }
}


bool Freenode::quit(String message) {
    if(_is_ready)
        send("QUIT "+message);
    
    return _is_ready;
}


void Freenode::sendNick() {
    String thisNick = _nick;
    
    if(_nick_number>1)
        thisNick += _nick_number;
    
    send("NICK "+thisNick);
    
    _nick_number++;
}


bool Freenode::joinChannel(String channel) {
    if(_is_ready)
        send("JOIN #"+channel);
    
    return _is_ready;
}


bool Freenode::sendMessage(String channel, String message) {
    if(_is_ready)
        send("PRIVMSG #"+channel+" :"+message);
    
    return _is_ready;
}


bool Freenode::leaveChannel(String channel, String message) {
    if(_is_ready)
        send("PART #"+channel+" :"+message);
    
    return _is_ready;
}


void Freenode::RGBcolor(uint8_t r, uint8_t g, uint8_t b) {
    _last_color[0] = r;
    _last_color[1] = g;
    _last_color[2] = b;
    
    RGB.control(true);
    RGB.color(r, g, b);
}


void Freenode::RGBrevert() {
    RGB.control(true);
    RGB.color(_last_color[0], _last_color[1], _last_color[2]);
}


bool Freenode::isReady() {
    return _is_ready;
}
