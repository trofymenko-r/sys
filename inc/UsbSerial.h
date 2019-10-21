/*
 * UsbSerial.h
 *
 *  Created on: Oct 4, 2019
 *      Author: ruslantrofymenko
 */

#ifndef SRC_USBSERIAL_H_
#define SRC_USBSERIAL_H_

#include <string>
#include <vector>
#include <termios.h>

namespace sys {

class CUsbSerial {
public:
	typedef struct {
		std::string Name;
		std::string Device;
	} SDeviceEntry;

public:
	CUsbSerial();
	virtual ~CUsbSerial();

public:
	static std::vector<SDeviceEntry> GetDevicesList(std::string Type = "");
	static int GetDevNum(const std::string& ttyDev);
	static int InitPort(std::string ttyDev, speed_t speed, bool bExclusive = false, struct termios *ptty = nullptr);
};

} /* namespace sys */

#endif /* SRC_USBSERIAL_H_ */
