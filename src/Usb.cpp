/*
 * Usb.cpp
 *
 *  Created on: Oct 17, 2019
 *      Author: ruslantrofymenko
 */

#include <Usb.h>
#include <App.h>
#include <ustring.h>

using namespace std;

namespace sys {

CUsb::CUsb() {
	// TODO Auto-generated constructor stub
}

CUsb::~CUsb() {
	// TODO Auto-generated destructor stub
}

void CUsb::GetDevNum()
{

}

string CUsb::GetDeviceClass(const string& Path)
{
	string Result;
	string Path_ = Path;

	if (Path_.back() != '/')
		Path_.append("/");

	string ClassFile = Path_ + "bDeviceClass";
	if (!CApp::FileExists(ClassFile))
		return Result;

	Result = CApp::Exec("cat " + Path_ + "bDeviceClass");
	RemoveNewLineChar(Result);

	return Result;
}

} /* namespace sys */

