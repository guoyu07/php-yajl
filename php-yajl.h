#ifndef PHP_YAJL_H
#define PHP_YAJL_H

#define PHP_YAJL_VERSION "1.0"
#define PHP_YAJL_EXTNAME "php-yajl"

PHP_FUNCTION(hello_world);

extern zend_module_entry php_yajl_module_entry;
#define phpext_yajl_ptr &hello_module_entry

#endif

