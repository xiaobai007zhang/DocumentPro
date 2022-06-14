#pragma once

namespace ys
{
    // 文档类型
    enum class DocType
    {
        Others = 0,             // 其他（户外场景、非文字图像、影印插图等）
        BankCard = 1,           // 银行卡
        IDCard = 2,             // 身份证
        Driver = 3,             // 驾照
        Vehicle = 4,            // 行驶证
        BizCard = 5,            // 名片
        Railway = 6,            // 火车票
        TaxiCard = 7,           // 出租车票
        BizLcn = 8,             // 营业执照
        EMS = 9,                // 邮政EMS快递单
        Lawyer = 10,            // 律师证
        Census = 11,            // 户籍证
        Household = 12,         // 房产证
        Organization = 13,      // 组织机构代码
        TaxInvoice = 14,        // 国税地税增值税
        Document = 15,          // 场景文档
        MarriageDivorce = 16,   // 结婚证，离婚证
        Birth = 17,             // 出生证明
        Passport = 18,          // 护照
        WorkReg = 19,           // 作品登记证书
        PlaneTicket = 20,       // 航空行程单
        BrandReg = 21,          // 商标注册证
        AccountOpen = 22,       // 开户许可证
        FinancialLcn = 23,      // 金融许可证
        TaxReg = 24,            // 税务登记证
        JudicialAuth = 25,      // 司法鉴定许可证
    };

    // 文档方向
    // （正向文档的逆时针角度旋转至当前文档状态
    // or 当前文档按顺时针角度可旋转为正向文档状态）
    enum class DocDirection
    {
        Angle0 = 0,
        Angle90 = 90,
        Angle180 = 180,
        Angle270 = 270,
    };
}