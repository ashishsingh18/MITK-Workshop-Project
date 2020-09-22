project(upenn_cbica_captk_examplepy)

include_directories(${CTK_INCLUDE_DIRS})

mitk_create_plugin(
	EXPORT_DIRECTIVE UPENN_CBICA_CAPTK_EXAMPLEPY
	MODULE_DEPENDS MitkExamplePythonInvoker
)