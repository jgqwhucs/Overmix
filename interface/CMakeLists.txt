cmake_minimum_required(VERSION 3.0)
project(Overmix)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14") # Enable C++14 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") # Turn on warnings

# Qt compilers setup
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCE_CLI
	cli/AlignParser.cpp
	cli/CommandParser.cpp
	cli/ComparatorParser.cpp
	cli/RenderParser.cpp
	cli/Processor.cpp
	)

set(SOURCE_GUI
	gui/ExceptionCatcher.cpp
	gui/mainwindow.cpp
	gui/ImagesModel.cpp
	gui/MaskModel.cpp
	gui/FullscreenViewer.cpp
	gui/AlignMethodSelector.cpp
	gui/importers/VideoImporter.cpp
	gui/importers/VideoPreviewModel.cpp
	)

set(SOURCE_GUI_CONFIGS
	gui/configs/AlignerConfigs.cpp
	gui/configs/ComparatorConfigs.cpp
	gui/configs/RenderConfigs.cpp
	)
	
set(SOURCE_GUI_PROCESSORS
	gui/processors/AProcessor.cpp
	gui/processors/AProcessorFactory.cpp
	gui/processors/ProcessorList.cpp
	gui/processors/ProcessBinarize.cpp
	gui/processors/ProcessBinarizeAdaptive.cpp
	gui/processors/ProcessBlur.cpp
	gui/processors/ProcessColor.cpp
	gui/processors/ProcessCrop.cpp
	gui/processors/ProcessDeVlc.cpp
	gui/processors/ProcessDilate.cpp
	gui/processors/ProcessDither.cpp
	gui/processors/ProcessDeconvolve.cpp
	gui/processors/ProcessEdge.cpp
	gui/processors/ProcessInpaint.cpp
	gui/processors/ProcessLevels.cpp
	gui/processors/ProcessScale.cpp
	)
if(W2XC)
	set(SOURCE_GUI_PROCESSORS ${SOURCE_GUI_PROCESSORS} gui/processors/ProcessWaifu.cpp)
endif()

set(SOURCE_GUI_SAVERS
	gui/savers/DumpSaver.cpp
	)

set(SOURCE_GUI_VISUALISATION
	gui/visualisations/MovementGraph.cpp
	gui/visualisations/SkipRenderPreview.cpp
	)

if( WIN32 ) # Don't use registry on windows for saving options
	add_definitions( -DPORTABLE )
endif( WIN32 )

add_executable(OvermixCli ${SOURCE_CLI} main-cli.cpp)
add_executable(Overmix WIN32 ${SOURCE_CLI} ${SOURCE_GUI} ${SOURCE_GUI_CONFIGS} ${SOURCE_GUI_PROCESSORS} ${SOURCE_GUI_SAVERS} ${SOURCE_GUI_VISUALISATION} ${SOURCE_GUI_VIEWER} ${extra_moc} main.cpp)

# Set-up libraries
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

include_directories ("${PROJECT_SOURCE_DIR}/../src")
target_link_libraries(OvermixCli OvermixCore Qt5::Widgets)
target_link_libraries(Overmix OvermixCore ImgViewer Qt5::Widgets Qt5::PrintSupport -lqcustomplot)

install(TARGETS Overmix RUNTIME DESTINATION bin)
install(TARGETS OvermixCli RUNTIME DESTINATION bin)


# C++14
set_property(TARGET Overmix    PROPERTY CXX_STANDARD 14)
set_property(TARGET OvermixCli PROPERTY CXX_STANDARD 14)
set_property(TARGET Overmix    PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET OvermixCli PROPERTY CXX_STANDARD_REQUIRED ON)
