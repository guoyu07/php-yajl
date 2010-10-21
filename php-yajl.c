#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "php-yajl.h"

#include <yajl/yajl_parse.h>

/*
 * When PHP binds functions to C code, it uses this table to dispatch from.
 */
static function_entry yajl_functions[] = {
    PHP_FE(yajl_alloc, NULL)

    PHP_FE(hello_world, NULL)
    PHP_FE(add_two_numbers, NULL)
    PHP_FE(my_call_back, NULL)

    {NULL, NULL, NULL}
};

/*
 * This structure is used by PHP to discover the existence of the extension and to learn bits of knowledge about it.
 */
zend_module_entry php_yajl_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_YAJL_EXTNAME,
    yajl_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_YAJL_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_YAJL
ZEND_GET_MODULE(php_yajl)
#endif

/* {{{ dummy callback for testing purposes. */
static int yajl_callback_null(void *ctx)                                                            { return 1; }
static int yajl_callback_boolean(void *ctx, int v)                                                  { return 1; }
static int yajl_callback_integer(void *ctx, long v)                                                 { return 1; }
static int yajl_callback_double(void *ctx, double v)                                                { return 1; }
static int yajl_callback_string(void *ctx, const unsigned char *s, const unsigned int sLength)      { return 1; }
static int yajl_callback_start_map(void *ctx)                                                       { return 1; }
static int yajl_callback_map_key(void *ctx, const unsigned char *k, const unsigned int kLength)     { return 1; }
static int yajl_callback_end_map(void *ctx)                                                         { return 1; }
static int yajl_callback_start_array(void *ctx)                                                     { return 1; }
static int yajl_callback_end_array(void *ctx)                                                       { return 1; }
/* }}} */

static yajl_callbacks callbacks = {
    yajl_callback_null,
    yajl_callback_boolean,
    yajl_callback_integer,
    yajl_callback_double,
    NULL,
    yajl_callback_string,
    yajl_callback_start_map,
    yajl_callback_map_key,
    yajl_callback_end_map,
    yajl_callback_start_array,
    yajl_callback_end_array
};

PHP_FUNCTION(yajl_alloc) {
    yajl_handle hYajl;
    yajl_parser_config cfg = { 0, 0 };

    hYajl = yajl_alloc(&callbacks, &cfg, NULL, NULL);
    RETVAL_LONG((long)hYajl);
}

PHP_FUNCTION(hello_world) {
    RETURN_STRING("Hello world", 1);
}

PHP_FUNCTION(add_two_numbers) {
    long a, b;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &a, &b)) {
        RETURN_NULL();
    }

    RETURN_LONG(a+b);
}

PHP_FUNCTION(my_call_back) {
    zval *return_handle = NULL;
    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;
    zval *the_parameter;
    char tmp[30];

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &fci, &fci_cache)) {
        RETURN_NULL();
    }

    /* Tell our function call execution context where to put its return results. */
    fci.retval_ptr_ptr = &return_handle;

    /* Synthesize its input parameters. */
    memset(tmp, 0, 30);
    tmp[0] = '0';
    tmp[1] = 'x';
    sprintf(&tmp[2], "%016lX", (unsigned long)tmp);
    ZVAL_STRING(the_parameter, tmp, 0);

    /* Bundle all the input parameters into an array. */
    fci.params = safe_emalloc(sizeof(zval *), 1, 0);
    fci.param_count = 1;
    fci.params[0] = &the_parameter;

    /* Call the supplied function. */
    zend_call_function(&fci, &fci_cache TSRMLS_CC);

    /* If the allocation succeeded, free the parameter array. */
    if(fci.params) {
        efree(fci.params);
    }

    /* Without this, PHP seems to want to abort trap on me. */
    RETURN_NULL();
}

