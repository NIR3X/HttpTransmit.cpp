#include "Base64.h"
#include <sstream>

// Define the base64 character set
const std::string CBase64::Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Function to encode a vector of bytes to base64
std::string CBase64::Encode(const std::vector<uint8_t>& input) {
	// Create an output stream to store the encoded data
	std::stringstream encodedStream;

	// Iterate through the input string in groups of 3 bytes
	for (size_t i = 0; i < input.size(); i += 3) {
		// Extract 3 bytes from the input
		uint8_t byte1 = input[i];
		uint8_t byte2 = (i + 1 < input.size()) ? input[i + 1] : 0;
		uint8_t byte3 = (i + 2 < input.size()) ? input[i + 2] : 0;

		// Encode the 3 bytes into 4 base64 characters
		uint8_t enc1 = byte1 >> 2;
		uint8_t enc2 = ((byte1 & 0x03) << 4) | (byte2 >> 4);
		uint8_t enc3 = ((byte2 & 0x0F) << 2) | (byte3 >> 6);
		uint8_t enc4 = byte3 & 0x3F;

		// Append the encoded characters to the output stream
		encodedStream << Alphabet[enc1] << Alphabet[enc2] << Alphabet[enc3] << Alphabet[enc4];
	}

	// Pad the output with '=' characters if necessary
	size_t padding = input.size() % 3;
	for (size_t i = 0; i < padding; ++i) {
		encodedStream.put('=');
	}

	// Return the encoded string
	return encodedStream.str();
}

// Function to decode a base64-encoded string to a vector of bytes
std::vector<uint8_t> CBase64::Decode(const std::string& input) {
	// Create an output stream to store the decoded data
	std::vector<uint8_t> decodedStream;

	// Create a reverse lookup table for base64 characters
	static std::string reverseLookup;
	if (reverseLookup.empty()) {
		reverseLookup.resize(256, 255);
		for (size_t i = 0; i < Alphabet.size(); ++i) {
			reverseLookup[Alphabet[i]] = static_cast<char>(i);
		}
	}

	// Iterate through the input string in groups of 4 characters
	for (size_t i = 0; i < input.size(); i += 4) {
		// Extract 4 characters from the input
		char char1 = reverseLookup[input[i]];
		char char2 = reverseLookup[input[i + 1]];
		char char3 = reverseLookup[input[i + 2]];
		char char4 = reverseLookup[input[i + 3]];

		// Decode the 4 characters into 3 bytes
		uint8_t byte1 = (char1 << 2) | (char2 >> 4);
		uint8_t byte2 = ((char2 & 0x0F) << 4) | (char3 >> 2);
		uint8_t byte3 = ((char3 & 0x03) << 6) | char4;

		// Append the decoded bytes to the output stream
		decodedStream.push_back(byte1);
		if (char3 != '=') {
			decodedStream.push_back(byte2);
		}
		if (char4 != '=') {
			decodedStream.push_back(byte3);
		}
	}

	// Return the decoded vector of bytes
	return decodedStream;
}
