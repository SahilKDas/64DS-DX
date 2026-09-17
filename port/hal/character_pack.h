#ifndef PORT_CHARACTER_PACK_H
#define PORT_CHARACTER_PACK_H

// Draw a locally imported host character at the player's root matrix. Returns
// true when a pack was found and submitted, false for the normal DS fallback.
bool port_character_pack_draw(int logical_character, const int model[12]);

#endif
