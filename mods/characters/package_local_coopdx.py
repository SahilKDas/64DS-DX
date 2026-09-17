"""Build local 64DS-DX packs from a user-supplied CoopDX checkout."""

from __future__ import annotations

import argparse
import aifc
import json
import shutil
import sys
import wave
from pathlib import Path

TOOLS = Path(__file__).resolve().parents[1] / "waluigi" / "tools"
sys.path.insert(0, str(TOOLS))
from import_sm64coopdx_actor import convert_textures, parse_model, write_obj

PACKS = {
    "mario": ("mario", None),
    "luigi": ("luigi", "sfx_custom_luigi"),
    "wario": ("wario", "sfx_custom_wario"),
    "waluigi": ("waluigi", "sfx_custom_wario"),
    "toad": ("toad_player", "sfx_custom_toad"),
}


def aiff_to_wav(source: Path, destination: Path) -> None:
    with aifc.open(str(source), "rb") as src:
        params = src.getparams()
        frames = src.readframes(params.nframes)
    destination.parent.mkdir(parents=True, exist_ok=True)
    with wave.open(str(destination), "wb") as dst:
        dst.setnchannels(params.nchannels)
        dst.setsampwidth(params.sampwidth)
        dst.setframerate(params.framerate)
        dst.writeframes(frames)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("coopdx", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    manifests = Path(__file__).resolve().parent
    failures = 0
    for pack, (actor, voice_bank) in PACKS.items():
        actor_dir = args.coopdx / "actors" / actor
        destination = args.output / pack
        destination.mkdir(parents=True, exist_ok=True)
        shutil.copy2(manifests / pack / "character.json", destination / "character.json")
        arrays, groups = parse_model(actor_dir / "model.inc.c")
        faces = write_obj(destination / "model-parts.obj", arrays, groups)
        textures = convert_textures(actor_dir, destination)
        voices = 0
        if voice_bank:
            voice_dir = args.coopdx / "sound" / "samples" / voice_bank
            for source in sorted(voice_dir.glob("*.aiff")):
                aiff_to_wav(source, destination / "voices" / f"{source.stem}.wav")
                voices += 1
        metadata = {
            "source": "https://github.com/coop-deluxe/sm64coopdx",
            "sourceCommit": "8cd6e5977d9f920d51ca71f2c61801d019ed79c6",
            "actor": actor,
            "faces": faces,
            "textures": textures,
            "voices": voices,
            "redistribution": "local-import-only until upstream grants a license",
        }
        (destination / "import.json").write_text(
            json.dumps(metadata, indent=2) + "\n", encoding="utf-8"
        )
        print(f"{pack}: {faces} faces, {textures} textures, {voices} voices")
        failures += faces == 0
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
