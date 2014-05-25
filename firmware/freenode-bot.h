#include "application.h"

class Freenode {
    private:
        IPAddress _server;
        
        uint16_t _port = 6667;
        
        String _nick = "";
        String _user = "";
        String _user_name = "Spark IRC Bot";
        String _user_host = "spark";
        String _user_server = "core";
        String _data = "";
        
        uint8_t _last_color[3] = { 0, 0, 0 };
        
        uint8_t _nick_number = 1;

        bool _connected = false;
        bool _use_onboard_led = true;
        bool _autologin = true;
        bool _use_serial_debug = false;
        bool _is_ready = false;
        
        TCPClient * _tcp;
        
        void generateUserString();
        void disconnected();
        void sendNick();
        void RGBcolor(uint8_t r, uint8_t g, uint8_t b);
        void RGBrevert();
    
    public:
        Freenode(TCPClient * tcp, IPAddress server, uint16_t port, String nick);
        void setNick(String nick);
        void setUserName(String username);
        void setUserHost(String host);
        void setUserServer(String server);
        void useAutoLogin(bool autologin);
        void useOnboardLED(bool use_onboard_led);
        void useSerialDebug(bool use_serial_debug);
        bool connect();
        bool quit(String message);
        void send(String str);
        void poll();
        bool joinChannel(String channel);
        bool leaveChannel(String channel, String message);
        bool sendMessage(String channel, String message);
        bool isReady();
};
