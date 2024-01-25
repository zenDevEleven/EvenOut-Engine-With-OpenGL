#include "epch.h"
#include "DataSaver.h"
#include "Engine/LogSystem/Log.h"

namespace Engine {

	HKEY DataSaver::hKey;

	LPCWSTR DataSaver::StringToLPCWSTR(const std::string& str) {

		int length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	
		wchar_t* wideStr = new wchar_t[length];


		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wideStr, length);

	
		return wideStr;
	}

	void DataSaver::SaveInt(std::string keyName, int value)
	{
		if (RegCreateKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\EventOutEngine", 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {

			RegSetValueExW(hKey, StringToLPCWSTR(keyName), 0, REG_DWORD, reinterpret_cast<BYTE*>(&value), sizeof(int));

			RegCloseKey(hKey);

			LOG_CORE("Data saved successfully", LOG_INFO);
		}
		else {
			LOG_CORE("Unable to open or create registry key.", LOG_ERROR);
		}
		
	}

	void DataSaver::SaveFloat(std::string keyName, float value)
	{
		if (RegCreateKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\EventOutEngine", 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {

			RegSetValueExW(hKey, StringToLPCWSTR(keyName), 0, REG_BINARY, reinterpret_cast<BYTE*>(&value), sizeof(float));

			RegCloseKey(hKey);

			LOG_CORE("Data saved successfully", LOG_INFO);
		}
		else {
			LOG_CORE("Unable to open or create registry key.", LOG_ERROR);
		}
	}

	void DataSaver::SaveString(std::string keyName, std::string value)
	{
		if (RegCreateKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\EventOutEngine", 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {

			RegSetValueExW(hKey, StringToLPCWSTR(keyName), 0, REG_SZ, reinterpret_cast<BYTE*>(value.data()),
				(value.size() + 1) * sizeof(wchar_t));

			RegCloseKey(hKey);

			LOG_CORE("Data saved successfully", LOG_INFO);
		}
		else {
			LOG_CORE("Unable to open or create registry key.", LOG_ERROR);
		}
	}

}

