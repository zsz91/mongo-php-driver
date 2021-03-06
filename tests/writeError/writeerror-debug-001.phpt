--TEST--
MongoDB\Driver\WriteError debug output
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE_30'); CLEANUP(STANDALONE_30) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE_30);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 1]);

try {
    $manager->executeBulkWrite(NS, $bulk);
} catch(MongoDB\Driver\Exception\BulkWriteException $e) {
    var_dump($e->getWriteResult()->getWriteErrors()[0]);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteError)#%d (%d) {
  ["message"]=>
  string(95) "E11000 duplicate key error index: phongo.writeError_writeerror_debug_001.$_id_ dup key: { : 1 }"
  ["code"]=>
  int(11000)
  ["index"]=>
  int(1)
  ["info"]=>
  NULL
}
===DONE===
