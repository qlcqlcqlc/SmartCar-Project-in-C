/*
 * VadcApp.h
 *
 *  Created on: 2017��12��8��
 *      Author: Administrator
 */

#ifndef VADCAPP_H_
#define VADCAPP_H_
#include "Vadc/Adc/IfxVadc_Adc.h"
#include "IfxVadc_regdef.h"
void VADC_init(void);
extern void VADCresult_run(void);//��ȡAD���
extern unsigned int  VADCresult[9];//AD����洢λ�� 0.0---0.8 AN0....AN8
#endif /* 0_SRC_0_APPSW_TRICORE_API_VADC_VADCAPP_H_ */
