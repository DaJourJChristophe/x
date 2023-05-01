#ifndef X_INTERNAL_MODULE_H
#define X_INTERNAL_MODULE_H

#include "common.h"

char *get_module_name(void);

void export_module_name(char *name);

#define X_MODULE_NAME_SYMBOL \
  get_module_name()

#define __INTERNAL_MODULE_NAME_SYMBOL__    __internal_module_name__

#define X_EXPORT_MODULE_NAME_SYMBOL() \
  export_module_name(__INTERNAL_MODULE_NAME_SYMBOL__)

#endif/*X_INTERNAL_MODULE_H*/
