set(appname iCubGui)

file(GLOB covers ${CMAKE_CURRENT_SOURCE_DIR}/covers/*.obj)
file(GLOB files ${CMAKE_CURRENT_SOURCE_DIR}/*.ini)
file(GLOB icon_files ${CMAKE_CURRENT_SOURCE_DIR}/icons/*.png)
file(GLOB scripts ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.template)

yarp_install(FILES ${covers} DESTINATION ${ICUB_CONTEXTS_INSTALL_DIR}/${appname}/covers)
yarp_install(FILES ${icon_files} DESTINATION ${ICUB_CONTEXTS_INSTALL_DIR}/${appname}/icons)
yarp_install(FILES ${files} DESTINATION ${ICUB_CONTEXTS_INSTALL_DIR}/${appname})
yarp_install(FILES ${scripts} DESTINATION ${ICUB_APPLICATIONS_TEMPLATES_INSTALL_DIR})


