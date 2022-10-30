project(MASC VERSION 0.1.0)

set (BaseTargetName MASC)

juce_add_plugin("${BaseTargetName}"
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "Neux Ltd"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        AU_MAIN_TYPE kAudioUnitType_MIDIProcessor
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Juce
        PLUGIN_CODE MFxt
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "MASC")

juce_generate_juce_header(${BaseTargetName})

target_sources(${BaseTargetName} PRIVATE
        Source/PluginEditor.cpp
        Source/LooperData.cpp
        Source/LooperComponent.cpp
        Source/MidiProcessor.cpp
        Source/PluginProcessor.cpp)

target_compile_definitions(${BaseTargetName}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${BaseTargetName} PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        shared_plugin_helpers
        shared_processing_code
        juce_recommended_config_flags
        juce_recommended_lto_flags
        juce_recommended_warning_flags)
