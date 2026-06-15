
/*!
 * @file Receiver.h
 * @class Receiver
 * @brief Interface klasse voor de Receiver in de Communicatie package
 * 
 * Regelt het ontvangen en van berichten op een socket.
 */

#pragma once

#include <vector>
#include <stdint.h>

namespace Communication {

	class Receiver {
	public:
		Receiver()=default;

		virtual ~Receiver() = default;

		/*!
		* @brief Checkt of er een bericht is ontvangen.
		*
		* Checkt of er een bericht is ontvangen, zo ja wordt de data opgeslagen
		* in de pointers die meegegeven worden als parameters.
		* 
		* @param int* - Een pointer naar een plaats waar het adres van het ontvangen bericht opgeslagen kan worden.
		* @param vector<uint8_t>* - Een pointer naar een plaats waar de data van het ontvangen bericht opgeslagen kan worden.
		* 
		* @return bool - Geeft terug of er een bericht is ontvangen. True, als er een bericht is ontvangen. False, als er GEEN bericht is ontvangen.
		*/
		virtual bool receive(int* address, std::vector<uint8_t>* data) = 0;
	};
}