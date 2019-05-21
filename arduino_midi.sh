#This script requires "brew install gbevin/tools/sendmidi"
#Usage for Arduino Serial USBTiny ./arduino_midi.sh -a usbtiny 
#Usage for Arduino Serial USBASP ./arduino_midi.sh -a usbasp
#Usage for Arduino MIDI USBTiny ./arduino_midi.sh -m usbtiny
#Usage for Arduino MIDI USBASP ./arduino_midi.sh -m usbasp

echo "======== PROGRAMANDO O PANDIVÁ ========="

while getopts a:m: option
do
case "${option}"
in
a) 
    cd /Users/fcac/Dropbox/Arduino/_estudos/midi/hiduino-master/compiled_firmwares
    avrdude -p at90usb82 -F -P usb -c $OPTARG -U flash:w:usbserial_mega_8u2.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m
    ls /dev/tty.*
    echo "Agora é Arduino Serial"
;;
m) 
    cd /Users/fcac/Dropbox/Arduino/_estudos/midi/hiduino-master/src_PANDIVA/arduino_midi
    avrdude -p at90usb82 -F -P usb -c $OPTARG -U flash:w:arduino_midi.hex U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m
    sendmidi list
    echo "Agora é Arduino MIDI"
;;
esac
done
echo "========================================"



