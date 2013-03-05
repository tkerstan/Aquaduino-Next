#include <Time.h>
#include <EthernetUdp.h>
#include <Aquaduino.h>

/*
 * UDP definitions to retrieve the current time by NTP during runtime
 * By default NTP resync is performed every 300s.
 */
const int NTP_PACKET_SIZE = 48;

IPAddress sntpServerIP(192, 53, 103, 108); //ptbtime1.ptb.de

time_t NTPSync()
{
	EthernetUDP Udp;

	long timeZoneOffset = TIME_ZONE * SECS_PER_HOUR;
	unsigned char packetBuffer[NTP_PACKET_SIZE];
	const unsigned long seventy_years = 2208988800UL;
	unsigned long secsSince1900;

	uint8_t status = 0;
	uint16_t timeout = 1000;
#ifdef DEBUG
	Serial.println(F("Syncing NTP Time..."));
#endif

	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     		// Stratum, or type of clock
	packetBuffer[2] = 6;     		// Polling Interval
	packetBuffer[3] = 0xEC;  		// Peer Clock Precision

	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;

	Udp.begin(8888);
	Udp.beginPacket(*(aquaduino->getNTP()), 123);
	Udp.write(packetBuffer, NTP_PACKET_SIZE);
	Udp.endPacket();

	while (timeout-- > 0)
	{

		if (Udp.parsePacket())
		{
			//OK we got a response
			Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

			//the timestamp starts at byte 40 of the received packet and is four bytes,
			// or two words, long. First, esxtract the two words:

			unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
			unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
			// combine the four bytes (two words) into a long integer
			// this is NTP time (seconds since Jan 1 1900):
			secsSince1900 = highWord << 16 | lowWord;
			status = 1;

		}
		delay(1);
	}

	Udp.stop();

	if (status)
	{
#ifdef DEBUG
		Serial.println(F("NTP sync successful."));
#endif
		return secsSince1900 - seventy_years + timeZoneOffset;
	}
	else
	{
#ifdef DEBUG
		Serial.println(F("NTP sync failed."));
#endif
		return 0;
	}
}
