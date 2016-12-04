/*
 * arm_math.h
 *
 *  Created on: Nov 11, 2014
 *      Author: kchristopher
 */

#ifndef SOURCE_ARM_MATH_H_
#define SOURCE_ARM_MATH_H_



#define TABLE_SIZE			256
#define TABLE_SPACING_Q31	0x800000
#define TABLE_SPACING_Q15	0x80


#ifdef __cplusplus
 extern "C" {
#endif

  float arm_cos_f32(
  float x);

  float arm_sin_f32(
  float x);

#ifdef __cplusplus
}
#endif
#endif /* SOURCE_ARM_MATH_H_ */
