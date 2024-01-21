#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class CHttpTransmit {
protected:
	static std::wstring NewSessionId();

	bool connected;
	std::wstring transmitUrl, sessionId;
	std::vector<uint8_t> sessionKey;

public:
	static constexpr uint32_t AuthorizationRequired = 401;
	static constexpr int SessionIdLen = 32;

	class CResponse {
	public:
		bool success;
		uint32_t statusCode;
		std::vector<uint8_t> data;

		__forceinline operator bool() {
			return success;
		}
	};

	CHttpTransmit(
		const std::vector<uint8_t>& secretKey,
		const std::wstring& connectUrl,
		const std::wstring& transmitUrl
	);

	__forceinline operator bool() {
		return connected;
	}

	CResponse TransmitTo(
		const std::string& url,
		const std::string& method,
		const std::unordered_map<std::string, std::string>& headers,
		const std::vector<uint8_t>& data
	);
};
