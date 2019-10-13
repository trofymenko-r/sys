/*
 * App.cpp
 *
 *  Created on: 19 вер. 2016
 *      Author: root
 */

#include <iostream>
#include <iosfwd>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <string.h>

#include "App.h"
#include "ustring.h"

using namespace std;

namespace sys {

map<string, ArchType> ArchTypeStrMap = {
                                          {"n/a",     ArchType::NONE},
                                          {"x86_64",  ArchType::X86_64},
                                          {"armv7l",  ArchType::ARMV7L}
                                      };

int CApp::iLaunch = 0;

CApp* CApp::pApp = nullptr;
SHwInfo CApp::Platform;

CApp::CApp():iValue(0)
{
   try{
      if(iLaunch>0) throw;
      iLaunch++;

      pApp = this;
      PollHwInfo();
   }
   catch(...){
      cout << "only one instance can be started" << endl;
   }

}

CApp::~CApp()
{
   // TODO Auto-generated destructor stub
}

/******************************************************************************/
/*  RevisionAppHandle                                                         */
/******************************************************************************/
string CApp::getConfFilename() const {return sConfFile;}

/******************************************************************************/
/*  RevisionAppHandle                                                         */
/******************************************************************************/
void CApp::RevisionAppHandle()
{
   if(pApp==nullptr){
      cerr << "pApp error" << endl;
      //pApp = this;
   }
}

/******************************************************************************/
/*  Exec                                                                      */
/******************************************************************************/
string CApp::Exec(const char* cmd)
{
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while(!feof(pipe.get())){
       if(fgets(buffer, 128, pipe.get())!=NULL)
           result += buffer;
    }
    return result;
}

string CApp::Exec(const string& sCmd)
{
   return Exec(sCmd.c_str());
}

string CApp::Exec(const string& sCmd, bool bOutResult)
{
   string sResult = Exec(sCmd.c_str());
   if(bOutResult){
      cout << sResult << endl;
   }

   return sResult;
}

/******************************************************************************/
/*  ExecBash                                                                  */
/******************************************************************************/
string CApp::ExecBash(const string& sCmd)
{
   Exec("bash");
   string sResult = Exec(sCmd);
   Exec("exit");
   return sResult;
}

/******************************************************************************/
/*  FileExists                                                                */
/******************************************************************************/
bool CApp::FileExists(const string &FileName)
{
   return(access(FileName.c_str(), 0)==0);
}

/******************************************************************************/
/*  DynKernelModuleExists                                                     */
/******************************************************************************/
bool CApp::DynKernelModuleExists(const string &ModuleName)
{
   string sList = CApp::Exec("/bin/lsmod");
   if(sList.find(ModuleName)==string::npos) return false;

   return true;
}

/******************************************************************************/
/*  PollHwInfo                                                                */
/******************************************************************************/
void CApp::PollHwInfo()
{
   PollHwInfo(Platform, true);
}

/******************************************************************************/
/*  PollHwInfo                                                                */
/******************************************************************************/
void CApp::PollHwInfo(SHwInfo& Platform, bool bPrint)
{
   if(!bPrint) cout.clear( ios_base::badbit);

   string str = Exec("/bin/uname -m");
   str.erase(remove(str.begin(), str.end(), '\n'), str.end());
   auto it = ArchTypeStrMap.find(str);
   if(it!=ArchTypeStrMap.end()){
      Platform.sArch = it->first;
      Platform.Arch = it->second;
   }else{

   }

   cout << "architecture: " << Platform.sArch /*<< " (" << (int)Platform.Arch << ")"*/ << endl;

   Platform.iCores = sysconf(_SC_NPROCESSORS_CONF);
   cout << "core count: " << Platform.iCores << endl;

   str = Exec("/bin/cat /proc/cpuinfo");
   Platform.sHardware = "n/a";
   vector<string> strs = SplitByWaitSpace(str);
   unsigned int iStrCount = 0;
   for(auto& s: strs){
      if(s=="Hardware" && iStrCount < (strs.size()-2)){
         Platform.sHardware = strs[iStrCount+2];
      }
      iStrCount++;
   }

   cout << "platform: " << Platform.sHardware << endl;

   if(!bPrint) cout.clear( ios_base::goodbit);
}

/******************************************************************************/
/*  getHw                                                                     */
/******************************************************************************/
const SHwInfo& CApp::getHw() {return Platform;}

/******************************************************************************/
/*  getApp                                                                    */
/******************************************************************************/
const CApp* CApp::getApp() {return pApp;}

/******************************************************************************/
/*  GetCurrentDir                                                             */
/******************************************************************************/
string CApp::GetCurrentDir()
{
   char cCurrentPath[FILENAME_MAX];
   try{
      if(!getcwd(cCurrentPath, sizeof(cCurrentPath))) throw;
   }
   catch(exception& e){
      cout << e.what();
   }

   cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

   return(string(cCurrentPath));
}

/******************************************************************************/
/*  AssignThreadToCore                                                        */
/******************************************************************************/
void CApp::AssignThreadToCore(int core_id)
{
   int ret;
   pthread_t current_thread = pthread_self();
   do{
      int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
      if (core_id < 0 || core_id >= num_cores){
         ret = EINVAL;
         break;
      }

      cpu_set_t cpuset;
      CPU_ZERO(&cpuset);
      CPU_SET(core_id, &cpuset);

      ret = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
   }while(false);

   if(ret!=0){
      cerr << "error(" << ret << ") affinity thread " << current_thread << " to core " << core_id << endl;
   }
}

/******************************************************************************/
/*  Rdtsc                                                                     */
/******************************************************************************/
U64 CApp::Rdtsc(void)
{
#if defined(__ARMEL__)//__ARMV7)//__GNUC__) && defined(__ARM_ARCH_7A__)
   return 0;
#else
   unsigned int hi, lo;
   __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
   return ((U64)hi << 32) | lo;
#endif
}

/******************************************************************************/
/*  FindProcess                                                               */
/******************************************************************************/
int CApp::FindProcess(const string sName)
{
   char buf[128] = "/bin/pidof ";
   char* p = &buf[0];
   strcat(buf, sName.c_str());

   FILE* f = popen(buf, "r");
   if(!f){
      perror("popen():");
      return -1;
   }
   while(!feof(f)){
      *p++ = fgetc(f);
   }
   pclose(f);
   return (p > &buf[0]) ? atoi(buf) : -1;
}

} /* namespace sys */
