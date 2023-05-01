#include "internal/module.h"

#include <stddef.h>

char *module_name = NULL;

char *get_module_name(void)
{
  return module_name;
}

void export_module_name(char *name)
{
  module_name = name;
}
