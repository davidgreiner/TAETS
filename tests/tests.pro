include(../defaults.pri)

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

TARGET = taetstest

CONFIG   += console
CONFIG   -= app_bundle

LIBS += -lunittest++

# Directories
INCLUDEPATH += ../src

SOURCES += \
    TemplateSerialization.cpp \
    TestMain.cpp \
    PSNRNodeTest.cpp \
    BlurNodeTest.cpp \
    BrightnessContrastNodeTest.cpp \
    ConstantNodeTest.cpp \
    ImageInputNodeTest.cpp \
    GradientInputNodeTest.cpp \
    MixNodeTest.cpp \
    NoiseInputNodeTest.cpp \
    SharpenNodeTest.cpp \
    TransformNodeTest.cpp \
    VideoInputNodeTest.cpp \
    ../src/AnalysisToolController.cpp \
    ../src/AnalysisToolWindow.cpp \
    ../src/BinaryNode.cpp \
    ../src/BinaryTemplate.cpp \
    ../src/BlurNode.cpp \
    ../src/BrightnessContrastNode.cpp \
    ../src/ChoiceParam.cpp \
    ../src/ChoiceSetting.cpp \
    ../src/ColorNode.cpp \
    ../src/ColorSetting.cpp \
    ../src/ConnectionItem.cpp \
    ../src/Connector.cpp \
    ../src/ConnectorItem.cpp \
    ../src/ConstantNode.cpp \
    ../src/DiagramAnalyzer.cpp \
    ../src/DiagramAnalyzerWidget.cpp \
    ../src/DiagramViewer.cpp \
    ../src/FileParam.cpp \
    ../src/FileSetting.cpp \
    ../src/FrameAnalyzer.cpp \
    ../src/FrameAnalyzerPicturePreview.cpp \
    ../src/FrameAnalyzerWidget.cpp \
    ../src/Graph.cpp \
    ../src/GraphCanvas.cpp \
    ../src/GraphEditorWindow.cpp \
    ../src/Histogram.cpp \
    ../src/HistogramAnalyzer.cpp \
    ../src/HistogramAnalyzerWidget.cpp \
    ../src/HistogramViewer.cpp \
    ../src/InputConnector.cpp \
    ../src/InputConnectorBase.cpp \
    ../src/InputConnectorItem.cpp \
    ../src/InputNode.cpp \
    ../src/ImageInputNode.cpp \
    ../src/InspectorNode.cpp \
    ../src/InspectorNodeItem.cpp \
    ../src/MixNode.cpp \
    ../src/ModifierNode.cpp \
    ../src/Node.cpp \
    ../src/NodeItem.cpp \
    ../src/NodeSetting.cpp \
    ../src/NodeSettingWidgetFactory.cpp \
    ../src/NumberParam.cpp \
    ../src/NumberSetting.cpp \
    ../src/OutputConnector.cpp \
    ../src/OutputConnectorBase.cpp \
    ../src/OutputConnectorItem.cpp \
    ../src/SharpenNode.cpp \
    ../src/SourcesSelectionWidget.cpp \
    ../src/TableAnalyzer.cpp \
    ../src/TableAnalyzerWidget.cpp \
    ../src/TemplateEditorWindow.cpp \
    ../src/TemplateParameter.cpp \
    ../src/TestData.cpp \
    ../src/TimeManipulator.cpp \
    ../src/TransformNode.cpp \
    ../src/VideoInputNode.cpp \
    ../src/TemplateManager.cpp \
    ../src/PSNRNode.cpp \
    ../src/NodeSettingsWidgetFactoryImpl.cpp \
    ../src/NodeSettingsListWidget.cpp \
    ../src/ParameterWidgetFactoryImpl.cpp \
    ../src/NoiseInputNode.cpp \
    ../src/GradientInputNode.cpp \
    FloatItemTest.cpp \
    FloatSeriesTest.cpp \
    GraphTest.cpp \
    ImageItemTest.cpp \
    ImageSeriesTest.cpp \
    GraphSortTest.cpp \
    ../src/InspectorInputConnectorItem.cpp \
    TestHelper.cpp \
    ../src/LanguageHandler.cpp \
    HistogramTest.cpp \
    ColorNodeTest.cpp \
    ../src/ColorSelectionWidget.cpp \
    ../src/FileSelectionWidget.cpp \
    ../src/GraphScene.cpp

HEADERS += \
    ../src/AnalysisToolController.h \
    ../src/AnalysisToolWindow.h \
    ../src/Analyzer.h \
    ../src/BinaryNode.h \
    ../src/BinaryTemplate.h \
    ../src/BlurNode.h \
    ../src/BrightnessContrastNode.h \
    ../src/ChoiceParam.h \
    ../src/ChoiceSetting.h \
    ../src/Color.h \
    ../src/ColorNode.h \
    ../src/ColorSetting.h \
    ../src/ConnectionItem.h \
    ../src/Connector.h \
    ../src/ConnectorItem.h \
    ../src/ConstantNode.h \
    ../src/DataItem.h \
    ../src/DataSeries.h \
    ../src/DiagramAnalyzer.h \
    ../src/DiagramAnalyzerWidget.h \
    ../src/DiagramViewer.h \
    ../src/FileParam.h \
    ../src/FileSetting.h \
    ../src/FloatItem.h \
    ../src/FloatSeries.h \
    ../src/FrameAnalyzer.h \
    ../src/FrameAnalyzerPicturePreview.h \
    ../src/FrameAnalyzerWidget.h \
    ../src/Graph.h \
    ../src/GraphCanvas.h \
    ../src/GraphEditorWindow.h \
    ../src/Histogram.h \
    ../src/HistogramAnalyzer.h \
    ../src/HistogramAnalyzerWidget.h \
    ../src/HistogramViewer.h \
    ../src/ImageDataHelper.h \
    ../src/ImageItem.h \
    ../src/ImageSeries.h \
    ../src/InputConnector.h \
    ../src/InputConnectorBase.h \
    ../src/InputConnectorItem.h \
    ../src/InputNode.h \
    ../src/InspectorNode.h \
    ../src/InspectorNodeItem.h \
    ../src/MixNode.h \
    ../src/ModifierNode.h \
    ../src/Node.h \
    ../src/NodeItem.h \
    ../src/NodeSetting.h \
    ../src/NodeSettingWidgetFactory.h \
    ../src/NumberParam.h \
    ../src/NumberSetting.h \
    ../src/OutputConnector.h \
    ../src/OutputConnectorBase.h \
    ../src/OutputConnectorItem.h \
    ../src/ParameterWidgetFactory.h \
    ../src/SharpenNode.h \
    ../src/SourcesSelectionWidget.h \
    ../src/TableAnalyzer.h \
    ../src/TableAnalyzerWidget.h \
    ../src/TemplateEditorWindow.h \
    ../src/TemplateParameter.h \
    ../src/TestData.h \
    ../src/TimeManipulator.h \
    ../src/TransformNode.h \
    ../src/VideoInputNode.h \
    ../src/TemplateManager.h \
    ../src/PSNRNode.h \
    ../src/NodeSettingsWidgetFactoryImpl.h \
    ../src/NodeSettingsListWidget.h \
    ../src/ParameterWidgetFactoryImpl.h \
    ../src/InspectorInputConnectorItem.h \
    TestHelper.h \
    ../src/LanguageHandler.hpp \
    ../src/ColorSelectionWidget.h \
    ../src/FileSelectionWidget.h \
    ../src/GraphScene.h


FORMS += \
    ../src/AnalysisToolWindow.ui \
    ../src/DiagramAnalyzerWidget.ui \
    ../src/FrameAnalyzerWidget.ui \
    ../src/GraphEditorWindow.ui \
    ../src/HistogramAnalyzerWidget.ui \
    ../src/TemplateEditorWindow.ui \
    ../src/TableAnalyzerWidget.ui
