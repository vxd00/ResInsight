import os
from pathlib import Path

files = os.listdir("./_build/pydocmd")
#print (files)

for f in files:
   file_path = "./_build/pydocmd/" + f
   with open(file_path, "r+") as file_object:
     s = file_object.read()
     file_object.seek(0)
     
     # generated markdown file is like "pyEclipseCase.md" Remove first two and last three to get class name
     class_name = f[2:-3]
     
     header_text = "+++\ntitle = \"" + class_name + "\"\npublished = true\n+++\n\n"
     file_object.write( header_text + s)
