#include "controllerstore.h"

Controllerstore::Controllerstore(QString type_, QString param_name_, QString controller_name_, QString imageName_, QString low_value_,
                                 QString high_value_, QString default_value_, QString x_pos_, QString y_pos_, QString shadowImageName_)
{
    m_type = type_;
    m_param_name = param_name_;
    m_controller_name = controller_name_;
    m_imageName = imageName_;
    m_low_value = low_value_;
    m_high_value = high_value_;
    m_default_value = default_value_;
    int highVal = high_value_.toInt();
    int posNum =  highVal + 1;
    QString posNumS = QString::number(posNum);
    m_switch_positions = posNumS;
    m_x_pos = x_pos_;
    m_y_pos = y_pos_;
    m_shadow_image_name =  shadowImageName_;
}
QString Controllerstore::type()
{
    return m_type;
}
QString Controllerstore::param_name()
{
    return m_param_name;
}
QString Controllerstore::controller_name()
{
    return m_controller_name;
}
QString Controllerstore::imageName()
{
    return m_imageName;
}
QString Controllerstore::low_value()
{
    return m_low_value;
}
QString Controllerstore::high_value()
{
    return m_high_value;
}
QString Controllerstore::default_value()
{
    return m_default_value;
}
QString Controllerstore::switch_positions()
{
    return m_switch_positions;
}
QString Controllerstore::x_pos()
{
    return m_x_pos;
}
QString Controllerstore::y_pos()
{
    return m_y_pos;
}
QString Controllerstore::shadowImageName()
{
    return m_shadow_image_name;
}
