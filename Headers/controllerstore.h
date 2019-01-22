#ifndef CONTROLLERSTORE_H
#define CONTROLLERSTORE_H
#include <QString>

class Controllerstore
{
public:
    Controllerstore(QString type_, QString param_name_, QString controller_name_, QString imageName_, QString low_value_,
                    QString high_value_, QString default_value_, QString x_pos_, QString y_pos_, QString shadowImageName);
    QString type();
    QString param_name();
    QString controller_name();
    QString imageName();
    QString low_value();
    QString high_value();
    QString default_value();
    QString switch_positions();
    QString x_pos();
    QString y_pos();
    QString shadowImageName();
private:
    QString m_type;
    QString m_param_name;
    QString m_controller_name;
    QString m_imageName;
    QString m_low_value;
    QString m_high_value;
    QString m_default_value;
    QString m_switch_positions;
    QString m_x_pos;
    QString m_y_pos;
    QString m_shadow_image_name;
};

#endif // CONTROLLERSTORE_H
