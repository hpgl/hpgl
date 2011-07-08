#!/bin/sh
scons -c
scons -j2
notify-send Scons "Build complete"
