/*
 * Usb.h
 *
 *  Created on: Oct 17, 2019
 *      Author: ruslantrofymenko
 */

#ifndef SRC_USB_H_
#define SRC_USB_H_

#include <string>

namespace sys {

class CUsb {
public:
	CUsb();
	virtual ~CUsb();

public:
	static void GetDevNum();
	static std::string GetDeviceClass(const std::string& Path);
};

} /* namespace sys */

#endif /* SRC_USB_H_ */
