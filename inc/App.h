/*
 * App.h
 *
 *  Created on: 19 вер. 2016
 *      Author: root
 */

#pragma once

#include <string>
#include <map>

#include "types.h"

using namespace std;

namespace sys {

enum class ArchType{NONE, X86_64, ARMV7L};
extern map<string, ArchType> ArchTypeStrMap;

typedef struct{
   string      sArch;
   ArchType    Arch;
   int         iCores;
   string      sHardware;
}SHwInfo;



class CApp
{
public:
   CApp();
   virtual ~CApp();

private:
   int iValue;
   static int iLaunch;

protected:
   string sConfFile;

protected:
   virtual void Main()=0;

private:
   void PollHwInfo();

private:
   static CApp* pApp;
   static SHwInfo Platform;

public:

   string getConfFilename() const;// {return sConfFile;}
   void RevisionAppHandle();

   static void PollHwInfo(SHwInfo& Platform, bool bPrint);
   static const SHwInfo& getHw();//{return Platform;}

   static const CApp* getApp();//{return pApp;}
   static string Exec(const char* cmd);
   static string Exec(const string& sCmd);
   static string Exec(const string& sCmd, bool bOutResult);
   static string ExecBash(const string& sCmd);
   static bool FileExists(const string &FileName);
   static bool DynKernelModuleExists(const string &ModuleName);
   static string GetCurrentDir();
   static void AssignThreadToCore(int core_id);
   static U64 Rdtsc(void);
   static int FindProcess(const string sName);
};

} /* namespace sys */
