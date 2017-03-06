#!/usr/bin/env python

from scripts.general import *

from pathlib import Path

# Basic paths
project_root = Path(__file__).parent
build_path   = str(project_root / "build")
destination  = str(project_root / "dist")

# Copy Library
exec_globs    = ['*.exe']
files = included_files(build_path, exec_globs)
copy_files(files, build_path, destination, preserve_structure=False)
