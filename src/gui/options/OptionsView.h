#pragma once
#include "common/String.h"
#include "gui/interface/Window.h"
#include "gui/interface/ScrollPanel.h"

namespace ui
{
	class Checkbox;
	class DropDown;
	class Textbox;
	class Button;
	class Label;
}

class OptionsModel;
class OptionsController;
class OptionsView: public ui::Window
{
	OptionsController *c{};
	ui::Checkbox *heatSimulation{};
	ui::Checkbox *ambientHeatSimulation{};
	ui::Checkbox *newtonianGravity{};
	ui::Checkbox *waterEqualisation{};
	ui::DropDown *airMode{};
	ui::Textbox *ambientAirTemp{};
	ui::Button *ambientAirTempPreview{};
	ui::Textbox *vorticityCoeff{};
	ui::DropDown *gravityMode{};
	ui::DropDown *edgeMode{};
	ui::DropDown *temperatureScale{};
	ui::DropDown *scale{};
	ui::Checkbox *resizable{};
	ui::Checkbox *fullscreen{};
	ui::Checkbox *changeResolution{};
	ui::Checkbox *forceIntegerScaling{};
	ui::Checkbox *blurryScaling{};
	ui::Checkbox *fastquit{};
	ui::Checkbox *globalQuit{};
	ui::DropDown *decoSpace{};
	ui::Checkbox *showAvatars{};
	ui::Checkbox *momentumScroll{};
	ui::Checkbox *mouseClickRequired{};
	ui::Checkbox *includePressure{};
	ui::Checkbox *perfectCircle{};
	ui::Checkbox *graveExitsConsole{};
	ui::Checkbox *nativeClipoard{};
	ui::Checkbox *threadedRendering{};
	ui::Checkbox *redirectStd{};
	ui::Checkbox *autoStartupRequest{};
	ui::Label *startupRequestStatus{};
	ui::ScrollPanel *scrollPanel{};
	float customGravityX, customGravityY;
	void UpdateAmbientAirTempPreview(float airTemp, bool isValid);
	void AmbientAirTempToTextBox(float airTemp);
	void UpdateAirTemp(String temp, bool isDefocus);
	void VorticityCoeffToTextBox(float vorticity);
	void UpdateVorticityCoeff(String cort, bool isDefocus);
	void UpdateStartupRequestStatus();
public:
	OptionsView();
	void NotifySettingsChanged(OptionsModel * sender);
	void AttachController(OptionsController * c_);
	void OnDraw() override;
	void OnTick() final override;
	void OnTryExit(ExitMethod method) override;
};
