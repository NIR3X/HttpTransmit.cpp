#include "Fxms.cpp/Fxms.h"
#include "Fxms.cpp/Rand.h"
#include "HttpRequest.cpp/HttpRequest.h"
#include "Base64.h"
#include "HttpTransmit.h"

std::wstring CHttpTransmit::NewSessionId() {
	std::vector<uint8_t> sessionIdRaw(SessionIdLen);
	CRand::Read(sessionIdRaw.data(), SessionIdLen);
	std::string sessionId = CBase64::Encode(sessionIdRaw);
	return { sessionId.begin(), sessionId.end() };
}

CHttpTransmit::CHttpTransmit(
	const std::vector<uint8_t>& secretKey,
	const std::wstring& connectUrl,
	const std::wstring& transmitUrl
) : connected(false), transmitUrl(transmitUrl),
sessionId(NewSessionId()), sessionKey(CFxms::GenKey()) {
	std::vector<uint8_t> sessionKeyEnc = {};
	CFxms::Status status = CFxms::Encrypt(secretKey, sessionKey, sessionKeyEnc, CFxms::Mode::OptimizeDecryption);
	if (status != CFxms::Status::Success) {
		return;
	}

	CHttpRequest::COptions options = {};
	options.headers[L"HT-Session-ID"] = sessionId;
	std::string sessionKeyEncBase64 = CBase64::Encode(sessionKeyEnc);
	options.headers[L"HT-Session-Key"] = { sessionKeyEncBase64.begin(), sessionKeyEncBase64.end() };

	uint32_t statusCode = 0;
	std::vector<uint8_t> resultEnc = CHttpRequest::Request(connectUrl, options, statusCode);
	if (resultEnc.size() >= CFxms::HashLen + CFxms::MaskLen) {
		std::vector<uint8_t> result = {};
		CFxms::Status status = CFxms::Decrypt(sessionKey, resultEnc, result, CFxms::Mode::OptimizeEncryption);
		if (status == CFxms::Status::Success) {
			connected = true;
		}
	}
}

CHttpTransmit::CResponse CHttpTransmit::TransmitTo(
	const std::string& url,
	const std::string& method,
	const std::unordered_map<std::string, std::string>& headers,
	const std::vector<uint8_t>& data
) {
	std::string headersStr = url + "\n" + method;
	for (const auto& header : headers) {
		headersStr += "\n" + header.first + ": " + header.second;
	}
	std::vector<uint8_t> headersEnc = {};
	CFxms::Status status = CFxms::Encrypt(sessionKey, { headersStr.begin(), headersStr.end() }, headersEnc, CFxms::Mode::OptimizeDecryption);
	if (status != CFxms::Status::Success) {
		return {};
	}

	CHttpRequest::COptions options = {};
	options.headers[L"HT-Session-ID"] = sessionId;
	std::string headersEncBase64 = CBase64::Encode(headersEnc);
	options.headers[L"HT-Session-Headers"] = { headersEncBase64.begin(), headersEncBase64.end() };
	status = CFxms::Encrypt(sessionKey, data, options.data, CFxms::Mode::OptimizeDecryption);
	if (status != CFxms::Status::Success) {
		return {};
	}

	CHttpTransmit::CResponse response = {};
	std::vector<uint8_t> resultEnc = CHttpRequest::Request(transmitUrl, options, response.statusCode);
	if (resultEnc.size() >= CFxms::HashLen + CFxms::MaskLen) {
		std::vector<uint8_t> result = {};
		CFxms::Status status = CFxms::Decrypt(sessionKey, resultEnc, result, CFxms::Mode::OptimizeEncryption);
		if (status == CFxms::Status::Success) {
			response.success = true;
			response.data = result;
		}
	}

	return response;
}
