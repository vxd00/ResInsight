1. Generate documentation config from ResInsight
Execute ResInsight with the following command line statement
  --generate d:\gitroot\ResInsight\ApplicationCode\GrpcInterface\Python\pydocmd\rips.md
This will produce the following files
  rips_class.md     : Describes the intherintance relationship between classes
  rips_commands.md  : Documentation for all commands also available from text file
  ri-pydocmd.yml    : Generated file used as basis to create pydocmd.yml, can be reneamed and used directly as pydocmd.yml
  pydocmd.yml       : Configuration file used by pydocmd
  
2. Generate markdown files 
From the command line, execute
  pydocmd build
  python .\add_hugo_header.py
    
This will create markdown files in the _build/pydocmd folder, and apply the relevant Hugo header
  
3. Create markdown based on examples
  python .\build_examples_in_one_file.py 
  
3. Copy generated files from the _build folder into the ResInsight User documentation
d:\gitroot\ResInsight-UserDocumentation\content\python 
