option(BUILD_CaPTkExample4CmdApp "Build command-line app for CaPTkExample4" ON)

if(BUILD_CaPTkExample4CmdApp)
  mitkFunctionCreateCommandLineApp(
    NAME CaPTkExample4CmdApp
    CPP_FILES CaPTkExample4CmdApp.cpp
    DEPENDS MitkExample3
  )
endif()
