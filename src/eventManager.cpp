/* ==========================================================================
 * EventManager.cpp
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
 * This Class is managing event coming from the sensors. This is the main
 * entry point where you can modify behavior of the system by implementing
 * your logic
 * --------------------------------------------------------------------------
 */
#include <iostream>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "eventManager.h"
#include "tools.h"
#include "singleton.h"
#include "version.h"
#include "Sensor.h"

//#define TRACEEVENTMNG

/* ==========================================================
 * Construction / Destruction
 */
EventManager::EventManager(char * _piId) {
	this->eventListMutex = PTHREAD_MUTEX_INITIALIZER;
	this->piId = _piId;
    pthread_create(&this->myThread,NULL, eventLoop, this);
}

EventManager::~EventManager() {
	pthread_cancel(myThread);
	pthread_join(myThread, NULL);
	myThread = pthread_self();
}



/* ==========================================================
 * Event enqueuing
 */
bool EventManager::enqueue(int newEvent, char * _strValue) {
	Event ev;
	ev.eventType = newEvent;
	if ( _strValue != NULL ) {
		if ( strlen(_strValue) > RFRPI_EVENT_MAX_SIZE-1 ) {
#ifdef TRACEEVENTMNG
	printf("** ERROR ** EventManager::enqueue() - enqueue (%s) TOO LONG \n",_strValue);
#endif
		}
		strncpy( ev.strValue, _strValue, RFRPI_EVENT_MAX_SIZE-1);
	} else ev.strValue[0] = '\0';

	pthread_mutex_lock( &this->eventListMutex );
#ifdef TRACEEVENTMNG
	printf("EventManager::enqueue() - enqueue (%s)\n",_strValue);
#endif
	this->eventList.push_back(ev);
	pthread_mutex_unlock( &this->eventListMutex );
}


/* ==========================================================
 * Thread to manage events ...
 */
void * EventManager::eventLoop( void * _param ) {
	EventManager * myEventManager = (EventManager *) _param;

	while(1) {
		while ( ! myEventManager->eventList.empty() ) {

			pthread_mutex_lock( &myEventManager->eventListMutex );
			Event ev = myEventManager->eventList.front();
			myEventManager->eventList.pop_front();
			pthread_mutex_unlock( &myEventManager->eventListMutex );

			switch (ev.eventType) {

			// ------------------------------------------------------------
   		    // INIT OF MODULE - check leds & RX / TX
			case RFRPI_EVENT_INIT:
				#ifdef TRACEEVENTMNG
					printf("EventManager::eventLoop() - proceed INIT \n");
				#endif
				if ( Singleton::get() ) {
					Singleton::get()->getLedManager1()->setMode(LM_BLINKM_TWICE);
					Singleton::get()->getLedManager2()->setMode(LM_BLINKM_TWICE);
					Singleton::get()->getLedManager3()->setMode(LM_BLINKM_TWICE);
					usleep(2000000);

					// Send welcome message .. should be received right after !
					Singleton::get()->getCore433()->sendMess(0x123456,CORE433_MOD_RCS);
				}

				break;


			// ------------------------------------------------------------
   		    // Just received an event from the RF module
			case RFRPI_EVENT_GETSENSORDATA:
				#ifdef TRACEEVENTMNG
					printf("EventManager::eventLoop() - proceed GETSENSORDATA \n");
				#endif
				if ( Singleton::get() ) {
					Singleton::get()->getLedManager1()->setMode(LM_BLINKM_TWICE);
				}

				{
					Sensor * s = Sensor::getRightSensor(ev.strValue);
					if ( s != NULL && s->isDecoded() ) {
						time_t ltime;
						struct tm * curtime;
						char buffer[80];

						time( &ltime );
						curtime = localtime( &ltime );
						strftime(buffer,80,"%F %T", curtime);
                        if ( s->availablePower() ) {

                            printf("{\"datetime\": \"%s\", \"name\": \"%s\", \"power\": \"%.0f\", \"total\": \"%.0f\"}", buffer, s->getSensName(), s->getPower(), s->getPowerTotal());
                            std::cout << std::endl;
                        } else {
                      
                            printf("{\"datetime\": \"%s\", \"name\": \"%s\", \"temperature\": \"%.2f\", \"humidity\": \"%.0f\", \"channel\": \"%d\" }", buffer, s->getSensName(), s->getTemperature(), s->getHumidity(), s->getChannel());
                            std::cout << std::endl;
                        }

					} else {
						if ( s == NULL ) {
							// assuming it is the welcome message sent for test
							if ( ev.strValue != NULL && strlen(ev.strValue) >= 11 ) {
								if (strcmp(ev.strValue,"RCSW 123456") == 0) {
									printf("[INFO] Welcome message correctly received, Rx/Tx working fine\n");
								}
							}
						}
					}
				}
				break;



			default:
				#ifdef TRACEEVENTMNG
					printf("EventManager::eventLoop() - proceed UNKNOWN (%s) \n",ev.strValue);
				#endif
				break;


			}

		}
	    usleep(300000); // 300 ms de repos c'est pas un mal
	}
	return NULL;
}
