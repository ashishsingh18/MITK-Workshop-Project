option(BUILD_Example5PythonCmdApp "Build command-line app for CaPTkExample5Python" ON)

if(BUILD_Example5PythonCmdApp)
  mitkFunctionCreateCommandLineApp(
    NAME Example5PythonCmdApp
    CPP_FILES Example5PythonCmdApp.cpp
    DEPENDS CppMicroServices MitkExample5
	PACKAGE_DEPENDS
  )
endif()

