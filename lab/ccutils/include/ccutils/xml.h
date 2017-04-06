#ifndef __CCUTILS_XML_H__
#define __CCUTILS_XML_H__

#include <libxml/xpath.h>

#include "ccutils/macro.h"

CU_DEPRECATED_FOR(cxml_get_xpath_object)
xmlXPathObjectPtr get_xpath_object(xmlDocPtr doc, unsigned char *path);

CU_DEPRECATED_FOR(cxml_for_each_xobject)
int	parse_xpath_object(xmlXPathObjectPtr object, int (*func)(xmlNodePtr, void *), void *data);

extern int cxml_get_prop_bool(xmlNodePtr node, const char *name, int default_value);
extern int cxml_get_prop_integer(xmlNodePtr node, const char *name, int default_value);
extern float cxml_get_prop_float(xmlNodePtr node, const char *name, float default_value);
extern char *cxml_get_prop_string(xmlNodePtr node, const char *name, char *default_value, char *dest);

CU_DEPRECATED_FOR(cxml_for_each_node)
int	cxml_foreach_node(xmlNodePtr node, int (*f)(xmlNodePtr, void *), void *p);

extern
xmlXPathObjectPtr	cxml_get_xpath_object(xmlDocPtr doc, unsigned char *path);

extern
int	cxml_for_each_node(xmlNodePtr node, int (*f)(xmlNodePtr, void *), void *p);

/* for each a xpath object nodeset */
extern
int	cxml_for_each_xobject(xmlXPathObjectPtr object, int (*f)(xmlNodePtr, void *), void *data);

extern
int	cxml_for_each_xobject_children(xmlXPathObjectPtr object, int (*f)(xmlNodePtr, void *),
		void *p);

#endif
