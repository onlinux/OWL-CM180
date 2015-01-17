/* ==========================================================================
 * LedManager.h
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
#ifndef LEDMANAGER_H_
#define LEDMANAGER_H_

#include <pthread.h>
#include <unistd.h>

#define LM_BLINKM_ON		0			/* light on */
#define LM_BLINKM_OFF		1			/* light off */
#define LM_BLINKM_NORMAL	2			/* 1s blink */
#define LM_BLINKM_FAST		3			/* 100ms blink */
#define LM_BLINKM_TWICE		4			/* 2 blink 1/2s */


class LedManager
{
	private:
		pthread_t    myThread;	// Thread structure
		int			 pin;		// pin to be used for LED
		long		 loopCount;	// 100ms loop count
		long 		 fastCount;	// count loop to automatically stop after 1s
		int			 blinkMode;	// chosen mode
		int			 previMode;	// previous mode
		bool		 ledState;	// ON / OFF

		static void * ledLoop(void *);	// Thread function

	public:
		LedManager(int _pin);
		~LedManager();

		void loop (void);
		void setMode( int );	// change le mode de clignotement.

		int getPin(void);
};




#endif /* LEDMANAGER_H_ */
