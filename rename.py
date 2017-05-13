#!/usr/bin/python3

import os

i = 0
for entry in os.scandir("samples/"):
    os.rename(entry.path, "samples/" + str(i))
    i += 1
