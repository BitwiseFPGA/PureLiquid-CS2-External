#pragma once
#include <GlobalData/Include.h>
#include <CS2/Patterns.h>
#include <string>


namespace CS2 {

	class CGameTraceManager;
	class CGameResourceService;
	class CMaterialSystem2;
	class CCSGOInput;
	class CPanoramaUIEngine;
	class CSchemaSystem;
	class CLegacyGameUI;
	class CSource2Client;
	class CInterfaceManager {
	private:
		template <typename T>
		using CreateInterfaceFn = T * (__thiscall*)(const char* interfaceName, int unknown);

	public:
		inline static CGameResourceService* pGameResourceService = nullptr;
		inline static CPanoramaUIEngine* pPanoramaUIEngine = nullptr;

		inline static CGameTraceManager* pGameTraceManager = nullptr;
		inline static CCSGOInput* pCsGoInput = nullptr;

		inline static CMaterialSystem2* pMaterialSystem = nullptr;
		inline static CSchemaSystem* pSchemaSystem = nullptr;
		inline static CLegacyGameUI* pLegacyGameUI = nullptr;
		inline static CSource2Client* pSource2Client = nullptr;

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
					pClient->ScanMemory(C_GAME_TRACE_MANAGER_PATTERN)
					, 0x3, 0x7)
			);

			pCsGoInput = reinterpret_cast<CCSGOInput*>(
				::Globals::proc.ReadDirect<uintptr_t>(
					pClient->ResolveRIP(
						pClient->ScanMemory(C_CSGO_INPUT_PATTERN), 0x3, 0x7)
				)
				);

			pGameResourceService = CreateInterface<CGameResourceService>("engine2.dll", "GameResourceServiceClientV001");
			pMaterialSystem = CreateInterface<CMaterialSystem2>("materialsystem2.dll", "VMaterialSystem2_001");
			pPanoramaUIEngine = CreateInterface<CPanoramaUIEngine>("panorama.dll", "PanoramaUIEngine001");
			pSchemaSystem = CreateInterface<CSchemaSystem>("schemasystem.dll", "SchemaSystem_001");
			pLegacyGameUI = CreateInterface<CLegacyGameUI>("client.dll", "LegacyGameUI001");
			pSource2Client = CreateInterface<CSource2Client>("client.dll", "Source2Client002");


			LogAll();
		}

		inline static void LogAll() {
			printf("[+] Interfaces\n");
			printf("[+] CGameResourceService: 0x%p\n", pGameResourceService);
			printf("[+] CPanoramaUIEngine: 0x%p\n", pPanoramaUIEngine);
			printf("[+] CGameTraceManager: 0x%p\n", pGameTraceManager);
			printf("[+] CMaterialSystem2: 0x%p\n", pMaterialSystem);
			printf("[+] CSource2Client: 0x%p\n", pSource2Client);
			printf("[+] CLegacyGameUI: 0x%p\n", pLegacyGameUI);
			printf("[+] CCSGOInput: 0x%p\n", pCsGoInput);
			printf("[+] CSchemaSystem: 0x%p\n\n", pSchemaSystem);
		}
	};

	using I = CInterfaceManager;

}