
//{007,232,001,202,5,1304b000c8}
//{007,232,001,202,5,1445650a}
//{507,232,4,301,6,11,00,00,00,04,78,78,82}
#include <mcp_can.h>
#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008


MCP_CAN CAN0(9);   //CS pin 10
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 101);
IPAddress bc(192, 168, 1, 255);
unsigned int localPort = 56800;      // local port to listen on
unsigned int bcPort = 56801;      // local port to listen on

// buffers for receiving and sending data

String packet[6];

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  Serial.begin(9600);
  CAN0.begin(CAN_500KBPS);                       // init can bus : baudrate = 500k 
  pinMode(2, INPUT);                            // Setting pin 2 for /INT input
  Serial.println("Device is ready...");
  unsigned char pingRes[8] = {0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  CAN0.sendMsgBuf(200, 0, 8, pingRes);
}

void loop() {
  char packetBuffer[50] = {};  //buffer to hold incoming packet,
  // if there's data available, read a packet
  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];
  int packetSize = Udp.parsePacket();
  if (packetSize) 
  {
    // read the packet into packetBufffer
    Udp.read(packetBuffer, 50);
    String input = packetBuffer;
    input.remove(0,1);
    input.remove(input.length()-1,1);
    packetSplit(input);
    sendCanPacket(packet[3].toInt(), packet[4].toInt(), packet[5]);
  }
  if(!digitalRead(2))                         // If pin 2 is low, read receive buffer
  {
    CAN0.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
    rxId = CAN0.getCanId();                    // Get message ID
    sendEthPacket(rxId, len, rxBuf);
  }
}

void packetSplit(String input)
{
  for(int i=0; i<6; i++)
  {
    packet[i] = "";
  }
  
  // Keep track of current position in array
  int counter = 0;
  
  // Keep track of the last comma so we know where to start the substring
  int lastIndex = 0;
 
  for (int i = 0; i < input.length(); i++) 
  {
    // Loop through each character and check if it's a comma
    if (input.substring(i, i+1) == ",") 
    {
      // Grab the piece from the last index up to the current position and store it
      packet[counter] = input.substring(lastIndex, i);
      // Update the last position and add 1, so it starts from the next character
      lastIndex = i + 1;
      // Increase the position in the array that we store into
      counter++;
    }

    // If we're at the end of the string (no more commas to stop us)
    if (i == input.length() - 1) 
    {
      // Grab the last part of the string from the lastIndex to the end
      packet[counter] = input.substring(lastIndex, input.length());
    }
  }
}

void sendCanPacket(int id, int msgLen, String msg)
{
  unsigned char canMsg[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//  Serial.println(msg);
  for(int i=0; i<msgLen; i++)
  {
    String temp = msg.substring(i*2, (i*2)+2);  
    const char *hexstring = temp.c_str();
    int number = (int)strtol(hexstring, NULL, 16);
    canMsg[i] = number;
  }
  CAN0.sendMsgBuf(id+312, 0, msgLen, canMsg);
}

void sendEthPacket(unsigned int id, unsigned char msgLen, unsigned char msg[])
{
  char  *reply;       // a string to send back
  String  ReplyBuffer = "{507,232,4," + String(id, HEX) + "," + msgLen;       // a string to send back
  for(int i=0; i<msgLen; i++)
  {
    ReplyBuffer += ",";
    if(String(msg[i], HEX).length() < 2)
    {
      ReplyBuffer += "0";
    }
    ReplyBuffer += String(msg[i], HEX);
  }
  ReplyBuffer += "}";
  Udp.beginPacket(bc, bcPort);
  reply = ReplyBuffer.c_str();
  Udp.write(reply);
  Udp.endPacket();
  
}

unsigned int hexToDec(String hexString) {
  unsigned int decValue = 0;
  int nextInt;
  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}
// NOTE: This function can handle a positive decimal value from 0 - 255, and it will pad it
//       with 0's (on the left) if it is less than the desired string length.
//       For larger/longer values, change "byte" to "unsigned int" or "long" for the decValue parameter.
String decToHex(byte decValue, byte desiredStringLength) {
  String hexString = String(decValue, HEX);
  while (hexString.length() < desiredStringLength) hexString = "0" + hexString;
  return hexString;
}

