#define pr_fmt(fmt)	"calcinf ] " fmt

#include <ccutils/log.h>
#include "stkmon/stkmon.h"
#include "geye/ge.h"

int calc_pr_info(struct stk_xmlcfg *sxc)
{
	struct stk_stock *stock;
	struct stk_stkdat *dat;
	struct ge_index *idx;
	struct ge_idxdat *idxd;

	pr_info("%8s %8s %12s %8s\n", "code", "chg", "chg_min", "name");
	for(stock = sxc->stock_list.cqh_first; stock != (void *)&sxc->stock_list;
			stock = stock->list.cqe_next) {
		if(!stock->pull_data)
			continue;

		dat = &(((struct stk_stkdat_container *)stock->pull_data)->common);
		pr_info("%8s %8.3f %12.3f\n", stock->code, stock->chg_rate, stock->chg_rate_min);
	}
	pr_info("\n");
	pr_info("%16s %8s %12s %8s\n", "code", "roc", "diff", "name");
	for(idx = sxc->index_list.cqh_first; idx != (void *)&sxc->index_list;
			idx = idx->list.cqe_next) {

		idxd = &idx->data;
		pr_info("%16s %8.3f %12.2f\n", idxd->code, idxd->roc, idxd->diff);
	}
	pr_info("\n");

	return 0;
}

int calc_print_info(struct golden_eye_2 *ge)
{
	struct ge_stock *stock;
	struct ge_stkdat *dat;
	struct ge_index *idx;
	struct ge_idxdat *idxd;

	pr_info("%8s %8s %8s %12s %8s\n", "code", "price", "chg", "chg_min", "name");
	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		dat = &stock->data;
		pr_info("%8s %8.2f %8.3f %12.3f\n",
				stock->code, dat->price, stock->roc, stock->mproc);
	}
	pr_info("\n");

	pr_info("%16s %8s %8s %12s %8s\n", "index", "code", "roc", "diff", "name");
	for(idx = ge->index_list.cqh_first; idx != (void *)&ge->index_list;
			idx = idx->list.cqe_next) {

		idxd = &idx->data;
		pr_info("%16s %8.2f %8.3f %12.2f\n",
				idxd->code, idxd->index, idxd->roc, idxd->diff);
	}
	pr_info("\n");

	return 0;
}

int calc_realtime_info_v2(struct golden_eye_2 *ge)
{
	struct ge_stock *stock;
	struct ge_stkdat *dat;

	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		dat = &stock->data;
		stock->roc = (dat->price - dat->pre_close) / dat->pre_close;
		stock->mproc = (dat->price - stock->cfg.min_price) / stock->cfg.min_price;
	}

	return 0;
}

int calc_realtime_info(struct stk_xmlcfg *sxc)
{
	struct stk_stock *stock;
	struct stk_stkdat *dat;

	for(stock = sxc->stock_list.cqh_first; stock != (void *)&sxc->stock_list;
			stock = stock->list.cqe_next) {
		if(!stock->pull_data)
			continue;

		dat = &(((struct stk_stkdat_container *)stock->pull_data)->common);
		stock->chg_rate = (dat->price - dat->pre_close) / dat->pre_close;
		stock->chg_rate_min = (dat->price - stock->cfg.min_price.f) / stock->cfg.min_price.f;
	}

	return 0;
}

static float calc_alert(struct stk_alert_level *alert_lv, int chg_rate)
{
	if(chg_rate > alert_lv->lv3.f)
		return -1;
	else if(chg_rate > alert_lv->lv2.f)
		return alert_lv->lv2.f;
	else if(chg_rate > alert_lv->lv1.f)
		return alert_lv->lv1.f;
	else
		return 0;
}

float calc_daily_alert(struct stk_xmlcfg *sxc, struct stk_stock *stock)
{
	struct stk_alert *alert = &sxc->alert;
	int rate;

	if(stock->chg_rate_alert < 0)
		return 0;

	if(stock->chg_rate <= stock->chg_rate_alert)
		return 0;

	rate = calc_alert(&alert->short_term, stock->chg_rate);
	if(rate)
		stock->chg_rate_alert = rate;

	return rate;
}

