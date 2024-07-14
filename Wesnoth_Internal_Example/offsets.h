#pragma once
#include <Windows.h>
#include <vector>

// Base offset structure
struct BaseOffsets {
    DWORD_PTR* baseAddress;
    std::vector<int> offsets;

    BaseOffsets(DWORD_PTR* base = nullptr) : baseAddress(base) {}

    void addOffset(int offset) {
        offsets.push_back(offset);
    }

    DWORD_PTR* getOffset() const {
        if (!baseAddress) return nullptr;

        DWORD_PTR* currentAddress = baseAddress;
        for (size_t i = 0; i < offsets.size(); ++i) {
            if (currentAddress == nullptr) return nullptr;
            currentAddress = (DWORD_PTR*)(*currentAddress + offsets[i]);
        }

        return currentAddress ? (DWORD_PTR*)currentAddress : nullptr;
    }

    DWORD_PTR* getOffset(int additionalOffset) const {
        DWORD_PTR* result = getOffset();
        // 0x8 INVALID ADDRESS !
		if (result == (DWORD_PTR*)0x8) return nullptr;
        return result ? (DWORD_PTR*)(*result + additionalOffset) : nullptr;
    }
};


// Unit-specific offsets
struct UnitOffsets : BaseOffsets {
    static constexpr DWORD NAME_OFFSET           = 0xA8;
    static constexpr DWORD HEALTH_CURRENT_OFFSET = 0x110;
    static constexpr DWORD HEALTH_MAX_OFFSET     = 0x114;
    static constexpr DWORD XP_CURRENT_OFFSET     = 0x118;
    static constexpr DWORD XP_MAX_OFFSET         = 0x11C;
    static constexpr DWORD MOVE_CURRENT_OFFSET   = 0x1A8;
    static constexpr DWORD MOVE_TOTAL_OFFSET     = 0x1AC;
    static constexpr DWORD ATTACK_CURRENT_OFFSET = 0x31C;
    static constexpr DWORD ATTACK_TOTAL_OFFSET   = 0x320;

    UnitOffsets(DWORD_PTR* base = nullptr) : BaseOffsets(base) {}

    DWORD* getName()         const { return (DWORD*)getOffset(NAME_OFFSET); }
    DWORD* getHealthCurrent()const { return (DWORD*)getOffset(HEALTH_CURRENT_OFFSET); }
    DWORD* getHealthMax()    const { return (DWORD*)getOffset(HEALTH_MAX_OFFSET); }
    DWORD* getXPCurrent()    const { return (DWORD*)getOffset(XP_CURRENT_OFFSET); }
    DWORD* getXPMax()        const { return (DWORD*)getOffset(XP_MAX_OFFSET); }
    DWORD* getMoveCurrent()  const { return (DWORD*)getOffset(MOVE_CURRENT_OFFSET);};
    DWORD* getMoveMax()      const { return (DWORD*)getOffset(MOVE_TOTAL_OFFSET);};
    DWORD* getAttackCurrent()const { return (DWORD*)getOffset(ATTACK_CURRENT_OFFSET);};
    DWORD* getAttackTotal()  const { return (DWORD*)getOffset(ATTACK_TOTAL_OFFSET);};
};

// Player-specific offsets
struct PlayerOffsets : BaseOffsets {
    static constexpr DWORD NAME_OFFSET_PLAYER1 = 0x1A8;
    static constexpr DWORD NAME_OFFSET_PLAYER2 = 0x578;
    static constexpr DWORD GOLD_OFFSET_PLAYER1 = 0x8;
    static constexpr DWORD GOLD_OFFSET_PLAYER2 = 0x3D8;

    PlayerOffsets(DWORD_PTR* base = nullptr) : BaseOffsets(base) {}

    DWORD* getNamePlayer1() const { return (DWORD*)getOffset(NAME_OFFSET_PLAYER1); }
    DWORD* getNamePlayer2() const { return (DWORD*)getOffset(NAME_OFFSET_PLAYER2); }
    DWORD* getGoldPlayer1() const { return (DWORD*)getOffset(GOLD_OFFSET_PLAYER1); }
    DWORD* getGoldPlayer2() const { return (DWORD*)getOffset(GOLD_OFFSET_PLAYER2); }
};

// Main Wesnoth offsets structure
struct Offsets_Wesnoth {
    DWORD_PTR* unit_base;
    DWORD_PTR* player_base;
    UnitOffsets Unit;
    PlayerOffsets player;

    Offsets_Wesnoth() : unit_base(nullptr), player_base(nullptr), Unit(nullptr), player(nullptr) {}

    void updateBases(DWORD_PTR* newUnitBase, DWORD_PTR* newPlayerBase) {
        unit_base = newUnitBase;
        player_base = newPlayerBase;
        Unit = UnitOffsets(unit_base);
        player = PlayerOffsets(player_base);
    }
};

// Unit Wesnoth structure (data access)
struct Unit_Wesnoth {
    UnitOffsets offsets;

    Unit_Wesnoth(DWORD_PTR* base = nullptr) : offsets(base) {}

    DWORD getName()          const { return offsets.getName()          ? *offsets.getName()          : 0; }
    DWORD getHealthCurrent() const { return offsets.getHealthCurrent() ? *offsets.getHealthCurrent() : 0; }
    DWORD getHealthMax()     const { return offsets.getHealthMax()     ? *offsets.getHealthMax()     : 0; }
    DWORD getXPCurrent()     const { return offsets.getXPCurrent()     ? *offsets.getXPCurrent()     : 0; }
    DWORD getXPMax()         const { return offsets.getXPMax()         ? *offsets.getXPMax()         : 0; }
    DWORD getMoveCurrent()   const { return offsets.getMoveCurrent()   ? *offsets.getMoveCurrent()   : 0; }
    DWORD getMoveMax()       const { return offsets.getMoveMax()       ? *offsets.getMoveMax()       : 0; }
    DWORD getAttackCurrent() const { return offsets.getAttackCurrent() ? *offsets.getAttackCurrent() : 0; }
    DWORD getAttackTotal()   const { return offsets.getAttackTotal()   ? *offsets.getAttackTotal()   : 0; }

    void setHealth(int value) {
        if (offsets.getHealthCurrent()) *offsets.getHealthCurrent() = value;
        if (offsets.getHealthMax()) *offsets.getHealthMax() = value;
    }

    void setXP() {
        if (offsets.getXPCurrent()) *offsets.getXPCurrent() = *offsets.getXPMax();
    }

    void setMove() {
        if (offsets.getMoveCurrent()) *offsets.getMoveCurrent() = *offsets.getMoveMax();
    }

    void setAtack() {
        if (offsets.getAttackCurrent()) *offsets.getAttackCurrent() = *offsets.getAttackTotal();
    }

};

// Player Wesnoth structure (data access)
struct Player_Wesnoth {
    PlayerOffsets offsets;

    Player_Wesnoth(DWORD_PTR* base = nullptr) : offsets(base) {}

    DWORD getNamePlayer1() const { return offsets.getNamePlayer1() ? *offsets.getNamePlayer1() : 0; }
    DWORD getNamePlayer2() const { return offsets.getNamePlayer2() ? *offsets.getNamePlayer2() : 0; }
    DWORD getGoldPlayer1() const { return offsets.getGoldPlayer1() ? *offsets.getGoldPlayer1() : 0; }
    DWORD getGoldPlayer2() const { return offsets.getGoldPlayer2() ? *offsets.getGoldPlayer2() : 0; }

    void setGoldPlayer1(int value){
        if(offsets.getGoldPlayer1()) *offsets.getGoldPlayer1() = value;
    }
};