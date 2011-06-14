<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


function next_id($table) {

//  Queries the $table in the drupal database and 
//  returns the next number for the id field

  $xid = db_select($table, 'x')
      ->fields('x', array('id'))
      ->orderBy('id', 'DESC')
      ->execute()
      ->fetchAssoc();

  $nxid = $xid['id'];

  $nxid++;

  return $nxid;
}

function csv2aa($file, $delimiter) {
  if (($handle = fopen($file, "r")) !== FALSE) {
    $i = 0;
    while (($lineArray = fgetcsv($handle, 4000, $delimiter)) !== FALSE) {
      for ($j = 0; $j < count($lineArray); $j++) {
        $arrarr[$i][$j] = $lineArray[$j];
      }
      $i++;
    }
    fclose($handle);
  }
  return $arrarr;
}

function db_list($table, $field) {

  $list = db_select($table, 't')
      ->fields('t', array('id', $field));
  $result = $list->execute();

  $lista = array();

  while ($value = $result->fetchAssoc()) {
    $lista[$value['id']] = $value[$field];
  }

  $listu = array_unique($lista);

  return $listu;
}

function dummy_node($form, $form_state, $nid) {
  $node = new stdClass();
//  $node->nid = $nid;
//  $node->vid = $nid;
  $node->title = 'Dummy node';
  $node->type = 'article';
  $node->status = 0;
  $node->promote = 0;
  $node->format = 0;
  node_validate($node, $form, $form_state);
  node_submit($node);
  node_save($node);
}


function check_node($nid) {
  if (db_query("SELECT vid FROM {node} WHERE nid = :nid", array(':nid' => $nid))->fetchField()) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
