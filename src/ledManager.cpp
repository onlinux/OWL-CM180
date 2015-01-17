/* ==========================================================================
 * LedManager.cpp
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
 * This Class is managing Leds on the shield
 *  Three leds are available on the shield. This management call allow Leds to be light on
 *  light off, blinking, bliking twice of blinking fastly. The LedManager is an
 *  independent Thread, so you just have to send it an order and the Led is executing it
 *  before being back to the previous state (when applicable)
 * --------------------------------------------------------------------------
 */
#include <wiringPi.h>
#include <stdio.h>
#include "ledManager.h"


/* ==========================================================
 * LedManager : initialize LED manager
 * ----------------------------------------------------------
 * Before being called, wiringPiSetup() must be called and success
 */
LedManager::LedManager(int _pin) {
	 this->pin = _pin;
  	 pinMode (this->pin, OUTPUT);
  	 digitalWrite(this->pin, LOW);

	 this->previMode = LM_BLINKM_ON;
	 this->blinkMode = LM_BLINKM_ON;
	 this->ledState  = true;
	 this->loopCount = 0;
     pthread_create(&myThread,NULL, ledLoop, this);
}

LedManager::~LedManager() {
	pthread_cancel(myThread);
	pthread_join(myThread, NULL);
	myThread = pthread_self();
}

/* ------------------------------------------------
 * Change the Led operation
 * Attention a ne pas boucler dans un mode FAST...
 */
void LedManager::setMode(int _mode) {
	if (   this->blinkMode != LM_BLINKM_FAST
		&& this->blinkMode != LM_BLINKM_TWICE ) {
	   this->previMode = this->blinkMode;
	   this->blinkMode = _mode;
   	   this->fastCount = 0;
	} else {
		if (   _mode != LM_BLINKM_FAST
			&& _mode != LM_BLINKM_TWICE )
		   this->previMode = _mode;
		/* reset is possible to get longer blink
		 * but in fact not the best result...
		else
	   	   this->fastCount = 0;
		 */
	}
}

/* -------------------------------------------------
 * return pin (mostly for debuging purpose
 */
int LedManager::getPin() {
	return this->pin;
}


void * LedManager::ledLoop( void * _param ) {
	LedManager * myLed = (LedManager *) _param;

	while(1) {

		switch ( myLed->blinkMode ) {
		// Always ON
		case LM_BLINKM_ON:
				digitalWrite(myLed->pin, HIGH);
				myLed->ledState = true;
				break;
		// Always OFF
		case LM_BLINKM_OFF:
				digitalWrite(myLed->pin, LOW);
				myLed->ledState = false;
				break;
		// 1000ms ON / 1000 ms OFF
		case LM_BLINKM_NORMAL:
			if ( (myLed->loopCount % 10) == 0 ) {
				if (myLed->ledState) {
					myLed->ledState = false;
					digitalWrite(myLed->pin, LOW);
				} else {
					myLed->ledState = true;
					digitalWrite(myLed->pin, HIGH);
				}
			}
			break;

		// 100ms ON / 100ms OFF - 10 x
		case LM_BLINKM_FAST:
			if (myLed->ledState) {
				myLed->ledState = false;
				digitalWrite(myLed->pin, LOW);
			} else {
				myLed->ledState = true;
				digitalWrite(myLed->pin, HIGH);
			}
			// Dans ce cas, on ne fera de clignotement rapide que durant
			// 1 seconde, puis on revient au mode normal
			if ( myLed->fastCount >= 10 ) {
				myLed->blinkMode = myLed->previMode;
				myLed->fastCount = 0;
			} else {
				myLed->fastCount++;
			}
			break;

		// 200ms ON / 200ms OFF - 2x
		case LM_BLINKM_TWICE:
			if ( (myLed->loopCount % 2) == 0 ) {
				if (myLed->ledState) {
					myLed->ledState = false;
					digitalWrite(myLed->pin, LOW);
				} else {
					myLed->ledState = true;
					digitalWrite(myLed->pin, HIGH);
				}
			}
			// Dans ce cas, on ne fera de clignotement rapide que durant
			// 600ms, puis on revient au mode normal
			if ( myLed->fastCount > 6 ) {
				myLed->blinkMode = myLed->previMode;
				myLed->fastCount = 0;
			} else {
				myLed->fastCount++;
			}
			break;

		default:
			break;

		}

	    usleep(100000); // 100 ms de repos c'est pas un mal
	    myLed->loopCount++;
	}
	return NULL;
}

void LedManager::loop( void ) {

	pthread_join(myThread,NULL);

}


