/*
 * BIT_MATH.h
 *
 * Created: 30-Oct-21 1:54:53 AM
 *  Author: hp
 */ 


#ifndef BIT_MATH_H_
#define BIT_MATH_H_

#define SET_BIT(reg,bit)   (reg |= (1<<bit))
#define CLR_BIT(reg,bit)   (reg &= ~(1<<bit))
#define TOG_BIT(reg,bit)   (reg ^= (1<<bit))
#define GET_BIT(REG,BIT)   ((REG >> BIT)&1)



#endif /* BIT_MATH_H_ */