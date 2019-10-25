/*
 * ICSEControl.cpp
 *
 *  Created on: Oct 17, 2019
 *      Author: ruslantrofymenko
 */

#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include <bitset>
#include <unistd.h>

#include <ICSEControl.h>
#include <App.h>
#include <UsbSerial.h>
#include <ustring.h>

using namespace std;

namespace sys {

const string CICSEControl::ConfDir = "icse/";

CICSEControl::CICSEControl() {
	// TODO Auto-generated constructor stub

}

CICSEControl::~CICSEControl() {
	close(fd);
}

bool CICSEControl::IsConfigExists(const string& ttyDev)
{
	return CApp::FileExists(ConfPath + ttyDev);
}

bool CICSEControl::IsConfigExists()
{
	return IsConfigExists(ttyDev);
}

bool CICSEControl::ReadConfig(SConfig& Config)
{
	bool bResult = false;
	ifstream fs;
	string Line;

	fs.open(ConfPath + ttyDev);
	if (!fs.is_open()) {
		cerr << "error open config file" << endl;
		return bResult;
	}

	do {
		getline(fs, Line);
		if (!IsIntValue(Line))
			break;

		Config.DevNum = stoi(Line, nullptr, 10);

		getline(fs, Line);
		if (Line.find_first_not_of("01") != std::string::npos)
			break;

		Config.Status = stoi(Line, nullptr, 2);

		bResult = true;
	} while (false);

	fs.close();

	return bResult;
}

bool CICSEControl::WriteConfig(const SConfig& Config)
{
	ofstream fs;
	string Line;

	fs.open(ConfPath + ttyDev, ofstream::trunc);
	if (!fs.is_open()) {
		cerr << "error open config file" << endl;
		return false;
	}

	fs << Config.DevNum << endl;
	fs << bitset<8>(Config.Status) << endl;

	fs.close();

	return true;
}

bool CICSEControl::WriteConfig()
{
	SConfig Config = {DevNum, Status};
	return WriteConfig(Config);
}

bool CICSEControl::CheckConfig()
{
	SConfig Config;

	if (!ReadConfig(Config))
		return false;

	if (Config.DevNum != DevNum)
		return false;

	return true;
}

bool CICSEControl::SendCmd(unsigned char *data, int bytes)
{
	int wbytes = write(fd, data, bytes);
	if (wbytes != bytes) {
		cerr << "ICSE: error write data" << endl;
		return false;
	}

	return true;
}

bool CICSEControl::SendCmd(unsigned char Cmd)
{
	return SendCmd(&Cmd, 1);
}

bool CICSEControl::GetResponse(unsigned char *data, int bytes)
{
	int rbytes = read(fd, data, bytes);
	if (rbytes != bytes) {
		cerr << "ICSE: error read data" << endl;
		return false;
	}

	return true;
}

unsigned char CICSEControl::GetResponse()
{
	unsigned char Response;

	GetResponse(&Response, 1);
	return Response;
}

bool CICSEControl::Init()
{
	ConfPath = CApp::GetHomeDir() + ConfDir;
	if (!CApp::PathExists(ConfPath))
		mkdir(ConfPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);;

	vector<CUsbSerial::SDeviceEntry> ttyList = CUsbSerial::GetDevicesList("pl2303");
	if (ttyList.empty()) {
		cerr << "device not detected" << endl;
		return false;
	}

	if (ttyList.size() > 1) {
		cerr << "clarify ICSE serial device:" << endl;
		for (auto& tty: ttyList)
			cerr << tty.Device << endl;
		return false;
	}

	ttyDev = ttyList[0].Device;
	DevNum = CUsbSerial::GetDevNum(ttyDev);
	cout << "detect ICSE serial device " << ttyDev << endl;

	fd = CUsbSerial::InitPort(ttyDev, B9600, true);
	if (fd <= 0) {
		cout << ttyDev << " not opened" << endl;
		return false;
	}

	SConfig Config;
	if (CheckConfig()) {
		cout << "ISCE control (" + ttyDev + ") already initialized" << endl;
		ReadConfig(Config);
		DevNum = Config.DevNum;
		Status = Config.Status;
		return true;
	}

	tcflush(fd, TCIOFLUSH);

	bool bInit = true;
	for (int ii = 0; ii < 3; ii++) {
		if (!SendCmd(0x50))
			return false;

		unsigned char Response = GetResponse();
		switch(Response) {
		case 0xAB:
			cout << "detect ICSE012A" << endl;
			break;
		case 0xAD:
			cout << "detect ICSE013A" << endl;
			break;
		case 0xAC:
			cout << "detect ICSE014A" << endl;
			break;
		default:
			bInit = false;
		}

		if (bInit)
			break;
	}

	if (!bInit) {
		cerr << "undefined ICSE device" << endl;
		return false;
	}

    if (!SendCmd(0x51))
		return false;

    Status = 0;
    Config.DevNum = DevNum;
    Config.Status = Status;
    WriteConfig(Config);

    usleep(1000);

	return true;
}

bool CICSEControl::SetChanel(int Chanel, bool State)
{
	if (State) {
		Status |= 1 << Chanel;
	} else {
		Status &= ~(1 << Chanel);
	}

	if (!SendCmd(~Status))
		return false;

	if (!WriteConfig())
		return false;

	return true;
}

bool CICSEControl::Set(unsigned char State)
{
	Status = State;

	if (!SendCmd(~Status))
		return false;

	if (!WriteConfig())
		return false;

	return true;
}

} /* namespace sys */

