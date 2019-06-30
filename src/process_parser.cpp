#include "process_parser.h"
#include <string>
#include <vector>

std::string ProcessParser::getCmd(std::string pid) { return std::string(); }
std::vector<std::string> ProcessParser::getPidList() { return std::vector<std::string>{}; }
std::string ProcessParser::getVmSize(std::string pid) { return std::string(); }
std::string ProcessParser::getCpuPercent(std::string pid) { return std::string(); }
long int ProcessParser::getSysUpTime() { return 0; }
std::string ProcessParser::getProcUpTime(std::string pid) { return 0; }
std::string ProcessParser::getProcUser(std::string pid) { return 0; }
std::vector<std::string> ProcessParser::getSysCpuPercent(std::string coreNumber) {
  return std::vector<std::string>{};
}
float ProcessParser::getSysRamPercent() { return 0; }
std::string ProcessParser::getSysKernelVersion() { return std::string(""); }
int ProcessParser::getNumberOfCores() { return 0; }
int ProcessParser::getTotalThreads() { return 0; }
int ProcessParser::getTotalNumberOfProcesses() { return 0; }
int ProcessParser::getNumberOfRunningProcesses() { return 0; }
std::string ProcessParser::getOSName() { return std::string(""); }
std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1,
                          std::vector<std::string> values2) {
  return std::string("");
}
bool ProcessParser::isPidExisting(std::string pid) { return 0; }