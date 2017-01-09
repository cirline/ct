#ifndef __CCUTILS_XML_H__
#define __CCUTILS_XML_H__

#include <libxml/xpath.h>

extern xmlXPathObjectPtr get_xpath_object(xmlDocPtr doc, unsigned char *path);
extern int parse_xpath_object(xmlXPathObjectPtr object, int (*func)(xmlNodePtr, void *), void *data);
extern int cxml_get_prop_bool(xmlNodePtr node, const char *name, int default_value);
extern int cxml_get_prop_integer(xmlNodePtr node, const char *name, int default_value);
extern float cxml_get_prop_float(xmlNodePtr node, const char *name, float default_value);
extern char *cxml_get_prop_string(xmlNodePtr node, const char *name, char *default_value, char *dest);
extern int cxml_foreach_node(xmlNodePtr node, int (*f)(xmlNodePtr, void *), void *p);

#endif
