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
#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include <pthread.h>
#include <list>

#include "ledManager.h"
#include "core_433.h"


#define RFRPI_EVENT_NONE				0
// Receiving a sensor Data
#define RFRPI_EVENT_INIT				1
#define RFRPI_EVENT_GETSENSORDATA		2



// Maximum size for an event message
#define RFRPI_EVENT_MAX_SIZE			8192

class EventManager
{
	private:

		class Event {
		public:
			int eventType;
			char strValue[RFRPI_EVENT_MAX_SIZE];
		};

		std::list<Event> eventList;			// event queue
		pthread_t   	 myThread;			// Thread structure
		pthread_mutex_t  eventListMutex;	// ensure queuing / dequeuing well managed

		char * piId;

		static void * eventLoop(void *);	// Thread function

	public:
		EventManager(char * _piId);
		~EventManager();

		bool enqueue(int newEvent, char * _strValue);			// Add an event with eventQueue type

};




#endif /* EVENTMANAGER_H_ */
