set(MESH_BEARER_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ad_listener.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ad_type_filter.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/adv_packet_filter.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/advertiser.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bearer_handler.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/broadcast.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/filter_engine.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gap_address_filter.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/radio_config.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rssi_filter.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/scanner.c"
    CACHE INTERNAL "")

if (EXPERIMENTAL_INSTABURST_ENABLED)
    set(MESH_BEARER_SOURCE_FILES ${MESH_BEARER_SOURCE_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}/src/instaburst.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/instaburst_rx.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/instaburst_tx.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/adv_ext_tx.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/adv_ext_packet.c"
        CACHE INTERNAL "")
endif()