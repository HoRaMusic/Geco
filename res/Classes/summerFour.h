#include <math.h>

class summerFour{
private:
   float m_term1;
   float m_term2;
   float m_term3;
   float m_term4;
   float m_result;
public:
   summerFour() :
       m_term1(0),
       m_term2(0),
       m_term3(0),
       m_term4(0)
       {


       };

       void setterm1(float term1);
       void setterm2(float term2);
       void setterm3(float term3);
       void setterm4(float term4);
       float getresult();
};
