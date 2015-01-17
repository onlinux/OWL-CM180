/* ==========================================================================
 * Rfrpi_test.cpp
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
 */

#define DEBUG

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

#include "singleton.h"
#include "version.h"



/* ======================================================
 * Main function
 * ------------------------------------------------------
 * Init processes and loop 4 ever
 * ======================================================
 */
int main(int argc, char *argv[]) {

	// wiring Pi startup
	if(wiringPiSetup() == -1) {
		printf("Error during wiringPi Initialization\n");
		exit(1);
	}

	Singleton::init();


	// Create Register event right after initialization
	//printf("Version (%d.%d) Branch (%s) Build date(%u)\n",RFRPI_VERSION_MAJOR,RFRPI_VERSION_MINOR,RFRPI_VERSION_BRANCH,&RFRPI_BUILD_DATE);

	// Send init event to test the shield
	Singleton::get()->getEventManager()->enqueue(RFRPI_EVENT_INIT,NULL);

	// infinite loop
	Singleton::get()->getCore433()->loop();
	delete Singleton::get();
    exit(0);

}

