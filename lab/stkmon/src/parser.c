#define pr_fmt(fmt)	"parser  - "
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <config.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <ccutils/log.h>

#include <stkmon.h>

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

int parse_configure(xmlNodePtr node, void *data)
{
	struct sm_desc *p = data;

	pr_debug("-- %s: %s\n", node->name, xmlNodeGetContent(node));
	if(strcmp(node->name, "delay_ms") == 0) {
		p->cfg.delay_ms = atoi(xmlNodeGetContent(node));
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
	}

	return 0;
}

int parse_stocks(xmlNodePtr node, void *data)
{
	struct sm_desc *p = data;
	struct sm_stock *ssp;

	if(strcmp(node->name, "stock") == 0) {
		ssp = malloc(sizeof(struct sm_stock));
		if(!ssp) {
			printf("%s, malloc failed: %s\n", __func__, strerror(errno));
			return -1;
		}
		parse_node(node->children, do_parse_stock, ssp);

		ssp->next = p->stock;
		p->stock = ssp;
	}
	return 0;
}

void print_configure(struct sm_desc *desc)
{
	printf("configure:\n");
	printf(" %12s: %d\n", "delay_ms", desc->cfg.delay_ms);

	struct sm_stock *p = desc->stock;
	int i = 0;
	while(p) {
		printf("stock(%d):\n", i++);
		printf(" %12s: %s\n", "code", p->code);
		printf(" %12s: %s\n", "stkex", p->stkex);

		p = p->next;
	}
}

void check_configure(struct sm_desc *desc)
{
	if(desc->cfg.delay_ms <= 0)
		desc->cfg.delay_ms = SM_DEFAULT_DELAY_MS;
}

int parse_xmlconfig(struct sm_desc *desc)
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
		parse_xojbect(object, parse_configure, desc);
		xmlXPathFreeObject(object);
	}

	desc->stock = NULL;
	unsigned char *stocks_path = "/root/stocks";
	object = get_xpath_object(docp, stocks_path);
	if(object) {
		parse_xojbect(object, parse_stocks, desc);
		xmlXPathFreeObject(object);
	}

	xmlFreeDoc(docp);

	check_configure(desc);
	print_configure(desc);

	return 0;
}

int do_save_xmlconfig(xmlNodePtr node, void *p)
{
	struct sm_xmlcfg *smxc = (struct sm_xmlcfg *)p;
	if(strcmp(node->name, "delay_ms") == 0) {
		printf("save: delay_ms = %s\n", smxc->delay_ms);
		xmlNodeSetContent(node, smxc->delay_ms);
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

