//cpp
// @symbol _ZN5Stage19BeforeInitResourcesEv
#include "Stage.h"

/* Slot 1 keeps the fader/sound reset and skips Scene's graphics setup.
 * The boolean result and this pointer pass through the ROM's tail call. */
bool Stage::BeforeInitResources()
{
    return ResetFadersAndSound();
}
