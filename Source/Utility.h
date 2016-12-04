/*
 * Utility.h
 *
 *  Created on: Jan 14, 2015
 *      Author: kchristopher
 */

#ifndef SOURCE_UTILITY_H_
#define SOURCE_UTILITY_H_

#include "Include.h"
#include "arm_math.h"

#ifdef __cplusplus
 extern "C" {
#endif

extern float vsqrtf(float op1);

#ifdef __cplusplus
}
#endif


struct FloatVector
{
   float x;
   float y;
   FloatVector(float ix=0.0, float iy=0.0):x(ix),y(iy){}

   FloatVector& operator-=(const FloatVector& rhs)
   {
     x-=rhs.x;
     y-=rhs.y;
     return *this; // return the result by reference
   }
   friend FloatVector operator-(FloatVector lhs,
                     const FloatVector& rhs)
   {
      return lhs -= rhs;
   }
   FloatVector& operator+=(const FloatVector& rhs)
   {
     x+=rhs.x;
     y+=rhs.y;
     return *this;
   }
   friend FloatVector operator+(FloatVector lhs,
                     const FloatVector& rhs)
   {
      return lhs += rhs;
   }
   friend FloatVector operator*(FloatVector lhs, float rhs)
   {
      lhs.x *= rhs;
      lhs.y *= rhs;
      return lhs;
   }
};

struct IntVector
{
   int x;
   int y;
   IntVector(int ix=0, int iy=0):x(ix),y(iy){}

   IntVector& operator=(const FloatVector& rhs)
   {
     x=rhs.x+0.5;
     y=rhs.y+0.5;
     return *this;
   }
   IntVector& operator-=(const IntVector& rhs)
   {
     x-=rhs.x;
     y-=rhs.y;
     return *this;
   }
   friend IntVector operator-(IntVector lhs,
                     const IntVector& rhs)
   {
      return lhs -= rhs;
   }
   IntVector& operator+=(const IntVector& rhs)
   {
     x+=rhs.x;
     y+=rhs.y;
     return *this; // return the result by reference
   }
   friend IntVector operator+(IntVector lhs,
                     const IntVector& rhs)
   {
      return lhs += rhs;
   }
};


class CPIControl
{
public:
     float m_kI;
     float m_kP;
     float m_kD;
     float m_kFF;
     float m_I;
     float m_dT;
     float m_Power;
     float m_fOffset;
     float m_fLastMeasured;

     CPIControl(float dT, float kP, float kI, float kD, float kFF, float fPower = 30.0, float fOffset = 5.0)
     {
         m_dT = dT;
         m_I = 0.0;
         m_kI = kI;
         m_kP = kP;
         m_kD = kD;
         m_kFF = kFF;
         m_Power = fPower;
         m_fOffset = fOffset;
         m_fLastMeasured = 0.0;
     }
     FloatVector RunPIControl(int target_pos, int measured_pos, float target_velocity, float acceleration);
     FloatVector Rotate(FloatVector in_vector,float angle_rad);
     FloatVector RotateUnity(float magnitude,float angle_offset)
     {
      FloatVector Result;
      if(angle_offset > 0.7853981633974483) angle_offset = 0.7853981633974483;
      if(angle_offset < -0.7853981633974483) angle_offset = -0.7853981633974483;
      angle_offset += 0.7853981633974483;
      Result.y = magnitude * arm_sin_f32(angle_offset);
      Result.x = magnitude * arm_cos_f32(angle_offset);
      return Result;
     }
     FloatVector ForceFunction(float angle_offset)
     {
         FloatVector Result;
         float fMag = 0.0;
         if(angle_offset>0)
         {
             fMag = angle_offset * m_Power + m_fOffset;
             if(fMag>m_Power) fMag = m_Power;
             Result.x = fMag;
             Result.y = m_fOffset;
         }
         else if(angle_offset<0)
         {
             fMag = m_fOffset - angle_offset * m_Power;
             if(fMag>m_Power) fMag = m_Power;
             Result.y = fMag;
             Result.x = m_fOffset;
         }
         return Result;
     }
};



class CPIControlInt
{
public:
     long m_kI_num;
     long m_kP_num;
     long m_kD_num;
     int m_gain_den;
     long m_I;
     long m_maxI;
     int m_dT_den;
     int m_MaxControl;
     unsigned int m_fLastMeasured;

     CPIControlInt(float dT,
             float kP,
             float kI,
             float kD,
             unsigned int MaxControl = 150)
     {
         m_dT_den = static_cast<unsigned int>(1.0/dT);
         m_I = 0.0;
         m_gain_den = 100000;
         m_MaxControl = MaxControl;
         SetkI(kI);
         SetkP(kP);
         SetkD(kD);
         m_fLastMeasured = 0;
     }
     void SetkI(float kI)
     {
         m_kI_num = kI * m_gain_den;
         m_maxI = m_MaxControl * m_gain_den / m_kI_num;
     }
     void SetkP(float kP)
     {
         m_kP_num = kP * m_gain_den;
     }
     void SetkD(float kD)
     {
         m_kD_num = kD * m_gain_den;
     }
     void SetMaxControl(unsigned int MaxControl)
     {
         m_MaxControl = MaxControl;
         m_maxI = m_MaxControl * m_gain_den / m_kI_num;
     }
     int RunPIControl(unsigned int target_pos,
             unsigned int measured_pos);

};

class PWM_Table
{
public:
    static const int SIZE_OF_POWER_TABLE = 760;
    static const int TABLE_CENTER = SIZE_OF_POWER_TABLE / 2;
    static const unsigned int power_table[SIZE_OF_POWER_TABLE];

    static unsigned int GetChannelA(int nControl)
    {
        int nIndex = TABLE_CENTER + nControl;
        if(nIndex >= SIZE_OF_POWER_TABLE)
        {
            nIndex = SIZE_OF_POWER_TABLE - 1;
        }
        else if(nIndex < 0)
        {
            nIndex = 0;
        }
        return power_table[nIndex];
    }
    static unsigned int GetChannelB(int nControl)
    {
        int nIndex = TABLE_CENTER - nControl;
        if(nIndex >= SIZE_OF_POWER_TABLE)
        {
            nIndex = SIZE_OF_POWER_TABLE - 1;
        }
        else if(nIndex < 0)
        {
            nIndex = 0;
        }
        return power_table[nIndex];
    }
};
#endif /* SOURCE_UTILITY_H_ */
