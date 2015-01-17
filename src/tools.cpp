/*
 * tools.cpp
 *
 * Misc helper
 *
 *  Created on: 10 sept. 2013
 *  Author: Paul Pinault / disk_91
 */


#include <regex.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#define DEBUG

/* =======================================================
 * Retourne vrai si la chaine str commence par la sous-chaine
 * ref, sinon false;
 * -------------------------------------------------------
 */
int startWith(char * str, char *ref ) {
	while ( *str != '\0' && *ref != '\0' && *str == *ref ) {
		str++;
		ref++;
	}
	if ( *ref == '\0' ) return 1;
	return 0;
}

/* ========================================================
 * Cut : type cut -d "seperator" -f field
 * Return in the destination string (allocated)
 * --------------------------------------------------------
 */
char * cut(char * dst, char * str, char separator, int field ) {
	int cField=1;
	char * _dst = dst;
	*_dst='\0';
	while ( *str != '\0' && *str != '\n' ) {
		if ( *str == separator) cField++;
		else if ( cField == field ) { *_dst = *str; _dst++ ;}
		str++;
	}
	*_dst = '\0';
	return dst;
}


/* ==========================================================
 * trim : supression des espaces, devant et derriere
 * ----------------------------------------------------------
 */
char * trim(char * str) {
	char * _s = str;
	char * _d = str;

	while ( *_s == ' ' ) _s++;
	while ( *_s != '\0' ) { *_d = *_s ; _s++ ; _d++; }
	do {
		_d--;
	} while ( *_d == ' ' );
	_d++;
	*_d='\0';
	return str;
}

/* ==========================================================
 * getIntFromChar : convert a Hex char into decimal number
 * ----------------------------------------------------------
 */
int getIntFromChar(char c) {
	char _hexDecod[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
						   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	for ( int i=0 ; i < 16 ; i++ )
	  if ( _hexDecod[i] == c ) return i;
	return -1;
}


/* ==========================================================
 * hexStr2int : convert a Hex string into decimal number
 * ----------------------------------------------------------
 */
int hexStr2int(char * str) {
	int v=0;
	while (*str != '\0') {
		int w = getIntFromChar(*str);
		if ( w < 0 ) return -1;
		v = v << 4;
		v = v | w;
		str++;
	}
	return v;
}

/* ===========================================================
 * Convert a number into binary representation
 */
char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength){
  static char bin[64];
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    }else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';

  return bin;
}


/* ====================================================
 * Extrait une donnée à partir d'une regex
 * On recupere 2 données dan la rex une fois executé
 * Ce qui map la totalité  \0
 * Ce qui est dans la première parenthèse et qui est ce que
 * l'on extrait \1
 * ------------------------------------
 * rex type :  "\"on\":\"([^\"]{12})\""
 */
bool extractWithRex(const char * rex, char * dest, char * buf){
	regex_t reg;
	bool ret=false;
	int err = regcomp(&reg,rex,REG_EXTENDED );
	if (err == 0) {
		size_t nmatch = 2;
		regmatch_t pmatch[2];
		if ( regexec(&reg, buf, nmatch, pmatch, 0 ) == 0 ) {
			int start = pmatch[1].rm_so;
			int end = pmatch[1].rm_eo;
			strncpy(dest,&buf[start],end-start);
			dest[end-start]='\0';
			ret=true;
		}
		regfree(&reg);
	}
	return ret;
}

/* ====================================================
 * Get time from String
 * String format is  : 2013-11-12_10:30:00
 * Return a time_t value
 */
bool getTimeFromString(char * _str, time_t * _dst) {

	struct tm * _tm;
	time( _dst );
	_tm = localtime( _dst );

	char year[10], month[10], day[10], hour[10], min[10], sec[10];
	extractWithRex("([0-9]{4})-[0-9]{2}-[0-9]{2}_[0-9]{2}:[0-9]{2}:[0-9]{2}",year,_str);
	extractWithRex("[0-9]{4}-([0-9]{2})-[0-9]{2}_[0-9]{2}:[0-9]{2}:[0-9]{2}",month,_str);
	extractWithRex("[0-9]{4}-[0-9]{2}-([0-9]{2})_[0-9]{2}:[0-9]{2}:[0-9]{2}",day,_str);
	extractWithRex("[0-9]{4}-[0-9]{2}-[0-9]{2}_([0-9]{2}):[0-9]{2}:[0-9]{2}",hour,_str);
	extractWithRex("[0-9]{4}-[0-9]{2}-[0-9]{2}_[0-9]{2}:([0-9]{2}):[0-9]{2}",min,_str);
	extractWithRex("[0-9]{4}-[0-9]{2}-[0-9]{2}_[0-9]{2}:[0-9]{2}:([0-9]{2})",sec,_str);

	int iyear,imonth,iday,ihour,imin,isec;
	iyear = atoi(year);
	imonth = atoi(month);
	iday = atoi(day);
	ihour = atoi(hour);
	imin = atoi(min);
	isec = atoi(sec);

#ifdef DEBUG
	printf("Tool:getTimeFromString() - read : %d/%d/%d %d:%d:%d \n",iyear,imonth,iday,ihour,imin,isec);
#endif

	_tm->tm_year = iyear - 1900;
	_tm->tm_mon = imonth -1;
	_tm->tm_mday = iday;
	_tm->tm_hour = ihour;
	_tm->tm_min = imin;
	_tm->tm_sec = isec;

	*_dst = mktime( _tm );

	return true;
}
