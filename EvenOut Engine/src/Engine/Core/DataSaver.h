#pragma once
namespace Engine {
	class DataSaver
	{
	public:

		static void SaveInt(std::string keyName, int value);
		static void SaveFloat(std::string keyName, float value);
		static void SaveString(std::string keyName, std::string value);

		static LPCWSTR StringToLPCWSTR(const std::string& str);

		template <typename T>
		static T GetKey(std::string keyName) 
		{
			static_assert(std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, std::wstring>,
				"T must be int, float, or std::wstring");

			HKEY hKey;
			T result{};
			if (RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\EventOutEngine", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
				DWORD dataSize;
				DWORD dataType;

				if (RegQueryValueExW(hKey, StringToLPCWSTR(keyName), NULL, &dataType, NULL, &dataSize) == ERROR_SUCCESS) {
					if ((dataType == REG_DWORD && std::is_same_v<T, int>) ||
						(dataType == REG_BINARY && std::is_same_v<T, float>) ||
						(dataType == REG_SZ && std::is_same_v<T, std::wstring>)) {

						// Allocate buffer based on data size
						if constexpr (std::is_same_v<T, std::wstring>) {
							std::wstring buffer(dataSize / sizeof(wchar_t), L'\0');
							if (RegQueryValueExW(hKey, StringToLPCWSTR(keyName), NULL, NULL, reinterpret_cast<BYTE*>(buffer.data()), &dataSize) == ERROR_SUCCESS) {
								RegCloseKey(hKey);
								return std::move(buffer);
							}
						}
						else {
							// For int and float values
							if (RegQueryValueExW(hKey, StringToLPCWSTR(keyName), NULL, NULL, reinterpret_cast<BYTE*>(&result), &dataSize) == ERROR_SUCCESS) {
								RegCloseKey(hKey);
								return result;
							}
						}
					}
				}
				RegCloseKey(hKey);
			}
			return result;
		}

		template <typename T>
		static bool RegistryValueExists(std::string keyName) {
			HKEY hKey;
			if (RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\EventOutEngine", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
				DWORD dataSize;
				DWORD dataType;

				if (RegQueryValueExW(hKey, StringToLPCWSTR(keyName), NULL, &dataType, NULL, &dataSize) == ERROR_SUCCESS) {
					RegCloseKey(hKey);
					return true;
				}

				RegCloseKey(hKey);
			}
			return false;
		}


	private: 
		static HKEY hKey;
	};
}

