#include "summerFour.h"
void summerFour::setterm1(float term1)
{
m_term1 = term1;
}
void summerFour::setterm2(float term2)
{
m_term2 = term2;
}
void summerFour::setterm3(float term3)
{
m_term3 = term3;
}
void summerFour::setterm4(float term4)
{
m_term4 = term4;
}
float summerFour::getresult()
{
    m_result = m_term1 + m_term2 + m_term3 + m_term4;
return m_result;
}
