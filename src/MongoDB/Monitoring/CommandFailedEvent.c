/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2016-2016 MongoDB, Inc.                                         |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2016-2016 MongoDB, Inc.                                     |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuff */
#include "php_phongo.h"
#include "php_bson.h"

zend_class_entry *php_phongo_commandfailedevent_ce;

zend_object_handlers php_phongo_handler_commandfailedevent;

/* {{{ proto MongoDB\Driver\Server CommandFailedEvent::getServer()
   Returns the Server from which the event originated */
PHP_METHOD(CommandFailedEvent, getServer)
{
	php_phongo_commandfailedevent_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

#if PHP_VERSION_ID >= 70000
	phongo_server_init(return_value, &intern->manager, intern->server_id TSRMLS_CC);
#else
	phongo_server_init(return_value, intern->manager, intern->server_id TSRMLS_CC);
#endif
}
/* }}} */

/**
 * Event thrown when a command has failed to execute.
 *
 * This class is only constructed internally.
 */
/* {{{ MongoDB\Driver\Monitoring\CommandFailedEvent */

ZEND_BEGIN_ARG_INFO_EX(ai_CommandFailedEvent_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_commandfailedevent_me[] = {
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_CommandFailedEvent_void, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	PHP_ME(CommandFailedEvent, getServer, ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_commandfailedevent_t object handlers */
static void php_phongo_commandfailedevent_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_commandfailedevent_t *intern = Z_OBJ_COMMANDFAILEDEVENT(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->error) {
		bson_destroy(intern->error);
	}

	zval_ptr_dtor(&intern->manager);

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_commandfailedevent_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_commandfailedevent_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_commandfailedevent_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_commandfailedevent;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_commandfailedevent_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_commandfailedevent;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_commandfailedevent_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_commandfailedevent_t *intern;
#if PHP_VERSION_ID >= 70000
	zval                      retval;
#else
	zval                      retval = zval_used_for_init;
#endif

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(object);
	*is_temp = 1;
	array_init_size(&retval, 6);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_commandfailedevent_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "CommandFailedEvent", php_phongo_commandfailedevent_me);
	php_phongo_commandfailedevent_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_commandfailedevent_ce->create_object = php_phongo_commandfailedevent_create_object;
	PHONGO_CE_FINAL(php_phongo_commandfailedevent_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_commandfailedevent_ce);

	memcpy(&php_phongo_handler_commandfailedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_commandfailedevent.get_debug_info = php_phongo_commandfailedevent_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_commandfailedevent.free_obj = php_phongo_commandfailedevent_free_object;
	php_phongo_handler_commandfailedevent.offset = XtOffsetOf(php_phongo_commandfailedevent_t, std);
#endif

	return;
} /* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
