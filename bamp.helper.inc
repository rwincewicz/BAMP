<?php

/**
 * Function that gives the next value from a field in the specified table
 * @param type $table
 * @return type 
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

/**
 * Function that parses a csv file into an array and returns the array
 * @param type $file
 * @param type $delimiter
 * @return type 
 */

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

/**
 * Function will populate a list with unique values from the database
 * Useful for creating drop down menus
 * @param type $table
 * @param type $field
 * @return type 
 */

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

/**
 * Function to create empty nodes to use Views to display content
 * Views expects nodes to be linked to content in the db
 * @param type $form
 * @param type $form_state
 * @param type $nid 
 */

function dummy_node($form, $form_state, $nid) {
  $node = new stdClass();
  $node->title = 'Dummy node';
  $node->type = 'article';
  $node->status = 0;
  $node->promote = 0;
  $node->format = 0;
  node_validate($node, $form, $form_state);
  node_submit($node);
  node_save($node);
}

/**
 * Function to check if a node exists or not
 * @param type $nid
 * @return type 
 */

function check_node($nid) {
  if (db_query("SELECT vid FROM {node} WHERE nid = :nid", array(':nid' => $nid))->fetchField()) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Function to calculate the sum of the field displayed on the page
 * @param type $field
 * @param type $rows
 * @return type 
 */

function bamp_sum($field, $rows) {

  $sum = 0;

  $list_no = count($rows);

  for ($i = 0; $i < $list_no; $i++) {

    $sum += $rows[$i][$field];
  }

  return $sum;
}

function r_implode( $glue1, $glue2, $pieces=NULL )
{
  $retVal = array();
  
	foreach( $pieces as $r_pieces )
	{
    		if( is_array( $r_pieces ) )
    		{
      			$retVal[] = r_implode( $glue2, $glue2, $r_pieces );
    		}
    		else	
    		{
      			$retVal[] = (int)$r_pieces;
    		}
  	}
    
    if($retVal == NULL) {
      return NULL;
    } else {
     return implode( $glue1, $retVal ); 
    }
} 

function d_implode( $glue, $pieces ) 
{ 
  foreach( $pieces as $r_pieces ) 
  { 
    if( is_array( $r_pieces ) ) 
    { 
      $retVal[] = d_implode( $glue, $r_pieces ); 
    } 
    else 
    { 
      $retVal[] = $r_pieces; 
    } 
  } 
  return implode( $glue, $retVal ); 
} 

/**
 * Function to export MySQL data as CSV
 * @param type $result
 * @param type $filename 
 */

function export_to_csv($data,  $filename = 'myfile.csv'){ 
  drupal_add_http_header('Content-Type', 'text/csv');
  drupal_add_http_header('Content-Disposition: attachment', 'filename=' . $filename);
  
  print($data) . "\r\n";
 
  exit; 
}