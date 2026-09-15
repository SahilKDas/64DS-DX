# Waluigi (fifth playable character)

This mod adds Waluigi as character ID 4. It is deliberately not a Wario skin:
the target cardinality is five playable characters everywhere the game models a
character choice.

The retail-matching `src/` tree remains untouched. Changes that diverge from the
retail ROM belong in this directory as patches or generated overrides so the
upstream decompilation can continue to be merged and validated independently.

## Character IDs

| ID | Character |
|---:|---|
| 0 | Mario |
| 1 | Luigi |
| 2 | Wario |
| 3 | Yoshi |
| 4 | Waluigi |

The shared constants live in `include/waluigi/character.h`. Mod code must use
`WALUIGI_CHARACTER_COUNT`; a new literal `4` used as an upper bound is a bug.

## Implementation waves

1. **Core identity and persistence** — accept ID 4 in character validation,
   preserve it in `SaveData::mCharacter`, and keep it through scene transitions.
2. **Player resources** — expand body/head/cap model tables, animation tables,
   material and texture sequences, and cleanup loops from four entries to five.
3. **Gameplay properties** — add Waluigi's movement, jump, swim, damage, voice,
   and power-flower dispatch entries without aliasing another character.
4. **Selection and UI** — add a fifth selection target, portrait, name, map/cap
   icons, unlock state, and layout changes required by the extra choice.
5. **World integration** — rabbits, caps, doors/messages, star-gate checks,
   multiplayer packets, and every character-indexed lookup found by the audit.
6. **Assets and verification** — install original model/animation/texture/audio
   assets supplied by the mod author, build a patched ROM, and test all five IDs.

Run the initial cardinality audit with:

```powershell
python mods/waluigi/tools/audit_character_cardinality.py
```

This is a candidate finder, not permission to replace every `4`: the Nintendo DS
codebase also contains four-player, four-component, four-byte, and 4x4-matrix
logic that must remain unchanged.
