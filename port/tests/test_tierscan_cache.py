import importlib.util
from pathlib import Path
import tempfile
import unittest

SPEC = importlib.util.spec_from_file_location(
    "tierscan", Path(__file__).parents[1] / "tools" / "tierscan.py")
tierscan = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(tierscan)


class TierCacheTest(unittest.TestCase):
    def test_cached_safe_cycle_does_not_hide_divergent_sibling(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            files = {"a.h": '#include "b.h"\n', "b.h": '#include "a.h"\n',
                     "safe.cpp": '#include "a.h"\n',
                     "variant.h": '#ifdef NTR_HIRES\n#endif\n',
                     "variant.cpp": '#include "a.h"\n#include "variant.h"\n'}
            for name, text in files.items():
                (root / name).write_text(text)
            scanner = tierscan.Scanner([str(root)], ["NTR_HIRES"])
            self.assertFalse(scanner.divergent(str(root / "safe.cpp")))
            # Once safe.cpp's entire closure is cached, repeat queries need
            # neither scan() nor include resolution.
            original_scan = scanner.scan
            scanner.scan = lambda path: self.fail("Safe closure was rescanned")
            self.assertFalse(scanner.divergent(str(root / "safe.cpp")))
            scanner.scan = original_scan
            self.assertTrue(scanner.divergent(str(root / "variant.cpp")))

    def test_divergent_cycle_and_comments_stay_per_target(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "a.h").write_text('#include "b.h"\n')
            (root / "b.h").write_text('#include "a.h"\n// NTR_HIRES\n')
            scanner = tierscan.Scanner([str(root)], ["NTR_HIRES"])
            self.assertTrue(scanner.divergent(str(root / "a.h")))
            self.assertTrue(scanner.divergent(str(root / "b.h")))


if __name__ == "__main__":
    unittest.main()
