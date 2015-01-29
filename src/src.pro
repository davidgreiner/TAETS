include(../defaults.pri)

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TAETS
TEMPLATE = app
TRANSLATIONS = ../res/languages/taets_en.ts ../res/languages/taets_de.ts

FORMS += \
    AnalysisToolWindow.ui \
    DiagramAnalyzerWidget.ui \
    FrameAnalyzerWidget.ui \
    GraphEditorWindow.ui \
    HistogramAnalyzerWidget.ui \
    TemplateEditorWindow.ui \
    TableAnalyzerWidget.ui

HEADERS += \
    AnalysisToolController.h \
    AnalysisToolWindow.h \
    Analyzer.h \
    BinaryNode.h \
    BinaryTemplate.h \
    BlurNode.h \
    BrightnessContrastNode.h \
    ChoiceParam.h \
    ChoiceSetting.h \
    Color.h \
    ColorNode.h \
    ColorSetting.h \
    ConnectionItem.h \
    Connector.h \
    ConnectorItem.h \
    ConstantNode.h \
    DataItem.h \
    DataSeries.h \
    DiagramAnalyzer.h \
    DiagramAnalyzerWidget.h \
    DiagramViewer.h \
    FileParam.h \
    FileSetting.h \
    FloatItem.h \
    FloatSeries.h \
    FrameAnalyzer.h \
    FrameAnalyzerPicturePreview.h \
    FrameAnalyzerWidget.h \
    Graph.h \
    GraphCanvas.h \
    GraphEditorWindow.h \
    Histogram.h \
    HistogramAnalyzer.h \
    HistogramAnalyzerWidget.h \
    HistogramViewer.h \
    ImageDataHelper.h \
    ImageItem.h \
    ImageSeries.h \
    InputConnector.h \
    InputConnectorBase.h \
    InputConnectorItem.h \
    InputNode.h \
    InspectorNode.h \
    MixNode.h \
    ModifierNode.h \
    Node.h \
    NodeItem.h \
    NodeSetting.h \
    NodeSettingWidgetFactory.h \
    NumberParam.h \
    NumberSetting.h \
    OutputConnector.h \
    OutputConnectorBase.h \
    OutputConnectorItem.h \
    SharpenNode.h \
    SourcesSelectionWidget.h \
    TemplateParameter.h \
    TestData.h \
    TimeManipulator.h \
    TransformNode.h \
    VideoInputNode.h \
    TemplateEditorWindow.h \
    ParameterWidgetFactory.h \
    TableAnalyzer.h \
    TableAnalyzerWidget.h \
    PSNRNode.h \
    ImageInputNode.h \
    NoiseInputNode.h \
    GradientInputNode.h \
    TemplateManager.h \
    NodeSettingsWidgetFactoryImpl.h \
    NodeSettingsListWidget.h \
    ParameterWidgetFactoryImpl.h \
    InspectorNodeItem.h \
    InspectorInputConnectorItem.h \
    LanguageHandler.h \
    FileSelectionWidget.h \
    ColorSelectionWidget.h \
    GraphScene.h \
    boost_class_exports.h

SOURCES += \
    AnalysisToolController.cpp \
    AnalysisToolWindow.cpp \
    BinaryNode.cpp \
    BinaryTemplate.cpp \
    BlurNode.cpp \
    BrightnessContrastNode.cpp \
    ChoiceParam.cpp \
    ChoiceSetting.cpp \
    ColorNode.cpp \
    ColorSetting.cpp \
    ConnectionItem.cpp \
    Connector.cpp \
    ConnectorItem.cpp \
    ConstantNode.cpp \
    DiagramAnalyzer.cpp \
    DiagramAnalyzerWidget.cpp \
    DiagramViewer.cpp \
    FileParam.cpp \
    FileSetting.cpp \
    FrameAnalyzer.cpp \
    FrameAnalyzerPicturePreview.cpp \
    FrameAnalyzerWidget.cpp \
    Graph.cpp \
    GraphCanvas.cpp \
    GraphEditorWindow.cpp \
    Histogram.cpp \
    HistogramAnalyzer.cpp \
    HistogramAnalyzerWidget.cpp \
    HistogramViewer.cpp \
    InputConnector.cpp \
    InputConnectorBase.cpp \
    InputConnectorItem.cpp \
    InputNode.cpp \
    InspectorNode.cpp \
    main.cpp \
    MixNode.cpp \
    ModifierNode.cpp \
    Node.cpp \
    NodeItem.cpp \
    NodeSetting.cpp \
    NodeSettingWidgetFactory.cpp \
    NumberParam.cpp \
    NumberSetting.cpp \
    OutputConnector.cpp \
    OutputConnectorBase.cpp \
    OutputConnectorItem.cpp \
    SharpenNode.cpp \
    SourcesSelectionWidget.cpp \
    TemplateParameter.cpp \
    TestData.cpp \
    TimeManipulator.cpp \
    TransformNode.cpp \
    VideoInputNode.cpp \
    TemplateEditorWindow.cpp \
    TableAnalyzer.cpp \
    TableAnalyzerWidget.cpp \
    PSNRNode.cpp \
    ImageInputNode.cpp \
    NoiseInputNode.cpp \
    TemplateManager.cpp \
    GradientInputNode.cpp \
    NodeSettingsWidgetFactoryImpl.cpp \
    NodeSettingsListWidget.cpp \
    ParameterWidgetFactoryImpl.cpp \
    InspectorNodeItem.cpp \
    InspectorInputConnectorItem.cpp \
    LanguageHandler.cpp \
    FileSelectionWidget.cpp \
    ColorSelectionWidget.cpp \
    GraphScene.cpp \
    boost_class_exports.cpp

RESOURCES += \
    ../res/icons.qrc \
    ../res/languages.qrc

