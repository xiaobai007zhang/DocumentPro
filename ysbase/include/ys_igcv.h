// 组件实现，不对外暴露
//
#pragma once
#undef free
#include "opencv2/opencv.hpp"
using namespace cv;

namespace ys
{
    // OpenCV Mat对象接口
    interface IGCV : public IBase
    {
        // 获得内部的Mat
        virtual Mat& GetMat() = 0;

        // 设置Mat到内部
        virtual void SetMat(Mat& mat) = 0;
    };

    // OpenCV Mat数组对象接口
    interface IGCVArray : public IBase
    {
        // 获得数组内Mat个数
        virtual int Count() const = 0;

        // 获得数组内Mat
        virtual Mat& GetMat(int idx) = 0;

        // 添加Mat到数组
        virtual void AddMat(Mat& mat) = 0;
    };
}
