#include <CS2/Panorama/CUI.h>

#include <CS2/Interfaces/Include.h>
#include <GlobalData/Include.h>
using namespace Globals;


namespace CS2 {
	namespace panorama {

		void CUI::CreateMenu() {
		}

		void CUI::ShowWelcomeMsg() {
			I::pPanoramaUIEngine->GetCUIEngineSource2()->RunScript(R"(
				$.Msg('Pure Liquid v0.1 Loaded!');
			UiToolkitAPI.ShowGenericPopup('PureLiquid v0.1 by xSip', 'Pure Liquid Has been loaded!', '');
			)");
		}
		void CUI::ShowShutdownMsg() {
			I::pPanoramaUIEngine->GetCUIEngineSource2()->RunScript(R"(
				$.Msg('Pure Liquid v0.1 Unloaded!');
				UiToolkitAPI.ShowGenericPopup('PureLiquid v0.1 by xSip', 'Pure Liquid Has been unloaded!', '');
			)");
		}
	}
}