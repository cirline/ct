
#define pr_fmt(fmt)	"sinajs  ] " fmt

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/queue.h>
#include <ccutils/log.h>
#include <ccutils/net.h>
#include <ccutils/string.h>

#include "stkmon/stkmon.h"
#include "geye/sinajs.h"
#include "geye/ge.h"

/*
 * var hq_str_sh601668="prop1,prop2,...";
 */
#define sinajs_decode_debug(format, ...)//	pr_info("%s, %d, "format, __func__, __LINE__, ##__VA_ARGS__)
int sinajs_stock_decode(char *buffer, struct sinajs_stock *sjs)
{
	struct ge_stkdat *sj;
	char *sptr1, *sptr2;
	char *token;
	char *p;
	int i;
	int rc;
	int inb_len;
	int outb_len;

	if(!buffer)
		return -1;
	sinajs_decode_debug("buffer = %s\n", buffer);

	memset(sjs, 0, sizeof(*sjs));
	sj = GE_GET_STKDAT(sjs);

	/* decode variable */
	token = strtok_r(buffer, "\"", &sptr1);		/* split variable and value */
	for(p = token; *p && *p != '='; p++)
		;
	*p = 0;						/* variable '=' --> 0 */
	sinajs_decode_debug("1, token = %s\n", token);
	rc = sscanf(token, "var hq_str_%s", sj->code);
	if(rc <= 0) {
		pr_err("not found code (%s)\n", token);
		return -1;
	}
	sinajs_decode_debug("sj->code = %s\n", sj->code);

	/* decode value */
	token = strtok_r(NULL, "\"", &sptr1);		/* split end '"' and newline symbol */

	/* prop is split by ',' */
	token = strtok_r(token, ",", &sptr2);
	sinajs_decode_debug("3, token = %s\n", token);
	inb_len = strlen(token);
	outb_len = STK_NAME_SZ;
	convert_charset("UTF-8", "GB18030", sj->name, outb_len, token, inb_len);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("4, token = %s\n", token);
	sj->open = atof(token);
	sinajs_decode_debug("sj->open = %f\n", sj->open);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("5, token = %s\n", token);
	sj->pre_close = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("6, token = %s\n", token);
	sj->price = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("7, token = %s\n", token);
	sj->high = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("8, token = %s\n", token);
	sj->low = atof(token);

	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("9, token = %s\n", token);
	sj->bid = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("10, token = %s\n", token);
	sj->ask = atof(token);

	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("11, token = %s\n", token);
	sj->volume = atol(token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("12, token = %s\n", token);
	sj->amount = atol(token);

	for(i = 0; i < 5; i++) {
		token = strtok_r(NULL, ",", &sptr2);
		sinajs_decode_debug("13, token = %s\n", token);
		sj->bv[i] = atoi(token);
		token = strtok_r(NULL, ",", &sptr2);
		sinajs_decode_debug("14, token = %s\n", token);
		sj->bp[i] = atof(token);
	}

	for(i = 0; i < 5; i++) {
		token = strtok_r(NULL, ",", &sptr2);
		sinajs_decode_debug("15, token = %s\n", token);
		sj->av[i] = atoi(token);
		token = strtok_r(NULL, ",", &sptr2);
		sinajs_decode_debug("16, token = %s\n", token);
		sj->ap[i] = atof(token);
	}

	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("17, token = %s\n", token);
	strcpy(sj->date, token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("18, token = %s\n", token);
	strcpy(sj->time, token);

	return 0;
}

int sinajs_decode(char *buffer, struct sinajs *sj)
{
	char *sptr1, *sptr2;
	char *token;
	char *p;
	int i;
	int rc;
	int inb_len;
	int outb_len;

	if(!buffer)
		return -1;
	sinajs_decode_debug("buffer = %s\n", buffer);

	memset(sj, 0, sizeof(*sj));
	/* decode variable */
	token = strtok_r(buffer, "\"", &sptr1);		/* split variable and value */
	for(p = token; *p && *p != '='; p++)
		;
	*p = 0;						/* variable '=' --> 0 */
	sinajs_decode_debug("1, token = %s\n", token);
	rc = sscanf(token, "var hq_str_%s", sj->code);
	if(rc <= 0) {
		pr_err("not found code (%s)\n", token);
		return -1;
	}
	sinajs_decode_debug("sj->code = %s\n", sj->code);

	/* decode value */
	token = strtok_r(NULL, "\"", &sptr1);		/* split end '"' and newline symbol */

	/* prop is split by ',' */
	token = strtok_r(token, ",", &sptr2);
	sinajs_decode_debug("3, token = %s\n", token);
	inb_len = strlen(token);
	outb_len = STK_NAME_SZ;
	convert_charset("UTF-8", "GB18030", sj->common.name, outb_len, token, inb_len);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("4, token = %s\n", token);
	sj->open = atof(token);
	sinajs_decode_debug("sj->open = %f\n", sj->open);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("5, token = %s\n", token);
	sj->pre_close = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("6, token = %s\n", token);
	sj->price = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("7, token = %s\n", token);
	sj->high = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("8, token = %s\n", token);
	sj->low = atof(token);

	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("9, token = %s\n", token);
	sj->bid = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("10, token = %s\n", token);
	sj->ask = atof(token);

	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("11, token = %s\n", token);
	sj->volume = atol(token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("12, token = %s\n", token);
	sj->amount = atol(token);

	for(i = 0; i < 5; i++) {
		token = strtok_r(NULL, ",", &sptr2);
		sinajs_decode_debug("13, token = %s\n", token);
		sj->bv[i] = atoi(token);
		token = strtok_r(NULL, ",", &sptr2);
		sinajs_decode_debug("14, token = %s\n", token);
		sj->bp[i] = atof(token);
	}

	for(i = 0; i < 5; i++) {
		token = strtok_r(NULL, ",", &sptr2);
		sinajs_decode_debug("15, token = %s\n", token);
		sj->av[i] = atoi(token);
		token = strtok_r(NULL, ",", &sptr2);
		sinajs_decode_debug("16, token = %s\n", token);
		sj->ap[i] = atof(token);
	}

	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("17, token = %s\n", token);
	strcpy(sj->date, token);
	token = strtok_r(NULL, ",", &sptr2);
	sinajs_decode_debug("18, token = %s\n", token);
	strcpy(sj->time, token);

	return 0;
}


void sinajs_print(struct sinajs *sj)
{
	int i;

	if(!sj) {
		pr_info("sj = null\n");
		return;
	}

	printf("code: %s\n", sj->code);
	printf("open: %f\n", sj->open);
	printf("pre_close: %f\n", sj->pre_close);
	printf("price: %f\n", sj->price);
	printf("high: %f\n", sj->high);
	printf("low: %f\n", sj->low);
	printf("bid: %f\n", sj->bid);
	printf("ask: %f\n", sj->ask);
	printf("volume: %ld\n", sj->volume);
	printf("amount: %ld\n", sj->amount);
	for(i = 0; i < 5; i++) {
		printf("bv%d: %d\n", i, sj->bv[i]);
		printf("bp%d: %f\n", i, sj->bp[i]);
		printf("av%d: %d\n", i, sj->av[i]);
		printf("ap%d: %f\n", i, sj->ap[i]);
	}
	printf("date: %s\n", sj->date);
	printf("time: %s\n\n", sj->time);
}

static int sinajs_index_attach(struct sstkmon *ss, struct sinajs_index *index)
{
	struct stk_index *si;

	for(si = ss->index_list.cqh_first; si != (void *)&ss->index_list;
			si = si->list.cqe_next) {

		if(strcmp(si->data.code, index->common.data.code) == 0)
			break;
	}

	if(si == (void *)&ss->index_list)
		return - ENODATA;

	memcpy(&si->data, &index->common.data, sizeof(si->data));

	return 0;
}

static int sinajs_index_attach_v2(struct golden_eye_2 *ge, struct sinajs_index *index)
{
	struct ge_index *gi;

	for(gi = ge->index_list.cqh_first; gi != (void *)&ge->index_list;
			gi = gi->list.cqe_next) {

		if(strcmp(gi->data.code, index->common.data.code) == 0)
			break;
	}

	if(gi == (void *)&ge->index_list)
		return - ENODATA;

	memcpy(&gi->data, &index->common.data, sizeof(gi->data));

	return 0;
}

#undef sinajs_decode_debug
#define sinajs_decode_debug(format, ...)	//pr_info("%s, %d, "format, __func__, __LINE__, ##__VA_ARGS__)
int sinajs_index_decode(char *pbuf, struct sinajs_index *index)
{
	char *sptr1, *sptr2;
	char *token;
	char *p;
	int i;
	int rc;
	int inb_len;
	int outb_len;
	struct ge_idxdat *idxd = &index->common.data;

	if(!pbuf)
		return - EINVAL;
	sinajs_decode_debug("buffer = %s\n", pbuf);

	memset(index, 0, sizeof(*index));
	/* decode variable */
	token = strtok_r(pbuf, "\"", &sptr1);		/* split variable and value */
	for(p = token; *p && *p != '='; p++)
		;
	*p = 0;						/* variable '=' --> 0 */
	sinajs_decode_debug("1, token = %s\n", token);
	rc = sscanf(token, "var hq_str_%s", idxd->code);
	if(rc <= 0) {
		pr_err("not found index code (%s)\n", token);
		return -1;
	}
	sinajs_decode_debug("code = %s\n", idxd->code);

	/* decode value */
	token = strtok_r(NULL, "\"", &sptr1);		/* split end '"' and newline symbol */

	/* prop is split by ',' */
	/* name */
	token = strtok_r(token, ",", &sptr2);
	sinajs_decode_debug("3, token = %s\n", token);
	inb_len = strlen(token);
	outb_len = STK_NAME_SZ;
	convert_charset("UTF-8", "GB18030", idxd->name, outb_len, token, inb_len);
	/* index */
	cstr_token(token, NULL, ",", &sptr2, atof, idxd->index);
	sinajs_decode_debug("4, token = %s\n", token);
	/* diff */
	cstr_token(token, NULL, ",", &sptr2, atof, idxd->diff);
	sinajs_decode_debug("5, token = %s\n", token);
	/* roc */
	cstr_token(token, NULL, ",", &sptr2, atof, idxd->roc);
	sinajs_decode_debug("6, token = %s\n", token);
	/* volume */
	cstr_token(token, NULL, ",", &sptr2, atol, idxd->volume);
	sinajs_decode_debug("7, token = %s\n", token);
	/* amount */
	cstr_token(token, NULL, ",", &sptr2, atol, idxd->amount);
	sinajs_decode_debug("8, token = %s\n", token);

	return 0;
}

int sinajs_build_index_url(char *url, struct golden_eye_2 *ge)
{
	return 0;
}

int sinajs_pull_index_data(struct sstkmon *ss)
{
	char url[1024] = "hq.sinajs.cn/list=s_sh000001";
	char buffer[4096];
	int rc;
	char *token, *sptr1;

	pr_info("index url: %s\n", url);
	rc = http_get(url, buffer);
	if(rc < 0) {
		pr_err("http get fail, %s\n", url);
		return - EINVAL;
	}
	buffer[rc] = 0;

	for(token = strtok_r(buffer, "\n", &sptr1); token;
			token = strtok_r(NULL, "\n", &sptr1)) {
		struct sinajs_index index;

		if(sinajs_index_decode(token, &index) < 0) {
			pr_err("%s, index decode fail\n", __func__);
			return - EINVAL;
		}

		/* find index and attach data to pull_data */
		rc = sinajs_index_attach(ss, &index);
		if(rc < 0) {
			pr_err("%s, attach fail, %d\n", __func__, rc);
			return rc;
		}
	}

	return 0;
}

int sinajs_pull_index_data_v2(struct golden_eye_2 *ge)
{
	char url[1024] = "hq.sinajs.cn/list=s_sh000001";
	char buffer[4096];
	int rc;
	char *token, *sptr1;

	if(sinajs_build_index_url(url, ge) < 0) {
		pr_err("%s, build index url fail\n", __func__);
		return - EINVAL;
	}

	pr_info("%s: index url: %s\n", __func__, url);
	rc = http_get(url, buffer);
	if(rc < 0) {
		pr_err("http get fail, %s\n", url);
		return - EINVAL;
	}
	buffer[rc] = 0;

	for(token = strtok_r(buffer, "\n", &sptr1); token;
			token = strtok_r(NULL, "\n", &sptr1)) {
		struct sinajs_index index;

		if(sinajs_index_decode(token, &index) < 0) {
			pr_err("%s, index decode fail\n", __func__);
			return - EINVAL;
		}

		/* find index and attach data to pull_data */
		rc = sinajs_index_attach_v2(ge, &index);
		if(rc < 0) {
			pr_err("%s, attach fail, %d\n", __func__, rc);
			return rc;
		}
	}

	return 0;
}

int sinajs_pull_stock_data(struct golden_eye_2 *ge)
{
	struct ge_stock *stock;
	char slist[1024] = "hq.sinajs.cn/list=";
	char buffer[4096];
	char *sptr1, *token;
	int rc;

	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		if(stock->cfg.visible) {
			strcat(slist, stock->exchange);
			strcat(slist, stock->code);
			strcat(slist, ",");
		}
	}

	pr_info("%s, slist url = %s\n", __func__, slist);
	rc = http_get(slist, buffer);
	if(rc < 0) {
		pr_err("%s, http get fail\n", __func__);
		return -1;
	}
	buffer[rc] = 0;

	token = strtok_r(buffer, "\n", &sptr1);
	for(rc = 0; token; rc++) {
		struct sinajs_stock sdata;

		rc = sinajs_stock_decode(token, &sdata);
		do {
			if(rc < 0) {
				pr_warn("decode failed\n");
				return - EINVAL;
				//break;
			}

			/* find stock and attach data to pull_data */
			for(stock = ge->stock_list.cqh_first;
					stock != (void *)&ge->stock_list;
					stock = stock->list.cqe_next) {
				char local[20];

				sprintf(local, "%s%s", stock->exchange, stock->code);
				if(strcmp(local, sdata.common.code) == 0)
					break;
			}
			if(!stock)
				break;

			memcpy(&stock->data, &sdata, sizeof(stock->data));
		} while(0);

		token = strtok_r(NULL, "\n", &sptr1);
	}

	return 0;
}

int sinajs_pull_data(struct stk_xmlcfg *sxc)
{
	struct sm_stock *stock;
	char slist[1024] = "hq.sinajs.cn/list=";
	char buffer[4096];
	char *sptr1, *token;
	int rc;

	for(stock = sxc->stock_list.cqh_first; stock != (void *)&sxc->stock_list;
			stock = stock->list.cqe_next) {
//		pr_info("stock: %s%s\n", stock->stkex, stock->code);
		if(stock->visible) {
			strcat(slist, stock->exchange);
			strcat(slist, stock->code);
			strcat(slist, ",");
		}
	}

	pr_info("slist url = %s\n", slist);
	rc = http_get(slist, buffer);
	if(rc < 0) {
		pr_err("http get failed.\n");
		return -1;
	}
	buffer[rc] = 0;

	token = strtok_r(buffer, "\n", &sptr1);
	for(rc = 0; token; rc++) {
		struct sinajs sdata;

		rc = sinajs_decode(token, &sdata);
		do {
			if(rc < 0) {
				pr_warn("decode failed\n");
				return -1;
				//break;
			}

			/* find stock and attach data to pull_data */
			for(stock = sxc->stock_list.cqh_first; stock != (void *)&sxc->stock_list;
					stock = stock->list.cqe_next) {
				char local[20];

				sprintf(local, "%s%s", stock->exchange, stock->code);
				if(strcmp(local, sdata.code) == 0) {
					break;
				}
			}
			if(!stock)
				break;

			if(!stock->pull_data)
				stock->pull_data = malloc(sizeof(struct sinajs));
			if(!stock->pull_data) {
				pr_err("malloc failed: %s\n", strerror(errno));
				break;
			}
			memcpy(stock->pull_data, &sdata, sizeof(struct sinajs));
			//sinajs_print(stock->pull_data);
		} while(0);

		token = strtok_r(NULL, "\n", &sptr1);
	}

	return 0;
}

int sinajs_apply_data(struct stk_xmlcfg *sxc)
{
	struct stk_stock *stock;
	struct sinajs *js;

	for(stock = sxc->stock_list.cqh_first; stock != (void *)&sxc->stock_list;
			stock = stock->list.cqe_next) {
		if(!stock->pull_data)
			continue;

		js = stock->pull_data;
		js->common.price = js->price;
		js->common.pre_close = js->pre_close;
	}

	return 0;
}

