/*
 * ICSEControl.h
 *
 *  Created on: Oct 17, 2019
 *      Author: ruslantrofymenko
 */

#ifndef CONTROL_ICSECONTROL_H_
#define CONTROL_ICSECONTROL_H_

#include <string>

namespace sys {

class CICSEControl {
public:
	typedef struct {
		int DevNum;
		unsigned char Status;
	}SConfig;

public:
	CICSEControl(/*std::string ttyDev*/);
	virtual ~CICSEControl();

private:
	std::string ttyDev;
	int DevNum;
	unsigned char Status;
	int fd;

public:
	static const std::string ConfDir;
	std::string ConfPath;

public:
	bool Init();
	bool SetChanel(int Chanel, bool State);
	bool Set(unsigned char State);

private:
	bool IsConfigExists(const std::string& ttyDev);
	bool IsConfigExists();
	bool ReadConfig(SConfig& Config);
	bool WriteConfig(const SConfig& Config);
	bool WriteConfig();
	bool CheckConfig();
	bool SendCmd(unsigned char *data, int bytes);
	bool SendCmd(unsigned char Cmd);
	bool GetResponse(unsigned char *data, int bytes);
	unsigned char GetResponse();
};

} /* namespace sys */

#endif /* CONTROL_ICSECONTROL_H_ */
