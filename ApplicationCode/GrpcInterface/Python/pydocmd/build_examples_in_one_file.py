import os
from pathlib import Path

input_path = "../rips/PythonExamples"
output_filename = "./_build/PythonExamples/PythonExamples.md"
files = os.listdir(input_path)
#print (files)

with open(output_filename, "w+") as output_file_object:
    title = "Python Examples"
    header_text = "+++\ntitle = \"" + title + "\"\npublished = true\n+++\n\n"
    header_text += "This pages is created based on the content in the **PythonExamples** folder located inside the **rips** module, made available online for convenience.\n\n"
    output_file_object.write(header_text)
    
    for f in files:
     file_path = input_path + "/" + f
     with open(file_path, "r+") as file_object:
       s = file_object.read()
         
       # generated markdown file is like "pyEclipseCase.md" Remove first two and last three to get class name
       title = f[0:-3]
       header_text = "## " + title + "\n"
       output_file_object.write(header_text)
       output_file_object.write("```\n" + s + "\n```\n")
