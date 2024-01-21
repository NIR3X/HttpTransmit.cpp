#include "Base64.h"
#include <sstream>

std::string CBase64::Encode(const std::vector<uint8_t>& data) {
	std::stringstream encodedStream = {};

	size_t i = 0;
	do {
		uint8_t o1 = data[i]; ++i;
		uint8_t o2 = (i < data.size()) ? data[i] : 0; ++i;
		uint8_t o3 = (i < data.size()) ? data[i] : 0; ++i;

		uint32_t bits = ((uint32_t)o1 << 16) | ((uint32_t)o2 << 8) | (uint32_t)o3;

		uint8_t h1 = bits>>18 & 0x3f;
		uint8_t h2 = bits>>12 & 0x3f;
		uint8_t h3 = bits>>6 & 0x3f;
		uint8_t h4 = bits & 0x3f;

		encodedStream << Alphabet[h1] << Alphabet[h2] << Alphabet[h3] << Alphabet[h4];
	} while (i < data.size());

	switch (data.size() % 3) {
	case 1:
		encodedStream.seekp(-2, std::ios_base::end);
		encodedStream << "==";
		break;
	case 2:
		encodedStream.seekp(-1, std::ios_base::end);
		encodedStream << "=";
		break;
	}

	return encodedStream.str();
}

std::vector<uint8_t> CBase64::Decode(const std::string& data) {
	std::vector<uint8_t> decodedStream = {};

	static std::string reverseLookup = {};
	if (reverseLookup.empty()) {
		reverseLookup.resize(256, 255);
		for (size_t i = 0; i < Alphabet.size(); ++i) {
			reverseLookup[Alphabet[i]] = i;
		}
	}

	size_t i = 0;
	do {
		char h1 = reverseLookup[data[i]]; ++i;
		char h2 = reverseLookup[data[i]]; ++i;
		char h3 = reverseLookup[data[i]]; ++i;
		char h4 = reverseLookup[data[i]]; ++i;

		uint32_t bits = (uint32_t)h1<<18 | (uint32_t)h2<<12 | (uint32_t)h3<<6 | (uint32_t)h4;

		uint8_t o1 = bits>>16 & 0xff;
		uint8_t o2 = bits>>8 & 0xff;
		uint8_t o3 = bits & 0xff;

		if (h3 == 64) {
			decodedStream.push_back(o1);
		} else if (h4 == 64) {
			decodedStream.push_back(o1);
			decodedStream.push_back(o2);
		} else {
			decodedStream.push_back(o1);
			decodedStream.push_back(o2);
			decodedStream.push_back(o3);
		}
	} while (i < data.size());

	return decodedStream;
}
