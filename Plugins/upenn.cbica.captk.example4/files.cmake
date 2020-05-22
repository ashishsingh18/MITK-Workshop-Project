set(SRC_CPP_FILES
  
)

set(INTERNAL_CPP_FILES
  mitkPluginActivator.cpp
  QmitkCaPTkExample4View.cpp
)

set(UI_FILES
  src/internal/QmitkCaPTkExample4Controls.ui
)

set(MOC_H_FILES
  src/internal/mitkPluginActivator.h
  src/internal/QmitkCaPTkExample4View.h
)

set(CACHED_RESOURCE_FILES
  resources/icon4.svg
  plugin.xml
)

set(QRC_FILES
	resources/QExample4.qrc
)

### Don't change below

set(CPP_FILES)

foreach(file ${SRC_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})
