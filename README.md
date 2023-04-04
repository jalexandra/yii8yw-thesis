# Managed Access Control System Physical Implementation

<p align="center">
  Jámbor Alexandra Thesis Work <br />
  YII8YW <br />
  Eszterházy Károly Catholic University <br />
  Eger, Hungary
  <br />
  <br />
</p>

## Introduction

This project is two possible implementation of a simple managed access control system. This work is a continuation of the thesis work of Dombi Tibor Dávid. See the [original project](https://github.com/dombidav/hl5u4v-thesis) for more information.

## Hardware

This project can be run in a variety of hardware configurations. The following are the ones that were tested and are known to work:

**Common:**

- RFID-RDM6300
- 125kHz RFID Keyfob
- 1x Green LED
- 1x Red LED
- L293D Motor Driver
- 12V DC Motor

**Only for fully-distributed Version:**

- NodeMCU (ESP8266, CP2102)
- Any WiFi Router with 2.4GHz band

**Only for centralized version:**

- Raspberry Pi 4 Model B
- Arduino Uno

## Software

### Webservice:

To setup the webservice follow the instructions in the [webservice repository](https://github.com/dombidav/hl5u4v-thesis/blob/main/README.md)

### Physical Implementation:

To deploy this project, you need to install the following software:

- [Arduino IDE](https://www.arduino.cc/en/software)
  - Also add ESP8266 Board Support 
  > Add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to the `Additional Boards Manager URLs` in the `File` -> `Preferences` menu
- [CP2102 Driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers?tab=downloads) (only for NodeMCU)
- [Raspberry Pi imager](https://www.raspberrypi.org/software/) (only for Raspberry Pi)
- [Node-RED](https://nodered.org/docs/getting-started/raspberrypi) (only for Raspberry Pi)

## Setup

1. Follow the [setup instructions](https://github.com/dombidav/hl5u4v-thesis/blob/main/README.md) for the webservice and start it. Make sure the webservice is reachable from the physical implementation. 

> If you are using `php artisan serve` command, you can add the `--host=0.0.0.0:8000` flag to make it reachable from other devices on the same network.

2. Clone this repository

```bash
git clone https://github.com/jalexandra/yii8yw-thesis.git
```

### Fully-distributed Version

3. Connect the LEDs, RFID Reader and Motor Driver to the NodeMCU as shown in the following diagram:

   <p align="center">
     <a href="https://raw.githubusercontent.com/jalexandra/yii8yw-thesis/main/doc/images/distributed_bp.png"><img src="https://raw.githubusercontent.com/jalexandra/yii8yw-thesis/main/doc/images/distributed_bp.png" alt="Fully-distributed Version"></a>
   </p>

4. Open the `esp_web.ino` file in the `src/distributed` folder in the Arduino IDE
5. Configure the defined values according to your setup
6. Upload the code to the NodeMCU
7. Restart the NodeMCU (Arduino IDE will restart it automatically)

The NodeMCU should automatically connect to the configured router and it is ready to use.

### Centralized Version

3. Connect the LEDs, RFID Reader and Motor Driver to the Arduino Uno as shown in the following diagram:
   
   <p align="center">
     <a href="https://raw.githubusercontent.com/jalexandra/yii8yw-thesis/main/doc/images/centralized_bp.png"><img src="https://raw.githubusercontent.com/jalexandra/yii8yw-thesis/main/doc/images/centralized_bp.png" alt="Centralized Version"></a>
   </p>

4. Open the `arduino_raspb.ino` file in the `src/centralized` folder in the Arduino IDE
5. Configure the defined values according to your setup
6. Upload the code to the Arduino Uno
7. Restart the Arduino Uno (Arduino IDE will restart it automatically)
8. Connect the Arduino Uno to the Raspberry Pi using a USB cable
9. Enter the Raspberry Pi through SSH or by connecting a monitor and keyboard
10. Install Node-RED by following the [instructions](https://nodered.org/docs/getting-started/raspberrypi)
11. Install the following Node-RED nodes:

```bash
npm install node-red-contrib-serialport
```

11. Copy the contents of the `node-red.json` file in the `src/centralized` folder to the clipboard
12. Open the Node-RED editor in your browser and click on the `Import` button in the top right corner
13. Paste the contents of the clipboard into the text area and click on the `Import` button
14. Click on the `Deploy` button in the top right corner

The Raspberry Pi should automatically restart and it is ready to use.

## Usage
Use the admin panel in the original project to add the locks and workers.

Touch the RFID keyfob to the RFID reader to open the door. The LEDs will indicate the status of the door.

## License
This project is licensed under the MIT License - see the [LICENSE](https://github.com/jalexandra/yii8yw-thesis/blob/main/LICENSE) file for details

## Acknowledgements
- **[Dombi Tibor Dávid](https://github.com/dombidav)** for the original project
- **Tajti Tibor** as the supervisor of the thesis work
- **[Eszterházy Károly Catholic University](https://uni-eszterhazy.hu/)** for the opportunity to work on this project
