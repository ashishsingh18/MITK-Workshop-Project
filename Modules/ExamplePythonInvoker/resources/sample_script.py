#####################
# sample_script.py
#####################
# This script is invoked from the MitkExamplePythonInvoker module.
# This script in the install/build tree can be edited or replaced.
# If you do, the functionality inside CaPTk will change accordingly. Try it out!
# Author: Alexander Getka

### Diagnostic information ###
# Begin by testing imports
print("Testing imports...")

import os # built-in module, doesn't need special installing
import sys
print("os and sys imported successfully.")

print("CWD is:")
print(os.getcwd())
print("Path info:")
for p in sys.path:
	print(p)
### End of Diagnostic stuff ###	

############# EDIT BELOW THIS LINE ########################
print("The sample script says Hello, World!")
print("If you are seeing this message, sample_script has been invoked.")

import sample_module # our custom sample module
print("sample_module imported successfully.")

# Try to use a function from our sample_module
my_string = "BIG UPPERCASE LETTERS"
print("Converting...")
result = sample_module.my_lowercase_function(my_string)
print(my_string + " -> " + result)

CAPTK_RESULT_STRING = result

import numpy as np # this module is installed via pip and can fail to import if the package is missing
print("numpy imported successfully.")

if __name__ == "__main__":
	# This block will execute if this file was run directly -- not through import, etc.
	# This pattern is useful for debugging and testing.
	print("sample_script is running as __main__")
	
# TODO: create example for code only run under captk (requires magic variable management from CaPTk)
	




