/* ==========================================================================
 * core_433.h
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
 * --------------------------------------------------------------------------
 */

#ifndef CORE_433_H_
#define CORE_433_H_

#include <pthread.h>
#include <unistd.h>
#include "RCSwitch.h"
#include "RcOok.h"
#include "ledManager.h"


#define CORE433_MOD_RCS	1
#define CORE433_MOD_DIO 2
#define CORE433_MOD_PHENIX 3

class core_433
{
	private:
		RCSwitch   * mySwitch;	// Used for RF433 communication
		pthread_t    myThread;	// Thread structure

		static void * receptionLoop(void *);	// Thread function

	public:
		core_433(int _rxpin, int _txpin, int _rxepin, int _txepin);

		~core_433();

		void sendMess(char * _m, int _len,int module); 		// send a message like sensors hexcode in string, len in bit <= 256
		void sendMess(char * _m, int module); 					// send a 24b message like sensors
		void sendMess(unsigned long int _v, int module); 	// send a 24b message like sensors
		void sendMess(unsigned long int _v, int _l, int module); // send a message like sensors with size _l <= 32
		void loop (void);

};


#endif /* CORE_433_H_ */
