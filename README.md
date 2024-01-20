# HTTP Transmit C++ Client

This C++ package provides a client implementation for interacting with an HTTP Transmit server. The client is designed to securely communicate with the server using the FXMS symmetric encryption algorithm. It includes features for establishing a connection and transmitting encrypted data to the server.

## Installation

To use this package, you can clone the repository and compile it using a C++ compiler:

```bash
git clone https://github.com/NIR3X/HttpTransmit.cpp --recurse-submodules
cd HttpTransmit.cpp
make
```

## Usage

Here is an example of how to use the HTTP Transmit C++ client:

```cpp
#include "HttpTransmit.cpp/HttpTransmit.h"
#include <iostream>

int main() {
	std::vector<uint8_t> key = {
		183, 100, 137, 132, 244, 170, 22, 121, 90, 211, 232, 159, 161, 191, 191, 204,
		90, 151, 91, 93, 160, 170, 49, 24, 217, 40, 144, 53, 158, 135, 18, 155,
		53, 246, 43, 92, 16, 194, 241, 104, 81, 173, 18, 60, 202, 244, 194, 252,
		173, 58, 168, 3, 125, 142, 188, 238, 14, 76, 215, 179, 251, 127, 129, 129,
		86, 230, 199, 145, 65, 9, 141, 11, 235, 192, 237, 167, 207, 93, 234, 98,
		229, 239, 163, 105, 138, 151, 77, 3, 45, 79, 181, 162, 157, 38, 176, 7,
		163, 172, 160, 55, 3, 57, 149, 148, 54, 91, 54, 87, 192, 191, 62, 100,
		176, 215, 90, 229, 110, 197, 103, 166, 224, 32, 212, 115, 32, 189, 128, 1,
		27, 96, 170, 0, 154, 229, 207, 62, 117, 165, 69, 72, 20, 162, 41, 76,
		235, 93, 70, 18, 1, 99, 48, 134, 52, 51, 176, 178, 20, 251, 168, 211,
		12, 181, 65, 102, 190, 103, 73, 11, 224, 221, 115, 48, 144, 236, 206, 171,
		95, 2, 222, 207, 8, 57, 165, 202, 102, 73, 86, 67, 62, 239, 89, 212,
		237, 44, 216, 121, 161, 38, 82, 65, 247, 130, 133, 52, 234, 162, 167, 191,
		109, 16, 239, 99, 89, 18, 156, 211, 77, 179, 94, 73, 97, 175, 1, 39,
		68, 81, 101, 217, 117, 82, 220, 181, 177, 120, 109, 2, 107, 208, 74, 228,
		242, 188, 34, 174, 33, 107, 184, 237, 200, 153, 41, 13, 131, 80, 234, 202,
	};
	CHttpTransmit httpTransmit(key, L"http://127.0.0.1:53440/connect", L"http://127.0.0.1:53440/transmit");
	if (httpTransmit) {
		std::cout << "Connected!" << std::endl;
	} else {
		std::cout << "Not connected!" << std::endl;
		return 1;
	}

	CHttpTransmit::CResponse resp = httpTransmit.TransmitTo("https://example.com", "GET", {} /* headers */, {} /* data */);
	if (resp) {
		std::cout << "Success!" << std::endl;
		std::cout << "Status code: " << resp.statusCode << std::endl;
		std::cout << "Response data: " << std::string(resp.data.begin(), resp.data.end()) << std::endl;
	} else if (resp.statusCode == CHttpTransmit::AuthorizationRequired) {
		std::cout << "Connection closed!" << std::endl;
		// You can try to reconnect here if you want
	} else {
		std::cout << "Failure!" << std::endl;
	}
}
```

This example demonstrates how to create an HTTP Transmit client, establish a connection, and transmit an encrypted request to the server. Adjust the parameters and usage according to your specific requirements.

## License
[![GNU AGPLv3 Image](https://www.gnu.org/graphics/agplv3-155x51.png)](https://www.gnu.org/licenses/agpl-3.0.html)  

This program is Free Software: You can use, study share and improve it at your
will. Specifically you can redistribute and/or modify it under the terms of the
[GNU Affero General Public License](https://www.gnu.org/licenses/agpl-3.0.html) as
published by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
