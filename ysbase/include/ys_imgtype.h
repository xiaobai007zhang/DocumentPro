#pragma once

namespace ys
{
    // 图像类型
    enum class ImgType
    {
        Unknown = 0,
        BMP = 1,
        JPG = 2,
        GIF = 3,
        TIF = 4,
        PNG = 5,
        J2K = 6,
        JP2 = 7,
        PDF = 8,
        OFD = 9,
    };

    // 获取图像扩展名
    inline pcchar ImgExtname(ImgType type)
    {
        switch (type)
        {
        case ImgType::BMP:
            return ".bmp";
        case ImgType::JPG:
            return ".jpg";
        case ImgType::GIF:
            return ".gif";
        case ImgType::TIF:
            return ".tif";
        case ImgType::PNG:
            return ".png";
        case ImgType::J2K:
            return ".j2k";
        case ImgType::JP2:
            return ".jp2";
        case ImgType::PDF:
            return ".pdf";
        case ImgType::OFD:
            return ".ofd";
        default:
            return ".unknown";
        }
    }
}