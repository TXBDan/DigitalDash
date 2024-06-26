#ifndef NMEA_H
#define NMEA_H
/*
    File:       nmea.h
    Version:    0.1.0
    Date:       Feb. 23, 2013
	License:	GPL v2
    
	NMEA GPS content parser
    
    ****************************************************************************
    Copyright (C) 2013 Radu Motisan  <radu.motisan@gmail.com>
	
	http://www.pocketmagic.net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    ****************************************************************************
 */
#include "mbed.h"


class NMEA {
	
	private:
		bool			m_bFlagRead,					// flag used by the parser, when a valid sentence has begun
						m_bFlagDataReady;				// valid GPS fix and data available, user can call reader functions
		char			tmp_words[20][15],				//	hold parsed words for one given NMEA sentence
						tmp_szChecksum[15];				//	hold the received checksum for one given NMEA sentence
		
		// will be set to true for characters between $ and * only
		bool			m_bFlagComputedCks ;			// used to compute checksum and indicate valid checksum interval (between $ and * in a given sentence)
		int				m_nChecksum ;					// numeric checksum, computed for a given sentence
		bool			m_bFlagReceivedCks ;			// after getting  * we start cuttings the received checksum
		int				index_received_checksum ;		// used to parse received checksum
		
		// word cutting variables
		int				m_nWordIdx ,					// the current word in a sentence
						m_nPrevIdx,						// last character index where we did a cut
						m_nNowIdx ;						// current character index
		
		// globals to store parser results
		float			res_fLongitude;					// GPRMC and GPGGA
		float			res_fLatitude;					// GPRMC and GPGGA
		unsigned char	res_nUTCHour, res_nUTCMin, res_nUTCSec,		// GPRMC and GPGGA 
						res_nUTCDay, res_nUTCMonth, res_nUTCYear;	// GPRMC
		int				res_nSatellitesUsed;			// GPGGA
		int				res_nFix;						// GPGGA
		float			res_fAltitude;					// GPGGA
		float			res_fSpeed;						// GPRMC
		float			res_fBearing;					// GPRMC
		
		// the parser, currently handling GPRMC and GPGGA, but easy to add any new sentences
		void			parsedata();
		// aux functions
		int				digit2dec(char hexdigit);
		float			string2float(char* s);
		int				mstrcmp(const char *s1, const char *s2);
		
	public:
		// constructor, initing a few variables
		NMEA() {
			m_bFlagRead = false; //are we in a sentence?
			m_bFlagDataReady = false; //is data available?
		}
		
		/*
		 * The serial data is assembled on the fly, without using any redundant buffers. 
		 * When a sentence is complete (one that starts with $, ending in EOL), all processing is done on 
		 * this temporary buffer that we've built: checksum computation, extracting sentence "words" (the CSV values), 
		 * and so on.
		 * When a new sentence is fully assembled using the fusedata function, the code calls parsedata. 
		 * This function in turn, splits the sentences and interprets the data. Here is part of the parser function, 
		 * handling both the $GPRMC NMEA sentence:
		 */
		int				fusedata(char c);
		

		// READER functions: retrieving results, call isdataready() first
		bool			isdataready();
		int				getHour();
		int				getMinute();
		int				getSecond();
		int				getDay();
		int				getMonth();
		int				getYear();
		int				getFix();
		float			getLatitude();
		float			getLongitude();
		int				getSatellites();
		float			getAltitude();
		float			getSpeed();
		float			getBearing();
	};

#endif