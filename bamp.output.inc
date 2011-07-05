<?php

// $Id: bamp.output.inc $

/**
 * @file
 * Code to create bamp output forms
 */

/**
 * Create BAMP report menu
 */
function bampreport_form($form_state) {
  drupal_set_title(t('Report menu'));

  $form = array();

  $content['form'] = array(
    '#markup' => '<p> This is the BAMP report menu</p>',
  );

  return $form;
}

function bampreport_menu_callback() {
  return drupal_get_form('bampreport_form');
}

/**
 * Form to display combined results of field and lab data where overlap exists
 * @param type $form
 * @param type $form_state
 * @param string $species_id
 * @param string $trip_id
 * @param string $site_id
 * @param type $no_results
 * @return string 
 */

function bamprepres_form($form, &$form_state, $species_id, $trip_id, $site_id, $no_results) {
  drupal_set_title(t('Combined results report form'));

  // Values can be passed in the URL to narrow the search
  // If the value isn't passed then all the results are returned
  
  if ($species_id == NULL) {
    $species_message = 'Species: All<br />';
  }
  else {
    $species_message = 'Species: ' . $species_id . '<br />';
  }

  if ($trip_id == NULL) {
    $trip_message = 'Trip: All<br />';
  }
  else {
    $trip_message = 'Trip: ' . $trip_id . '<br />';
  }

  if ($site_id == NULL) {
    $site_message = 'Site: All<br />';
  }
  else {
    $site_message = 'Site: ' . $site_id . '<br />';
  }

  // Check if there is sorting request
  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Result ID':
        $order = 'id';
        break;
      case 'Fish ID':
        $order = 'fish_id';
        break;
      default:
        $order = 'id';
    }
  }
  else {
    // Default sort
    $sort = 'ASC';
    $order = 'id';
  }

  $form = array();

// Build array for the headers. id and fish_id can be sorted
  $header = array(
    array('data' => t('Result ID'), 'field' => 'id'),
    array('data' => t('Fish ID'), 'field' => 'fish_id'),
    t('Trip ID'),
    t('Site ID'),
    t('Lab'),
    t('Date'),
    t('Crew'),
    t('Species'),
    t('Weight/kg'),
    t('Length/mm'),
    t('Total lice - Field'),
    t('Total lice - Lab'),
    t('Species - Lab'));

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search details'),
  );

//  Add a message into the fieldset indicating the number of results returned
  if ($no_results == 'all') {
    $list_no = next_id_bamp('bamp_field_results');
    $no_message = 'Showing all results</br>';
  } 
  else {
    $list_no = $no_results;
    $no_message = 'Showing ' . $no_results . ' results per page </br>';
  }
  
//  Start the DB query specifying a sort of the table output and using the pager functionality. 
//  Limit the number of results per page to $list_no
  $select = db_select('bamp_field_results', 'f')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($list_no);

//  Translate "" to NULL for each of the entries so that consistent results are returned
  if ($species_id == "") {
    $species_id = NULL;
  }

  if ($trip_id == "") {
    $trip_id = NULL;
  }

  if ($site_id == "") {
    $site_id = NULL;
  }

//  If the value passed isn't NULL then add the value as a condition in the DB query
  if ($species_id !== NULL) {
    $select->condition('f.species', $species_id, '=');
  }

  if ($trip_id !== NULL) {
    $select->condition('f.trip_id', $trip_id, '=');
  }

  if ($site_id !== NULL) {
    $select->condition('f.site_name', $site_id, '=');
  }

//  Put a message in the fieldset indicating which values for each field are being queried
  $form['form']['details'] = array(
    '#markup' => $species_message . $trip_message . $site_message . $no_message,
  );
  
//  Create a 'download CSV' button to allow the results of the query to be exported
  $form['form']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Download CSV')
    );
  
//  Add the relevant fields into the query and execute it
  $select->leftJoin('bamp_lab_results', 'l', 'f.fish_id = l.fish_id');
  $select->addField('f', 'id');
  $select->addField('f', 'fish_id');
  $select->addField('f', 'trip_id');
  $select->addField('f', 'site_name');
  $select->addField('f', 'to_lab');
  $select->addField('f', 'date');
  $select->addField('f', 'crew');
  $select->addField('f', 'species');
  $select->addField('l', 'weight');
  $select->addField('l', 'length');
  $select->addField('f', 'total_lice');
  $select->addField('l', 'total_lice');
  $select->addField('l', 'species');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

//  Create an array for the results and populate it from the DB query
  $rows = array();

  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
  }

//  If no results are returned then indicate this, otherwise create the results table
  if ($rows == NULL) {
    $form['form']['none'] = array(
      '#markup' => '<p><h1>No results found</h1></p>',
    );
  }
  else {
    $form['Form'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );

//    Perform sum and average calculations on relevant columns
    if ($no_results == 'all') {
      $no_results = count($rows);
    }
    
    $sum_weight = bamp_sum('weight', $rows);

    $ave_weight = round($sum_weight / $no_results, 2);

    $sum_length = bamp_sum('length', $rows);

    $ave_length = round($sum_length / $no_results, 2);

//    $sum_chalimus = bamp_sum('chalimus', $rows);
//
//    $ave_chalimus = round($sum_chalimus / $no_results, 2);
//
//    $sum_caligus = bamp_sum('caligus', $rows);

//    $ave_caligus = round($sum_caligus / $no_results, 2);

    $sum_total_lice = bamp_sum('total_lice', $rows);

    $ave_total_lice = round($sum_total_lice / $no_results, 2);

//    Create the rows for the second table containing the calculations
    $sum = array(
      array('sum' => 'Total',
        'weight' => $sum_weight,
        'length' => $sum_length,
//        'caligus' => $sum_caligus,
//        'chalimus' => $sum_chalimus,
        'total_lice' => $sum_total_lice),
      array('sum' => 'Average',
        'weight' => $ave_weight,
        'length' => $ave_length,
//        'caligus' => $ave_caligus,
//        'chalimus' => $ave_chalimus,
        'total_lice' => $ave_total_lice));

//    Create the table header
    $header_calc = array(
      t(''),
      t('Weight/kg'),
      t('Length/mm'),
//      t('Caligus - Lab'),
//      t('Chalimus - Field'),
      t('Total lice'));

//    Create the table
    $form['sum'] = array(
      '#theme' => 'table',
      '#header' => $header_calc,
      '#rows' => $sum,
    );

//    Add in the pager at the bottom
    $form['pager'] = array(
      '#theme' => 'pager',
      '#quantity' => 10,
    );
    
  }
  
$result = prep_for_csv($header, $rows);
  
//  Pass the data as a hidden variable in the form_state array
  $form['data'] = array(
    '#type' => 'hidden',
    '#value' => $result,
  );
  
  return $form;
}

function bamprepres_menu_callback($species_id=NULL, $trip_id=NULL, $site_id=NULL, $no_results=20) {
  return drupal_get_form('bamprepres_form', $species_id, $trip_id, $site_id, $no_results);
}

/**
 * Submit function to download a CSV file of the selected results 
 * @param type $form
 * @param type $form_state 
 */
function bamprepres_form_submit($form, &$form_state) {
  
$result = $form_state['values']['data'];

$filename = 'data.csv';

drupal_add_http_header('Content-Type', 'text/csv');
drupal_add_http_header('Content-Disposition: attachment', 'filename=' . $filename);
  
print($result) . "\r\n";
 
  exit;

}
/**
 * Report for sites
 */
function bamprepsite_form($form, &$form_state) {
  drupal_set_title(t('Site report form'));

  // Check if there is sorting request
  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Result ID':
        $order = 'id';
        break;
      case 'Fish ID':
        $order = 'fish_id';
        break;
      default:
        $order = 'id';
    }
  }
  else {
    // Default sort
    $sort = 'ASC';
    $order = 'id';
  }

  $form = array();

  $page_limit = 20;

//  Create header array with the first two fields being sortable
  $header = array(
    array('data' => t('Site ID'), 'field' => 'id'),
//    array('data' => t('Site ID-2'), 'field' => 'craw_site_id'),
    t('Name'),
    t('Latitude'),
    t('Longitude'),
    t('Type'),
    t('Description'),
    t('Farm ID'),
    t('Zone'),
    t('Area'),
    t('Date'),
    t('Year'),
    t('Repetition'),
    t('Route'),
    t('Distance'));

//  Select table and specify fields to return for DB query
  $select = db_select('bamp_sites', 's')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($page_limit);
  $select->addField('s', 'id');
//  $select->addField('s', 'craw_site_id');
  $select->addField('s', 'name');
  $select->addField('s', 'min_latitude');
  $select->addField('s', 'min_longitude');
  $select->addField('s', 'type');
  $select->addField('s', 'description');
  $select->addField('s', 'farm_id');
  $select->addField('s', 'zone');
  $select->addField('s', 'area');
  $select->addField('s', 'date');
  $select->addField('s', 'year');
  $select->addField('s', 'rep');
  $select->addField('s', 'route');
  $select->addField('s', 'distance');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

//  Loop through results and add them to an array
  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
  }

  $nr = count($rows);

//  Loop to replace the ID text with a link to the edit form for that entry
  for ($i = 0; $i < $nr; $i++) {
    $id = $rows[$i]['id'];
    $rows[$i]['id'] = l($id, 'bamp/Edit forms/Individual Site data/' . $id);
  }
  
//  Create table
  $form['Form'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

//  Add pager to the end of the table
  $form['pager'] = array(
    '#theme' => 'pager',
    '#quantity' => 10,
  );

  $result = prep_for_csv($header, $rows);
  
//  Pass the data as a hidden variable in the form_state array
  $form['data'] = array(
    '#type' => 'hidden',
    '#value' => $result,
  );
  
  return $form;
}

function bamprepsite_menu_callback() {
  return drupal_get_form('bamprepsite_form');
}

/**
 * Report form for field (farm + wild) results
 * @param type $form
 * @param type $form_state
 * @param string $species_id
 * @param string $trip_id
 * @param string $site_id
 * @param type $no_results
 * @return string 
 */
function bamprepfield_form($form, &$form_state, $species_id, $trip_id, $site_id, $no_results, $data) {
  drupal_set_title(t('Field results report form'));

//  Identify is an argument is present or not and if so create a message for the search details
  if ($species_id == NULL) {
    $species_message = 'Species: All<br />';
  }
  else {
    $species_message = 'Species: ' . $species_id . '<br />';
  }

  if ($trip_id == NULL) {
    $trip_message = 'Trip: All<br />';
  }
  else {
    $trip_message = 'Trip: ' . $trip_id . '<br />';
  }

  if ($site_id == NULL) {
    $site_message = 'Site: All<br />';
  }
  else {
    $site_message = 'Site: ' . $site_id . '<br />';
  }

  // Check if there is sorting request
  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Result ID':
        $order = 'id';
        break;
      case 'Fish ID':
        $order = 'fish_id';
        break;
      default:
        $order = 'id';
    }
  }
  else {
    // Default sort
    $sort = 'ASC';
    $order = 'id';
  }

  $form = array();

  $page_limit = 20;

//  Create header array with the first two fields being sortable
  $header = array(
    array('data' => t('Result ID'), 'field' => 'id'),
    array('data' => t('Fish ID'), 'field' => 'fish_id'),
    t('Trip ID'),
    t('Sample ID'),
    t('Site name'),
    t('Site number'),
    t('Set number'),
    t('Blind number'),
    t('To Lab'),
    t('Date'),
    t('Pen'),
    t('Input name'),
    t('Route'),
    t('Repetition number'),
    t('Number of fish sampled'),
    t('Crew'),
    t('Species'),
    t('Height/mm'),
    t('Length/mm'),
    t('Weight'));
  
  if ($data == 'y') {
    $header = array(
       array('data' => t('Result ID'), 'field' => 'id'),
    array('data' => t('Fish ID'), 'field' => 'fish_id'),
    t('Trip ID'),
    t('Sample ID'),
    t('Site name'),
    t('Site number'),
    t('Set number'),
    t('Blind number'),
    t('To Lab'),
    t('Date'),
    t('Pen'),
    t('Input name'),
    t('Route'),
    t('Repetition number'),
    t('Number of fish sampled'),
    t('Crew'),
    t('Species'),
    t('Height/mm'),
    t('Length/mm'),
    t('Weight'),
    t('Lep copepods'),
    t('Cal copepods'),
    t('Total copepods'),
    t('Chalimus a'),
    t('Chalimus b'),
    t('Total chalimus'),
    t('Lep pre-adult male'),
    t('Lep pre-adult female'),
    t('Lep pre-adult unidentified'),
    t('Total lep pre-adult'),
    t('Lep adult male'),
    t('Lep adult female'),
    t('Lep adult gravid'),
    t('Lep adult unidentified'),
    t('Total lep adult'),
    t('Total lep'),
    t('Caligus pre-adult male'),
    t('Caligus pre-adult female'),
    t('Caligus pre-adult unidentified'),
    t('Total caligus pre-adult'),
    t('Caligus adult male'),
    t('Caligus adult female'),
    t('Caligus adult gravid'),
    t('Caligus adult unidentified'),
    t('Total caligus adult'),
    t('Total caligus'),
    t('Total motile'),
    t('Total lice'),
    t('Scar - chalimus'),
    t('Scar - motile'),
    t('Predation marks'),
    t('Hemorrhaging'),
    t('Mate guarding'),
    t('Pinched belly'),
    t('Tissue damage'),
    t('Comments'));
  }

//  Fieldset to hold the HTML details about the search
  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search details'),
  );
  
//  Check if the search returns any results and display a relevant message
  if ($no_results == 'all') {
    $list_no = next_id_bamp('bamp_field_results');
    $no_message = 'Showing all results</br>';
  } 
  else {
    $list_no = $no_results;
    $no_message = 'Showing ' . $no_results . ' results per page </br>';
  }

//  Select table to query, specify table output and use the pager
  $select = db_select('bamp_field_results', 'f')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($list_no);

//  Clear up input so that "" is set to NULL
  if ($species_id == "") {
    $species_id = NULL;
  }

  if ($trip_id == "") {
    $trip_id = NULL;
  }

  if ($site_id == "") {
    $site_id = NULL;
  }

//  If the values aren't NULL then narrow the search based on the value
  if ($species_id !== NULL) {
    $select->condition('f.species', $species_id, '=');
  }

  if ($trip_id !== NULL) {
    $select->condition('f.trip_id', $trip_id, '=');
  }

  if ($site_id !== NULL) {
    $select->condition('f.site_name', $site_id, '=');
  }

//  Specify fields to return from the query and execute the query
  $select->addField('f', 'id');
  $select->addField('f', 'fish_id');
  $select->addField('f', 'trip_id');
  $select->addField('f', 'sample_id');
  $select->addField('f', 'site_name');
  $select->addField('f', 'site_no');
  $select->addField('f', 'set_no');
  $select->addField('f', 'blind_no');
  $select->addField('f', 'to_lab');
  $select->addField('f', 'date');
  $select->addField('f', 'pen');
  $select->addField('f', 'input_name');
  $select->addField('f', 'route');
  $select->addField('f', 'rep_no');
  $select->addField('f', 'no_sampled');
  $select->addField('f', 'crew');
  $select->addField('f', 'species');
  $select->addField('f', 'height');
  $select->addField('f', 'length');
  $select->addField('f', 'weight');
  
  if ($data == 'y') {
  $select->addField('f', 'cop_lep');
  $select->addField('f', 'cop_cal');
  $select->addField('f', 'total_cop');
  $select->addField('f', 'chal_a');
  $select->addField('f', 'chal_b');
  $select->addField('f', 'total_chal');
  $select->addField('f', 'lep_pre_male');
  $select->addField('f', 'lep_pre_female');
  $select->addField('f', 'lep_pre_unid');
  $select->addField('f', 'total_lep_pre');
  $select->addField('f', 'lep_adult_male');
  $select->addField('f', 'lep_adult_female');
  $select->addField('f', 'lep_gravid');
  $select->addField('f', 'lep_adult_unid');
  $select->addField('f', 'total_lep_adult');
  $select->addField('f', 'total_lep');
  $select->addField('f', 'cal_pre_male');
  $select->addField('f', 'cal_pre_female');
  $select->addField('f', 'cal_pre_unid');
  $select->addField('f', 'total_cal_pre');
  $select->addField('f', 'cal_adult_male');
  $select->addField('f', 'cal_adult_female');
  $select->addField('f', 'cal_gravid');
  $select->addField('f', 'cal_adult_unid');
  $select->addField('f', 'total_cal_adult');
  $select->addField('f', 'total_cal');
  $select->addField('f', 'total_motile');
  $select->addField('f', 'total_lice');
  $select->addField('f', 'scar_chal');
  $select->addField('f', 'scar_mob');
  $select->addField('f', 'pred_mark');
  $select->addField('f', 'hem');
  $select->addField('f', 'mate_guard');
  $select->addField('f', 'pin_belly');
  $select->addField('f', 'tissue_damage');
  $select->addField('f', 'comments');
  }
  
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();

//  Add the results from the query into an array
  foreach ($entries as $result) {
    $rows[] = array_map('check_plain', $result);
  }

  $nr = count($rows);

//  Loop to replace the ID text with a link to the edit form for that entry
  for ($i = 0; $i < $nr; $i++) {
    $id = $rows[$i]['id'];
    $rows[$i]['id'] = l($id, 'bamp/Edit forms/Individual Field results/' . $id);
  }
  
//  Add the message identifying what criteria were used to create the results
  $form['form']['details'] = array(
    '#markup' => $species_message . $trip_message . $site_message . $no_message,
  );
  
  //  Create a 'download CSV' button to allow the results of the query to be exported
  $form['form']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Download CSV')
    );
  
//  If the query doesn't return any results then notify the user, otherwise create the table
  if ($rows == NULL) {
    $form['form']['none'] = array(
      '#markup' => '<p><h1>No results found</h1></p>',
    );
  }
  else {
    $form['Form'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );

    if ($no_results == 'all') {
      $no_results = count($rows);
    }
    
//    $sum_chalimus = bamp_sum('total_chal', $rows);
//
//    $ave_chalimus = $sum_chalimus / $no_results;
//
////    $sum_caligus = bamp_sum('caligus', $rows);
////
////    $ave_caligus = $sum_caligus / $page_limit;
//
//    $sum_total_lice = bamp_sum('total_lice', $no_results);
//
//    $ave_total_lice = $sum_total_lice / $no_results;
////
//    $sum = array(
//      array('sum' => 'Total',
////        'caligus' => $sum_caligus,
//        'chalimus' => $sum_chalimus,
//        'total_lice' => $sum_total_lice),
//      array('sum' => 'Average',
////        'caligus' => $ave_caligus,
//        'chalimus' => $ave_chalimus,
//        'total_lice' => $ave_total_lice));
//
//    $header_calc = array(
//      t(''),
////      t('Caligus'),
//      t('Chalimus'),
//      t('Total lice'));
//
//    $form['sum'] = array(
//      '#theme' => 'table',
//      '#header' => $header_calc,
//      '#rows' => $sum,
//    );

    $form['pager'] = array(
      '#theme' => 'pager',
      '#quantity' => 10,
    );
  }
  
  $result = prep_for_csv($header, $rows);
  
//  Pass the data as a hidden variable in the form_state array
  $form['data'] = array(
    '#type' => 'hidden',
    '#value' => $result,
  );
  
  return $form;
}

function bamprepfield_menu_callback($species_id=NULL, $trip_id=NULL, $site_id=NULL, $no_results=20, $data='y') {
  return drupal_get_form('bamprepfield_form', $species_id, $trip_id, $site_id, $no_results, $data);
}

/**
 * Create the report form for the lab result data
 * @param type $form
 * @param type $form_state
 * @param string $species_id
 * @param string $trip_id
 * @param string $lab
 * @return int 
 */

function bamprepfield_form_submit($form, &$form_state) {
  
$result = $form_state['values']['data'];

$filename = 'data.csv';

drupal_add_http_header('Content-Type', 'text/csv');
drupal_add_http_header('Content-Disposition: attachment', 'filename=' . $filename);
  
print($result) . "\r\n";
 
  exit;

}

function bampreplab_form($form, &$form_state, $species_id, $trip_id, $lab) {
  drupal_set_title(t('Lab results report form'));

  if ($species_id == NULL) {
    $species_message = 'Species: All<br />';
  }
  else {
    $species_message = 'Species: ' . $species_id . '<br />';
  }

  if ($trip_id == NULL) {
    $trip_message = 'Trip: All<br />';
  }
  else {
    $trip_message = 'Trip: ' . $trip_id . '<br />';
  }

  if ($lab == NULL) {
    $lab_message = 'Lab: All<br />';
  }
  else {
    $lab_message = 'Lab: ' . $lab . '<br />';
  }

  // Check if there is sorting request
  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Result ID':
        $order = 'id';
        break;
      case 'Fish ID':
        $order = 'fish_id';
        break;
      default:
        $order = 'id';
    }
  }
  else {
    // Default sort
    $sort = 'ASC';
    $order = 'id';
  }

  $form = array();

  $page_limit = 20;

  $header = array(
    array('data' => t('Result ID'), 'field' => 'id'),
    array('data' => t('Fish ID'), 'field' => 'fish_id'),
    t('Trip ID'),
    t('Lab'),
    t('Lab ID'),
    t('Date'),
    t('Group ID'),
    t('Method'),
    t('People'),
    t('Species'),
    t('Weight/kg'),
    t('Length/mm'),
    t('Lep_C0'),
    t('Lep_C1'),
    t('Lep_C2'),
    t('Lep_C3'),
    t('Lep_C4M'),
    t('Lep_C4F'),
    t('Lep_PAM'),
    t('Lep_PAF'),
    t('Lep_AM'),
    t('Lep_AF'),
    t('Lep_NM_Notid'),
    t('Lep_Mot_Notid'),
    t('Total Lep'),
    t('Cal_C0'),
    t('Cal_C1'),
    t('Cal_C2'),
    t('Cal_C3'),
    t('Cal_C4M'),
    t('Cal_C4F'),
    t('Cal_PAM'),
    t('Cal_PAF'),
    t('Cal_AM'),
    t('Cal_AF'),
    t('Cal_NM_Notid'),
    t('Cal_Mot_Notid'),
    t('Total Caligus'),
    t('Total lice'),
    t('Condition factor'),
    t('Scarring'),
    t('Comments'),
    );

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search details'),
  );

  $select = db_select('bamp_lab_results', 'l')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($page_limit);

  if ($species_id == "") {
    $species_id = NULL;
  }

  if ($trip_id == "") {
    $trip_id = NULL;
  }

  if ($lab == "") {
    $lab = NULL;
  }

  if ($species_id !== NULL) {
    $select->condition('l.species', $species_id, '=');
  }

  if ($trip_id !== NULL) {
    $select->condition('l.trip_id', $trip_id, '=');
  }

  if ($lab !== NULL) {
    $select->condition('l.lab', $lab, '=');
  }

  $form['form']['details'] = array(
    '#markup' => $species_message . $trip_message . $lab_message,
  );

  $select->addField('l', 'id');
  $select->addField('l', 'fish_id');
  $select->addField('l', 'trip_id');
  $select->addField('l', 'lab');
  $select->addField('l', 'lab_id');
  $select->addField('l', 'date');
  $select->addField('l', 'group_id');
  $select->addField('l', 'method');
  $select->addField('l', 'people');
  $select->addField('l', 'species');
  $select->addField('l', 'weight');
  $select->addField('l', 'length');
  $select->addField('l', 'lep_c0');
  $select->addField('l', 'lep_c1');
  $select->addField('l', 'lep_c2');
  $select->addField('l', 'lep_c3');
  $select->addField('l', 'lep_c4m');
  $select->addField('l', 'lep_c4f');
  $select->addField('l', 'lep_pam');
  $select->addField('l', 'lep_paf');
  $select->addField('l', 'lep_am');
  $select->addField('l', 'lep_af');
  $select->addField('l', 'lep_nm_notid');
  $select->addField('l', 'lep_mot_notid');
  $select->addField('l', 'total_lep');
  $select->addField('l', 'cal_c0');
  $select->addField('l', 'cal_c1');
  $select->addField('l', 'cal_c2');
  $select->addField('l', 'cal_c3');
  $select->addField('l', 'cal_c4m');
  $select->addField('l', 'cal_c4f');
  $select->addField('l', 'cal_pam');
  $select->addField('l', 'cal_paf');
  $select->addField('l', 'cal_am');
  $select->addField('l', 'cal_af');
  $select->addField('l', 'cal_nm_notid');
  $select->addField('l', 'cal_mot_notid');
  $select->addField('l', 'total_caligus');
  $select->addField('l', 'total_lice');
  $select->addField('l', 'condition_factor');
  $select->addField('l', 'scarring');
  $select->addField('l', 'comments');  
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();

  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
  }

  $nr = count($rows);

//  Loop to replace the ID text with a link to the edit form for that entry
  for ($i = 0; $i < $nr; $i++) {
    $id = $rows[$i]['id'];
    $rows[$i]['id'] = l($id, 'bamp/Edit forms/Individual Lab results/' . $id);
  }
  
  if ($rows == NULL) {
    $form['form']['none'] = array(
      '#markup' => '<p><h1>No results found</h1></p>',
    );
  }
  else {
    $form['Form'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );

    $sum_weight = bamp_sum('weight', $rows);

    $ave_weight = round($sum_weight / $page_limit, 2);

    $sum_length = bamp_sum('length', $rows);

    $ave_length = round($sum_length / $page_limit, 2);
    
    $sum_caligus = bamp_sum('total_caligus', $rows);

    $ave_caligus = $sum_caligus / $page_limit;

    $sum_lep = bamp_sum('total_lep', $rows);

    $ave_lep = $sum_lep / $page_limit;

    $sum_total_lice = bamp_sum('total_lice', $rows);

    $ave_total_lice = $sum_total_lice / $page_limit;

    $sum = array(
      array('sum' => 'Total',
        'weight' => $sum_weight,
        'length' => $sum_length,
        'caligus' => $sum_caligus,
        'lep' => $sum_lep,
        'total_lice' => $sum_total_lice),
      array('sum' => 'Average',
        'weight' => $ave_weight,
        'length' => $ave_length,
        'caligus' => $ave_caligus,
        'lap' => $ave_lep,
        'total_lice' => $ave_total_lice));

    $header_calc = array(
      t(''),
      t('Weight/kg'),
      t('Length/mm'),
      t('Caligus'),
      t('Lep'),
      t('Total lice'));

    $form['sum'] = array(
      '#theme' => 'table',
      '#header' => $header_calc,
      '#rows' => $sum,
    );

    $form['pager'] = array(
      '#theme' => 'pager',
      '#quantity' => 10,
    );
  }

  return $form;
}

function bampreplab_menu_callback($species_id=NULL, $trip_id=NULL, $site_id=NULL) {
  return drupal_get_form('bampreplab_form', $species_id, $trip_id, $site_id);
}

function bamprepenv_form($form_state) {
  drupal_set_title(t('Environmental data'));

  $form = array();

  $content['form'] = array(
    '#markup' => '<p> This is the BAMP environmental form</p>',
  );

  // Check if there is sorting request
  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Result ID':
        $order = 'id';
        break;
      case 'Date':
        $order = 'date';
        break;
      default:
        $order = 'id';
    }
  }
  else {
    // Default sort
    $sort = 'ASC';
    $order = 'id';
  }

  $form = array();

  $page_limit = 20;

  $header = array(
    array('data' => t('Result ID'), 'field' => 'id'),
    array('data' => t('Date'), 'field' => 'date'),
    t('Site name'),
    t('Temp'),
    t('Oxygen'),
    t('Salinity'),
    t('Depth'),
    );
  
   $select = db_select('bamp_environment', 'e')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($page_limit);
   
  $select->addField('e', 'id');
  $select->addField('e', 'date');
  $select->addField('e', 'site_name');
  $select->addField('e', 'temp');
  $select->addField('e', 'oxygen');
  $select->addField('e', 'sal');
  $select->addField('e', 'depth');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();

  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
  }
  
   $nr = count($rows);
  
  //  Loop to replace the ID text with a link to the edit form for that entry
  for ($i = 0; $i < $nr; $i++) {
    $id = $rows[$i]['id'];
    $rows[$i]['id'] = l($id, 'bamp/Edit forms/Environment data/' . $id);
  }
  
  $form['Form'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  
    $form['pager'] = array(
      '#theme' => 'pager',
      '#quantity' => 10,
    );
  
  return $form;
}

function bamprepenv_menu_callback() {
  return drupal_get_form('bamprepenv_form');
}

function bamprepweather_form($form_state) {
  drupal_set_title(t('Weather data'));

  $form = array();

  $content['form'] = array(
    '#markup' => '<p> This is the BAMP weather form</p>',
  );

  // Check if there is sorting request
  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Result ID':
        $order = 'id';
        break;
      case 'Date':
        $order = 'date';
        break;
      default:
        $order = 'id';
    }
  }
  else {
    // Default sort
    $sort = 'ASC';
    $order = 'id';
  }

  $form = array();

  $page_limit = 20;

  $header = array(
    array('data' => t('Result ID'), 'field' => 'id'),
    array('data' => t('Date'), 'field' => 'date'),
    t('Time'),
    t('Site name'),
    t('Temp'),
    t('High temp'),
    t('Low temp'),
    t('Humidity'),
    t('Dew point'),
    t('Wind speed'),
    t('High wind speed'),
    t('Wind direction'),
    t('Wind direction (deg)'),
    t('Rain'),
    t('Pressure'),
    t('Arc int'),
    );
  
   $select = db_select('bamp_weather', 'w')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($page_limit);
   
  $select->addField('w', 'id');
  $select->addField('w', 'date');
  $select->addField('w', 'time');
  $select->addField('w', 'site_name');
  $select->addField('w', 'temp');
  $select->addField('w', 'temp_hi');
  $select->addField('w', 'temp_low');
  $select->addField('w', 'hum');
  $select->addField('w', 'dew_pt');
  $select->addField('w', 'wind_speed');
  $select->addField('w', 'wind_hi');
  $select->addField('w', 'wind_dir');
  $select->addField('w', 'wind_dir_deg');
  $select->addField('w', 'rain');
  $select->addField('w', 'bar');
  $select->addField('w', 'arc_int');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();

  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
  }
  
   $nr = count($rows);
  
  //  Loop to replace the ID text with a link to the edit form for that entry
  for ($i = 0; $i < $nr; $i++) {
    $id = $rows[$i]['id'];
    $rows[$i]['id'] = l($id, 'bamp/Edit forms/Weather data/' . $id);
  }
  
  $form['Form'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  
    $form['pager'] = array(
      '#theme' => 'pager',
      '#quantity' => 10,
    );
  
  return $form;
}

function bamprepweather_menu_callback() {
  return drupal_get_form('bamprepweather_form');
}

function bamp_trip_report_form($form_state) {
  drupal_set_title(t('Trip data'));

  $form = array();

  $content['form'] = array(
    '#markup' => '<p> This is the BAMP trip form</p>',
  );

  // Check if there is sorting request
  if (isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if ($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    // Which column will be sorted
    switch ($_GET['order']) {
      case 'Result ID':
        $order = 'id';
        break;
      case 'Date':
        $order = 'date';
        break;
      default:
        $order = 'id';
    }
  }
  else {
    // Default sort
    $sort = 'ASC';
    $order = 'id';
  }

  $form = array();

  $page_limit = 20;

  $header = array(
    array('data' => t('Result ID'), 'field' => 'id'),
    array('data' => t('Date'), 'field' => 'date'),
    t('Time'),
    t('Site name'),
    t('Site ID'),
    t('Vessel'),
    t('Method'),
    t('Comments'),
    );
  
   $select = db_select('bamp_trip', 't')
      ->extend('TableSort')
      ->extend("PagerDefault")
      ->limit($page_limit);
   
  $select->addField('t', 'id');
  $select->addField('t', 'start_date');
  $select->addField('t', 'start_time');
  $select->addField('t', 'name');
  $select->addField('t', 'site_id');
  $select->addField('t', 'vessel');
  $select->addField('t', 'method');
  $select->addField('t', 'comments');
  $select->orderBy($order, $sort);
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();

  foreach ($entries as $result) {

    $rows[] = array_map('check_plain', $result);
  }
  
   $nr = count($rows);
  
  //  Loop to replace the ID text with a link to the edit form for that entry
  for ($i = 0; $i < $nr; $i++) {
    $id = $rows[$i]['id'];
    $rows[$i]['id'] = l($id, 'bamp/Edit forms/Trip data/' . $id);
  }
  
  $form['Form'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  
    $form['pager'] = array(
      '#theme' => 'pager',
      '#quantity' => 10,
    );
  
  return $form;
}

function bamp_trip_report_menu_callback() {
  return drupal_get_form('bamp_trip_report_form');
}