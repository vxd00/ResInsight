import os
from pathlib import Path

input_path = "../rips/PythonExamples"
output_path = "./_build/PythonExamples"
files = os.listdir(input_path)
#print (files)

for f in files:
   file_path = input_path + "/" + f
   with open(file_path, "r+") as file_object:
     s = file_object.read()
     
     output_filename = output_path + "/" + f[0:-2] + "md"
     with open(output_filename, "w+") as output_file_object:
       # generated markdown file is like "pyEclipseCase.md" Remove first two and last three to get class name
       title = f[0:-3]
       header_text = "+++\ntitle = \"" + title + "\"\npublished = true\n+++\n\n"
       output_file_object.write(header_text)
       output_file_object.write("```\n" + s + "\n```")
       
     
     
#     file_object.seek(0)
     
     # generated markdown file is like "pyEclipseCase.md" Remove first two and last three to get class name
 #    class_name = f[2:-3]
     
  #   header_text = "+++\ntitle = \"" + class_name + "\"\npublished = true\n+++\n\n"
   #  file_object.write( header_text + s)
