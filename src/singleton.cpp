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
 * This file creates and managed all the Thread used to control the shield
 *  * Core433 object is used for RF433 communication
 *  * LedManager{1..3} object is used to control the 3 led behavior
 *  * EvenManager object is managing the events received from the RF433 object
 *        made to be expendable
 * --------------------------------------------------------------------------
 */

#include <cstdio>
#include <cstring>
#include <iostream>


#include "singleton.h"

Singleton * Singleton::m_instance = NULL;

Singleton::Singleton() {
	this->core433 = NULL;
	this->eventManager = NULL;
	this->ledManager1 = NULL;
	this->ledManager2 = NULL;
	this->ledManager3 = NULL;
}

Singleton::~Singleton() {
	delete this->core433;
	delete this->eventManager;
	delete this->ledManager1;
	delete this->ledManager2;
	delete this->ledManager3;
}


core_433 	  * Singleton::getCore433() { return this->core433; }
EventManager  * Singleton::getEventManager() { return this->eventManager; }
LedManager	  * Singleton::getLedManager1() { return this->ledManager1; }
LedManager	  * Singleton::getLedManager2() { return this->ledManager2; }
LedManager	  * Singleton::getLedManager3() { return this->ledManager3; }

// ========================================================
// Build instances
// ========================================================
void Singleton::init()
{
	Singleton * s = new Singleton();

#ifdef TRACESINGLETON
	printf("Singleton::init() - entering \n");
#endif

	// This led is used fro RF433 communication
	// blink twice when a new data is received
	// fast blink during data transmission
	s->ledManager1 = new LedManager(LED);
	s->ledManager1->setMode(LM_BLINKM_OFF);

	// This led is not used for any special function
	s->ledManager2 = new LedManager(LED2);
	s->ledManager2->setMode(LM_BLINKM_OFF);

	// This led is not used for any special function
	s->ledManager3 = new LedManager(LED3);
	s->ledManager3->setMode(LM_BLINKM_OFF);

#ifdef TRACESINGLETON
	printf("Singleton::init() - ledManager created \n");
#endif


	// Lancement de la reception des signaux 433MHz
	s->core433 = new core_433(RX_PIN,TX_PIN,RX_ENA,TX_ENA);

#ifdef TRACESINGLETON
	printf("Singleton::init() - core_433 created \n");
#endif


	// Event Manager ... ici on a la plupart des fonctionnalités
	s->eventManager = new EventManager("rfrpi0");			// rpi Id

#ifdef TRACESINGLETON
	printf("Singleton::init() - EventManager created \n");
#endif

	Singleton::m_instance = s;

#ifdef TRACESINGLETON
	printf("Singleton::init() - Singleton is ready \n");
#endif

}

// ===========================================================================
// Get instance
// ===========================================================================
Singleton * Singleton::get() {
	 if( Singleton::m_instance != NULL ) {
		 return Singleton::m_instance;
	 } else {
		 std::cout << "Singleton::get() - singleton not initialized" << std::endl;
		 return NULL;
	 }
}
