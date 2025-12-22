#pragma once
#include <GlobalData/Include.h>
#include <string>
namespace CS2 {

	class CGameTraceManager;
	class CGameResourceService;
	class CInterfaceManager {
	private:
		template <typename T>
		using CreateInterfaceFn = T * (__thiscall*)(const char* interfaceName, int unknown);

	public:
		inline static CGameResourceService* pGameResourceService = nullptr;

		inline static CGameTraceManager* pGameTraceManager = nullptr;

		template <typename T>
		inline static T* CreateInterface(std::string module, std::string interfaceName) {
			auto m = ::Globals::proc.GetRemoteModule(module);
			auto fn = (CInterfaceManager::CreateInterfaceFn<T>)m->GetProcAddress("CreateInterface");
			return (T*)fn(interfaceName.c_str(), NULL);
		}

		inline static void Initialize() {
			auto pClient = ::Globals::proc.GetRemoteModule("client.dll");

			pGameTraceManager = ::Globals::proc.ReadDirect<CGameTraceManager*>(
				pClient->ResolveRIP(
					pClient->ScanMemory("4C 8B 2D ? ? ? ? 24")
					, 3, 7)
			);

			pGameResourceService = CreateInterface<CGameResourceService>("engine2.dll", "GameResourceServiceClientV001");

			LogAll();
		}

		inline static void LogAll() {
			printf("[+] Interfaces\n");
			printf("[+] CGameTraceManager: 0x%p\n\n", pGameTraceManager);
			printf("[+] CGameResourceService: 0x%p\n\n", pGameResourceService);
		}
	};

	using I = CInterfaceManager;

}