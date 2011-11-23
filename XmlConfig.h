#ifndef _XML_CONFIG_H_
#define _XML_CONFIG_H_

#include <string>

void LoadXmlConfigPart1(size_t& pDelay);
bool LoadXmlConfigPart2();
void XmlDeinit();
void XmlSaveSettings();
void XmlLoadSettings();
void XmlSave();
bool ReplaceEnvironmentVars(std::string& str);

#endif // _XML_CONFIG_H_

