/* ==========================================================================
 * core_433.cpp
 * --------------------------------------------------------------------------
 * RF433 demonstrator for rfrpi Raspberry PI shield
 * see : http://www.disk91.com/?p=1323
 * --------------------------------------------------------------------------
 * This software is under GPLv3
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * --------------------------------------------------------------------------
 *
 *  Created on: 23 Feb. 2014
 *   Author: disk91 - Paul Pinault (c) 2014
 * --------------------------------------------------------------------------
 * This Class is managing RF433 reception, it is working as a Thread listening
 * to RCSwitch results, analyze the data and create a message with the data for
 * the event manager.
 * The shield is able to send and receive and share one antenna for both RX & TX
 * as a consequence, a rfswitch is used to separate RX & TX. As this rf switch is not
 * stoping totally the signal, data send are also received (witch is a good way to debug)
 * --------------------------------------------------------------------------
 */

#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "core_433.h"
#include "eventManager.h"
#include "singleton.h"


//#define TRACECORE433

/* ==========================================================
 * core_433 : initialize 433Mhz communication system
 * ----------------------------------------------------------
 * Before being called, wiringPiSetup() must be called and success
 */
core_433::core_433(int _rxpin, int _txpin, int _rxepin, int _txepin) {
	 mySwitch = NULL;
	 mySwitch = new RCSwitch(_rxpin,_txpin,_rxepin,_txepin);
     pthread_create(&myThread,NULL, receptionLoop, this);

}

core_433::~core_433() {
	mySwitch = NULL;
}


/* =========================================================
 * Send message prototypes, remap to the RCSwitch_ interface
 * ---------------------------------------------------------
 */
void core_433::sendMess(char * _m,int _len, int module) {
	DecodeOOK * m = NULL;
    switch ( module ) {
    case CORE433_MOD_PHENIX :
    case CORE433_MOD_RCS :
        m = new RCSwitch_(1,mySwitch);
    	break;

    case CORE433_MOD_DIO :
        m = new DIO(mySwitch);
    	break;

    }
	if ( m != NULL ) {
    	m->send(_m,_len);
		delete m;
	}
}

void core_433::sendMess(char * _m, int module) {
	DecodeOOK * m = NULL;
    switch ( module ) {
    case CORE433_MOD_PHENIX :
    case CORE433_MOD_RCS :
        m = new RCSwitch_(1,mySwitch);
    	break;

    case CORE433_MOD_DIO :
        m = new DIO(mySwitch);
    	break;

    }
	if ( m != NULL ) {
    	m->send(_m);
		delete m;
	}

}

void core_433::sendMess(unsigned long int _v, int module) {
	DecodeOOK * m = NULL;
    switch ( module ) {
    case CORE433_MOD_PHENIX :
    case CORE433_MOD_RCS :
        m = new RCSwitch_(1,mySwitch);
    	break;

    case CORE433_MOD_DIO :
        m = new DIO(mySwitch);
    	break;

    }
	if ( m != NULL ) {
    	m->send(_v,24);
		delete m;
	}
}

void core_433::sendMess(unsigned long int _v,int _l, int module) {
	DecodeOOK * m = NULL;
    switch ( module ) {
    case CORE433_MOD_PHENIX :
    case CORE433_MOD_RCS :
        m = new RCSwitch_(1,mySwitch);
    	break;

    case CORE433_MOD_DIO :
        m = new DIO(mySwitch);
    	break;

    }
	if ( m != NULL ) {
    	m->send(_v,_l);
		delete m;
	}
}


/* ========================================================
 * Thread to manage reception
 */
void * core_433::receptionLoop( void * _param ) {
	core_433 * myCore = (core_433 *) _param;
	RCSwitch * mySwitch = myCore->mySwitch;
	char _tmpStr[RCSWITCH_MAX_MESS_SIZE];

	while(1) {

		// Scan for sensor code
		if ( mySwitch->getOokCode(_tmpStr) ) {
#ifdef TRACECORE433
		std::cout << "* Core_433::receptionLoop() - received message [" << _tmpStr << "]" << std::endl;
#endif
			if ( _tmpStr && strlen(_tmpStr) > 4 ) {
				if ( Singleton::get() != NULL ) {
				   Singleton::get()->getEventManager()->enqueue(RFRPI_EVENT_GETSENSORDATA,_tmpStr);
				}
			}
		}
	    usleep(5000); // 5 ms sleep before next pooling
	}
}

void core_433::loop( void ) {

	pthread_join(myThread,NULL);

}



