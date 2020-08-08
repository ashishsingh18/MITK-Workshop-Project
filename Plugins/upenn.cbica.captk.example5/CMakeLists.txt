project(upenn_cbica_captk_example5)

include_directories(${CTK_INCLUDE_DIRS})

mitk_create_plugin(
	EXPORT_DIRECTIVE UPENN_CBICA_CAPTK_EXAMPLE5
	MODULE_DEPENDS MitkExample3 MitkPython MitkExample5
)