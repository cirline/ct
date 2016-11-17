//#define DEBUG
#define pr_fmt(fmt)	"parser  ] " fmt
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <config.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <ccutils/log.h>
#include <ccutils/xml.h>

#include "stkmon/stkmon.h"

#define DATAFILE_PATH	"data.xml"

xmlXPathObjectPtr get_xpath_object(xmlDocPtr docp, unsigned char *path)
{
	xmlChar *xpath;
	xmlXPathContextPtr context;
	xmlXPathObjectPtr object;

	xpath = BAD_CAST(path);
	context = xmlXPathNewContext(docp);
	if(!context) {
		printf("xmlXPathNewContext null\n");
		return NULL;
	}
	object = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if(!object) {
		printf("xmlXPathEvalExpression null\n");
		return NULL;
	}
	if(xmlXPathNodeSetIsEmpty(object->nodesetval)) {
		xmlXPathFreeObject(object);
		printf("xmlXPath nodeset is empty\n");
		return NULL;
	}

	return object;
}

int parse_node(xmlNodePtr node, int (*func)(xmlNodePtr, void *), void *data)
{
	pr_debug("%s\n", __func__);
	while(node) {
		func(node, data);
		node = node->next;
	}

	return 0;
}

int parse_xojbect(xmlXPathObjectPtr object, int (*func)(xmlNodePtr, void *), void *data)
{
	int i;

	for(i < 0; i < object->nodesetval->nodeNr; i++) {
		xmlNodePtr node = object->nodesetval->nodeTab[i];
		parse_node(node->children, func, data);
	}

	return i;
}

int parse_xobjects(xmlXPathObjectPtr object, int (*f)(xmlNodePtr, void *), void *param)
{
	int i;

	for(i < 0; i < object->nodesetval->nodeNr; i++) {
		xmlNodePtr node = object->nodesetval->nodeTab[i];
		parse_node(node, f, param);
	}
}

void load_alert_lv(xmlNodePtr node, char *pname, struct stk_float *data)
{
	char *prop;

	prop = xmlGetProp(node, pname);
	if(prop) {
		strcpy(data->c, prop);
		data->f = atof(prop);
		xmlFree(prop);
	} else {
		*data->c = 0;
		data->f = 0;
	}
}

int load_alert(xmlNodePtr node, void *data)
{
	struct sm_xmlcfg	*p = data;
	struct stk_alert_level	*sal;
	char *prop;

	pr_debug("%s - name: %s\n", __func__, node->name);
	if(strcmp(node->name, "short_term") == 0) {
		sal = &p->alert.short_term;
	} else if(strcmp(node->name, "medium_term") == 0){
		sal = &p->alert.medium_term;
	} else if(strcmp(node->name, "long_term") == 0) {
		sal = &p->alert.long_term;
	} else {
		return 0;
	}
	load_alert_lv(node, "lv1", &sal->lv1);
	load_alert_lv(node, "lv2", &sal->lv2);
	load_alert_lv(node, "lv3", &sal->lv3);

	return 0;
}

int load_configure(xmlNodePtr node, void *data)
{
	struct sm_xmlcfg *p = data;

	pr_debug("%s - %s: %s\n", __func__, node->name, xmlNodeGetContent(node));
	if(strcmp(node->name, "interval") == 0) {
		strcpy(p->interval, xmlNodeGetContent(node));
	} else if(strcmp(node->name, "alert") == 0) {
		parse_node(node->children, load_alert, p);
	}

	return 0;
}

int do_parse_stock(xmlNodePtr node, void *data)
{
	struct sm_stock *p = data;

	pr_debug("%d -- %s: %s\n", __LINE__, node->name, xmlNodeGetContent(node));

	if(strcmp(node->name, "code") == 0) {
		strcpy(p->code, xmlNodeGetContent(node));
	} else if(strcmp(node->name, "stkex") == 0) {
		strcpy(p->stkex, xmlNodeGetContent(node));
	} else if(strcmp(node->name, "last_minprice") == 0) {
		strcpy(p->last_minprice, xmlNodeGetContent(node));
		strcpy(p->cfg.min_price.c, xmlNodeGetContent(node));
		p->cfg.min_price.f = atof(p->cfg.min_price.c);
	} else if(strcmp(node->name, "stop_profit") == 0) {
		strcpy(p->stop_profit, xmlNodeGetContent(node));
	} else if(strcmp(node->name, "stop_loss") == 0) {
		strcpy(p->stop_loss, xmlNodeGetContent(node));
	}

	return 0;
}

int load_xmlstocks(xmlNodePtr node, void *data)
{
	struct sm_xmlcfg *p = data;
	struct sm_stock *ssp;
	char	*prop;

	if(strcmp(node->name, "stock") == 0) {
		ssp = malloc(sizeof(struct sm_stock));
		if(!ssp) {
			printf("%s, malloc failed: %s\n", __func__, strerror(errno));
			return -1;
		}
		parse_node(node->children, do_parse_stock, ssp);

		/* read prop */
		prop = xmlGetProp(node, "visible");
		if(prop && (strcmp(prop, "true") == 0)) {
			ssp->visible = 1;
			xmlFree(prop);
		} else
			ssp->visible = 0;

		prop = xmlGetProp(node, "avg_price");
		if(prop) {
			strcpy(ssp->avg_price.c, prop);
			ssp->avg_price.f = atof(prop);
			xmlFree(prop);
		} else
			ssp->avg_price.f = 0;

		ssp->pull_data = NULL;
		ssp->next = p->stock;
		p->stock = ssp;
		p->stocks_count++;
	}
	return 0;
}

int load_xmlstock(xmlNodePtr node, void *data)
{
	struct stk_xmlcfg *p = data;
	struct stk_stock *stock;
	char	*prop;

	if(strcmp(node->name, "stock") != 0)
		return;

	stock = malloc(sizeof(*stock));
	if(!stock) {
		pr_err("%s malloc failed %s\n", __func__, strerror(errno));
		return -1;
	}
	prop = cxml_get_prop_string(node, "code", NULL, stock->code);
	if(!prop)
		goto out;
	prop = cxml_get_prop_string(node, "exchange", NULL, stock->exchange);
	if(!prop)
		goto out;

	stock->visible = cxml_get_prop_bool(node, "visible", 0);
	cxml_get_prop_string(node, "avg_price", "0", stock->cfg.avg_price.c);
	stock->cfg.avg_price.f = atof(stock->cfg.avg_price.c);
	cxml_get_prop_string(node, "min_price", "0", stock->cfg.min_price.c);
	stock->cfg.min_price.f = atof(stock->cfg.min_price.c);
	cxml_get_prop_string(node, "aim_price", "0", stock->cfg.aim_price.c);
	stock->cfg.aim_price.f = atof(stock->cfg.aim_price.c);

	cxml_get_prop_string(node, "stop_profit", "0", stock->cfg.stop_profit.c);
	stock->cfg.stop_profit.f = atof(stock->cfg.stop_profit.c);
	cxml_get_prop_string(node, "stop_loss", "0", stock->cfg.stop_loss.c);
	stock->cfg.stop_loss.f = atof(stock->cfg.stop_loss.c);

	stock->pull_data = NULL;
	stock->next = p->stock_list;
	p->stock_list = stock;
	p->stock_count++;

	return 0;
out:
	free(stock);
	return -1;
}

void print_xmlcfg(struct sm_xmlcfg *smxc)
{
	struct stk_alert_level *sal;

	printf("configure:\n");
	printf(" %12s: %s\n", "interval", smxc->interval);

	sal = &smxc->alert.short_term;
	pr_info("short alert:\n");
	pr_info("%8.2f, %8.2f, %8.2f\n", sal->lv1.f, sal->lv2.f, sal->lv3.f);
	sal = &smxc->alert.medium_term;
	pr_info("medium alert:\n");
	pr_info("%8.2f, %8.2f, %8.2f\n", sal->lv1.f, sal->lv2.f, sal->lv3.f);
	sal = &smxc->alert.long_term;
	pr_info("long alert:\n");
	pr_info("%8.2f, %8.2f, %8.2f\n", sal->lv1.f, sal->lv2.f, sal->lv3.f);

	struct sm_stock *p = smxc->stock;
	int i = 0;
	pr_info("stock count: %d\n", smxc->stocks_count);
	pr_info("%4s %8s %8s %8s %8s\n", "n", "visible", "code", "stkex", "p_avg");
	while(p) {
		pr_info("%4d %8d %8s %8s %8.2f\n", i++, p->visible, p->code, p->stkex,
				p->avg_price.f);
		p = p->next;
	}

	p = smxc->stock_list;
	i = 0;
	pr_info("stock_count: %d\n", smxc->stock_count);
	pr_info("%4s %8s %8s %8s %8s %8s %8s\n", "n", "visible", "code", "stkex", "p_avg", "p_min", "p_aim");
	while(p) {
		pr_info("%4d %8d %8s %8s %8.2f %8.2f %8.2f\n", i++, p->visible, p->code, p->exchange,
				p->cfg.avg_price.f, p->cfg.min_price.f, p->cfg.aim_price.f);
		p = p->next;
	}
}

int do_save_xmlconfig(xmlNodePtr node, void *p)
{
	struct sm_xmlcfg *smxc = (struct sm_xmlcfg *)p;
	if(strcmp(node->name, "interval") == 0) {
		//xmlNodeSetContent(node, smxc->delay_ms);
	}

	return 0;
}

int save_xmlconfig(struct sm_xmlcfg *smxc)
{
	xmlDocPtr docp;
	xmlXPathObjectPtr object;
	unsigned char *cfg_path = "/root/configure";

	docp = xmlParseFile(DATAFILE_PATH);
	if(!docp) {
		printf("%d, parse error %s\n", __LINE__, strerror(errno));
		return -1;
	}

	object = get_xpath_object(docp, cfg_path);
	if(object) {
		parse_xojbect(object, do_save_xmlconfig, smxc);
		xmlXPathFreeObject(object);
	}

	xmlSaveFile(DATAFILE_PATH, docp);
	xmlFreeDoc(docp);

	return 0;
}

int load_xmlconfig(struct sm_xmlcfg *smxc)
{
	xmlDocPtr docp;
	xmlXPathObjectPtr object;
	unsigned char *xpath_stock = "/root/stock";

	docp = xmlParseFile(DATAFILE_PATH);
	if(!docp) {
		printf("%d, parse error %s\n", __LINE__, strerror(errno));
		return -1;
	}

	/* parse configure */
	unsigned char *cfg_path = "/root/configure";
	object = get_xpath_object(docp, cfg_path);
	if(object) {
		parse_xojbect(object, load_configure, smxc);
		xmlXPathFreeObject(object);
	}

	smxc->stock = NULL;
	smxc->stocks_count = 0;
	unsigned char *stocks_path = "/root/stocks";
	object = get_xpath_object(docp, stocks_path);
	if(object) {
		parse_xojbect(object, load_xmlstocks, smxc);
		xmlXPathFreeObject(object);
	}

	xmlFreeDoc(docp);

	pr_info("----- 1\n");
	docp = xmlParseFile(top_datadir "/stocks.xml");
	if(!docp) {
		pr_err("open " top_datadir "/stocks.xml failed: %s\n", strerror(errno));
		return -1;
	}
	smxc->stock_list = NULL;
	smxc->stock_count = 0;
	object = get_xpath_object(docp, xpath_stock);
	if(object) {
		parse_xobjects(object, load_xmlstock, smxc);
		xmlXPathFreeObject(object);
	}
	xmlFreeDoc(docp);
	pr_info("----- 2\n");

	print_xmlcfg(smxc);

	return 0;
}

