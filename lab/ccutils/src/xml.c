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

