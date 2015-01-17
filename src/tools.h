/*
 * tools.h
 *
 *  Created on: 10 sept. 2013
 *      Author: paul
 */

#ifndef TOOLS_H_
#define TOOLS_H_

int startWith(char * str, char *ref );
char * cut(char * dst, char * str, char separator, int field );
char * trim(char * str);
int hexStr2int(char * str);
int getIntFromChar(char c);
char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength);

bool extractWithRex(const char * rex, char * dest, char * buf);
bool getTimeFromString(char * _str, time_t * _dst);

#endif /* TOOLS_H_ */
