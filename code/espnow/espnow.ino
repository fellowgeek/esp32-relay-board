/*

To test configure another ESP8266 or 32 and send an ESPNOW message to the boradcast address ff:ff:ff:ff:ff:ff
make sure the message is formatted like this:

ESPNOW message format: >{MAC},{COMMAND}<

example (assuming your device's MAC address is aa:bb:cc:11:22:33):
uint8_t broadcastAddress[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

// send the sommand to activate relay 1
String message = ">aa:bb:cc:11:22:33,RLC1,1";
message.toCharArray(payload, message.length() + 1);
esp_now_send(broadcastAddress, (uint8_t *) &payload, sizeof(payload));

// send the sommand to perform a board test
String message = ">aa:bb:cc:11:22:33,TEST";
message.toCharArray(payload, message.length() + 1);
esp_now_send(broadcastAddress, (uint8_t *) &payload, sizeof(payload));

*/

#include <WiFi.h>
#include <esp_now.h>

// Debug flag
bool debug = false;
// Store the device's MAC address 
String MAC;
// Store the last received command
String lastCommand = "";
// Buffer to hold payload data
char payload[64];
// Timing variables for sending periodic ping messages
unsigned long lastTimePing = 0;
unsigned long timerPing = 55000;
// Broadcast MAC address
uint8_t broadcastAddress[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
// Information about the ESP-NOW peer
esp_now_peer_info_t peerInfo;

// Define GPIO pins for relays and LEDs
const int RLY1 = 22;
const int RLY2 = 21;
const int RLY3 = 17;
const int RLY4 = 16;
const int RLY5 = 26;
const int RLY6 = 18;
const int LED1 = 33;
const int LED2 = 4;

// Function to reset the state of relays and LEDs
void resetBoard() {
    // Reset all relays and LEDs
    if(debug == true) { 
        Serial.println("setting everything: LOW"); 
    }
    digitalWrite(RLY1, LOW);
    digitalWrite(RLY2, LOW);
    digitalWrite(RLY3, LOW);
    digitalWrite(RLY4, LOW);
    digitalWrite(RLY5, LOW);
    digitalWrite(RLY6, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
}

// Function to test the board by toggling relays and LEDs
void testBoard() {
    // Reset the board
    resetBoard();
    delay(2500);

    // Test each relay by setting it to HIGH
    if(debug == true) { Serial.println("setting Relay 1: HIGH"); }
    digitalWrite(RLY1, HIGH);
    delay(1000);

    if(debug == true) { Serial.println("setting Relay 2: HIGH"); }
    digitalWrite(RLY2, HIGH);
    delay(1000);

    if(debug == true) { Serial.println("setting Relay 3: HIGH"); }
    digitalWrite(RLY3, HIGH);
    delay(1000);

    if(debug == true) { Serial.println("setting Relay 4: HIGH"); }
    digitalWrite(RLY4, HIGH);
    delay(1000);

    if(debug == true) { Serial.println("setting Relay 5: HIGH"); }
    digitalWrite(RLY5, HIGH);
    delay(1000);

    if(debug == true) { Serial.println("setting Relay 6: HIGH"); }
    digitalWrite(RLY6, HIGH);
    delay(1000);

    // Test each LED by setting it to HIGH
    if(debug == true) { Serial.println("setting LED 1: HIGH"); }
    digitalWrite(LED1, HIGH);
    delay(1000);

    if(debug == true) { Serial.println("setting LED 2: HIGH"); }
    digitalWrite(LED2, HIGH);
    delay(1000);

    // Reset the board again
    delay(2500);
    resetBoard();

    // Reboot the ESP
    ESP.restart();
}

// Function to update the state of a relay
void updateRelay(int relay, int state) {
    int relays[] = {RLY1, RLY2, RLY3, RLY4, RLY5, RLY6};
    for (int x = 0; x < 6; x++) {
        if (relays[x] == relay) {
            digitalWrite(relay, state);
        }
    }
}

// Callback function for handling received data
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int data_len) {
    // Copy the incoming data to the payload buffer
    memcpy(&payload, incomingData, sizeof(payload));
    // Convert the sender's MAC address to a string
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // Find the delimiter in the payload
    int delimiter;
    String payloadString = payload;
    delimiter = payloadString.indexOf(",");

    if (delimiter != -1) {
        // Extract the target MAC address and command from the payload
        String targetMAC = payloadString.substring(0, delimiter);
        String command = payloadString.substring(delimiter + 1, payloadString.length());

        // Check if the command is intended for this device
        if (targetMAC != MAC) {
            return;
        }

        // Check if this is a duplicate command 
        if (command == lastCommand) {
            if (debug == true) {
                Serial.println("duplicate command ignored: " + command);
            }
            return;
        } else {
            lastCommand = command;
        }

        if (debug == true) {
            Serial.println("processing command: " + command);
        }

        // Blink the LED on command received
        digitalWrite(LED1, HIGH);
        delay(100);
        digitalWrite(LED1, LOW);

        // Handle reboot command
        if (command == "REBOOT") {
            ESP.restart();
        }

        // Handle relay control commands
        if (command == "RLY1,1") { updateRelay(RLY1, HIGH); }
        if (command == "RLY1,0") { updateRelay(RLY1, LOW); }
        if (command == "RLY2,1") { updateRelay(RLY2, HIGH); }
        if (command == "RLY2,0") { updateRelay(RLY2, LOW); }
        if (command == "RLY3,1") { updateRelay(RLY3, HIGH); }
        if (command == "RLY3,0") { updateRelay(RLY3, LOW); }
        if (command == "RLY4,1") { updateRelay(RLY4, HIGH); }
        if (command == "RLY4,0") { updateRelay(RLY4, LOW); }
        if (command == "RLY5,1") { updateRelay(RLY5, HIGH); }
        if (command == "RLY5,0") { updateRelay(RLY5, LOW); }
        if (command == "RLY6,1") { updateRelay(RLY6, HIGH); }
        if (command == "RLY6,0") { updateRelay(RLY6, LOW); }

        // Handle a forced ping
        if (command == "PING") {
            String message = "RLC1,PING";
            message.toCharArray(payload, message.length() + 1);
            esp_now_send(broadcastAddress, (uint8_t *) &payload, sizeof(payload));
        }

        // Handle test command
        if (command == "TEST") {
            testBoard();
        }
    }
}

// Callback function for handling sent data
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if(debug == true) {
        Serial.println("ESPNOW message sent.");
    }

    // Blink the LED on message sent
    digitalWrite(LED2, HIGH);
    delay(100);
    digitalWrite(LED2, LOW);
}

// System setup
void setup() {
    // Begin serial IO
    Serial.begin(115200);
    delay(random(1000, 5000));

    // Setup GPIO pins and set their initial states
    pinMode(RLY1, OUTPUT);
    pinMode(RLY2, OUTPUT);
    pinMode(RLY3, OUTPUT);
    pinMode(RLY4, OUTPUT);
    pinMode(RLY5, OUTPUT);
    pinMode(RLY6, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    digitalWrite(RLY1, LOW);
    digitalWrite(RLY2, LOW);
    digitalWrite(RLY3, LOW);
    digitalWrite(RLY4, LOW);
    digitalWrite(RLY5, LOW);
    digitalWrite(RLY6, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);

    // Configure the device as a Wi-Fi station
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Disable sleep mode
    WiFi.setSleep(false);

    // Get the device's MAC address
    MAC = WiFi.macAddress();
    MAC.toLowerCase();
    Serial.println("MAC: " + MAC);

    // Initialize ESP-NOW
    if (esp_now_init() != 0) {
      Serial.println("error initializing ESP-NOW");
      delay(1000);
      ESP.restart();
    }

    // Register callback functions for received and sent data
    esp_now_register_recv_cb(OnDataRecv);
    esp_now_register_send_cb(OnDataSent);

    // Register an ESP-NOW peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("failed to add peer.");
      delay(1000);
      ESP.restart();
    }
}

// Main loop
void loop() {
    // Periodically send ping messages
    if ((millis() - lastTimePing) > timerPing) {
        String message = "PING,RLC1";
        message.toCharArray(payload, message.length() + 1);
        esp_now_send(broadcastAddress, (uint8_t *) &payload, sizeof(payload));
        lastTimePing = millis();
    }
}