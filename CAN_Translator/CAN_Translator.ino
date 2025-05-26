#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10
#define CAN_INT 2

MCP_CAN CAN(CAN_CS);  // Set CS pin

const uint32_t originalID = 0x334;
const uint32_t newID = 0x513;

unsigned long lastRxTime = 0;
unsigned long lastTestSendTime = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  SPI.begin();  // Arduino Nano default SPI pins

  while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ)) {
    Serial.println("CAN init failed, retrying...");
    delay(100);
  }
  Serial.println("CAN init success");

  pinMode(CAN_INT, INPUT);
  CAN.setMode(MCP_NORMAL);  // Enable normal CAN mode
}

void loop() {
  // === Read incoming CAN messages ===
  if (!digitalRead(CAN_INT)) {
    long unsigned int rxId;
    byte len = 0;
    byte rxBuf[8];
    byte ext = 0;

    if (CAN.readMsgBuf(&rxId, &ext, &len, rxBuf) == CAN_OK) {
      lastRxTime = millis();  // Update time of last valid message

      Serial.print("Received ID: 0x"); Serial.println(rxId, HEX);
      Serial.print("Frame type: "); Serial.println(ext ? "Extended" : "Standard");
      Serial.print("Length: "); Serial.println(len);
      Serial.print("Data: ");
      for (int i = 0; i < len; i++) {
        Serial.print(rxBuf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();

      // Forward specific message
      if ((rxId & 0x1FFFFFFF) == originalID && len == 8) {
        uint16_t lf = rxBuf[1] << 8 | rxBuf[0];
        uint16_t rf = rxBuf[3] << 8 | rxBuf[2];
        uint16_t lr = rxBuf[5] << 8 | rxBuf[4];
        uint16_t rr = rxBuf[7] << 8 | rxBuf[6];

        Serial.print("LF: "); Serial.print(lf);
        Serial.print(" RF: "); Serial.print(rf);
        Serial.print(" LR: "); Serial.print(lr);
        Serial.print(" RR: "); Serial.println(rr);

        byte txBuf[8];
        memcpy(txBuf, rxBuf, 8);
        byte sendResult = CAN.sendMsgBuf(newID, 0, 8, txBuf);
        if (sendResult == CAN_OK) {
          Serial.println("Message re-sent with new ID");
        } else {
          Serial.print("Error sending message. Code: ");
          Serial.println(sendResult);
        }

        delay(1);  // Short buffer delay
      }
    }
  }

  // === Send test frame only if no real CAN traffic ===
  if (millis() - lastRxTime > 5000 && millis() - lastTestSendTime > 5000) {
    byte testBuf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    byte result = CAN.sendMsgBuf(newID, 0, 8, testBuf);
    Serial.print("Test frame sent to 0x");
    Serial.print(newID, HEX);
    Serial.print(", result: ");
    Serial.println(result == CAN_OK ? "Success" : "Failure");
    lastTestSendTime = millis();
  }
}
