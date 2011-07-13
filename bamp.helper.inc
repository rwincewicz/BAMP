<?php

// $Id: bamp.helper.inc $

/**
 * @file
 * Helper functions for the BAMP module
 */

/**
 * Function that gives the next value from a field in the specified table
 * @param type $table
 * @return type 
 */

//  Added conditions to the functions because drupal was complaining about the functions
//  already having been declared.
if (!function_exists('bamp_next_id')) {

  function bamp_next_id($table) {

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
}

/**
 * Function that parses a csv file into an array and returns the array
 * @param type $file
 * @param type $delimiter
 * @return type 
 */
if (!function_exists('bamp_csv2aa')) {

  function bamp_csv2aa($file, $delimiter) {
    if (($handle = fopen($file, "r")) !== FALSE) {
      $i = 0;
      while (($line_array = fgetcsv($handle, 4000, $delimiter)) !== FALSE) {
        for ($j = 0; $j < count($line_array); $j++) {
          $arrarr[$i][$j] = $line_array[$j];
        }
        $i++;
      }
      fclose($handle);
    }
    return $arrarr;
  }
}

/**
 * Function will populate a list with unique values from the database
 * Useful for creating drop down menus
 * @param type $table
 * @param type $field
 * @return type 
 */
if (!function_exists('bamp_db_list')) {

  function bamp_db_list($table, $field) {

    $list = db_select($table, 't')
        ->fields('t', array('id', $field));
    $result = $list->execute();

    $list_array = array();

//    Pull out all of the values within the given field and populate an array
    while ($value = $result->fetchAssoc()) {
      $list_array[$value['id']] = $value[$field];
    }

//    Only return the unique ones
//    TO DO: Is there a way of just pulling the unique values from the db in the first place?
    $list_unique = array_unique($list_array);

    return $list_unique;
  }
}

/**
 * Function to calculate the sum of the field displayed on the page
 * @param type $field
 * @param type $rows
 * @return type 
 */
if (!function_exists('bamp_sum')) {

  function bamp_sum($field, $rows) {

    $sum = 0;

    $list_no = count($rows);

    for ($i = 0; $i < $list_no; $i++) {

      $sum += $rows[$i][$field];
    }

    return $sum;
  }
}

/**
 * Function to recursively implode a multidimensional array
 */
if (!function_exists('bamp_r_implode')) {

  function bamp_r_implode($glue1, $glue2, $pieces=NULL) {
    $ret_val = array();

    foreach ($pieces as $r_pieces) {
      if (is_array($r_pieces)) {
        $ret_val[] = bamp_r_implode($glue2, $glue2, $r_pieces);
      }
      else {
        $ret_val[] = (int) $r_pieces;
      }
    }

    if ($ret_val == NULL) {
      return NULL;
    }
    else {
      return implode($glue1, $ret_val);
    }
  }
}

/**
 * Function to recursively implode a multidimensional array using only one 'glue' character
 */
if (!function_exists('bamp_d_implode')) {

  function bamp_d_implode($glue, $pieces) {
    foreach ($pieces as $r_pieces) {
      if (is_array($r_pieces)) {
        $ret_val[] = bamp_d_implode($glue, $r_pieces);
      }
      else {
        $ret_val[] = $r_pieces;
      }
    }
    return implode($glue, $ret_val);
  }
}

/**
 * Function to export MySQL data as CSV
 * @param type $result
 * @param type $filename 
 */
if (!function_exists('bamp_export_to_csv')) {

  function bamp_export_to_csv($data, $filename = 'myfile.csv') {
    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition: attachment', 'filename=' . $filename);

    print($data) . "\r\n";

    exit;
  }
}

/**
 * Function to sort the returned data into a suitable form for CSV download
 * @param type $header
 * @param type $rows
 * @return string 
 */
if (!function_exists('bamp_prep_for_csv')) {

  function bamp_prep_for_csv($header, $rows) {

    $no = count($header);

//  The first two headers might be arrays because they can be sorted
//  This code checks to see if they are array and pulls out the relevant data
    
    if (is_array($header[0]) == TRUE) {
    $result = $header[0]['data'];
    }
    else {
      $result = $header[0];
    }
    $result .= ',';
    
    if (is_array($header[1]) == TRUE) {
    $result .= $header[1]['data'];
    }
    else {
      $result .= $header[1];
    }

//  Loop through the remaining headers
    for ($i = 2; $i < $no; $i++) {

      $result .= ',';
      $result .= $header[$i];
    }

//  Add a line break at the end
    $result .= "\r\n";

    $nr = count($rows);

//  Loop through the data in the rows putting commas and line breaks where required
//  Strip tags will remove the HTML tags inserted to make the entry id a link to the edit form
    for ($i = 0; $i < $nr; $i++) {
      $result .= strip_tags(implode(',', $rows[$i]));
      $result .= "\r\n";
    }
    
    return $result;
  }
}

/**
 * Function to convert the uid to a username so that it makes sense in the changelog forms
 */
if (!function_exists('bamp_user_name')) {

  function bamp_user_name($userid) {
    $query = db_select('users', 'u');
    $query->condition('u.uid', $userid, '=');
    $query->addField('u', 'name');
    $name = $query->execute()->fetchAssoc();
    $username = implode(',', $name);
    return $username;
  }
}