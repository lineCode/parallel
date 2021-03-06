/*
  +----------------------------------------------------------------------+
  | parallel                                                              |
  +----------------------------------------------------------------------+
  | Copyright (c) Joe Watkins 2019                                       |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe                                                      |
  +----------------------------------------------------------------------+
 */
#ifndef HAVE_PARALLEL_COPY_H
#define HAVE_PARALLEL_COPY_H

zend_function* php_parallel_copy(const zend_function *function, zend_bool persistent);
void php_parallel_copy_free(zend_function *function, zend_bool persistent);
void php_parallel_copy_zval(zval *dest, zval *source, zend_bool persistent);
zend_bool php_parallel_copy_check(php_parallel_entry_point_t *parallel, zend_execute_data *execute_data, const zend_function * function, int argc, zval *argv, zend_bool *returns);

static zend_always_inline void php_parallel_ht_dtor(HashTable *table, zend_bool persistent) {
	zend_hash_destroy(table);
	pefree(table, persistent);
}

static zend_always_inline void php_parallel_zval_dtor(zval *zv) {
	if (Z_TYPE_P(zv) == IS_ARRAY) {
#if PHP_VERSION_ID < 70300
		php_parallel_ht_dtor(Z_ARRVAL_P(zv), Z_ARRVAL_P(zv)->u.flags & HASH_FLAG_PERSISTENT);
#else
		php_parallel_ht_dtor(Z_ARRVAL_P(zv), GC_FLAGS(Z_ARRVAL_P(zv)) & IS_ARRAY_PERSISTENT);
#endif
	} else if (Z_TYPE_P(zv) == IS_STRING) {
		zend_string_release(Z_STR_P(zv));
	} else {
		if (Z_REFCOUNTED_P(zv)) {
			zval_ptr_dtor(zv);
		}
	}
}


#endif
