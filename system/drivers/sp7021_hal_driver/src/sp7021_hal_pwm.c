#include "sp7021_hal_pwm.h"

#ifdef PWM_DEBUG
#define pwm_printf	printf
#else
#define pwm_printf(s ...)
#endif

static void __PWM_Register_Init(void)
{
	static int init_flag = 0;

	if(init_flag == 0)
	{
		/* disable pwm */
		PWM_CTRL_REG->grp244_0 = 0x0000;
		/* disable pwm duty divisor */
		PWM_CTRL_REG->grp244_1 = 0x0f00;
		PWM_CTRL_REG->pwm_dd[0].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_dd[1].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_dd[2].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_dd[3].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_du[0].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_du[1].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_du[2].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_du[3].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_du[4].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_du[5].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_du[6].idx_all = 0x0000;
		PWM_CTRL_REG->pwm_du[7].idx_all = 0x0000;

		init_flag = 1;
	}
}

static int __Get_available_new_dd(int pwm_num,uint32_t dd_freq)
{
    int i;
    uint32_t tmp2;
  	int dd_sel_new = ePWM_DD_MAX,dd_sel_old = ePWM_DD_MAX;

  	if (PWM_CTRL_REG->grp244_1 & (1 << pwm_num))
		dd_sel_old = PWM_CTRL_REG->pwm_du[pwm_num].pwm_du_dd_sel;
	else
		dd_sel_old = ePWM_DD_MAX;
	/* find the same freq and turnon clk source */
	for (i = 0; i < ePWM_DD_MAX; ++i) {
		if ((PWM_CTRL_REG->grp244_1 & (1 << i))
			&& (PWM_CTRL_REG->pwm_dd[i].dd == dd_freq))
			break;
	}
	if (i != ePWM_DD_MAX)
		dd_sel_new = i;

	/* dd_sel only myself used */
	if (dd_sel_new == ePWM_DD_MAX) {
		for (i = 0; i < PWM_MAX; ++i) {
			if (i == pwm_num)
				continue;

			tmp2 = PWM_CTRL_REG->pwm_du[i].pwm_du_dd_sel;
			if ((PWM_CTRL_REG->grp244_1 & (1 << i))
					&& (tmp2 == dd_sel_old))
				break;
		}
		if (i == PWM_MAX)
			dd_sel_new = dd_sel_old;
	}

	/* find unused clk source */
	if (dd_sel_new == ePWM_DD_MAX) {
		for (i = 0; i < ePWM_DD_MAX; ++i) {
			if (!(PWM_CTRL_REG->grp244_1 & (1 << i)))
				break;
		}
		dd_sel_new = i;
	}

	if (dd_sel_new == ePWM_DD_MAX) {
		return -1;
	}

	if ((dd_sel_old != dd_sel_new) && (dd_sel_old != ePWM_DD_MAX))
		PWM_CTRL_REG->grp244_1 &= ~(1 << dd_sel_old);

	return dd_sel_new;
}

/*frequency = (1 / sys clk * PWM DD0(16bit)) * 256    freq = 12Hz~791Khz*/
static int __PWM_Set_by_period(int pwm_num,uint32_t freq,uint32_t duty_cycle)
{
	uint32_t dd_sel_new = ePWM_DD_MAX;
	uint32_t duty = 0, dd_freq = 0;
	uint32_t period_ns,duty_ns;
	uint64_t tmp;

	/* get period and duty*/
	period_ns = 1000000000/freq;
	duty_ns = period_ns*duty_cycle/100;

	tmp = 202500000 * (uint64_t)period_ns+(256000000000ULL >> 1);
	dd_freq = (uint32_t)(tmp/256000000000ULL);

	pwm_printf("set pwm:%d period_ns %d source clk rate:%llu duty_freq:0x%x(%d)\n",pwm_num, period_ns,tmp, dd_freq, dd_freq);

	if (dd_freq == 0)
		return -1;

	dd_sel_new = __Get_available_new_dd(pwm_num,dd_freq);

	if(dd_sel_new == -1){
		pwm_printf("pwm%d Can't found clk source[0x%x(%d)/256].\n",pwm_num, dd_freq, dd_freq);
		return -1;
	}

	PWM_CTRL_REG->grp244_1 |= (1 << dd_sel_new);
	PWM_CTRL_REG->pwm_dd[dd_sel_new].dd = dd_freq;

	pwm_printf("%s:%d found clk source:%d and set [0x%x(%d)/256]\n",__func__, __LINE__, dd_sel_new, dd_freq, dd_freq);

	if (duty_ns == period_ns) {
		PWM_CTRL_REG->pwm_bypass |= (1 << pwm_num);
		pwm_printf("%s:%d set pwm:%d bypass duty\n",__func__, __LINE__, pwm_num);
	} else {
		PWM_CTRL_REG->pwm_bypass &= ~(1 << pwm_num);

		tmp = (uint64_t)duty_ns * 256 + (period_ns >> 1);
		duty = (uint32_t)(tmp/(uint32_t)period_ns);
		pwm_printf("%s:%d set pwm:%d duty:0x%x(%d)\n",__func__, __LINE__, pwm_num, duty, duty);

		PWM_CTRL_REG->pwm_du[pwm_num].pwm_du = duty;
		PWM_CTRL_REG->pwm_du[pwm_num].pwm_du_dd_sel = dd_sel_new;
	}

	pwm_printf("%s:%d pwm:%d, output freq:%lu Hz, duty:%u \n\n",__func__, __LINE__,
			pwm_num, 202500000 / (dd_freq * 256),(duty * 100) / 256);

	return 0;
}

int HAL_PWM_INIT(PWM_InitTypeDef *PWM_Init)
{

	pwm_assert_param(PWM_Init);
	pwm_assert_param(PWM_Init->pwm_freq);
	pwm_assert_param(IS_PWM_NUM_VALID(PWM_Init->pwm_num));
	pwm_assert_param(IS_PWM_DUTY_VALID(PWM_Init->duty_cycle));
	
	PWM_Init->Pin = GPIO_TO_PINMUX(PWM_Init->Pin);
	if(IS_VALID_PINMUX(PWM_Init->Pin) == 0)
	{
		return -1;
	}
	/* set pwm pinmux */
	HAL_PINMUX_Cfg((PINMUX_PWM0+PWM_Init->pwm_num),PWM_Init->Pin);

	/* only init the pwm ctrl regster once */
	__PWM_Register_Init();

	return __PWM_Set_by_period(PWM_Init->pwm_num,PWM_Init->pwm_freq,PWM_Init->duty_cycle);
}


void HAL_PWM_ENABLE(int pwm_num)
{
	pwm_assert_param(IS_PWM_NUM_VALID(pwm_num));

	PWM_CTRL_REG->pwm_bypass &= ~(1<<pwm_num); //bypass disable
	PWM_CTRL_REG->pwm_en |= 1<<pwm_num; 	   // set pwm enable
}

void HAL_PWM_DISABLE(int pwm_num)
{
	pwm_assert_param(IS_PWM_NUM_VALID(pwm_num));

	PWM_CTRL_REG->pwm_bypass &= ~(1<<pwm_num);	//bypass disable
	PWM_CTRL_REG->pwm_en &= ~(1<<pwm_num); 		//  pwm  disable
}
