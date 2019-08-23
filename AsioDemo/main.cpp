#include <iostream>
#include "bass.h"
#include "bassasio.h"

using namespace std;

void main() {
	
	setlocale(LC_ALL, "Rus");

	//Before initializing device we can get some info about availible devices
	//Here I just get ids of different devices and output if it's availible

	cout << "Devices info:" << endl;
	int a, count = 0;
	BASS_DEVICEINFO info;
	for (a = 1; BASS_GetDeviceInfo(a, &info); a++) {
		if (info.flags&BASS_DEVICE_ENABLED) // device is enabled
			cout << "Device " << a << ") " << info.name << " is availible" << endl;
		else
			cout << "Device " << a << ") " << info.name << " is unable" << endl;
	}
	cout << " ________ " << endl;



	//Also we need to get all devices for asio driver
	cout << "ASIO Devices info:" << endl;
	a = 0; count = 0;
	BASS_ASIO_DEVICEINFO asio_info;
	for (a = 0; BASS_ASIO_GetDeviceInfo(a, &asio_info); a++)
		cout << "Device " << a << ") " << asio_info.name << endl;
	cout << " ________ " << endl;



	//Initialize Bass
	//Set some params and catch errors
	try {
		if ( ! BASS_Init( 1, 44100, 0, 0, NULL) )
			throw BASS_ErrorGetCode();
		if ( ! BASS_ASIO_Init( 0, NULL ) )
			throw BASS_ASIO_ErrorGetCode();
	}
	catch ( int err ) {
		cout << "Err no - " << err << endl;
		system("pause");
		return;	
	}
	
	//Get info about input channels 
	a = 0; BASS_ASIO_CHANNELINFO channel_info;
	cout << "inputs: " << endl;	
	for (a = 0; BASS_ASIO_ChannelGetInfo(0, a, &channel_info ); a++ )
		cout << a << ") " << channel_info.name << " format: " << channel_info.format << endl;
	
	//Get info about output channels 
	cout << "Outputs: " << endl;
	for (a = 0; BASS_ASIO_ChannelGetInfo(1, a, &channel_info); a++)
		cout << a << ") " << channel_info.name << " format: " << channel_info.format << endl;

	cout << "__________" << endl;

	//mirror the mic to the left headphone
	BASS_ASIO_ChannelEnableMirror( 0, 1, 0 );
	BASS_ASIO_ChannelEnableMirror( 1, 1, 0 );
	
	BASS_ASIO_Start(0, 0);
	
	system("pause");
	
	//Free memory and stop all stuff
	BASS_ASIO_Stop();
	BASS_ASIO_Free();
	BASS_Stop();
	BASS_Free();
	return;
}