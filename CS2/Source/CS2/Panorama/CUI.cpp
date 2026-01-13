#include <CS2/Panorama/CUI.h>

#include <CS2/Interfaces/Include.h>
#include <GlobalData/Include.h>
using namespace Globals;


namespace CS2 {
	namespace panorama {

		void CUI::CreateMenu() {
                I::pPanoramaUIEngine->GetCUIEngineSource2()->RunScript(R"js(
(function() {
    var root = $.GetContextPanel();
    

    var browserContainer = $.CreatePanel("Panel", root, "CustomBrowser");
    browserContainer.style.width = "1200px";
    browserContainer.style.height = "800px";
    browserContainer.style.backgroundColor = "#2b2b2b";
    browserContainer.AddClass("browser-container");
    
    var addressBar = $.CreatePanel("TextEntry", browserContainer, "AddressBar");
    addressBar.style.width = "100%";
    addressBar.style.height = "40px";
    addressBar.text = "https://www.google.com";
    
    var goButton = $.CreatePanel("Button", browserContainer, "GoButton");
    var goLabel = $.CreatePanel("Label", goButton, "GoLabel");
    goLabel.text = "Go";
    
    function LoadURL(url) {
        contentFrame.SetURL(url);
    }
    
    goButton.SetPanelEvent("onactivate", function() {
        var url = addressBar.text;
        $.Msg("Navigating to: " + url);
        LoadURL(url);
    });
    
    var contentFrame = $.CreatePanel("HTML", browserContainer, "ContentFrame");
    contentFrame.style.width = "100%";
    contentFrame.style.height = "calc(100% - 40px)";
    
    browserContainer.style.visibility = "visible";
})();
)js");
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