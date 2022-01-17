set(component_srcs "mirf.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS ".")
