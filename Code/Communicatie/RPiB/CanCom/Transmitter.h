
/*!
 * @file Transmitter.h
 * @class Transmitter
 * @brief Interface klasse voor de Transmitter in de Communicatie package
 * 
 * Regelt het versturen van berichten op een socket voor een controller.
 */

#pragma once

#include <vector>
#include <stdint.h>

namespace Communication {
	class Transmitter {
	public:
		Transmitter() = default;
		~Transmitter() = default;

		/*!
		* @brief Verstuurt een bericht.
		*
		* Verstuurt een bericht naar het meegegeven adres.
		* 
		* @param int - Het adres waar het bericht naartoe verzonden moet worden.
		* @param vector<uint8_t>* - De data die in het bericht meegegeven moet worden.
		*/
		virtual void sendData(int address, std::vector<uint8_t> data) = 0;

		/*!
		* @brief Verstuurt een bericht zonder data.
		*
		* Verstuurt een bericht zonder data naar het meegegeven adres.
		* 
		* @param int - Het adres waar het bericht naartoe verzonden moet worden.
		*/
		virtual void sendRequest(int address) = 0;
	};
}