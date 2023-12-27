idf_build_get_property(project_dir PROJECT_DIR)

# Initialize an empty list to hold conditional requirements
set(conditionally_required_components "")

if(EXISTS "${project_dir}/components/drv_adc")
    list(APPEND conditionally_required_components "drv_adc")
endif()

idf_component_register(
                    SRCS 
                        "drv_ntc.c" 
                    INCLUDE_DIRS 
                        "." 
                    REQUIRES
                        ""
                        ${conditionally_required_components} 
                                      )
                 

