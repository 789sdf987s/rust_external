#include "../include.hpp"
#include "Menu.h"
#include "MenuControls.h"
#include <iostream>
#include "../globals/globals.hpp"
#include <sstream>
#include "../drawing/imgui/imgui.h"
#include "../drawing/imgui/imgui_impl_dx11.h"
#include "../drawing/imgui/imgui_impl_win32.h"
#include "../drawing/imgui/imgui_internal.h"
#include <inttypes.h>
#include "../drawing/drawing.hpp"

extern ImFont* menuFont{};
extern ImFont* keybindsFont{};
extern ImFont* tabFont{};
extern ImFont* tabFont2;
extern ImFont* tabFont3;
extern ImFont* controlFont{};
extern ID3D11Texture1D* menuBg{};

static int tab = 0;
extern bool unload;

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}

void Menu::ColorPicker(const char* name, float* color, bool alpha) {

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiStyle* style = &ImGui::GetStyle();

	auto alphaSliderFlag = alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha;

	ImGui::SameLine(219.f);
	ImGui::ColorEdit4(std::string{ "##" }.append(name).append("Picker").c_str(), color, alphaSliderFlag | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
}

void Menu::Watermark()
{
	ImGui::SetNextWindowSize(ImVec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("main", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_None | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing);
	{
		if (settings->visuals.crosshair.enabled)
		{
			drawing->crosshair(ImVec4(1.f, 1.f, 1.f, 1.f), 1.f, settings->visuals.crosshair.size, settings->visuals.crosshair.x_offset, settings->visuals.crosshair.y_offset);
		}

		if (settings->visuals.watermark.enabled) {
			drawing->text({ 10, 3 }, "<3 skit#2567", drawing->ToImVec(utils->color_cycle()), true);
		}
	} ImGui::End();
}

void Menu::Render() {

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(6, 6);

	ImGui::PushFont(menuFont);

	ImGui::SetNextWindowSize(ImVec2(660.f, 330.f));
	ImGui::Begin("method.gatekeepers client", &isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar); {
		ImGui::SameLine(6.f);

		style->Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0);

		ImGui::BeginChild("Menu Contents", ImVec2(648.f, 548.f), false); {

			style->ItemSpacing = ImVec2(0.f, -1.f);

			ImGui::BeginTabs("Tabs", ImVec2(75.f, 320.f), false); {

				style->ItemSpacing = ImVec2(0.f, 0.f);

				style->ButtonTextAlign = ImVec2(0.5f, 0.47f);

				ImGui::PopFont();
				ImGui::PushFont(tabFont);

				switch (tab) {

				case 0:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::SelectedTab("Visuals", ImVec2(75.f, 75.f))) tab = 0;
					if (ImGui::Tab("Aimbot", ImVec2(75.f, 75.f))) tab = 1;
					if (ImGui::Tab("Misc", ImVec2(75.f, 75.f))) tab = 2;
					if (ImGui::Tab("Players", ImVec2(75.f, 75.f))) tab = 3;

					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				case 1:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::Tab("Visuals", ImVec2(75.f, 75.f))) tab = 0;
					if (ImGui::SelectedTab("Aimbot", ImVec2(75.f, 75.f))) tab = 1;
					if (ImGui::Tab("Misc", ImVec2(75.f, 75.f))) tab = 2;
					if (ImGui::Tab("Players", ImVec2(75.f, 75.f))) tab = 3;

					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				case 2:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::Tab("Visuals", ImVec2(75.f, 75.f))) tab = 0;
					if (ImGui::Tab("Aimbot", ImVec2(75.f, 75.f))) tab = 1;
					if (ImGui::SelectedTab("Misc", ImVec2(75.f, 75.f))) tab = 2;
					if (ImGui::Tab("Players", ImVec2(75.f, 75.f))) tab = 3;
					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				case 3:
					ImGui::TabSpacer("##Top Spacer", ImVec2(75.f, 10.f));

					if (ImGui::Tab("Visuals", ImVec2(75.f, 75.f))) tab = 0;
					if (ImGui::Tab("Aimbot", ImVec2(75.f, 75.f))) tab = 1;
					if (ImGui::Tab("Misc", ImVec2(75.f, 75.f))) tab = 2;
					if (ImGui::SelectedTab("Players", ImVec2(75.f, 75.f))) tab = 3;

					ImGui::TabSpacer2("##Bottom Spacer", ImVec2(75.f, 7.f));
					break;
				}

				ImGui::PopFont();
				ImGui::PushFont(menuFont);

				style->ButtonTextAlign = ImVec2(0.5f, 0.5f);

			} ImGui::EndTabs();

			ImGui::SameLine(75.f);

			ImGui::BeginChild("Tab Contents", ImVec2(572.f, 322.f), false); {

				style->Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);

				switch (tab) {

				case 0:
					Visuals();
					break;
				case 1:
					Aimbot();
					break;
				case 2:
					Misc();
					break;
				case 3:
					Players();
					break;
				}

				style->Colors[ImGuiCol_Border] = ImColor(10, 10, 10, 255);

			} ImGui::EndChild();

			style->ItemSpacing = ImVec2(4.f, 4.f);
			style->Colors[ImGuiCol_ChildBg] = ImColor(17, 17, 17, 255);

		} ImGui::EndChild();

		ImGui::PopFont();

	} ImGui::End();
}

void Menu::Shutdown() {

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
}

void Menu::Aimbot() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {

		InsertGroupBoxLeft("Aimbot", 286.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			InsertCheckbox("Aimbot Enabled", settings->aimbot.enabled);
			ImGui::Keybind("AimbotKeybind", &settings->aimbot.hotkey, NULL);

			ImGui::Dummy({ 0.f, 10.f });

			InsertSlider("Aimbot FOV", settings->aimbot.fov, 15, GetSystemMetrics(SM_CYSCREEN) * 1.03, "%1.0f");

			InsertCheckbox("Aimbot Smoothing", settings->aimbot.smooth);
			if (settings->aimbot.smooth)
			{
				InsertIntSlider("Smoothing Amount", settings->aimbot.smoothing, 1, 5, "%d");
			}

			InsertCheckbox("Aimbot Prediction", settings->aimbot.prediction);

			InsertCheckbox("Attack Teammates", settings->aimbot.attack_team);

			InsertCheckbox("Compensate Recoil", settings->aimbot.compensate_recoil);

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Aimbot Cover", "Aimbot");
	}
	ImGui::NextColumn(); {

		InsertGroupBoxRight("Antiaim", 185.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			/* none for you :) */

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);
		} InsertEndGroupBoxRight("Antiaim Cover", "Antiaim");

		ImGui::Dummy({ 0.f, 10.f });

		InsertGroupBoxRight("Silent Aim", 87.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			/* none for you :) */

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);
		} InsertEndGroupBoxRight("Silent Aim Cover", "Silent Aim");
	}
}

void Menu::Visuals() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {

		InsertGroupBoxLeft("Player ESP", 286.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			InsertCheckbox("Name ESP", settings->visuals.players.name.enabled);
			InsertColorPicker("##NameESP color", settings->visuals.players.name.color, true);

			InsertCheckbox("Box ESP", settings->visuals.players.box.enabled);
			if (settings->visuals.players.box.enabled) {
				ColorPicker("Box ESP Outline Color", settings->visuals.players.box.outline_color, false);
			}

			InsertCheckbox("Skeleton ESP", settings->visuals.players.skeleton.enabled);
			InsertColorPicker("##SkeletonESP color", settings->visuals.players.skeleton.color, true);
			InsertIntSlider("Skeleton ESP Thickness", settings->visuals.players.skeleton.thickness, 1, 3, "%d");

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Player ESP Cover", "Player ESP");
	}
	ImGui::NextColumn(); {

		InsertGroupBoxLeft("Enviroment", 185.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			/* none for you :) */

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Enviroment Cover", "Enviroment");

		InsertSpacer("Enviroment - Visual Options Spacer");

		InsertGroupBoxRight("Visual Options", 87.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			InsertIntSlider("Font Size", settings->visuals.font_size, 6, 14, "%d");
			InsertCheckbox("Font Outline", settings->visuals.font_outline);
			InsertCheckbox("Font Fading", settings->visuals.font_fading);

			InsertCheckbox("Show watermark", settings->visuals.watermark.enabled);
			InsertColorPicker("##Watermark color", settings->visuals.watermark.color, true);

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Visual Options Cover", "Visual Options");
	}
}

void Menu::Misc() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	ImGui::Columns(2, NULL, false); {
		InsertGroupBoxLeft("Movement", 286.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			/* none for you :) */

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxLeft("Miscellaneous Cover", "Miscellaneous");
	}
	ImGui::NextColumn(); {

		InsertGroupBoxRight("Weapons", 192.f); {

			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			/* none for you :) */

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);

		} InsertEndGroupBoxRight("Settings Cover", "Weapons");

		InsertSpacer("Weapons - Other Spacer");

		InsertGroupBoxRight("Other", 80.f); {
			style->ItemSpacing = ImVec2(4, 2);
			style->WindowPadding = ImVec2(4, 4);
			ImGui::CustomSpacing(9.f);

			InsertCheckbox("Admin Flags", settings->features.fake_admin);

			InsertCheckbox("Crosshair Enabled", settings->visuals.crosshair.enabled);
			InsertIntSlider("Crosshair Size", settings->visuals.crosshair.size, 6, 22, "%d");
			InsertIntSlider("Crosshair X Offset", settings->visuals.crosshair.x_offset, -5, 5, "%d");
			InsertIntSlider("Crosshair Y Offset", settings->visuals.crosshair.y_offset, -5, 5, "%d");

			style->ItemSpacing = ImVec2(0, 0);
			style->WindowPadding = ImVec2(6, 6);
		} InsertEndGroupBoxRight("Other Cover", "Other");
	}
}

void Menu::Players() {

	ImGuiStyle* style = &ImGui::GetStyle();
	InsertSpacer("Top Spacer");

	bool selected;

	ImGui::Columns(2, NULL, false); {
		InsertGroupBoxLeft("Players", 286.f); {
			/* copy mutex into local vector */
			globals->players.mutex.lock();
			std::vector<Player> local_players = std::vector<Player>(globals->players.vec.begin(), globals->players.vec.end());
			globals->players.mutex.unlock();

			/* iterate over vector */
			for (auto& entry : local_players)
			{
				/* change color based on team status */
				if ((std::find(globals->team_table.begin(), globals->team_table.end(), entry.ent) != globals->team_table.end()) || (entry.same_team))
					style->Colors[ImGuiCol_MenuTheme] = ImColor(147, 197, 57);
				else
					style->Colors[ImGuiCol_MenuTheme] = ImColor(197, 147, 57);

				ImGui::NewLine();
				
				if (ImGui::Selectable((" " + utils->wstring_to_string(entry.name)).c_str(), &selected, ImGuiSelectableFlags_DontClosePopups))
				{
					/* on click, set selected_menu_player to this entry */
					globals->selected_menu_player = &entry;
					selected = false;
				}
			}
		} InsertEndGroupBoxLeft("Players Cover", "Players");
	}
	ImGui::NextColumn(); {
		InsertGroupBoxRight("Actions", 80.f); {
			style->Colors[ImGuiCol_ButtonActive] = ImColor(0, 0, 0, 0);
			style->Colors[ImGuiCol_Button] = ImColor(0, 0, 0, 255);
			style->Colors[ImGuiCol_ButtonHovered] = ImColor(24, 24, 24, 255);

			ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(10.f);

			if (globals->selected_menu_player != nullptr)
			{
				bool same_team = (std::find(globals->team_table.begin(), globals->team_table.end(), globals->selected_menu_player->ent) != globals->team_table.end()) || (globals->selected_menu_player->same_team);
				if (!same_team) {
					if (ImGui::Button("Add To Friend List"))
					{
						/* insert into team_table */
						globals->team_table.push_back(globals->selected_menu_player->ent);
					}
				}
				else
				{
					if (ImGui::Button("Remove From Friend List"))
					{
						/* erase from team_table */
						globals->team_table.erase(std::remove(globals->team_table.begin(), globals->team_table.end(), globals->selected_menu_player->ent), globals->team_table.end());
					}
				}
			}

		} InsertEndGroupBoxRight("Actions Cover", "Actions");

		InsertSpacer("Actions - Player Info Spacer")

		InsertGroupBoxRight("Player Information", 192.f); {
			if (globals->selected_menu_player != nullptr)
			{
				if (globals->selected_menu_player->health > 0)
				{
					InsertStringProperty("Player Name: ", utils->wstring_to_string(globals->selected_menu_player->name).c_str());
					InsertStringProperty("Player Health: ", (std::to_string((int)globals->selected_menu_player->health) + "hp").c_str());
					InsertStringProperty("Player Distance: ", (globals->local_player.DistanceStr(globals->selected_menu_player) + "m").c_str());
					InsertVectorProperty("Player Position: \n", globals->selected_menu_player->position.x, globals->selected_menu_player->position.y, globals->selected_menu_player->position.z);
					
					InsertVectorProperty("Player Head Position: \n", globals->selected_menu_player->bones[head].x, globals->selected_menu_player->bones[head].y, globals->selected_menu_player->bones[head].z);

					/* ayo why the fuck do females have cock bones in this game */
					InsertStringProperty("Player Cock Size: ", (globals->selected_menu_player->CockStr() + "mm").c_str());
				}
			}
		} InsertEndGroupBoxRight("Player Information Cover", "Player Information");
	}
}

Menu* menu = new Menu();