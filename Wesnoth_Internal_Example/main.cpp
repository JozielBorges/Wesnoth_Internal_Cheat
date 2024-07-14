#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <TlHelp32.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "offsets.h"
#include <vector>

static HMODULE hModule;

Offsets_Wesnoth offsets_wesnoth;
Unit_Wesnoth current_unity;
Player_Wesnoth players;
int debung_console = -2;

void console_start() {

	if (debung_console == -1) return;
		system("cls");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::cout << "-----------------WESNOTH MENU-----------------"											 << std::endl;
	std::cout << "Press M" << " to get 999 gold"															 << std::endl;
	std::cout << "Press T" << " over a unit to display its information"										 << std::endl;
	std::cout << "Press Y" << " over a unit to change its health to 999"									 << std::endl;
	std::cout << "Press H" << " over a unit to reset its moves and attacks"									 << std::endl;
	std::cout << "----------------------------------------------"											 << std::endl;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (players.getGoldPlayer1())
		std::cout << "Player 1 : GOLD " << players.getGoldPlayer1()											 << std::endl;
	if (players.getGoldPlayer2())
		std::cout << "Player 2 : GOLD "<< players.getGoldPlayer2()											 << std::endl;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::cout << "----------------------------------------------"											 << std::endl;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch (debung_console)
	{
	case 0:
		std::cout << "Player 1 now have 999 GOLD!"															 << std::endl;
		std::cout << "----------------------------------------------"										 << std::endl;
		break;
	case 1:
		std::cout << "Unit   : " << current_unity.getName()													 << std::endl;
		std::cout << "Health : " << current_unity.getHealthCurrent()<< "/" << current_unity.getHealthMax()   << std::endl;
		std::cout << "XP     : " << current_unity.getXPCurrent()	<< "/" << current_unity.getXPMax()		 << std::endl;
		std::cout << "----------------------------------------------"										 << std::endl;
		break;
	case 2:
		std::cout << "Unit   : "<< current_unity.getName()			<< " has 999 Health!"					 << std::endl;
		std::cout << "----------------------------------------------"										 << std::endl;
		break;
	case 3:
		std::cout << "Unit   : "<< current_unity.getName()			<< " moves and attacks reset!"			 << std::endl;
		std::cout << "Move   : "<< current_unity.getMoveCurrent()	<< "/" << current_unity.getMoveMax()	 << std::endl;
		std::cout << "Attack : "<< current_unity.getAttackCurrent() << "/" << current_unity.getAttackTotal() << std::endl;
		std::cout << "----------------------------------------------"										 << std::endl;
		break;
	default:
		break;
	}
	debung_console = -1;
}

void setup_offsets() {

	// UNITS
	DWORD_PTR base = (DWORD_PTR)hModule + 0x1726BD0;
	DWORD_PTR* unit_base  = (DWORD_PTR*)base;

	// PLAYERS
	base = (DWORD_PTR)hModule + 0x1726C40;
	DWORD_PTR* player_base = (DWORD_PTR*)base;

	// SETUP ALL BASE OFFSETS
	offsets_wesnoth.updateBases(unit_base, player_base);

	current_unity = Unit_Wesnoth(offsets_wesnoth.unit_base);
	current_unity.offsets.addOffset(0x38);
	current_unity.offsets.addOffset(0x58);

	players = Player_Wesnoth(player_base);
	players.offsets.addOffset(0x8);

}
void injected_thread() {

	hModule = GetModuleHandle(L"wesnoth.exe");

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);


	if (hModule == NULL) {
		return;
	}

	setup_offsets();

	while (true) {
		console_start();

		if (GetAsyncKeyState('M')) {
			players.setGoldPlayer1(999);
			debung_console = 0;
		}

		if (GetAsyncKeyState('T')) {
			debung_console = 1;
		}
	

		if (GetAsyncKeyState('Y')) {
			current_unity.setHealth(999);
			debung_console = 2;
		}

		if (GetAsyncKeyState('H')) {
			current_unity.setMove();
			current_unity.setAtack();
			debung_console = 3;
		}

		Sleep(100);
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {

	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)injected_thread, NULL, 0, NULL);
	}

	return true;
}
