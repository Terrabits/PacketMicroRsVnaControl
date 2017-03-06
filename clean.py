#!/usr/bin/env python


from scripts.general import *

import sys
import os
from   pathlib import Path

args = [arg.lower() for arg in sys.argv]

# Paths
root_path  = Path(__file__).parent
build_path = root_path / "build"
dist_path  = root_path / "dist"

# clean _install
if "--skip-build" not in args:
    delete_except(str(build_path),   [])
    touch(str(build_path / '.keep'))

if "--skip-dist" not in args:
    delete_except(str(dist_path), [], [])
    touch(str(dist_path / '.keep'))
