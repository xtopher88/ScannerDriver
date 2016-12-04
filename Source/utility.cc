/*
 * utility.cc
 *
 *  Created on: Jan 14, 2015
 *      Author: kchristopher
 */

#include "Utility.h"
#include <math.h>
#include "misc.h"

float vsqrtf(float op1) {
    float result;
    __ASM volatile ("vsqrt.f32 %0, %1" : "=w" (result) : "w" (op1) );
    return (result);
}

FloatVector CPIControl::RunPIControl(int target, int measured, float target_velocity, float acceleration)
{

    float delta_position = measured - m_fLastMeasured;
    m_fLastMeasured = measured;
    float position_error = measured - target;
    float velocity_error = delta_position - target_velocity * m_dT;
    m_I += position_error * m_dT;

    float inegral_term = m_I*m_kI;
    if(inegral_term > 1.0){ m_I = 1.0/m_kI; }
    else if(inegral_term < -1.0){ m_I = -1.0/m_kI; }

    float fControl = position_error*m_kP + m_I*m_kI + m_kD*(velocity_error) + acceleration * m_kFF;

    //TODO implement a variable based on error
    //TODO rotation lookup table to replace sin and cos
    //TODO add offset and feed forward
    return ForceFunction(fControl);
    //return RotateUnity(m_Power,error*m_kP + m_I*m_kI);
}

FloatVector CPIControl::Rotate(FloatVector in_vector,float angle_rad)
{
    //float sin_angle = arm_sin_f32(angle_rad);
    //float cos_angle = arm_cos_f32(angle_rad);
    float sin_angle = sinf(angle_rad);
    float cos_angle = cosf(angle_rad);
    FloatVector Result;
    Result.x = (cos_angle * in_vector.x) - (sin_angle * in_vector.y);
    Result.y = (sin_angle * in_vector.x) + (cos_angle * in_vector.y);
    return Result;
}

int CPIControlInt::RunPIControl(unsigned int target_pos,
        unsigned int measured_pos)
{
    int delta_position = (int)measured_pos - (int)m_fLastMeasured;
    m_fLastMeasured = measured_pos;
    int position_error = measured_pos - target_pos;
    //TODO, look into using a filtered version of this to keep the  velocity term active
    //float velocity_error = delta_position - target_velocity * m_dT;
    //this may never happen for now not using the target velocity
    //10 moves per second at 4000 units per move, 40000/100000 still equals zero
    int velocity_error = delta_position;

    m_I += position_error;
    //limit the integral to minimize wind up
    if( m_I > m_maxI )
    {
        m_I = m_maxI;
    }
    else if( m_I < -m_maxI )
    {
        m_I = -m_maxI;
    }

    int control_signal = (m_I * m_kI_num +
            position_error * m_kP_num +
            velocity_error * m_kD_num) / m_gain_den;

    if(control_signal > m_MaxControl)
    {
        control_signal = m_MaxControl;
    }
    else if (control_signal < -m_MaxControl)
    {
        control_signal = -m_MaxControl;
    }
    return control_signal;
}

const unsigned int PWM_Table::power_table[SIZE_OF_POWER_TABLE] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
        50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
        60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
        70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
        80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
        90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
        100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
        110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
        120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
        130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
        140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
        150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
        160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
        170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
        180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
        190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
        200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
        210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
        220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
        230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
        240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
        250, 251, 252, 253, 254, 255, 256, 257, 258, 259,
        260, 261, 262, 263, 264, 265, 266, 267, 268, 269,
        270, 271, 272, 273, 274, 275, 276, 277, 278, 279,
        280, 281, 282, 283, 284, 285, 286, 287, 288, 289,
        290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
        300, 301, 302, 303, 304, 305, 306, 307, 308, 309,
        310, 311, 312, 313, 314, 315, 316, 317, 318, 319,
        320, 321, 322, 323, 324, 325, 326, 327, 328, 329,
        330, 331, 332, 333, 334, 335, 336, 337, 338, 339,
        340, 341, 342, 343, 344, 345, 346, 347, 348, 349,
        350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
        360, 361, 362, 363, 364, 365, 366, 367, 368, 369,
        370, 371, 372, 373, 374, 375, 376, 377, 378, 379,
        380, 381, 382, 383, 384, 385, 386, 387, 388, 389,
        390, 391, 392, 393, 394, 395, 396, 397, 398, 399,
    };
