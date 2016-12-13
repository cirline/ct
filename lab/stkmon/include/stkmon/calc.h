#ifndef __STKM_CALC_H__
#define __STKM_CALC_H__

int calc_pr_info(struct stk_xmlcfg *sxc);
int calc_realtime_info(struct stk_xmlcfg *sxc);
float calc_daily_alert(struct stk_xmlcfg *sxc, struct stk_stock *stock);

#endif
