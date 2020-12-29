# CSerialEx

## Overview

Simple UART Terminal Software for Linux (MacOS Compatible).  

## Usage

### Installation

 1. clone this repository.
 1. `make install` to install. The executable file `serterm` will be generated under `/usr/local/bin`.
 
**NOTE**: If you want to uninstall it, run `make uninstall`.

### Connect to serial ports

Running serterm, it will automatically scan for valid serial ports and display them in the list.  

    ~ $ ./main
    Discovering serial ports...
    [0] /dev/cu.Bluetooth-Incoming-Port
    [1] /dev/cu.usbserial-XXXXXXXX
    Which port do you use? 

Enter the number of the port you want to use and it will try to open that port.  

    Opening Serial port /dev/cu.usbserial-XXXXXXXX ...
    Open port
    Condigure settings
    Apply...

If serial-port connection could be established sucessfully, this message is showed.

    [SUCCESS] connection has been established.

But if some error has occured, error message will be output to console.  

    ERROR couldn't establish connection to serial port /dev/cu.usbserial-XXXXXXXX.

### Communicate

If the serial port can be opened successfully, start to listen to it.  
The process is executed other thread, main thread always waits user interactions.  
Type the content to be sent and press the Enter key to send it to the port.  

To close the connection, type `Ctrl-C (^C)`.  
## Licence

All files in this repository is published under the MIT Licence.  
In details, show [LICENSE](LICENSE).  
