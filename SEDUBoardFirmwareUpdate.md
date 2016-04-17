# SEDU-Board firmware update #

The SEDU-Board v1 with the standard Ambilight firmware can only control up to 64 channels/ RGB-LEDs. In order to enhance this limit to 256 channels, we need to update the SEDU-Board firmware with a modified one.<br><br>
<b>This manual is only for the SEDU-Board v1 (Hardware v1.9 with Atmega 16 controller)!!</b> <br><br>
The current available firmware update is not a full Ambilight software with integrated stand-alone programs or any configuration. It just receives the incoming miniDMX data and converts it into the WS2801 protocol for the LEDs. If the controller receives no data, the connected LED-Stripes will display their last displayed color.<br>
<br>
This is a short version of the update manual  without screenshots. You can download the whole manual as PDF <a href='https://ambilight-4-mediaportal.googlecode.com/git/Hardware/SEDU-Board/SEDU-Board_FirmwareUpdate.pdf'>here</a>.<br>
<br>
<h2>What you need</h2>

<ul><li>ISP-Programmer with a 6-pin ISP interface (or 6/8 pin adapter). Make sure that the programmer supports the Atmel16 controller!<br>
</li><li>Download and copy/ unzip the needed files<br>
<ul><li><a href='https://ambilight-4-mediaportal.googlecode.com/git/Hardware/SEDU-Board/Bootloader/chip45boot2_atmega16_uart0_v2.9I.zip'>Bootloader</a> + <a href='https://ambilight-4-mediaportal.googlecode.com/git/Hardware/SEDU-Board/Tools/chip45boot2%20GUI%20V1.12.zip'>Chip2BootGUI</a> – <a href='http://www.chip45.com/avr_bootloader_atmega_xmega_chip45boot2.php?en'>Homepage</a> of the developer<br>
</li><li><a href='https://ambilight-4-mediaportal.googlecode.com/git/Hardware/SEDU-Board/Driver/SEDU-FTDI.zip'>SEDU-Board FTDI driver</a>
</li><li><a href='https://ambilight-4-mediaportal.googlecode.com/git/Hardware/SEDU-Board/Firmware/v1.0%20SeduAmbilightCommunity.zip'>SEDU_AmbilightCommunitv v1.0</a> Firmware<br>
</li><li>AVR Programming software (I use <a href='https://ambilight-4-mediaportal.googlecode.com/git/Hardware/SEDU-Board/Tools/myAVR_ProgTool.exe'>MyAVR_ProgTool</a>)</li></ul></li></ul>

<h2>Driver installation</h2>
<ol><li>Connect your SEDU-Board to the PC. You may need an external power supply<br>
</li><li>Windows will automatically install a VCP (Virtual COM Port) driver  wait until its finished<br>
</li><li>Now go to the Device Manager, locate the installed “USB Serial Port”<br>
</li><li>Open up the Context menu (right click) of it and choose “Update Driver Software…”<br>
</li><li>Choose “Browse my computer for driver software”<br>
</li><li>Choose “Let me pick from a list…”<br>
</li><li>The driver was installed before on my computer so you see the “SEDU-Board” entry already in the list. Click “Have Disk…” and navigate the unzipped folder of the FTDI driver<br>
</li><li>Select the “ftdiport.inf” file and click “Open”<br>
</li><li>Now you should only see the “SEDU-Board entry inside the drivers list  select it and click “Next” (ignore the warning about the missing digital signing)<br>
</li><li>After the installation has finished,  open up the context menu of the SEDU-Board and select “Properties”</li></ol>

<ol><li>Set the Port speed to “115200”. (the driver is modified so internally he will work with 250kBaud)<br>
</li><li>DONE!!!</li></ol>

<h2>Bootloader installation</h2>
<ol><li>Connect your ISP Programmer to the PC and install the proper driver for the device<br>
</li><li>Connect the SEDU-Board to the ISP-Programmer<br>
</li><li>Open your AVR program software and select your ISP Programmer and the correct Controller type (Atmega16). For my ISP Programmer I need to select the STK500v2 protocol and COM7<br>
</li><li>Backup the current firmware<br>
</li><li>Select the “chip45boot2_atmega16_uart0_v2.9l.hex” file and set for burning and set every option exactly like it is shown in the screenshot!!! If you don’t use the correct settings for the fuses, you SEDU-Board may not be usable anymore!!!<br>
</li><li>Burn the Bootloader!!!<br>
</li><li>After burning you should see something like this<br>
</li><li>DONE – Disconnect the SEDU-Board from the ISP Programmer</li></ol>

<h2>Uploading the firmware to the Bootloader</h2>
<ol><li>Install the Chip45boot2 GUI software and start the application<br>
</li><li>Connect your SEDU-Board via USB to your computer and plug in an external power supply<br>
</li><li>Select the COM Port, the Baudrate and the firmware file “SEDU-Ambilight.hex”<br>
</li><li>You are only able to connect within the first 2-3 seconds after the SEDU-Board has been plugged in. Select “Connect to Bootloader” and then press the “Reset” button on the SEDU-Board.<br>
</li><li>If the connection is established click “Program Flash” to upload the firmware. After the upload has been finished click “Start Application”<br>
</li><li>DONE – Your SEDU-Board is now ready to use!!!