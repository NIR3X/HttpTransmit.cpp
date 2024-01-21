#pragma once

#include <cstdint>
#include <string>
#include <vector>

class CBase64 {
public:
	static constexpr std::string Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	static std::string Encode(const std::vector<uint8_t>& input);
	static std::vector<uint8_t> Decode(const std::string& input);
};
