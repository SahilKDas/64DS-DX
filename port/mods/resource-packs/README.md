# Client-side Lua resource packs (API 3)

Put each pack in `mods/resource-packs/<id>/pack.lua` beside the game. Set
`SM64DS_RESOURCE_PACKS` to select another root. Packs load at startup in both
single-player and multiplayer, independently of Tango's gameplay mods.

```lua
sm64ds.pack { id = "my-pack", name = "My Pack", version = "1" }
sm64ds.texture { target = "0123456789abcdef", source = "assets/texture.png" }
sm64ds.healthbar()
sm64ds.hitbox()
```

`texture` replaces a local renderer texture identified by its 16-digit content
hash. Paths must stay inside the pack. `healthbar()` requests a local-player
health display; `hitbox()` requests the projected bounds of the local player's
live body collision cylinder. Both display calls take no arguments: Lua cannot
choose health, radius, height, collision flags, or other gameplay values.
The outline is a screen-space bounding rectangle, not a cylinder wireframe.

Only metadata, textures, those two display requests, and diagnostic logging are
exposed. There are no character-profile, movement, damage, ability, spawn,
game-memory, networking, filesystem, process, or native-plugin APIs. There are
no per-frame Lua callbacks: scripts finish at startup, and the host draws from
live read-only values. Scripts cannot send pack state to a server or peer.

Each script has a 1 MiB source limit, 16 MiB Lua memory limit, and 1,000,000
instruction limit. Dynamic loading and protected Lua calls are unavailable.
One invalid pack is rejected independently; valid packs continue loading.
Conflicting texture hashes reject the conflicting pack. Registry ordering and
enabled preferences use the existing `resource-packs.state` file.

API 2 character registration is deliberately unavailable in API 3. Existing
packs using `sm64ds.character` fail validation rather than changing gameplay.
All clients may use different packs without changing simulation or packets.
