
print("If you are seeing this message, sample_module has been successfully imported.")

# This function is defined and will be called from sample_script
def my_lowercase_function(text):
	return text.lower()
	
# This function is defined, but sample_script won't call it.
# We can still call this function from c++ as needed.
def some_unused_function(text):
	return "blah"

if __name__ == "__main__":
	# Code in this block will only run if this script is run directly.
	# This will NOT run if this file is imported.
	print("sample_module says hello, world!")
	print("Running some tests!")
	# Test case
	print(my_lowercase_function("THE QUICK FOX JUMPED OVER THE LAZY DOG"))
    