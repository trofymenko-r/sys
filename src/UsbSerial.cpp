/*
 * UsbSerial.cpp
 *
 *  Created on: Oct 4, 2019
 *      Author: ruslantrofymenko
 */

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <stdlib.h>

#include <UsbSerial.h>
#include <App.h>
#include <Usb.h>
#include <ustring.h>
//#include <uexcept.h>

using namespace std;

namespace sys {

CUsbSerial::CUsbSerial() {
	// TODO Auto-generated constructor stub

}

CUsbSerial::~CUsbSerial() {
	// TODO Auto-generated destructor stub
}

vector<CUsbSerial::SDeviceEntry> CUsbSerial::GetDevicesList(string Type)
{
	vector<SDeviceEntry> Result;
	string SysSerialPath = "/sys/bus/usb-serial/drivers/";

	string DevicePathes = CApp::Exec("find " + SysSerialPath + " -name ttyUSB*");
	vector<string> List = Split(DevicePathes, '\n');

	for (auto& Path: List) {
		RemoveNewLineChar(Path);
		ReplaceStr(Path, SysSerialPath, "");

		vector<string> Pair = Split(Path, '/');
		if (Pair.size() != 2) {
			cerr << string(__func__) + ": parse error" << endl;
			break;
		}

		if (!Type.empty() && Pair[0] != Type)
			continue;

		SDeviceEntry Entry = {Pair[0], Pair[1]};
		Result.push_back(Entry);
	}

	return Result;
}

int CUsbSerial::GetDevNum(const string& ttyDev)
{
	struct dirent *entry;
	string SysBusPath = "/sys/bus/usb/devices";
	DIR *dir = opendir(SysBusPath.c_str());

	if (dir == NULL) {
		closedir(dir);
		return -1;
	}

	while ((entry = readdir(dir)) != NULL) {
		string Device = string(entry->d_name);
		if (Device == "." || Device == "..")
			continue;

		string Path = SysBusPath + "/" + Device + "/";
		string Result = CApp::Exec("find " + Path + " -name ttyUSB2");
		if (Result.empty())
			continue;

		if (CUsb::GetDeviceClass(Path) != "00")
			continue;

		string DevNumFile = Path + "devnum";
		if (!CApp::FileExists(DevNumFile))
			return -1;

		closedir(dir);
		return atoi(CApp::Exec("cat " + DevNumFile).c_str());
	}
	closedir(dir);

	return -1;
}

int CUsbSerial::InitPort(std::string ttyDev, speed_t speed, bool bExclusive, struct termios *ptty)
{
	int fd;
	int ret;
	string DevicePath = "/dev/" + ttyDev;
	struct termios tty;
	int oflag = O_RDWR|O_NOCTTY|O_SYNC;

	if (bExclusive)
		oflag |= O_CLOEXEC;

	fd = open(DevicePath.c_str(), oflag);
	if (fd < 0) {
		cout << DevicePath << " not opened" << endl;
        return fd;
	}

	if (bExclusive) {
		ret = ioctl(fd, TIOCEXCL);
		if (ret != 0) {
			cout << DevicePath << " error TIOCEXCL ioctl" << endl;
			return ret;
		}
	}

	if (ptty) {
		tty = *ptty;
	} else {
		tty.c_cflag = CBAUD | CS8 | CLOCAL | CREAD;
		tty.c_iflag = IGNPAR;
		tty.c_oflag = 0;
		tty.c_lflag = 0;
		tty.c_cc[VMIN] = 1;
		tty.c_cc[VTIME] = 0;
	}

	cfsetospeed(&tty, speed);
	cfsetispeed(&tty, speed);

	if (tcflush(fd,TCIFLUSH) == -1)
		return -1;
	if (tcflush(fd,TCOFLUSH) == -1)
		return -2;
	if (tcsetattr(fd, TCSANOW, &tty) == -1)
		return -3;

	return fd;
}

} /* namespace sys */
