/*
 * Copyright (c) 2013 Timo Kerstan.  All right reserved.
 *
 * This file is part of Aquaduino.
 *
 * Aquaduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Aquaduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Aquaduino.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <Time.h>
#include <EthernetUdp.h>
#include <Aquaduino.h>

/*
 * UDP definitions to retrieve the current time by NTP during runtime
 * By default NTP resync is performed every 300s.
 */
const int NTP_PACKET_SIZE = 48;

/** \brief ptbtime1.ptb.de
 *
 */
IPAddress sntpServerIP(192, 53, 103, 108);

/**
 * \brief Routine for NTP synchronization.
 *
 * This routine contacts the NTP server specified in sntpServerIP.
 * It waits 1 second for a response. If there is no response a timeout
 * occurs and the routine exits and returns 0 as time.
 *
 * \returns Time since 1970 in seconds + timeZoneOffset in seconds
 */

time_t NTPSync()
{
    EthernetUDP Udp;

    long timeZoneOffset = __aquaduino->getTimezone() * SECS_PER_HOUR;
    unsigned char packetBuffer[NTP_PACKET_SIZE];
    const unsigned long seventy_years = 2208988800UL;
    unsigned long secsSince1900;

    uint8_t status = 0;
    uint16_t timeout = 1000;

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
    Udp.beginPacket(*(__aquaduino->getNTP()), 123);
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
        return secsSince1900 - seventy_years + timeZoneOffset;
    }
    else
    {
        return 0;
    }
}
