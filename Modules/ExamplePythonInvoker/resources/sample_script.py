

print("The sample script says Hello, World!")
print("If you are seeing this message, sample_script has been invoked.")

# Begin testing imports
print("Testing imports...")

import os # built-in module, doesn't need special installing
import sys
print("os and sys imported successfully.")

import numpy as np # this module is installed via pip and can fail to import if the package is missing
print("numpy imported successfully.")

### Diagnostic information ###
print("CWD is:")
print(os.getcwd())
print("Path info:")
for p in sys.path:
	print(p)
### End of Diagnostic stuff ###	

import sample_module # our custom sample module
print("sample_module imported successfully.")

# Try to use a function from our sample_module
my_string = "BIG UPPERCASE LETTERS"
print("Converting...")
result = sample_module.my_lowercase_function(my_string)
print(my_string + " -> " + result)

if __name__ == "__main__":
	print("sample_script is running as __main__")
	




