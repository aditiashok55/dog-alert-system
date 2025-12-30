#include <SoftwareSerial.h> 
// Pin definitions 
#define SOUND_SENSOR A0 
#define SHOCK_SENSOR A1 
#define BLUETOOTH_TX 2 
#define BLUETOOTH_RX 3 
// Threshold values 
#define SOUND_THRESHOLD 500    // Adjust based on testing 
#define SHOCK_THRESHOLD 500    // Adjust based on testing 
// Time intervals 
#define SAMPLING_INTERVAL 1000 // 1 second 
#define ALERT_COOLDOWN 5000    // 5 seconds 
SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX); 
// Variables to store sensor readings 
int soundLevel; 
int shockLevel; 
unsigned long lastAlertTime = 0; 
bool alertActive = false;
void setup() { 
// Initialize serial communications 
Serial.begin(9600); 
bluetooth.begin(9600); 
// Configure pins 
pinMode(SOUND_SENSOR, INPUT); 
pinMode(SHOCK_SENSOR, INPUT); 
// Initial delay to allow sensors to stabilize 
delay(1000); 
} 
void loop() {
    // Read sensor values 
    soundLevel = analogRead(SOUND_SENSOR); 
    shockLevel = analogRead(SHOCK_SENSOR); 
        
    // Check for alerts 
    checkAlerts(); 
        
    // Send periodic updates 
    sendSensorData(); 
        
    delay(SAMPLING_INTERVAL); 
    } 
    
    void checkAlerts() { 
    unsigned long currentTime = millis(); 
        
    // Check if we're outside the cooldown period 
    if (currentTime - lastAlertTime >= ALERT_COOLDOWN) { 
        if (soundLevel > SOUND_THRESHOLD || shockLevel > SHOCK_THRESHOLD) { 
        // Generate alert 
        String alertMessage = createAlertMessage(); 
        sendAlert(alertMessage); 
            
        // Update alert timing 
        lastAlertTime = currentTime; 
        alertActive = true; 
        } 
    } 
    } 
    
    String createAlertMessage() { 
    String message = "ALERT:"; 
        
    if (soundLevel > SOUND_THRESHOLD) { 
        message += "HIGH_SOUND,"; 
    } 
    if (shockLevel > SHOCK_THRESHOLD) { 
        message += "IMPACT_DETECTED,"; 
    } 
        
    message += String(soundLevel) + "," + String(shockLevel); 
    return message;

} 
void sendAlert(String message) { 
// Send via Bluetooth 
bluetooth.println(message); 
// Also send via Serial for debugging 
Serial.println(message); 
} 
void sendSensorData() { 
String dataMessage = "DATA:" + String(soundLevel) + "," + String(shockLevel); 
bluetooth.println(dataMessage); 
Serial.println(dataMessage); 
}