/* ==========================================================================
 * Singleton.h
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
 * This file creates and managed all the Thread used to control the shiedl
 * --------------------------------------------------------------------------
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "core_433.h"
#include "eventManager.h"
#include "ledManager.h"

// Hardware setting for RF433 RX PIN
#define RX_PIN  0	// PIN 1.6 - GPIO0 - RF433 Receiver pin
					// This pin is not the first pin on the RPi GPIO header!
					// Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
					// for more information.
#define TX_PIN  7	// RF433 Transmitter pin - PIN 1.4 - GPIO7
#define RX_ENA  5	// RX - Antenna switch to Rx
#define TX_ENA  4   // TX - Antenna switch to Tx

#define LED   1		// PIN12 / GPIO1
#define LED2  2		// PIN1.7 / GPIO2
#define LED3  3		// PIN1.8 / GPIO3


class Singleton {
protected:
	core_433 			* core433;
	EventManager 		* eventManager;
	LedManager 			* ledManager1;
	LedManager 			* ledManager2;
	LedManager 			* ledManager3;

	static	Singleton	* m_instance;

public:
	Singleton();
    ~Singleton();

    core_433 	  * getCore433();
    EventManager  * getEventManager();
    LedManager	  * getLedManager1();
    LedManager	  * getLedManager2();
    LedManager	  * getLedManager3();

    static void init();
    static Singleton * get();

};




#endif /* SINGLETON_H_ */
