all: compile flash run

compile:
	particle compile photon ./photon --saveTo app.bin

usbflash:
	sudo particle flash --usb app.bin

flash:
	sudo particle flash hcin720 app.bin

monitor:
	sudo particle serial monitor

run:
	node IA2/server.js
