<?php

/**
 * @file
 * Administration pages provided by Turbo Pages module.
 */


/**
 * Retrieving content to form RSS from database.
 */
function _turbo_pages_get_content_rss() {
  $node_types = variable_get('turbo_pages_content_types', FALSE);

  $query = db_select('node', 'n');
  $query->fields('n', array('nid', 'nid'))
    ->condition('n.status', 1)
    ->condition('n.type', $node_types)
    ->orderBy('n.uid', 'ASC');
  $result = $query->execute()->fetchAllKeyed();

  $nodes = node_load_multiple($result);

  return $nodes;
}

/**
 * Getting a list of content types from the $table as an associative array,
 * where key = $col1, and value = $col2.
 *
 * @param $table
 * @param $col1
 * @param null $col2
 * @param array $conditions
 */
function _turbo_pages_get_col_from_db($table, $col1, $col2 = NULL, $conditions = array()) {
  if (!$col2) {
    $col2 = $col1;
  }

  $query = db_select($table, 't');
  $query->fields('t', array($col1, $col2));

  if(isset($conditions['module'])) {
    foreach ($conditions['module'] as $condition) {
      $query->condition('module', $condition);
    }
  }

  $columns = $query->execute()
    ->fetchAllKeyed();

  return $columns;
}
