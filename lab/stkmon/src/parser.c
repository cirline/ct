#define pr_fmt(fmt)	"parser  ] " fmt
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <config.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <ccutils/log.h>

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

int load_configure(xmlNodePtr node, void *data)
{
	struct sm_xmlcfg *p = data;

	pr_debug("-- %s: %s\n", node->name, xmlNodeGetContent(node));
	if(strcmp(node->name, "interval") == 0) {
		strcpy(p->interval, xmlNodeGetContent(node));
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

void print_xmlcfg(struct sm_xmlcfg *smxc)
{
	printf("configure:\n");
	printf(" %12s: %s\n", "interval", smxc->interval);

	struct sm_stock *p = smxc->stock;
	int i = 0;

	pr_info("stock count: %d\n", smxc->stocks_count);
	pr_info("%4s %8s %8s %8s %8s\n", "n", "visible", "code", "stkex", "p_avg");
	while(p) {
		pr_info("%4d %8d %8s %8s %8.2f\n", i++, p->visible, p->code, p->stkex,
				p->avg_price.f);
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

	print_xmlcfg(smxc);

	return 0;
}

