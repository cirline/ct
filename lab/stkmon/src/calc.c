#define pr_fmt(fmt)	"calcinf ] " fmt

#include <ccutils/log.h>
#include "stkmon/stkmon.h"

int calc_pr_info(struct stk_xmlcfg *sxc)
{
	struct stk_stock *stock;
	struct stk_stkdat *dat;

	pr_info("%8s %8s %12s\n", "code", "chg", "chg_min");
	for(stock = sxc->stock; stock; stock = stock->next) {
		if(!stock->pull_data)
			continue;

		dat = &(((struct stk_stkdat_container *)stock->pull_data)->common);
		pr_info("%8s %8.3f %12.3f\n", stock->code, stock->chg_rate, stock->chg_rate_min);
	}
	pr_info("\n");

	return 0;
}

int calc_realtime_info(struct stk_xmlcfg *sxc)
{
	struct stk_stock *stock;
	struct stk_stkdat *dat;

	for(stock = sxc->stock; stock; stock = stock->next) {
		if(!stock->pull_data)
			continue;

		dat = &(((struct stk_stkdat_container *)stock->pull_data)->common);
		stock->chg_rate = (dat->price - dat->pre_close) / dat->pre_close;
		stock->chg_rate_min = (dat->price - stock->cfg.min_price.f) / stock->cfg.min_price.f;
	}

	return 0;
}
