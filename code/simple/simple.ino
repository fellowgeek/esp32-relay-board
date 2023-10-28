// Debug flag
bool debug = true;

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
}

// Main loop
void loop() {
    testBoard();
}