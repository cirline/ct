#include <string.h>
#include <libxml/xpath.h>

#include "ccutils/log.h"

xmlXPathObjectPtr get_xpath_object(xmlDocPtr doc, unsigned char *path)
{
	xmlChar *xpath;
	xmlXPathObjectPtr object;
	xmlXPathContextPtr context;

	xpath = BAD_CAST(path);
	context = xmlXPathNewContext(doc);
	if(!context) {
		pr_err("xmlXPathNewContext = null\n");
		return NULL;
	}

	object = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if(!object) {
		pr_err("xmlXPathEvalExpression = null\n");
		return NULL;
	}
	if(xmlXPathNodeSetIsEmpty(object->nodesetval)) {
		xmlXPathFreeObject(object);
		pr_err("xpath object nodeset is empty\n");
		return NULL;
	}

	return object;
}

int parse_xpath_object(xmlXPathObjectPtr object, int (*func)(xmlNodePtr, void *), void *data)
{
	int i;

	for(i = 0; i < object->nodesetval->nodeNr; i++) {
		xmlNodePtr node;

		node = object->nodesetval->nodeTab[i];
		func(node, data);
	}

	return i;
}

int cxml_get_prop_bool(xmlNodePtr node, const char *name, int default_value)
{
	char *prop;

	prop = xmlGetProp(node, name);
	if(prop) {
		default_value = (strcmp(prop, "true") == 0);
		xmlFree(prop);
	}

	return default_value;
}

int cxml_get_prop_integer(xmlNodePtr node, const char *name, int default_value)
{
	char *prop;

	prop = xmlGetProp(node, name);
	if(prop) {
		default_value = atoi(prop);
		xmlFree(prop);
	}

	return default_value;
}

float cxml_get_prop_float(xmlNodePtr node, const char *name, float default_value)
{
	char *prop;

	prop = xmlGetProp(node, name);
	if(prop) {
		default_value = atof(prop);
		xmlFree(prop);
	}

	return default_value;
}

char *cxml_get_prop_string(xmlNodePtr node, const char *name, char *default_value, char *dest)
{
	char *prop;

	prop = xmlGetProp(node, name);
	if(prop) {
		strcpy(dest, prop);
		xmlFree(prop);
	} else if(default_value) {
		strcpy(dest, default_value);
	} else {
		*dest = 0;
	}

	return dest;
}

int cxml_foreach_node(xmlNodePtr node, int (*f)(xmlNodePtr, void *), void *p)
{
	int i;

	for(i = 0; node; node = node->next) {
		if(f && !f(node, p))
			i++;
	}
	return i;
}

