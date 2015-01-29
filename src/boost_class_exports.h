#ifndef BOOST_CLASS_EXPORTS_H
#define BOOST_CLASS_EXPORTS_H

#include <boost/serialization/export.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "FloatSeries.h"
#include "ImageSeries.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "ColorSetting.h"
#include "ChoiceSetting.h"
#include "FileSetting.h"
#include "NumberSetting.h"
#include "NumberParam.h"
#include "FileParam.h"
#include "ChoiceParam.h"
#include "InspectorNode.h"
#include "ConstantNode.h"
#include "ColorNode.h"
#include "MixNode.h"
#include "TransformNode.h"
#include "VideoInputNode.h"
#include "GradientInputNode.h"
#include "BlurNode.h"
#include "BrightnessContrastNode.h"
#include "PSNRNode.h"
#include "NoiseInputNode.h"
#include "SharpenNode.h"
#include "ImageInputNode.h"
#include "BinaryNode.h"

BOOST_CLASS_EXPORT_KEY(taets::FloatSeries)
BOOST_CLASS_EXPORT_KEY(taets::ImageSeries)
BOOST_CLASS_EXPORT_KEY(taets::Color)

BOOST_CLASS_EXPORT_KEY(taets::InputConnector<taets::FloatSeries>)
BOOST_CLASS_EXPORT_KEY(taets::InputConnector<taets::ImageSeries>)
BOOST_CLASS_EXPORT_KEY(taets::OutputConnector<taets::FloatSeries>)
BOOST_CLASS_EXPORT_KEY(taets::OutputConnector<taets::ImageSeries>)

BOOST_CLASS_EXPORT_KEY(taets::ColorSetting)
BOOST_CLASS_EXPORT_KEY(taets::ChoiceSetting)
BOOST_CLASS_EXPORT_KEY(taets::FileSetting)
BOOST_CLASS_EXPORT_KEY(taets::NumberSetting)

BOOST_CLASS_EXPORT_KEY(taets::NumberParam)
BOOST_CLASS_EXPORT_KEY(taets::FileParam)
BOOST_CLASS_EXPORT_KEY(taets::ChoiceParam)

BOOST_CLASS_EXPORT_KEY(taets::InspectorNode)
BOOST_CLASS_EXPORT_KEY(taets::ConstantNode)
BOOST_CLASS_EXPORT_KEY(taets::ColorNode)
BOOST_CLASS_EXPORT_KEY(taets::MixNode)
BOOST_CLASS_EXPORT_KEY(taets::TransformNode)
BOOST_CLASS_EXPORT_KEY(taets::VideoInputNode)
BOOST_CLASS_EXPORT_KEY(taets::GradientInputNode)
BOOST_CLASS_EXPORT_KEY(taets::BlurNode)
BOOST_CLASS_EXPORT_KEY(taets::BrightnessContrastNode)
BOOST_CLASS_EXPORT_KEY(taets::PSNRNode)
BOOST_CLASS_EXPORT_KEY(taets::NoiseInputNode)
BOOST_CLASS_EXPORT_KEY(taets::SharpenNode)
BOOST_CLASS_EXPORT_KEY(taets::ImageInputNode)
BOOST_CLASS_EXPORT_KEY(taets::BinaryNode)

#endif // BOOST_CLASS_EXPORTS_H
