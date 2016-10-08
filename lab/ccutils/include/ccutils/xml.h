#ifndef __CCUTILS_XML_H__
#define __CCUTILS_XML_H__

#include <libxml/xpath.h>

extern xmlXPathObjectPtr get_xpath_object(xmlDocPtr doc, unsigned char *path);
extern int parse_xpath_object(xmlXPathObjectPtr object, int (*func)(xmlNodePtr, void *), void *data);

#endif
