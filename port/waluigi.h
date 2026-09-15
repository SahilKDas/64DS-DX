#ifndef SM64DS_PORT_WALUIGI_H
#define SM64DS_PORT_WALUIGI_H

enum PortCharacter {
    PORT_CHARACTER_MARIO = 0,
    PORT_CHARACTER_LUIGI = 1,
    PORT_CHARACTER_WARIO = 2,
    PORT_CHARACTER_YOSHI = 3,
    PORT_CHARACTER_WALUIGI = 4,
    PORT_CHARACTER_COUNT = 5
};

inline int port_character_normalize(int character)
{
    character %= PORT_CHARACTER_COUNT;
    return character < 0 ? character + PORT_CHARACTER_COUNT : character;
}

inline unsigned port_character_resource(unsigned character)
{
    return character == PORT_CHARACTER_WALUIGI
        ? PORT_CHARACTER_WARIO
        : character;
}

#endif
