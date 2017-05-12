import os

i = 36
for entry in os.scandir("samples/"):
    os.rename(entry.path, "samples/" + str(i))
    i += 1
