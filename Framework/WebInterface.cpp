#include "WebInterface.h"
#include <WebServer.h>
#include <SD.h>
#include <Time.h>
#include "Aquaduino.h"
#include "TemplateParser.h"

const static char progMainTemplateFileName[] PROGMEM = "main.htm";
const static char progMainTemplateStringHour[] PROGMEM = "##HOUR##";
const static char progMainTemplateStringMinute[] PROGMEM = "##MINUTE##";
const static char progMainTemplateStringSecond[] PROGMEM = "##SECOND##";
const static char progMainTemplateStringTemperature[] PROGMEM = "##TEMPERATURE##";
const static char progMainTemplateStringCR[] PROGMEM = "##CONTROLLERROW##";

const static char progMainTemplateRowFileName[] PROGMEM = "mainrow.htm";
const static char progMainTemplateStringCL[] PROGMEM = "##CONTROLLERLINK##";
const static char progMainTemplateStringCN[] PROGMEM = "##CONTROLLERNAME##";

static const char* const mainTemplateStrings[] PROGMEM =
		{ progMainTemplateStringTemperature, progMainTemplateStringHour,
				progMainTemplateStringMinute, progMainTemplateStringSecond,
				progMainTemplateStringCR };

static const char* const mainTemplateRowStrings[] PROGMEM =
{ progMainTemplateStringCL, progMainTemplateStringCN };

const static char progConfigFileName[] PROGMEM = "CONTROL.CFG";
const static char progTemplateFileName[] PROGMEM = "conf.htm";

enum
{
	ACTORROW,
	DHCPSELECTOPTION,
	IP1,
	IP2,
	IP3,
	IP4,
	NETMASK1,
	NETMASK2,
	NETMASK3,
	NETMASK4,
	GATEWAY1,
	GATEWAY2,
	GATEWAY3,
	GATEWAY4,
	DNS1,
	DNS2,
	DNS3,
	DNS4,
	NTPSELECTOPTION,
	NTP1,
	NTP2,
	NTP3,
	NTP4,
	NTPPERIOD,
	HOUR,
	MINUTE,
	SECOND
};

enum
{
	COLOR,
	IACTUATOR,
	ACTUATORNAME,
	CSELECT,
	COPTIONS,
	ESELECT,
	EOPTIONS,
	ACTUATORSTATE
};

const static char progTemplateRow[] PROGMEM = "##ACTORROW##";
const static char progTemplateDHCPSelectOption[] PROGMEM
		= "##DHCPSELECTOPTION##";
const static char progTemplateIP1[] PROGMEM = "##IP1##";
const static char progTemplateIP2[] PROGMEM = "##IP2##";
const static char progTemplateIP3[] PROGMEM = "##IP3##";
const static char progTemplateIP4[] PROGMEM = "##IP4##";
const static char progTemplateNM1[] PROGMEM = "##NETMASK1##";
const static char progTemplateNM2[] PROGMEM = "##NETMASK2##";
const static char progTemplateNM3[] PROGMEM = "##NETMASK3##";
const static char progTemplateNM4[] PROGMEM = "##NETMASK4##";
const static char progTemplateGW1[] PROGMEM = "##GATEWAY1##";
const static char progTemplateGW2[] PROGMEM = "##GATEWAY2##";
const static char progTemplateGW3[] PROGMEM = "##GATEWAY3##";
const static char progTemplateGW4[] PROGMEM = "##GATEWAY4##";
const static char progTemplateDNS1[] PROGMEM = "##DNS1##";
const static char progTemplateDNS2[] PROGMEM = "##DNS2##";
const static char progTemplateDNS3[] PROGMEM = "##DNS3##";
const static char progTemplateDNS4[] PROGMEM = "##DNS4##";
const static char progTemplateNTPSelectOption[] PROGMEM = "##NTPSELECTOPTION##";
const static char progTemplateNTP1[] PROGMEM = "##NTP1##";
const static char progTemplateNTP2[] PROGMEM = "##NTP2##";
const static char progTemplateNTP3[] PROGMEM = "##NTP3##";
const static char progTemplateNTP4[] PROGMEM = "##NTP4##";
const static char progTemplateNTPPeriod[] PROGMEM = "##NTPPERIOD##";
const static char progTemplateTimeHour[] PROGMEM = "##HOUR##";
const static char progTemplateTimeMinute[] PROGMEM = "##MINUTE##";
const static char progTemplateTimeSecond[] PROGMEM = "##SECOND##";

const static char progTemplateRowFileName[] PROGMEM = "confrow.htm";
const static char progTemplateRowColor[] PROGMEM = "##COLOR##";
const static char progTemplateRowIActuator[] PROGMEM = "##IACTUATOR##";
const static char progTemplateRowActuatorName[] PROGMEM = "##ACTUATORNAME##";
const static char progTemplateRowCSelect[] PROGMEM = "##CSELECT##";
const static char progTemplateRowCOptions[] PROGMEM = "##COPTIONS##";
const static char progTemplateRowESelect[] PROGMEM = "##ESELECT##";
const static char progTemplateRowEOptions[] PROGMEM = "##EOPTIONS##";
const static char progTemplateRowActuatorState[] PROGMEM = "##ACTUATORSTATE##";

const char* const templateFileString[] PROGMEM =
{ progTemplateRow, progTemplateDHCPSelectOption, progTemplateIP1,
		progTemplateIP2, progTemplateIP3, progTemplateIP4, progTemplateNM1,
		progTemplateNM2, progTemplateNM3, progTemplateNM4, progTemplateGW1,
		progTemplateGW2, progTemplateGW3, progTemplateGW4, progTemplateDNS1,
		progTemplateDNS2, progTemplateDNS3, progTemplateDNS4,
		progTemplateNTPSelectOption, progTemplateNTP1, progTemplateNTP2,
		progTemplateNTP3, progTemplateNTP4, progTemplateNTPPeriod,
		progTemplateTimeHour, progTemplateTimeMinute, progTemplateTimeSecond };

const char* const templateRowFileString[] PROGMEM =
{ progTemplateRowColor, progTemplateRowIActuator, progTemplateRowActuatorName,
		progTemplateRowCSelect, progTemplateRowCOptions, progTemplateRowESelect,
		progTemplateRowEOptions, progTemplateRowActuatorState };

void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
	TemplateParser* parser;
	char templateFileString[sizeof(progMainTemplateFileName)];
	char templateRowString[sizeof(progMainTemplateRowFileName)];
	char* replacementStrings[2];
	File templateFile;
	File templateRow;
	int16_t matchIdx;
	uint8_t controller;

	char url[AQUADUINO_STRING_LENGTH];
	char name[AQUADUINO_STRING_LENGTH];

	parser = aquaduino->getTemplateParser();

	strcpy_P(templateFileString, progMainTemplateFileName);
	strcpy_P(templateRowString, progMainTemplateRowFileName);

	templateFile = SD.open(templateFileString, FILE_READ);
	templateRow = SD.open(templateRowString, FILE_READ);

	if (type != WebServer::HEAD)
	{
		server.httpSuccess();

		matchIdx = 0;

		while ((matchIdx = parser->processTemplateUntilNextMatch(&templateFile,
				mainTemplateStrings, 5, &server)) >= 0)
		{
			if (matchIdx == 0)
			{
				server.print(aquaduino->getTemperature());
			}
			else if (matchIdx == 1)
			{
				server.print(hour());
			}
			else if (matchIdx == 2)
			{
				server.print(minute());
			}
			else if (matchIdx == 3)
			{
				server.print(second());
			}
			else if (matchIdx == 4)
			{
				for (controller = 0;
						controller < aquaduino->getNrOfControllers();
						controller++)
				{
					strcpy(url, aquaduino->getController(controller)->getURL());
					strcpy(name, aquaduino->getController(controller)->getName());
					replacementStrings[0] = url;
					replacementStrings[1] = name;
					parser->processSingleTemplate(&templateRow,
							mainTemplateRowStrings, replacementStrings, 2, &server);
				}
			}
		}
	}
	templateFile.close();
	templateRow.close();
}

void printActuatorTable(WebServer* server)
{
	char templateRowFileName[sizeof(progTemplateRowFileName)];
	File templateRowFile;
	int i, j;
	int16_t matchIdx = 0;
	TemplateParser* parser;
	char actuatorID[5], controllerID[5], enabledID[5];

	Actuator* currentActor;
	Controller* currentController;

	parser = aquaduino->getTemplateParser();

	strcpy_P(templateRowFileName, progTemplateRowFileName);

	aquaduino->resetAcuatorIterator();

	while ((i = aquaduino->getNextActuator(&currentActor)) != -1)
	{
		templateRowFile = SD.open(templateRowFileName, FILE_READ);
		while ((matchIdx = parser->processTemplateUntilNextMatch(
				&templateRowFile, templateRowFileString, 8, server)) != -1)
		{

			switch (matchIdx)
			{
			case COLOR:
				if (i % 2 == 0)
				{
					server->print("#FFFFFF");
				}
				else
				{
					server->print("#99CCFF");
				}
				break;
			case IACTUATOR:
				actuatorID[0] = 'A';
				itoa(i, &actuatorID[1], 10);
				server->print(actuatorID);
				break;
			case ACTUATORNAME:
				server->print(currentActor->getName());
				break;
			case CSELECT:
				controllerID[0] = 'C';
				itoa(i, &controllerID[1], 10);
				server->print(controllerID);
				break;
			case COPTIONS:
				aquaduino->resetControllerIterator();
				while ((j = aquaduino->getNextController(&currentController)) != -1)
				{
					itoa(j, controllerID, 10);
					parser->optionListItem(currentController->getName(),
							controllerID,
							currentActor->getController() == i, server);
				}
				break;
			case ESELECT:
				enabledID[0] = 'E';
				itoa(i, &enabledID[1], 10);
				server->print(enabledID);
				break;
			case EOPTIONS:
				parser->optionListItem("Disabled", "0", 0, server);
				parser->optionListItem("Enabled", "1",
						currentActor->isEnabled(), server);
				break;
			case ACTUATORSTATE:
				if (currentActor->isOn())
				{
					server->print("On");
				}
				else
				{
					server->print("Off");
				}
				break;
			}
		}
		templateRowFile.close();
	}

}

void printTopLevelTemplate(WebServer* server)
{
	File templateFile;
	TemplateParser* parser;
	int16_t matchIdx = 0;
	IPAddress* ip, *netmask, *dns, *gw, *ntp;

	char templateFileName[sizeof(progTemplateFileName)];
	strcpy_P(templateFileName, progTemplateFileName);

	parser = aquaduino->getTemplateParser();
	templateFile = SD.open(templateFileName, FILE_READ);

	ip = aquaduino->getIP();
	netmask = aquaduino->getNetmask();
	gw = aquaduino->getGateway();
	dns = aquaduino->getDNS();
	ntp = aquaduino->getNTP();

	while ((matchIdx = parser->processTemplateUntilNextMatch(&templateFile,
			templateFileString, sizeof(templateFileString) / sizeof(char*),
			server)) >= 0)
	{
		switch (matchIdx)
		{
		case ACTORROW:
			printActuatorTable(server);
			break;
		case DHCPSELECTOPTION:
			if (aquaduino->isDHCPEnabled())
			{
				parser->optionListItem("Yes", "1", 1, server);
				parser->optionListItem("No", "0", 0, server);
			}
			else
			{
				parser->optionListItem("Yes", "1", 0, server);
				parser->optionListItem("No", "0", 1, server);
			}
			break;
		case IP1:
			server->print((*ip)[0]);
			break;
		case IP2:
			server->print((*ip)[1]);
			break;
		case IP3:
			server->print((*ip)[2]);
			break;
		case IP4:
			server->print((*ip)[3]);
			break;
		case NETMASK1:
			server->print((*netmask)[0]);
			break;
		case NETMASK2:
			server->print((*netmask)[1]);
			break;
		case NETMASK3:
			server->print((*netmask)[2]);
			break;
		case NETMASK4:
			server->print((*netmask)[3]);
			break;
		case GATEWAY1:
			server->print((*gw)[0]);
			break;
		case GATEWAY2:
			server->print((*gw)[1]);
			break;
		case GATEWAY3:
			server->print((*gw)[2]);
			break;
		case GATEWAY4:
			server->print((*gw)[3]);
			break;
		case DNS1:
			server->print((*dns)[0]);
			break;
		case DNS2:
			server->print((*dns)[1]);
			break;
		case DNS3:
			server->print((*dns)[2]);
			break;
		case DNS4:
			server->print((*dns)[3]);
			break;
		case NTPSELECTOPTION:
			if (aquaduino->isNTPEnabled())
			{
				parser->optionListItem("Yes", "1", 1, server);
				parser->optionListItem("No", "0", 0, server);
			}
			else
			{
				parser->optionListItem("Yes", "1", 0, server);
				parser->optionListItem("No", "0", 1, server);
			}
			break;
		case NTP1:
			server->print((*ntp)[0]);
			break;
		case NTP2:
			server->print((*ntp)[1]);
			break;
		case NTP3:
			server->print((*ntp)[2]);
			break;
		case NTP4:
			server->print((*ntp)[3]);
			break;
		case NTPPERIOD:
			server->print(aquaduino->getNtpSyncInterval());
			break;
		case HOUR:
			server->print(hour());
			break;
		case MINUTE:
			server->print(minute());
			break;
		case SECOND:
			server->print(second());
			break;
		}
	}
	templateFile.close();
}

int8_t configCmd(WebServer* server,
		WebServer::ConnectionType type)
{
	int8_t repeat;
	char name[30], value[30];

	if (type == WebServer::POST)
	{
		while ((repeat = server->readPOSTparam(name, 30, value, 30)) > 0)
		{
		}
		//server->httpSeeOther(this->m_URL);
	}
	else
	{
		server->httpSuccess();
		printTopLevelTemplate(server);
	}
	return true;
}

void controllerDispatchCommand(WebServer &server, WebServer::ConnectionType type,
        char **url_path, char *url_tail,
        bool tail_complete)
{
	Controller* controller;

	if (type != WebServer::HEAD)
	{
		if (strcmp("config", *url_path) == 0)
		{
			configCmd(&server, type);
			return;
		}

		aquaduino->resetControllerIterator();
		while (aquaduino->getNextController(&controller) != -1)
		{
			if (strcmp(controller->getURL(), *url_path) == 0)
			{
				controller->showWebinterface(&server, type);
				if (type == WebServer::POST)
				{
					aquaduino->writeConfig(controller);
				}
			}
		}
	}
}
