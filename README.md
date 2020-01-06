# Can-to-Eth
Can bus to Ethernet Converter Using Arduino Uno
This device will forward any packet that is receiving on can port to ethernet and vice versa.

## Setup Device
To setup this device we need:
* [Arduino UNO R3](https://www.amazon.com/Arduino-Uno-R3-Microcontroller-A000066/dp/B008GRTSV6) x1

![Arduino Uno R3](https://cdn.sparkfun.com//assets/parts/6/3/4/3/11021-04.jpg "Arduino Uno R3")

* [CAN to SPI module](https://www.amazon.com/MCP2515-Module-TJA1050-receiver-controller/dp/B01LYZ8874) x1

![CAN to SPI module](https://images-na.ssl-images-amazon.com/images/I/71uRRuiO1GL._SX425_.jpg "CAN to SPI module")

* [Ethernet module](https://www.amazon.com/SunFounder-Ethernet-Shield-W5100-Arduino/dp/B00HG82V1A?th=1) x1

![Ethernet module](https://cdn-shop.adafruit.com/970x728/201-00.jpg)

First of all attach Ethernet shield on you arduino Uno. Then connect the CAN to SPI module as the picture bellow.

![Can to SPI connection](https://14core.com/wp-content/uploads/2015/12/OBDCAN-Arduino-SPI-Communication-Bus-Illustration-Diagram.jpg "Arduino Can to SPI connection")

For more information you can visit the [reference](https://www.14core.com/wiring-the-mcp2515-controller-area-network-can-bus-diagnostics/) website.

Device Picture:

![Can To Ethernet Converter](https://i.imgur.com/zECSkJx.jpg "Can To Ethernet Converter")

## Details


## How to run
After setup phase open Arduino ide and program the device with code. Simply set device ip, mac address and communication ports on line 13-18. This device forward every message that receive on device "ip" address to Canbus and forward every messages that receive from Can to ethernet on "bc" ip.  

```
IPAddress ip(192, 168, 1, 101);
IPAddress bc(192, 168, 1, 255);
unsigned int localPort = 56800;      // local port to listen on
unsigned int bcPort = 56801;      // local port to listen on
```

## Author
Farshid Abdollahi

