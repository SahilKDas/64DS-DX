import importlib.util
from pathlib import Path
import tempfile
import unittest

SPEC = importlib.util.spec_from_file_location(
    "storage_budget", Path(__file__).parents[1] / "tools" / "storage_budget.py")
budget = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(budget)


class StorageBudgetTest(unittest.TestCase):
    def test_git_and_outputs_count_but_rom_assets_do_not(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            files = {".git/objects/pack/test.pack": 17,
                     "build/Release/game.exe": 31, "port/code.cpp": 7,
                     "game.nds": 101, "extracted/arm9.bin": 103,
                     "build/assets/romdata.bin": 107, "my-assets/model.bmd": 109}
            for name, size in files.items():
                path = root / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(b"x" * size)
            report = budget.measure(root, ["my-assets"])
            self.assertEqual(report["bytes"], 55)
            self.assertEqual(report["excluded_rom_asset_bytes"], 420)
            self.assertEqual(report["errors"], [])

    def test_no_automatic_executable_or_build_exclusion(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "build").mkdir()
            (root / "build" / "test.obj").write_bytes(b"x" * 123)
            self.assertEqual(budget.measure(root)["bytes"], 123)


if __name__ == "__main__":
    unittest.main()
