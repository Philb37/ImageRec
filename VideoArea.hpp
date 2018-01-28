/* 
 * File:   VideoArea.hpp
 * Author: imam
 *
 * Created on 3 October 2015, 1:03 AM
 */

#ifndef VIDEOAREA_HPP
#define	VIDEOAREA_HPP

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>

#include <opencv2/opencv.hpp>
#include "Kinect.h"


class VideoArea : public Gtk::DrawingArea
{

public:
    VideoArea ();
    void StartKinect();
    void StopKinect();
    virtual ~VideoArea();

    cv::Mat getChosenRoi() {
        return chosenROI;
    }

    bool hasChosenROI(){
        return chosedROI;
    }

protected:
    bool cv_opened;
    VideoSource * sourceFeed;


    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &cr);
    bool onMouseDown(GdkEventButton * event);
    bool onMouseMove(GdkEventMotion * event);
    bool onMouseUp(GdkEventButton * event);
    bool on_timeout ();



private:
    bool on_dragged;
    bool chosedROI;
    int x1,x2,y1,y2;
    cv::Rect rectROI;
    cv::Mat chosenROI;
    cv::Mat currentPic;

};

#endif	/* VIDEOAREA_HPP */
