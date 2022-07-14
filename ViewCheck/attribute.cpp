#include "attribute.h"

Attribute::Attribute(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);
    initAttribute();
}

Attribute::~Attribute()
{
}

void Attribute::initAttribute()
{
    setWindowModality(Qt::ApplicationModal);
}
