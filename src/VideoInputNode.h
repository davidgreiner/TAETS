#ifndef TAETS_VIDEOINPUTNODE_H
#define TAETS_VIDEOINPUTNODE_H
#include <QCoreApplication>
#include "InputNode.h"
#include <opencv2/core/core.hpp>
#include "NumberSetting.h"
#include "FileSetting.h"
#include "OutputConnector.h"
#include "ImageSeries.h"
#include "Graph.h"

namespace taets {

class VideoInputNode : public InputNode {
    Q_DECLARE_TR_FUNCTIONS(VideoInputNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<InputNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    VideoInputNode() {}

  public:
    VideoInputNode(Graph* graph, unsigned int id);

    /**
     * Lese und konvertiere gegebenfalls das Eingabevideo
     */
    virtual void compute();


  private:
    /**
     * @brief YUV_read Lese YUV Video ein
     * @param capture Speicher des YUV Videos
     * @return -1 falls etwas schiefgelaufen ist,sonst 0
     */
    int YUV_read();

    /**
     * Gebe Speicher wieder frei
     */
    void YUV_cleanUp();

    unsigned int width, height;
    cv::Mat y, cb, cr, cb_half, cr_half, ycrcb, rgb;
    FILE* input_file;
};
}
#endif
