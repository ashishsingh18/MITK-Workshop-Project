## ExamplePythonInvoker

Contains an example module that can run an existing python script (see resources directory).

The main advantage of having a binding to Python from C++, besides not needing to compile/freeze the python code into an executable, is that we can
utilize "cross-talk" between the two languages.

Python code can be run in a very simplistic/straightforward fashion by executing a plain python script directly,
but it's also possible to do some more intricate interaction.
We can read variables from the current Python execution context into C++.
Likewise we can set Python variables from C++. 
Usually these are transferred as strings, but other data types are possible(see http://docs.mitk.org/nightly/classmitk_1_1IPythonService.html). 

Most interestingly we can copy images from mitk::Image in C++ to simpleITK images in Python (and vice-versa). 

### Python context

There are some technical nuances to calling Python from C++, mostly involving context.
Python execution context can seem tricky at first, but is actually pretty simple. When python is run, it stores meta-information about
how the session was begun. Imported modules/files also have their own meta-information like this. 
Some methods of calling Python preserve/share this info, and some don't.


The following functions demonstrate different methods of running a python script file from MITK C++. 
See the function documentation and code for details.

RunSampleScriptWithSystemCall()

RunSampleScriptWithExecfile()

RunSampleScriptWithImport()

